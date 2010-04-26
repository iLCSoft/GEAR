#include "gearimpl/VersatileDiskRowLayout.h"

#include <cmath>
#include <iostream>
//#include <stdexcept>
#include <sstream>
#include <string>
//#include <cstdlib>
#include <gear/GEAR.h>

namespace gear {
  
VersatileDiskRowLayout::VersatileDiskRowLayout( double rMin ) :
    _nPad(0), _isFrayed(false)
{
    
    _extent.resize(4) ;
    _extent[0] = rMin ;
    _extent[1] = rMin ; // radial size is 0, so rMax = rMin
    _extent[2] = 0 ;
    _extent[3] = 0 ;
}
  
VersatileDiskRowLayout::VersatileDiskRowLayout( const VersatileDiskRowLayout & right)
{
    copy_and_assign(right);
}

void VersatileDiskRowLayout::copy_and_assign(const VersatileDiskRowLayout & right)
{
    _nPad = right._nPad;
    _rows = right._rows;
    _extent = right._extent;

    // just set the _padIndices to an vector of empty entries.
    // In every instance, also the copied ones, it is filled only at first
    // usage to save memory
    // just like in the constructor
    _padIndices.resize( _rows.size() ) ;
    for (std::vector< std::vector<int>* >::iterator rowIter = _padIndices.begin() ; 
	 rowIter < _padIndices.end(); rowIter++)
    {
	*rowIter=0;
    }
}
   
PadRowLayout2D* VersatileDiskRowLayout::clone() const
{
    return new VersatileDiskRowLayout(*this);
}

void VersatileDiskRowLayout::addRow( int nPads , double padPitch , double rowHeight , 
				     double offset, double padWidth, double padHeight,
				     int repeat )
{
    std::string routineName("gear::VersatileDiskRowLayout::addRow(): ");

    // a few sanity checks:
    if ( nPads <= 0 ) 
	throw Exception(routineName+"Error, number of pads must be larger than 0!");
    if ( padPitch < 0. ) 
	throw Exception(routineName+"Error, pad pitch must be positive!");
    if ( padWidth < 0. ) 
	throw Exception(routineName+"Error, pad width must be positive!");
    if ( padWidth > padPitch ) 
	throw Exception(routineName+"Error, pad width must be smaller or equal pad pitch!");
    if ( rowHeight <= 0. )
	throw Exception(routineName+"Error, row height must be larger than 0!");
    if ( repeat <= 0)
	throw Exception(routineName+"Error, number of repitions must be positive!");

    if (padWidth == 0.)
	padWidth = padPitch;
    if (padHeight == 0.)
	padHeight = rowHeight;

    double rCentre = _extent[1] + rowHeight*0.5;

    _nPad +=  repeat*nPads ;

    for (int i = 0; i < repeat; i++)
    {
	_rows.push_back( Row(_rows.size(), nPads, rCentre, padPitch , rowHeight , 
			     padWidth, padHeight, offset ) ); 
	_padIndices.push_back( 0 );
	rCentre += rowHeight;
    }

    // calculate correct plane extent
    _extent[1]+= repeat * rowHeight;
    if ( (_rows.size() ==1) || (_rows.back().getPhiMin() < _extent[2] ) )
	_extent[2] = _rows.back().getPhiMin();
    if ( (_rows.size() ==1) || (_rows.back().getPhiMax() > _extent[3] ) )
	_extent[3] = _rows.back().getPhiMax();
    
    if ( (_extent[3] - _extent[2]) > 2*M_PI )
    {
	_extent[2] = 0.;
	_extent[3] = 2*M_PI;
    }
  }

  int VersatileDiskRowLayout::getNRows() const {
    return _rows.size() ;
  }

  double VersatileDiskRowLayout::getRowHeight(int rowNumber) const 
  { 
      return _rows.at(rowNumber).getRowHeight() ; 
  }
  
  double VersatileDiskRowLayout::getPadHeight(int padIndex) const
  {
      int rowNum =  getRowNumber( padIndex ) ;
      return _rows[rowNum].getPadHeight( ) ;
  }
  
  
  double VersatileDiskRowLayout::getPadWidth(int padIndex) const {

    int rowNum =  getRowNumber( padIndex ) ;

//    return _rows[rowNum].getPadWidth() ; 
    return _rows[rowNum].getPadWidth() ; 
  } 

