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
						  double padGap ) : 
    _rMin( rMin ),
    _rMax( rMax ),
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

    _extent.resize(4) ;
    _extent[0] = _rMin ;
    _extent[1] = _rMax ;
    _extent[2] = 0.0 ;
    _extent[3] = 2. * M_PI ;

    // compute the row properties

    int nr = (int) std::floor ( ( rMax - rMin ) /  _padHeight ) ;

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

      double u = 2. * row.RCenter * M_PI  ;

      row.NPad = (int) std::floor( u / (_padWidth + padGap )  ) ; 

      row.PhiPad = 2. * M_PI  / row.NPad ;

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

    if( r < 0.0 ) r = 0.0 ;

    while( phi < 0.0 ) {  phi += 2. * M_PI  ; }
    while( phi > 2*M_PI ) {  phi -= 2. * M_PI  ; }

    int rowNum = r < _rMin ? 0 : (int) std::floor( ( r - _rMin ) / _rowHeight  ) ;

    if( rowNum >= _nRow  )       
      rowNum = _nRow -1 ;
      

    int padNum = (int) std::floor( phi / _rows[ rowNum ].PhiPad ) ;
    
    if( padNum >=  _rows[ rowNum ].NPad  )
      padNum = _rows[ rowNum ].NPad  - 1 ;
 
    //     std::cout << " FixedPadSizeDiskLayout::getNearestPad( " << r << " , " << phi << " ) " 
    // 	      << "  _rows[ rowNum ].PhiPad : " <<  _rows[ rowNum ].PhiPad 
    // 	      << std::endl ;

    return getPadIndex( rowNum , padNum ) ;
				  
  }

  int FixedPadSizeDiskLayout::getLeftNeighbour(int padIndex) const {
    
    int pn = getPadNumber( padIndex ) + 1  ;
    int rn = getRowNumber( padIndex)  ;
    
    int nPad = _rows.at(rn).NPad ;
    
    if( pn > nPad-1 ){ 

      pn = 0 ;
    }
    
    return getPadIndex(  rn , pn ) ;
  }

  int FixedPadSizeDiskLayout::getRightNeighbour(int padIndex) const {

    int pn = getPadNumber( padIndex ) - 1  ;
    int rn = getRowNumber( padIndex)  ;
    
    int nPad = _rows.at(rn).NPad ;
    
    if( pn < 0  ){ 

      pn = nPad - 1  ;
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

    return  isInsidePad( r , phi , getNearestPad( r, phi ) )  ;

  }
  
} // namespace

