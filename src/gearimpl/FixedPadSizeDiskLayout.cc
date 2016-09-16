#include "gearimpl/FixedPadSizeDiskLayout.h"

#include <cmath>
#include <math.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace gear {

  FixedPadSizeDiskLayout::~FixedPadSizeDiskLayout()
  {
      cleanup();
  }

    PadRowLayout2D* FixedPadSizeDiskLayout::clone() const
    {
	return new  FixedPadSizeDiskLayout(*this);
    }

  void FixedPadSizeDiskLayout::cleanup()
  {
    for( unsigned i=0; i<_padIndices.size(); ++i ){
      delete _padIndices[i] ;
    }
  }

  FixedPadSizeDiskLayout::FixedPadSizeDiskLayout( double rMin, double rMax,
						  double padHeight, double padWidth,
						  int nRow ,
						  double padGap,
						  double phiMax) :
    _rMin( rMin ),
    _rMax( rMax ),
    _phiMax( phiMax ),
    _padWidth( padWidth ) ,
    _padHeight(padHeight)  ,
    _padGap( padGap )
  {

    // minimal sanity check:
    if ( _padHeight <= 0.0  || _padWidth <= 0.0 ) {

      std::stringstream s ;
      s << " FixedPadSizeDiskLayout: missing or inconsistent parameters"
	<< " - padHeight: " << _padHeight
	<< " - padWidth : " << _padWidth ;

      throw ParseException( s.str() ) ;
    }

	// wrap phi to +- 2*pi, which is the max range for phiMin and phiMax
    
	while( _phiMax < -2.*M_PI ) {  _phiMax += 2. * M_PI  ; }
    while( _phiMax >  2.*M_PI ) {  _phiMax -= 2. * M_PI  ; }


    _extent.resize(4) ;
    _extent[0] = _rMin ;
    _extent[1] = _rMax ;
    _extent[2] = 0.0 ;
    _extent[3] = _phiMax ;

    // compute the row properties

    //int nr = (int) std::floor ( ( rMax - rMin ) /  _padHeight ) ;
    //int nr = (int) ( ( rMax - rMin ) /  _padHeight ) ;
    int nr = ( ( rMax + 0.01  - rMin ) /  _padHeight ) ;

    _nRow = ( nRow > 0 ) ? nRow : nr ;

    if( nRow > nr ) {

      std::cout << " WARNING: FixedPadSizeDiskLayout() :  cannot place  " <<  nRow
		<< " rows of height " <<  _padHeight
		<< " mm between rMin = " << rMin << " mm and rMax = " << rMax
		<< " mm -  will use nRow = " << nr << " !! "
		<< std::endl ;

      _nRow = nr ;


    }

    _padIndices.resize( _nRow ) ;
    // initialise all pointers to 0
    // this thing is only filled when accessed by getPadsInRow() the first time
    // to allocate the memory only when needed
    for (std::vector< std::vector<int>* >::iterator rowIter = _padIndices.begin() ;
	 rowIter < _padIndices.end(); rowIter++)
    {
	*rowIter=0;
    }

    _rowHeight =  ( rMax - rMin ) / _nRow ;

    _nPad  = 0 ;

    for( int i = 0 ; i < _nRow ; i++ ) {

      Row row ;

      row.RCenter  =  rMin + ( i * _rowHeight )  + .5 * _rowHeight ;

      double u = row.RCenter * _phiMax  ;

      row.NPad = static_cast<int>( std::floor( u / (_padWidth + padGap )  ) );

      row.PhiPad = _phiMax  / row.NPad ;

//       std::cout << "  row, row.NPad , row.PhiPad : " << i << " , " << row.NPad << " , " <<  row.PhiPad
// 		<< std::endl;

      _nPad += row.NPad ;

      _rows.push_back( row ) ;
    }

  }

  FixedPadSizeDiskLayout::FixedPadSizeDiskLayout( const FixedPadSizeDiskLayout & right)
  {
      copy_and_assign(right);
  }

  FixedPadSizeDiskLayout & FixedPadSizeDiskLayout::operator = ( const FixedPadSizeDiskLayout & right)
  {
      cleanup();
      copy_and_assign(right);
      return *this;
  }

  void FixedPadSizeDiskLayout::copy_and_assign(const  FixedPadSizeDiskLayout & right)
  {
      _rMin = right._rMin;
      _rMax = right._rMax;
      _phiMax = right._phiMax;
      _rowHeight = right._rowHeight;
      _padWidth = right._padWidth;
      _padHeight = right._padHeight;
      _padGap = right._padGap;
      _nRow = right._nRow;
      _nPad = right._nPad;
      _rows = right._rows;
      _extent = right._extent;

      // just set the _padIndices to an vector of empty entries.
      // In every instance, also the copied ones, it is filled only at first
      // usage to save memory
      // just like in the constructor
      _padIndices.resize( _nRow ) ;
      for (std::vector< std::vector<int>* >::iterator rowIter = _padIndices.begin() ;
	   rowIter < _padIndices.end(); rowIter++)
      {
	  *rowIter=0;
      }


  }

  int FixedPadSizeDiskLayout::getNRows() const {
    return _rows.size() ;
  }


  double FixedPadSizeDiskLayout::getPadWidth(int padIndex) const {

    // phi of pad in row :
    int rowNum =  getRowNumber( padIndex ) ;
    try {

      // need to return padWidth in radians !!
      return _padWidth / _rows.at( rowNum ).RCenter ;

    }
    catch(std::out_of_range){
      return 0. ;
    }
  }

  double FixedPadSizeDiskLayout::getPadPitch(int padIndex) const {

    // phi of pad in row :
    int rowNum =  getRowNumber( padIndex ) ;
    try {

      // need to return padPitch in radians !!
	return _rows.at( rowNum ).PhiPad;

    }
    catch(std::out_of_range){
      return 0. ;
    }
  }



  Vector2D FixedPadSizeDiskLayout::getPadCenter(int padIndex)  const {

    int rowNum =  getRowNumber( padIndex ) ;
    int padNum =  getPadNumber( padIndex ) ;

    double r = _rows[ rowNum ].RCenter ;

    double phi =  ( padNum + 0.5 ) * _rows[ rowNum ].PhiPad ;

    return Vector2D( r , phi ) ;
//     return std::make_pair( r , phi ) ;
  }

  const std::vector<int>& FixedPadSizeDiskLayout::getPadsInRow(int rowNumber) const {

    static std::vector<int> empty ;
    try {
      if( _padIndices.at(rowNumber )  == 0 ) {

	int nPad = _rows.at(rowNumber).NPad ;

	_padIndices[ rowNumber ]  = new std::vector<int>( nPad )  ;

	for(int i = 0 ; i < nPad ; i++){

	  _padIndices[rowNumber]->operator[](i)  = getPadIndex( rowNumber , i  ) ;
	}

      }
    } catch( std::out_of_range& r) {

      //       std::cout << " FixedPadSizeDiskLayout::getPadsInRow : no row " << rowNumber << std::endl ;
      return empty ;

    }

    return *_padIndices[ rowNumber ] ;

  }

  int FixedPadSizeDiskLayout::getPadIndex(int rowNum, int padNum) const {

    if( (unsigned) rowNum > _rows.size() - 1 ) {

      throw std::out_of_range(" FixedPadSizeDiskLayout::getPadIndex row number too large !");
    }

    if( padNum > _rows[rowNum].NPad - 1 ) {

      std::stringstream sstr ;

      sstr << "FixedPadSizeDiskLayout::getPadIndex: pad number too large: "
	   << padNum << " only " <<   _rows[rowNum].NPad << " pads in row " << rowNum ;

      throw std::out_of_range( sstr.str() );
    }

    return  (rowNum << 16 ) | ( 0x0000ffff & padNum ) ;


  }

  int FixedPadSizeDiskLayout::getNearestPad(double r, double phi) const {

	if (fabs(r) < 1e-6)
	  throw Exception("FixedPadSizeDiskLayout::getNearestPad: r can't be zero !");

	// Initialise with the radius itself. In case of a full circle there is no 
	// phiMin or phiMax, just to avoid compiler warnings
	double rProjectedOnPhiMax = r;
	double rProjectedOnPhiMin = r;
    
    if (fabs(_phiMax - 2*M_PI) < 1e-6) {
	  
	  // wrap phi to 0 < phi < 2*M_PI
	  while( phi < 0       ) {  phi += 2. * M_PI  ; }
	  while( phi > 2.*M_PI ) {  phi -= 2. * M_PI  ; }
	
	} else {
	 
	  while( phi < -2*M_PI       ) {  phi += 2. * M_PI  ; }
	  while( phi > 2.*M_PI ) {  phi -= 2. * M_PI  ; }
	  
	  rProjectedOnPhiMax = r * cos(phi - _phiMax);
	  rProjectedOnPhiMin = r * cos(phi - 0.);

	}
	  
	//
	  
	int rowNum = 0;
	int padNum = 0;

	// We make two distinct cases:
	// Area 1: The point (r, phi) is inside (r > 0, phiMin <= phi <= phiMax),
	// Area 2: Points outside Area 1.
	
	// Area 1
	// This was the original code for the whole circle. (Modified a bit)
	
	if ( r > 0. && phi >= 0. && phi <= _phiMax) {
	  rowNum = r < _rMin ? 0 : (int) std::floor( ( r - _rMin ) / _rowHeight  ) ;
  
	  if( rowNum >= _nRow  )
		rowNum = _nRow -1 ;
  
	  padNum = (int) std::floor( phi / _rows[ rowNum ].PhiPad ) ;
  
	  if( padNum >=  _rows[ rowNum ].NPad  )
		padNum = _rows[ rowNum ].NPad  - 1 ;
  
	  //     std::cout << " FixedPadSizeDiskLayout::getNearestPad( " << r << " , " << phi << " ) "
	  // 	      << "  _rows[ rowNum ].PhiPad : " <<  _rows[ rowNum ].PhiPad
	  // 	      << std::endl ;
  
	  return getPadIndex( rowNum , padNum ) ;
	}
	
	// Area 2 can be optimized by dividing it into more areas.
	// Then we don't need to calculate distances at all!
	// For best understanding you need to draw all the areas on paper by hand.
	//
	// Expensive condition checkings are moved towards the end.
	
	else if ( rProjectedOnPhiMin < _rMin && phi < 0 ) {
	  //std::cout << "r < _rMin && phi < 0" << std::endl;
	  
	  rowNum = 0;
	  padNum = 0;
	}
	
	else if ( rProjectedOnPhiMin > _rMax && phi < 0 ) {
	  //std::cout << "r > _rMax && phi < 0" << std::endl;
	  
	  rowNum = _nRow - 1;
	  padNum = 0;
	}
	
	else if ( rProjectedOnPhiMax < _rMin && phi > _phiMax ) {
	  //std::cout << "r < _rMin * cos(phi - _phiMax) && phi > _phiMax" << std::endl;
	  
	  rowNum = 0;
	  padNum = _rows[ rowNum ].NPad  - 1 ;
	}

	else if ( rProjectedOnPhiMax > _rMax && phi > _phiMax ) {
	  //std::cout << "r > _rMax * cos(phi - _phiMax) && phi > _phiMax" << std::endl;
	  
	  rowNum = _nRow - 1;
	  padNum = _rows[ rowNum ].NPad  - 1 ;
	}

	else if ( rProjectedOnPhiMin >= _rMin && rProjectedOnPhiMin <= _rMax && phi < 0 ) {
	  //std::cout << "r >= _rMin && r <= _rMax && phi < 0" << std::endl;
	  
	  r = rProjectedOnPhiMin;
	  
	  rowNum = (int) std::floor( ( r - _rMin ) / _rowHeight  ) ;

	  rowNum = rowNum < 0 ? 0 : rowNum;
	  rowNum = rowNum >= _nRow ? _nRow -1 : rowNum;
	
	  padNum = 0;
	}
	
	else if ( rProjectedOnPhiMax >= _rMin  && rProjectedOnPhiMax <= _rMax && phi > _phiMax ) {
	  //std::cout << "r >= _rMin * cos(phi - _phiMax) && r <= _rMax * cos(phi - _phiMax) && phi > _phiMax" << std::endl;
	  
	  r = rProjectedOnPhiMax;
	  
	  rowNum = (int) std::floor( ( r - _rMin ) / _rowHeight  ) ;
	  
	  rowNum = rowNum < 0 ? 0 : rowNum;
	  rowNum = rowNum >= _nRow ? _nRow -1 : rowNum;
	
	  padNum = _rows[ rowNum ].NPad  - 1 ;
	}

	else {
	  std::cout
		<< "I have no idea where the nearest pad is for the point ("
		<< r << ", "
		<< phi << ")"
		<< std::endl;
		
	  rowNum = 0;
	  padNum = 0;
	}

	return getPadIndex( rowNum , padNum ) ;
	
	// Another implementation that was implemented before the upper
	// implementation.
  
	// Area 2
	// Check each pad that is on the border, except for the pads on the most
	// outer circle, since that one is in Area 1.
	
	//float r_pad, p_pad;
	//float x_pad, y_pad;
	//
	//float minDistance = 0;
	//float curDistance = 0.;
	//
	//float minR   = 0.;
	//float minPhi = 0.;
	//
	//for (
	//  std::vector<Row>::const_iterator it = _rows.begin();
	//  it != _rows.end();
	//  ++it
	//) {
	//  
	//  for (
	//	float checkPhi = 0.5 * it->PhiPad;
	//	checkPhi < it->PhiPad * it->NPad;
	//	checkPhi += it->PhiPad
	//  ) {
	//	
	//	r_pad = it->RCenter;
	//	p_pad = checkPhi;
	//	
	//	x_pad = r_pad * cos(p_pad);
	//	y_pad = r_pad * sin(p_pad);
	//	
	//	curDistance = pow(x_pad - r*cos(phi), 2) + pow(y_pad - r*sin(phi), 2);
	//	
	//	if (
	//	  (curDistance < minDistance)
	//	  || (it == _rows.begin() && checkPhi - 0.5 * it->PhiPad < 1e-6)
	//	) {
	//	  minDistance = curDistance;
	//	  
	//	  minR   = r_pad;
	//	  minPhi = p_pad;
	//	}
	//	
	//	// When it is not the row closest to the origin,
	//	// jump to the second last pad in the row
	//	// (the foreach loop will then jump to the last pad)
	//	
	//	if (it != _rows.begin()) {
	//	  checkPhi += it->PhiPad * (it->NPad - 2);
	//	}
	//  
	//  }
	//  
	//}
	//
	//rowNum = (int) std::floor( ( minR - _rMin ) / _rowHeight  ) ;
	//   
	//padNum = (int) std::floor( minPhi / _rows[ rowNum ].PhiPad ) ;

	//return getPadIndex( rowNum , padNum ) ;
  }

  int FixedPadSizeDiskLayout::getLeftNeighbour(int padIndex) const {

    int pn = getPadNumber( padIndex ) + 1  ;
    int rn = getRowNumber( padIndex)  ;

    int nPad = _rows.at(rn).NPad ;

	if( pn > nPad-1 ) {
	  
	  // If it is a full circle, go back to the first one.
	  if (fabs(_phiMax - 2*M_PI) < 1e-6)
		pn = 0;

	  else
		throw Exception("FixedPadSizeDiskLayout::getLeftNeighbour: no left neighbour pad !");
	  
	}


    return getPadIndex(  rn , pn ) ;
  }

  int FixedPadSizeDiskLayout::getRightNeighbour(int padIndex) const {

    int pn = getPadNumber( padIndex ) - 1  ;
    int rn = getRowNumber( padIndex)  ;

    int nPad = _rows.at(rn).NPad ;

    if( pn < 0  ){

	  // If it is a full circle, go back to the last one.
	  if (fabs(_phiMax - 2*M_PI) < 1e-6)
		pn = nPad - 1;

	  else
		throw Exception("FixedPadSizeDiskLayout::getRightNeighbour: no right neighbour pad !");
		
    }

    return getPadIndex(  rn , pn ) ;
  }


  bool FixedPadSizeDiskLayout::isInsidePad(double r, double phi, int padIndex) const {

//     double rCenter =  0. ;
//     int rowNumber = getRowNumber( padIndex ) ;
//     try {  rCenter = _rows.at( rowNumber ).RCenter ;
//     }
//     catch(std::out_of_range){
//       return false ;
//     }

    if( r < _rMin || r > _rMax )
      return false ;

    // wrap phi to 0 < phi < 2*M_PI
    
	while( phi < 0       ) {  phi += 2. * M_PI  ; }
    while( phi > 2.*M_PI ) {  phi -= 2. * M_PI  ; }

    Vector2D p  = getPadCenter( padIndex ) ;

//     double phiPadHalf = 0.5 * _padWidth / rCenter ;

    double phiPadHalf = 0.5 * getPadWidth( padIndex ) ;


//     std::cout << " isInsidePad( " << rowNumber << ", " <<  phiPadHalf  << ", " << 0.5 *  _padHeight
// 	      << " - ( " << p.first << ", " << p.second << " ) "
// 	      << (std::abs( r - p.first ) <= 0.5 * _padHeight) << "&&"
// 	      << (std::abs( phi - p.second ) <= phiPadHalf)
// 	      << std::endl ;

//     return  ( std::abs( r - p.first ) <= 0.5 * _padHeight  &&
// 	      std::abs( phi - p.second ) <= phiPadHalf  ) ;

    return  ( std::abs( r - p[0] ) <= 0.5 * _padHeight  &&
	      std::abs( phi - p[1] ) <= phiPadHalf  ) ;

  }

  bool FixedPadSizeDiskLayout::isInsidePad(double r, double phi) const {

    if( r < _rMin || r > _rMax )
      return false ;

    // wrap phi to 0 < phi < 2*M_PI
    
	while( phi < 0       ) {  phi += 2. * M_PI  ; }
    while( phi > 2.*M_PI ) {  phi -= 2. * M_PI  ; }

    return  isInsidePad( r , phi , getNearestPad( r, phi ) )  ;

  }

} // namespace