  double VersatileDiskRowLayout::getPadPitch(int padIndex) const {

    int rowNum =  getRowNumber( padIndex ) ;

//    return _rows[rowNum].getPadWidth() ; 
    return _rows[rowNum].getPadPitch() ; 
  } 


  Vector2D VersatileDiskRowLayout::getPadCenter(int padIndex)  const {
    
    int rowNum =  getRowNumber( padIndex ) ;
    int padNum =  getPadNumber( padIndex ) ;
    
    const Row& row =  _rows[ rowNum ] ;

    double phi = row.getOffset() + (static_cast<double>(padNum) + 0.5) * row.getPadPitch();

    double r = row.getRCentre() ; 

    return Vector2D( r , phi ) ;
  }
  
  int VersatileDiskRowLayout::getPadLayoutType() const 
  {
//      std::cerr << "FixedDiskLayoutBase::getPadLayoutType() : Warning: " <<std::endl
//		<< "  This is deprecated (ambiguous for polar coordinate systems)"<< std::endl
//		<< "  Please use getCoordinateType() or getPadLayoutImplType() "<< std::endl;
      return getCoordinateType();
  } 

  VersatileDiskRowLayout::~VersatileDiskRowLayout()
  {
      cleanup();
  }
    
    VersatileDiskRowLayout & VersatileDiskRowLayout::operator = (const  VersatileDiskRowLayout & right)
    {
	cleanup();
	copy_and_assign(right);
	return *this;
    }

  void VersatileDiskRowLayout::cleanup()
  { 
    for( unsigned i=0; i<_padIndices.size(); ++i ){
      delete _padIndices[i] ;
    }
  }

  const std::vector<int>& VersatileDiskRowLayout::getPadsInRow(int rowNumber) const {
    
    static std::vector<int> empty ;
    
    try {
      
      if( _padIndices.at(rowNumber )  == 0 ) {
	
	  int nPad = _rows.at(rowNumber).getNPads() ;
	
	  _padIndices[ rowNumber ]  = new std::vector<int>( nPad )  ;
	  
	  for(int i = 0 ; i < nPad ; i++){
	      
	      _padIndices[rowNumber]->operator[](i)  = getPadIndex( rowNumber , i  ) ;
	  }
	  
      }
    } catch( std::out_of_range& r) {
      
      //       std::cout << " VersatileDiskRowLayout::getPadsInRow : no row " << rowNumber << std::endl ;
      return empty ;
      
    }
    
    return *_padIndices[ rowNumber ] ;
  }
  

  int VersatileDiskRowLayout::getRowNumber(int padIndex)  const { 

    int rn = ( 0xffff0000 & padIndex ) >> 16 ; 

    if( rn < 0 ||  rn > int(_rows.size() - 1) ){
      
      std::stringstream sstr ;
      
      sstr << "VersatileDiskRowLayout::getRowNumber: illegal rownumber: " 
	   << rn << " for padIndex  0x" << std::hex << padIndex << " nRows: " << _rows.size() << std::dec ;
      
      throw Exception( sstr.str() ) ;
    }

    return rn ;
  } 


  int VersatileDiskRowLayout::getPadIndex(int rowNum, int padNum) const { 


    if( (unsigned) rowNum > _rows.size() - 1 ) {
      
      throw std::out_of_range(" VersatileDiskRowLayout::getPadIndex row number too large !");
    }
    
    if( padNum > _rows[rowNum].getNPads() - 1 ) {
      
      std::stringstream sstr ;
      
      sstr << "VersatileDiskRowLayout::getPadIndex: pad number too large: "
	   << padNum << " only " <<   _rows[rowNum].getNPads() << " pads in row " << rowNum ;

      throw std::out_of_range( sstr.str() );
    }
    
    return  (rowNum << 16 ) | ( 0x0000ffff & padNum ) ;

  }

  
  int VersatileDiskRowLayout::getRightNeighbour(int padIndex) const {
    
    int pn = getPadNumber( padIndex ) - 1  ;
    int rn = getRowNumber( padIndex)  ;
    
    // anything larger than _nPadsInRow would have caused an out_of_range exception in
    // getPadNumber
    if(pn == -1)
    {
    // take into account wrap arround in case of full circle (incl. rounding errors)
	if (fabs(_extent[3] - _extent[2] - 2*M_PI) < 1e-6 )
	    pn =  _rows[rn].getNPads() -1 ;
	else
	    throw Exception("VersatileDiskRowLayout::getRightNeighbour: no right neighbour pad !");
    }

    return getPadIndex(  rn , pn ) ;
  }

