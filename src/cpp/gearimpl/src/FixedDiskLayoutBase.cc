#include "gearimpl/FixedDiskLayoutBase.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace gear {
  
  
int FixedDiskLayoutBase::getPadLayoutType() const 
{
//    std::cerr << "FixedDiskLayoutBase::getPadLayoutType() : Warning: " <<std::endl
//	      << "  This is deprecated (ambiguous for polar coordinate systems)"<< std::endl
//	      << "  Please use getCoordinateType() or getPadLayoutImplType() "<< std::endl;
    return getCoordinateType();
} 


double FixedDiskLayoutBase::getDistanceToPad(double r, double phi, int padIndex) const
{
	// calculate the outer coordinates of the pad (for code readability)
	Vector2D padCenter = getPadCenter( padIndex);

	double r_min = padCenter[0] - getRowHeight( getRowNumber(padIndex) ) /2.;
	double r_max = padCenter[0] + getRowHeight( getRowNumber(padIndex) ) /2.;
	double phi_min = padCenter[1]- getPadPitch( padIndex ) /2.;
	double phi_max = padCenter[1]+ getPadPitch( padIndex ) /2.;
	
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
	else // this is more complicated
	{    
	    // calculate pca to straight line along radius with phi_min;
	    
	    // determine r_intersect for x = r_intersect * cos(_phiMin) = r* cos(phi) - m * sin(_phiMin)
	    //                           y = r_intersect * sin(_phiMin) = r*sin(phi) + m * cos(_phiMin)
	    // solve this to r_intersect and m
	    double r_intersect_phiMin = r * (cos(phi)*cos(phi_min) + sin(phi)*sin(phi_min));
	    double m_phiMin =  r * (cos(phi)*sin(phi_min) - sin(phi)*cos(phi_min));
	    
	    double distance_phiMin;
	    if ( r_intersect_phiMin < r_min )
	    {
		// you can check this formula by calculaing sqrt( (x-x_min)^2 - (y-y_min)^2 )
		// where x_min = r_min cos(phi_min) etc.
		distance_phiMin = sqrt( r*r + r_min*r_min -2 * r_min * r_intersect_phiMin );
	    }
	    else
	    if ( r_intersect_phiMin > r_max ) 
	    {
		distance_phiMin = sqrt( r*r + r_max*r_max -2 * r_max * r_intersect_phiMin );
	    }
	    else
		distance_phiMin =  fabs( m_phiMin );


	    // now the same for _phiMax
	    double r_intersect_phiMax = r * (cos(phi)*cos(phi_max) + sin(phi)*sin(phi_max));
	    double m_phiMax =  r * (cos(phi)*sin(phi_max) - sin(phi)*cos(phi_max));
	    
	    double distance_phiMax;
	    if ( r_intersect_phiMax < r_min )
	    {
		// you can check this formula by calculaing sqrt( (x-x_max)^2 - (y-y_max)^2 )
		// where x_max = r_min cos(phi_max) etc.
		distance_phiMax = sqrt( r*r + r_min*r_min -2 * r_min * r_intersect_phiMax );
	    }
	    else
	    if ( r_intersect_phiMax > r_max ) 
	    {
		distance_phiMax = sqrt( r*r + r_max*r_max -2 * r_max * r_intersect_phiMax );
	    }
	    else
		distance_phiMax =  fabs( m_phiMax );

	    return (distance_phiMin < distance_phiMax ? distance_phiMin : distance_phiMax);

	} //else within phi range
    

}

} // namespace