  int VersatileDiskRowLayout::getLeftNeighbour(int padIndex) const {

    int pn = getPadNumber( padIndex ) + 1  ;
    int rn = getRowNumber( padIndex)  ;

    // anything larger than _nPadsInRow would have caused an out_of_range exception in
    // getPadNumber
    if (pn == _rows[rn].getNPads())
    {
    // take into account wrap arround in case of full circle (incl. rounding errors)
	if (fabs(_extent[3] - _extent[2] - 2*M_PI) < 1e-6 )
	    pn = 0 ;
	else
	    throw Exception("VersatileDiskRowLayout::getLeftNeighbour: no left neighbour pad !");
    }

    return getPadIndex(  rn , pn ) ;
  }

  int VersatileDiskRowLayout::getNearestPad(double r, double phi) const
  {
      // the brute force method: scan all rows
      int nearestPad = -1 ;
      double distance=0;
      for (std::vector<Row>::const_iterator rowIter = _rows.begin()++;
	   rowIter < _rows.end(); rowIter++)
      {
//	  int nearestPad  = rowIter->getNearestPad(r, phi);
	  double newDistance = rowIter->getDistanceToRow(r, phi);

	  if ( (newDistance < distance) || ( rowIter == _rows.begin() ) )
	  {
	      nearestPad  = rowIter->getNearestPad(r, phi);
	      distance = newDistance;
	  }
      }

      if (nearestPad == -1)
	  throw Exception(std::string("VersatileDiskRowLayout::getNearestPad(): no pad rows defined."));

      return nearestPad;
  }
  
  bool VersatileDiskRowLayout::isInsidePad(double r, double phi, int padIndex) const {
      
    int pn = getPadNumber( padIndex ) ;
    int rn = getRowNumber( padIndex) ;

    const Row& row =  _rows[rn] ;

    double rMin = row.getRCentre() - row.getRowHeight()*0.5;
    double rMax = row.getRCentre() + row.getRowHeight()*0.5;

    // the right edge of the pad
    double phiMin = row.getOffset() + row.getPadPitch()*pn;

    // wrap phi to phiMin .. phiMin + 2*pi
    if (phi < phiMin)
	phi += 2. * M_PI;

    if (phi >= phiMin + 2*M_PI)
	phi -= 2. * M_PI;

    double phiMax = phiMin + row.getPadPitch();

    return ( r >=  rMin && r <= rMax &&
	     phi <= phiMax ) ; // no need to check for phiMin because of wrap around 
  }

  bool VersatileDiskRowLayout::isInsidePad(double r, double phi) const {

    //  outside of pad plane
    if( r < _extent[0] || r > _extent[1] || 
	phi < _extent[2] || phi > _extent[3] )
      
      return false ;
    
    return  isInsidePad( r , phi , getNearestPad( r, phi ) )  ;
  }
  
VersatileDiskRowLayout::Row::Row(int rowNumber, int nPads, double rCentre, double padPitch_mm,
				 double rowHeight,
				 double padWidth_mm, double padHeight, double offset_mm) :
	 _rowNumber(rowNumber), _nPads(nPads), _padPitch_mm(padPitch_mm), _padWidth_mm(padWidth_mm),
	 _rowHeight(rowHeight), _padHeight(padHeight), _offset_mm(offset_mm), 
	 _rCentre(rCentre)
{
    _offset   = _offset_mm / rCentre;

    // wrap offset to +- 2*pi
    if (_offset < - 2. * M_PI)
    {
	_offset += 2. * M_PI;
	_offset_mm += rCentre*2. * M_PI;
    }

    if (_offset >=  2. * M_PI)
    {
	_offset -= 2. * M_PI;
	_offset_mm += rCentre*2. * M_PI;	
    }

    _padPitch = _padPitch_mm / rCentre;
    if (_padPitch * nPads > 2*M_PI)
    {
	std::stringstream sstr;
	sstr <<  "gear::VersatileDiskRowLayout::addRow(): "
	     <<  "Error: Too many pads ("<<nPads<<" in row "<<rowNumber<<"!";
	throw Exception( sstr.str() );
    }

    _padWidth = _padWidth_mm / rCentre;
    _offset   = _offset_mm / rCentre;

    _phiMax = _offset + _padPitch*_nPads;
    _rMin = _rCentre - rowHeight*0.5;
    _rMin = _rCentre + rowHeight*0.5;    
}

int VersatileDiskRowLayout::Row::getNearestPad(double r, double phi) const
{
    // wrap angle to phiMin .. phiMin + 2 pi
    if (phi < _offset)
	phi += 2. * M_PI;

    if (phi >= _offset + 2*M_PI)
	phi -= 2. * M_PI;

    // if the angle is in the active angle one can easily calculate the pad number
    if (phi < _offset + _nPads * _padPitch )
    {
	int padNumber = static_cast<int>(floor( (phi - _offset)/ _padPitch ));
	
	return ( (_rowNumber<< 16) | (0x000ffff & padNumber) );
    }
    else //outside the active angle
    { // it can only be one of the edge pad
	double distance1 = distanceToPhiMin( r , phi);
	double distance2 = distanceToPhiMax( r , phi);

	if ( distance1 < distance2)
	    return (_rowNumber << 16); // pad 0 in the row
	else
	    return ( (_rowNumber<< 16) | (0x000ffff & (_nPads-1)) );
    }
}

double VersatileDiskRowLayout::Row::getDistanceToRow(double r, double phi) const
{
    // copy the code from getDistanceToPad....

    double r_min = _rCentre - _rowHeight*0.5;
    double r_max = _rCentre + _rowHeight*0.5;
    double phi_min = _offset;
    double phi_max = _offset + _nPads * _padPitch;
	
//      if( (r  < padCenter[0] - _rowHeight/2.) || (r > padCenter[0] + _rowHeight/2.) ||
//	  (phi < padCenter[1]- _padAngle/2.)  || (phi < padCenter[1]+_padAngle/2. ) )
	if ( (phi >= phi_min)  && (phi <= phi_max ) )
	{
	    // this is the easy part, it's just distances in r
	    if( (r  >= r_min) && (r <= r_max) )
		// we are inside the pad
		return 0.;
	    else
	    {
		if (r  < r_min)
		    return r_min - r;
		else 
		    return r - r_max;
	    }
	}
	else // this is more complicated (code has been transferred for reusability)
	{    
	    double distance_phiMin = distanceToPhiMin(r, phi);
	    double distance_phiMax = distanceToPhiMax(r, phi);

	    return (distance_phiMin < distance_phiMax ? distance_phiMin : distance_phiMax);

	} //else within phi range
}

    
double VersatileDiskRowLayout::Row::distanceToPhiMin(double r, double phi) const
{
    // calculate pca to straight line along radius with phi_min;
	    
    // determine r_intersect for x = r_intersect * cos(_phiMin) = r* cos(phi) - m * sin(_phiMin)
    //                           y = r_intersect * sin(_phiMin) = r*sin(phi) + m * cos(_phiMin)
    // solve this to r_intersect and m
    // n. B. _offset ia _phiMin
    double r_intersect_phiMin = r * (cos(phi)*cos(_offset) + sin(phi)*sin(_offset));
    double m_phiMin =  r * (cos(phi)*sin(_offset) - sin(phi)*cos(_offset));
    
    if ( r_intersect_phiMin < _rMin )
    {
	// you can check this formula by calculaing sqrt( (x-x_min)^2 - (y-y_min)^2 )
	// where x_min = _rMin cos(phi_min) etc.
	return sqrt( r*r + _rMin*_rMin -2 * _rMin * r_intersect_phiMin );
    }
    else
    if ( r_intersect_phiMin > _rMax ) 
    {
	return sqrt( r*r + _rMax*_rMax -2 * _rMax * r_intersect_phiMin );
    }
    else
	return fabs( m_phiMin );
}

double VersatileDiskRowLayout::Row::distanceToPhiMax(double r, double phi) const
{
    // now the same for _phiMax
    double r_intersect_phiMax = r * (cos(phi)*cos(_phiMax) + sin(phi)*sin(_phiMax));
    double m_phiMax =  r * (cos(phi)*sin(_phiMax) - sin(phi)*cos(_phiMax));
    
    if ( r_intersect_phiMax < _rMin )
    {
	// you can check this formula by calculaing sqrt( (x-x_max)^2 - (y-y_max)^2 )
	// where x_max = _rMin cos(phiMax) etc.
	return sqrt( r*r + _rMin*_rMin -2 * _rMin * r_intersect_phiMax );
    }
    else
    if ( r_intersect_phiMax > _rMax ) 
    {
	return sqrt( r*r + _rMax*_rMax -2 * _rMax * r_intersect_phiMax );
    }
    else
	return fabs( m_phiMax );
}    


} // namespace


