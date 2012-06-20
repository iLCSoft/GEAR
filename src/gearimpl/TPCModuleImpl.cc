#include "gearimpl/TPCModuleImpl.h"
#include "gearimpl/FixedPadSizeDiskLayout.h"
#include "gearimpl/RectangularPadRowLayout.h"
#include <cmath>
#include <complex>
#include <iostream>

namespace gear {
    /* Transforms a point from the global coordinates to Layout2D coordinates.*/
    gear::Vector2D TPCModuleImpl::globalToLocal(double c0,double c1)const 
    {
        if ( _localIsGlobal )
        {
	  return Vector2D( c0, c1 );
	}

	// invert the x coordinate for negative half TPC
	if ( _zPosition <= 0 ) // 0 is also part of the negative half TPC
	{
	  switch (_momsCoordinateType)
	  {
	    case PadRowLayout2D::POLAR :
	      // this is the angle corresponding a mirroring at the y axis
	      c1 = M_PI - c1; 
	      break;
	    case  PadRowLayout2D::CARTESIAN :
	      c0 = -c0; // just invert the x coordinate
	      break;
	    default:
		throw gear::Exception("TPCModuleImpl::globalToLocal: unkown coorinate type");	
	  }
	}

	// now that the incomming global coordinates have the right position relative to the 
	// end plate we can start the conversion to local
	
	// the global coordinates in cartesian coordinates
	double x, y;
	switch (_momsCoordinateType)
	{
	    case PadRowLayout2D::POLAR :
	    {
	        // to speed this up: if the local coordinate system is polar and there is no offset 
	        // the calculation is much easyer
	        if ( (_padRowLayout->getCoordinateType() == PadRowLayout2D::POLAR ) && 
		     (_offset_cartesian[0] == 0 ) &&
		     (_offset_cartesian[0] == 0 ) )
		{
		    double localAngle = c1 - _angle ;
		    // wrap to 0..2pi
		    if (localAngle < 0) localAngle+= 2*M_PI;
		    if (localAngle > 2*M_PI) localAngle+= 2*M_PI;

		    return Vector2D(c0, localAngle);
		}
	      
		// there is an offset or pad plane is cartesian, we have to do the full stuff
		x = c0 * std::cos( c1 );
		y = c0 * std::sin( c1 );
	    }
	    break;
	    case  PadRowLayout2D::CARTESIAN :
	    {
		x = c0;
		y = c1;
	    }
	    break;
	    default:
		throw gear::Exception("TPCModuleImpl::globalToLocal: unkown coorinate type");	
	}

	// x_prime and y_prime are the cartesian coordinates after moving to local origin
	double x_prime = x - _offset_cartesian[0];
	double y_prime = y - _offset_cartesian[1];

	// now rotate arround local origin
	double  x_prime_rot = x_prime * _cos_angle + y_prime * _sin_angle;
	double  y_prime_rot = y_prime * _cos_angle - x_prime * _sin_angle;

	Vector2D toReturn;
	switch (_padRowLayout->getCoordinateType())
	{
	    case PadRowLayout2D::POLAR :
	    {
		toReturn[0] = std::sqrt(x_prime_rot*x_prime_rot + y_prime_rot*y_prime_rot);
		toReturn[1] = atan2( y_prime_rot, x_prime_rot);
		// atan gives +-pi as range, convert to 0..2pi
		if ( toReturn[1] < 0) 
		{
		  toReturn[1]+=2*M_PI;
		}
	    }
	    break;
	    case  PadRowLayout2D::CARTESIAN :
	    {
		toReturn[0] = x_prime_rot;
		toReturn[1] = y_prime_rot;
	    }
	    break;
	    default:
		throw gear::Exception("TPCModuleImpl::localToGlobal: unkown coorinate type");	
	}

	return toReturn;
    } 

    /* Transforms a point from the Layout2D coordinates to global coordinates. */
    gear::Vector2D TPCModuleImpl::localToGlobal(double c0,double c1)const 
    {
        if ( _localIsGlobal )
        {
	  return Vector2D( c0, c1 );
	}

	// x and y prime are c0 and c1 in local cartesian coordinates
	double x_prime, y_prime;
	switch (_padRowLayout->getCoordinateType())
	{
	    case PadRowLayout2D::POLAR :
	    {
		double r = c0;
		double phi = c1 + _angle;

		// if the TPC coordinate system is also polar and there is no offset we are
		// already done
		if ( (_momsCoordinateType == PadRowLayout2D::POLAR ) && 
		     (_offset_cartesian[0] == 0 ) &&
		     (_offset_cartesian[1] == 0 ) )
		{
		  // flip angle in case of negative half TPC
		  if ( _zPosition <=0 )
		  {
		    phi = M_PI - phi;
		  }

		  // wrap phi to 0..2pi
		  if (phi < 0)      phi += 2*M_PI;
		  if (phi > 2*M_PI) phi -= 2*M_PI;
		  return Vector2D( r, phi );
		}

		x_prime = r * std::cos( phi );
		y_prime = r * std::sin( phi );
	    }
	    break;
	    case  PadRowLayout2D::CARTESIAN :
	    {
		x_prime = c0 * _cos_angle - c1 * _sin_angle;
		y_prime = c1 * _cos_angle + c0 * _sin_angle;
	    }
	    break;
	    default:
		throw gear::Exception("TPCModuleImpl::localToGlobal: unkown coorinate type");	
	}

	// x and y are the global cartesian coordinates
	double x = x_prime + _offset_cartesian[0];
	double y = y_prime + _offset_cartesian[1];

	// flip x in case of negative half TPC
	if ( _zPosition <= 0 )
	{
	  x = -x;
	}

	// calculate the return value
	gear::Vector2D toReturn;
	switch (_momsCoordinateType)
	{
	    case PadRowLayout2D::POLAR :
	    {
	        toReturn[0] = std::sqrt(x*x+y*y);
		toReturn[1] = atan2(y,x) ;
		// atan gives +-pi as range, convert to 0..2pi
		if ( toReturn[1] < 0 )
		{
		    toReturn[1] += 2*M_PI;
		}
	    }
	    break;
	    case  PadRowLayout2D::CARTESIAN :
	    {
		toReturn[0] = x;
		toReturn[1] = y;
	    }
	    break;
	    default:
		throw gear::Exception("TPCModuleImpl::localToGlobal: unkown coorinate type");	
	}

	return toReturn;
    }
    /** Transforms a local plane Extend to a global Plane extend 
     */
    void TPCModuleImpl::convertLocalPlaneToGlobalPlaneExtend()
    {
	// first calculate the local module extend
	if (_border == 0) // no border: local module extend is pad layout extend
	{
	    _localModuleExtent = _padRowLayout->getPlaneExtent();
	}
	else
	{
	    switch (_padRowLayout->getCoordinateType())
	    {
		case PadRowLayout2D::POLAR :
		{
		    //first handle the radius
		    double rMin = _padRowLayout->getPlaneExtent()[0] - _border;
		    _localModuleExtent[0] = ( rMin >=0. ? rMin : 0. );
		    _localModuleExtent[1] = _padRowLayout->getPlaneExtent()[1] + _border;//rMax
		
		    // first calculate the angle which corresponds to the border
		    double phiBorder;
		    if (_padRowLayout->getPlaneExtent()[1] > 0) // inner radius of pad plane > 0
			                                        // to avoid divide by 0
		    {
			//phiBorder = _border/_padRowLayout->getPlaneExtent()[1] * 2. *M_PI;
			phiBorder = _border / _padRowLayout->getPlaneExtent()[0];
		    }
		    else // the plane extent is a full circle 
		    {
			phiBorder = M_PI;
		    }
		    
		    double phiMin = _padRowLayout->getPlaneExtent()[2] - phiBorder;
		    double phiMax = _padRowLayout->getPlaneExtent()[3] + phiBorder;

		    //phi needs some safety checks
		    if (phiMax - phiMin > 2*M_PI)
		    {
			// the border should be half of the gap between the pad layout edged
			phiBorder = (2*M_PI - phiMax + phiMin)/2.;
			phiMin = _padRowLayout->getPlaneExtent()[2] - phiBorder;
			phiMax = _padRowLayout->getPlaneExtent()[3] + phiBorder;
		    }
		    
		    _localModuleExtent[2] = phiMin;
		    _localModuleExtent[3] = phiMax;

		}
		break;
		case  PadRowLayout2D::CARTESIAN :
		{
		    // this is easy, just add / subtract the border to the local extent
		    _localModuleExtent[0] = _padRowLayout->getPlaneExtent()[0] - _border;//xMin
		    _localModuleExtent[1] = _padRowLayout->getPlaneExtent()[1] + _border;//xMax
		    _localModuleExtent[2] = _padRowLayout->getPlaneExtent()[2] - _border;//yMin
		    _localModuleExtent[3] = _padRowLayout->getPlaneExtent()[3] + _border;//yMax
		}
		break;
		default:
		    throw gear::Exception("TPCModuleImpl::convertLocalPlaneToGlobalPlaneExtend(): unkown coorinate type");	
		    
	    }// switch (_padRowLayout->getCoordinateType())
	} // else (_border == 0)


	// now calculate the global extents
	switch (_momsCoordinateType)
	{
	    case PadRowLayout2D::POLAR :
	    {
		switch (_padRowLayout->getCoordinateType())
		{
		    case PadRowLayout2D::POLAR :
		    {
			_moduleExtent= calculateGlobalExtentPolarPolar( _localModuleExtent );
			_planeExtent=  calculateGlobalExtentPolarPolar( _padRowLayout->getPlaneExtent() );
		    }
		    break;
		    case  PadRowLayout2D::CARTESIAN :
			throw gear::NotImplementedException("TPCModuleImpl::convertLocalPlaneToGlobalPlaneExtend: not implemented for global polar/cartesian coordinates.");
//		    {
//			_moduleExtent= calculateGlobalExtentPolarCartesian( _localModuleExtent );
//			_planeExtent=  calculateGlobalExtentPolarCartesian( _padRowLayout->getPlaneExtent() );
//		    }
		    break;
		    default:
			throw gear::Exception("TPCModuleImpl::convertLocalPlaneToGlobalPlaneExtend():: unkown coorinate type");	
		}

	    }// case _momsCoordinateSystem = POLAR
	    break;
	    case  PadRowLayout2D::CARTESIAN :
	    {
		switch (_padRowLayout->getCoordinateType())
		{
		    case PadRowLayout2D::POLAR :
		    {
			_moduleExtent= calculateGlobalExtentCartesianPolar( _localModuleExtent );
			_planeExtent=  calculateGlobalExtentCartesianPolar( _padRowLayout->getPlaneExtent() );
		    }
		    break;
		    case  PadRowLayout2D::CARTESIAN :
		    {
			_moduleExtent= calculateGlobalExtentCartesianCartesian( _localModuleExtent );
			_planeExtent=  calculateGlobalExtentCartesianCartesian( _padRowLayout->getPlaneExtent() );
		    }
		    break;
		    default:
			throw gear::Exception("TPCModuleImpl::convertLocalPlaneToGlobalPlaneExtend():: unkown coorinate type");	
		}
	    }
	    break;
	    default:
		throw gear::Exception("TPCModuleImpl::convertLocalPlaneToGlobalPlaneExtend(): unkown coorinate type");	
	}// switch _momsCoordinateType
	

	//   	throw gear::Exception("TPCModuleImpl::convertLocalPlaneToGlobalPlaneExtend:This Has Not Been Implimented");
    }

    std::vector<double>  TPCModuleImpl:: calculateGlobalExtentCartesianCartesian( std::vector<double> localExtent )
    {
	// this is esasy: just convert the local plane extent to global coordinates 
	// and find the minimum and maximum
	Vector2D firstEdge = localToGlobal(localExtent[0],localExtent[2] );// xmin, yMin
	
	double xMin=firstEdge[0];
	double xMax=firstEdge[0];
	double yMin=firstEdge[1];
	double yMax=firstEdge[1];
	
	Vector2D edges[3]; // the three other edges (combinations if x/y , min/max)
	edges[0] = localToGlobal(localExtent[0],localExtent[3] ); // xMin, yMax
	edges[1] = localToGlobal(localExtent[1],localExtent[2] ); // xMax, yMin
	edges[2] = localToGlobal(localExtent[1],localExtent[3] ); // xMax, yMax

	
	// look for maxima and minima
	for (unsigned int i = 0; i < 3 ; i++)
	{
	    if (xMin > edges[i][0]) xMin =  edges[i][0];
	    if (xMax < edges[i][0]) xMax =  edges[i][0];
	    if (yMin > edges[i][1]) yMin =  edges[i][1];
	    if (yMax < edges[i][1]) yMax =  edges[i][1];
	}

	std::vector<double> globalExtent;
	globalExtent.push_back(xMin);
	globalExtent.push_back(xMax);
	globalExtent.push_back(yMin);
	globalExtent.push_back(yMax);

	return globalExtent;
    }

    std::vector<double>  TPCModuleImpl:: calculateGlobalExtentCartesianPolar( std::vector<double> localExtent )
    {
	// this is comparatively esasy:
	// convert the edge coordinates to global coordinates 
	// add centreX+r, centreY+r, centreX-r and centreY-r to the list of test candidates
	//   only if they are in the active sector
	// find the minimum and maximum
	Vector2D firstEdge = localToGlobal(localExtent[0],localExtent[2] );// rMin, phiMin
	
	double xMin=firstEdge[0];
	double xMax=firstEdge[0];
	double yMin=firstEdge[1];
	double yMax=firstEdge[1];
	
	Vector2D edges[3]; // the three other edges (combinations if r/y , min/max)
	edges[0] = localToGlobal(localExtent[0],localExtent[3] ); // rMin, phiMax
	edges[1] = localToGlobal(localExtent[1],localExtent[2] ); // rMax, phiMin
	edges[2] = localToGlobal(localExtent[1],localExtent[3] ); // rMax, phiMax
	
	// look for maxima and minima
	for (unsigned int i = 0; i < 3 ; i++)
	{
	    if (xMin > edges[i][0]) xMin =  edges[i][0];
	    if (xMax < edges[i][0]) xMax =  edges[i][0];
	    if (yMin > edges[i][1]) yMin =  edges[i][1];
	    if (yMax < edges[i][1]) yMax =  edges[i][1];
	}

	// test the possible new yMax
	// Note: one cannot use _offset directly because the x coordinate might be 
	// inverted for negative half TPC. Just let loicalToGlobal take care of it.
	Vector2D localOriginInGlobalCoordinates = localToGlobal( 0 , 0 );
	double testYMax = localOriginInGlobalCoordinates[1] + localExtent[1]; // module centre + rMax
	double localPhi1 = globalToLocal( localOriginInGlobalCoordinates[0], testYMax )[1];
	// test for [0:2pi] and [-2pi:0]
	double localPhi2 ;
	if ( localPhi1 < 0 )
	    localPhi2 = localPhi1 + 2*M_PI;
	else
	    localPhi2 = localPhi1 - 2*M_PI;
	// change yMax if necessary
	if (  ( (localPhi1 > localExtent[2]) && (localPhi1 < localExtent[3]) ) ||
	       ( (localPhi2 > localExtent[2]) && (localPhi2 < localExtent[3]) )    )
	{   // testYMax is in active sector, so it is automatically larger than the 
	    // yMax of the edges
	    yMax = testYMax;
	}

	// now test the other max extents of the circle

	// test the possible new yMin
	double testYMin = localOriginInGlobalCoordinates[1] - localExtent[1]; // module centre - rMax
	localPhi1 = globalToLocal( localOriginInGlobalCoordinates[0], testYMin )[1];
	// std::cout <<"#DEBUG localPhi1 for ymin "<< localPhi1<<std::endl;
	// test for [0:2pi] and [-2pi:0]
	if ( localPhi1 < 0 )
	    localPhi2 = localPhi1 + 2*M_PI;
	else
	    localPhi2 = localPhi1 - 2*M_PI;
	//std::cout <<"#DEBUG localPhi2 for ymin "<< localPhi2<<std::endl;

	// change yMin if necessary
	if (  ( (localPhi1 > localExtent[2]) && (localPhi1 < localExtent[3]) ) ||
	       ( (localPhi2 > localExtent[2]) && (localPhi2 < localExtent[3]) )    )
	{   // testYMin is in active sector, so it is automatically larger than the 
	    // yMax of the edges
	    yMin = testYMin;
	}

	// test tho possible new xMax
	double testXMax = localOriginInGlobalCoordinates[0] + localExtent[1]; // module centre + rMax
	localPhi1 = globalToLocal( testXMax, localOriginInGlobalCoordinates[1] )[1];
	// test for [0:2pi] and [-2pi:0]
	if ( localPhi1 < 0 )
	    localPhi2 = localPhi1 + 2*M_PI;
	else
	    localPhi2 = localPhi1 - 2*M_PI;
	// change yMax if necessary
	if (  ( (localPhi1 > localExtent[2]) && (localPhi1 < localExtent[3]) ) ||
	       ( (localPhi2 > localExtent[2]) && (localPhi2 < localExtent[3]) )    )
	{   // testXMax is in active sector, so it is automatically larger than the 
	    // yMax of the edges
	    xMax = testXMax;
	}

	// test the possible new xMin
	double testXMin = localOriginInGlobalCoordinates[0] - localExtent[1]; // module centre - rMax
	localPhi1 = globalToLocal( testXMin, localOriginInGlobalCoordinates[1] )[1];
	// test for [0:2pi] and [-2pi:0]
	if ( localPhi1 < 0 )
	    localPhi2 = localPhi1 + 2*M_PI;
	else
	    localPhi2 = localPhi1 - 2*M_PI;
	// change yMax if necessary
	if (  ( (localPhi1 > localExtent[2]) && (localPhi1 < localExtent[3]) ) ||
	       ( (localPhi2 > localExtent[2]) && (localPhi2 < localExtent[3]) )    )
	{   // testYMax is in active sector, so it is automatically larger than the 
	    // yMax of the edges
	    xMin = testXMin;
	}

	std::vector<double> globalExtent;
	globalExtent.push_back(xMin);
	globalExtent.push_back(xMax);
	globalExtent.push_back(yMin);
	globalExtent.push_back(yMax);

	//std::cout << "#DEBUG global extent "<< xMin <<"\t" << xMax <<"\t" 
	//	  << yMin <<"\t" << yMax <<std::endl;

	return globalExtent;
    }

    std::vector<double>  TPCModuleImpl:: calculateGlobalExtentPolarPolar( std::vector<double> localExtent )
    {
	// first check if the two origins are identical (to avoid divide by zero)
	if (_offset[0]==0) // r is 0, no offset
	{
	    double phiMin, phiMax;

	    if ( _zPosition > 0 )
	    {
	      phiMin = localExtent[2] + _angle;
	      phiMax = localExtent[3] + _angle;
	    }
	    else // Negative half TPC, the angles are flipped at the y axis.
	    {    // This means the phi_minus is pi -  phi_minus and max and min are exchanged
	      phiMin = M_PI - localExtent[3] - _angle;
	      phiMax = M_PI - localExtent[2] - _angle;
	    }

	    // force phiMax to 0 .. 2*M_PI
	    while (phiMax > 2*M_PI)  phiMax -= 2*M_PI;
	    while (phiMax <  0     )  phiMax += 2*M_PI;
	    
	    // force phiMin to be smaller than phiMax, but max 
	    while ( phiMin > phiMax          )  phiMin -= 2*M_PI;
	    while ( phiMin < phiMax - 2*M_PI )  phiMin += 2*M_PI;

	    std::vector<double> globalExtent;
	    globalExtent.push_back(localExtent[0]);
	    globalExtent.push_back(localExtent[1]);
	    globalExtent.push_back(phiMin);
	    globalExtent.push_back(phiMax);
	    
	    return globalExtent;
	    
	}
	else // there is an offset
	{
	    // The potential global rmin and rmax are always on the straight line connecting 
	    // the global and the local origins. rMin is in the direction of the global origin,
	    // rMax is in the oposite direction

	    // Get coordinates of global origin in local coordinates
	    Vector2D globalOriginInLocal = globalToLocal( 0 , 0 );

	    // The vectors of the four edges in global coordinates
	    // needed either for rMin/Max or phiMin/Max
	    Vector2D edges_global[4];
	    edges_global[0] = localToGlobal(localExtent[0],localExtent[2] );// rMin, phiMin
	    edges_global[1] = localToGlobal(localExtent[0],localExtent[3] );// rMin, phiMax
	    edges_global[2] = localToGlobal(localExtent[1],localExtent[2] );// rMax, phiMin
	    edges_global[3] = localToGlobal(localExtent[1],localExtent[3] );// rMax, phiMax

	    for (int i=0; i<4; i++)
	    {
		//std::cout <<"#edges_global["<<i<<"] = "<< edges_global[i][0] <<"\t"
		//     << edges_global[i][1]<< std::endl;
	    }

	    // test if rMin direction is in pad plane
	    // test for [0:2pi] and [-2pi:0]
	    double phiTest1 =  fmod(globalOriginInLocal[1], 2*M_PI);
	    double phiTest2;
	    if ( phiTest1 < 0 )
		phiTest2 = phiTest1 + 2*M_PI;
	    else
		phiTest2 = phiTest1 - 2*M_PI;
	    
	    double rMin;
	    // one of the two has to be in the active area for this to be the real rMin
	    if ( ( (phiTest1 > localExtent[2]) && (phiTest1 < localExtent[3]) ) ||
		 ( (phiTest2 > localExtent[2]) && (phiTest2 < localExtent[3]) ) )
	    {

		//std::cout << "#rmin in active area"<< std::endl;
		// Three cases:
		// distance to center < rmin
		// rmin < distance to center < rmax
		// distance to center > rmax

		if ( globalOriginInLocal[0] < localExtent[0] ) // distance to center < rmin
		{
		    rMin =  localExtent[0] - globalOriginInLocal[0] ;
		}
		else
		if ( globalOriginInLocal[0] > localExtent[1] ) // distance to center > rmax
		{
		    rMin =  globalOriginInLocal[0] - localExtent[1];
		}
		else // rmin < distance to center < rmax // is inside the active area
		    rMin = 0; 
	    }
	    else // not the active area, test the four edges
	    {
		//std::cout << "#rmin not in active area"<< std::endl;
		rMin = edges_global[0][0];
		//std::cout << "#rMin = "<< rMin<< std::endl;

		for (int i=1; i <4 ; i++)
		{
		    if ( edges_global[i][0] < rMin ) rMin = edges_global[i][0];
		}
		//std::cout << "#rMin = "<< rMin<< std::endl;

		// there is a special case: if r > rMin
		// it might be that one of the sector edges causes the minimal global r
		if ( globalOriginInLocal[0] > localExtent[0] )
		{
		    //std::cout << "# special case" << std::endl;
		    // calculate the foot of the line perpendicular to the sector edge which goes 
		    // through the global origin (note: this does not have to be in the range between
		    // the radii. In this case the edges are limiting)

		    // use local coordinates
		    // the phiMin edge
		    double phiMinEdge = localExtent[2];
		    double phi= globalOriginInLocal[1];
		    double r =  globalOriginInLocal[0];
   // determine r_intersect for x = r_intersect * cos(_phiMinEdge) = r* cos(phi) - m * sin(_phiMinEdge)
   //                           y = r_intersect * sin(_phiMinEdge) = r*sin(phi) + m * cos(_phiMinEdge)
   // solve this to r_intersect and m
		    double r_intersect_phiMin = r * (cos(phi)*cos(phiMinEdge) + sin(phi)*sin(phiMinEdge));
		    double m_phiMin =  fabs (r * (cos(phi)*sin(phiMinEdge) - sin(phi)*cos(phiMinEdge)));
		    // one can take the abs here, it has been checked that we are outside the active area before

		    //std::cout << "#r_intersect_phiMin "  << r_intersect_phiMin << std::endl;
		    //std::cout << "#m_phiMin "  << m_phiMin << std::endl;

		    if ( (r_intersect_phiMin > localExtent[0] ) && 
			 (r_intersect_phiMin < localExtent[1] ) &&
			 (m_phiMin < rMin) )
		    {
			//std::cout << "beep"<< std::endl;
			rMin = m_phiMin;
		    }

		    // the phiMax edge
		    double phiMaxEdge = localExtent[3 ];
   // determine r_intersect for x = r_intersect * cos(_phiMaxEdge) = r* cos(phi) - m * sin(_phiMaxEdge)
   //                           y = r_intersect * sin(_phiMaxEdge) = r*sin(phi) + m * cos(_phiMaxEdge)
   // solve this to r_intersect and m
		    double r_intersect_phiMax = r * (cos(phi)*cos(phiMaxEdge) + sin(phi)*sin(phiMaxEdge));
		    double m_phiMax =  fabs (r * (cos(phi)*sin(phiMaxEdge) - sin(phi)*cos(phiMaxEdge)));
		    // one can take the abs here, it has been checked that we are outside the active area before

		    //std::cout << "#r_intersect_phiMax "  << r_intersect_phiMax << std::endl;
		    //std::cout << "#m_phiMax "  << m_phiMax << std::endl;


		    if ( (r_intersect_phiMax > localExtent[0] ) && 
			 (r_intersect_phiMax < localExtent[1] ) &&
			 (m_phiMax < rMin) )
		    {
			//std::cout << "baap"<< std::endl;
			rMin = m_phiMax;
		    }
		}
		//std::cout << "#rMin = "<< rMin<< std::endl;
	    }

	    // now rMax
	    // test for [0:2pi] and [-2pi:0], the direction opposite of the origin
	    double phiTest3 =  fmod(globalOriginInLocal[1] + M_PI, 2*M_PI);
	    double phiTest4;
	    if ( phiTest3 < 0 )
		phiTest4 = phiTest3 + 2*M_PI;
	    else
		phiTest4 = phiTest3 - 2*M_PI;
				
	    double rMax;
	    // one of the two has to be in the active area for this to be the real rMax
	    if ( ( (phiTest3 > localExtent[2]) && (phiTest3 < localExtent[3]) ) ||
		 ( (phiTest4 > localExtent[2]) && (phiTest4 < localExtent[3]) ) )
	    {
		//std::cout << "#rmax in active area"<< std::endl;
		// rMax is the sum of local rMax and the distance to the origin
		rMax=  globalOriginInLocal[0] + localExtent[1];
	    }
	    else // not the active area, test the four edges
	    {
		//std::cout << "#rmax not in active area"<< std::endl;
		rMax = edges_global[0][0];

		for (int i=1; i <4 ; i++)
		{
		    if ( edges_global[i][0] > rMax ) rMax = edges_global[i][0];
		}
	    }

	    // phiMin and phiMax

	    double phiMin, phiMax;

	    // r > rMax
	    // If the global origin is outside of local rMax, the tangents on the rMax-circle are
	    // phiMin and phiMax in case of a full circle. 
	    // In case the module is only a circle segment, the edged can define phi min
            // and/or phi max. In any case they have to be within the angles defined by
	    // the tangents. The max angle between the tangents ca be pi
	    // => wrap all angles to ( phi_tangent_min -- phi_tangent_min + pi ),
	    // so one can compare them (which in generell is not possible as the angles
	    // are only defined to modulo 2pi )

	    
	    if ( globalOriginInLocal[0] >= localExtent[1] )
	    {
	        // Note: one cannot use _offset directly because the x coordinate might be 
	        // inverted for negative half TPC. Just let loicalToGlobal take care of it.
	        Vector2D localOriginInGlobalCoordinates = localToGlobal( 0 , 0 );

		// calclulate the two tangents to the circle: (see TPCModule_tangent_rMax.pdf )
		double phi_tangent_min = localOriginInGlobalCoordinates[1]  - asin( localExtent[1] / globalOriginInLocal[0] );
		double phi_tangent_max = localOriginInGlobalCoordinates[1]  + asin( localExtent[1] / globalOriginInLocal[0] );

		//std::cout << "#localOriginInGlobalCoordinates " << globalOriginInLocal[0] << "\t"<<  localOriginInGlobalCoordinates[1]<< std:: endl;
		double angles[4];

		// force the angles of the edges into the range phi_tangent_min -- phi_tangent_min + 2*pi
		for (int i=0; i < 4; i++)
		{
		    angles[i] = edges_global[i][1];
		    while ( angles[i] < phi_tangent_min )
			angles[i] += 2*M_PI;
		    while ( angles[i] > phi_tangent_min + 2*M_PI)
			angles[i] -= 2*M_PI;
		}

		// check whether phi_tangent_min and phi_tangent_max are in the active area
		
		// Force local angle where the tangent touches the circle to phiMin -- phiMin+2pi.
		// In case of negative half TPC min and max are exchanged since globalToLocal
		// flips the x axis (= angle)
		double phiTestMin = globalToLocal( sqrt(localOriginInGlobalCoordinates[0]*
							localOriginInGlobalCoordinates[0] 
							- localExtent[1] * localExtent[1]),
						   ( _zPosition > 0 ? phi_tangent_min 
						                    : phi_tangent_max    )  )[1];

		//std::cout << "#phiTestMin "<< phiTestMin << std::endl;
		//std::cout << "#phi_tangen_min "<< phi_tangent_min << std::endl;
		while ( phiTestMin < localExtent[2] )
		     phiTestMin += 2*M_PI;
		while ( phiTestMin > localExtent[2] + 2*M_PI)
		    phiTestMin  -= 2*M_PI;
		//std::cout << "#phiTestMin "<< phiTestMin << std::endl;

		// test whether in active area
		if ( phiTestMin < localExtent[3] )
		{
		  // The phiTestMin is the minimum in local coordinates.
		  // For the negative half TPC it is the global maximum because the
		  // x coordinate is swapped
		  if ( _zPosition > 0 )
		  {
		    phiMin = phi_tangent_min;
		  }
		  else
		  {
		    phiMax = phi_tangent_max ;
		  }
		}
		else // not in active area, test edges
		{
		  if ( _zPosition > 0 )
		  {
		    phiMin = angles[0];
		    
		    for (int i=1; i <4 ; i++)
		    {
			if ( angles[i] < phiMin ) phiMin = angles[i];
		    }
		  }
		  else // _zPosition <= 0
		  {
		    phiMax = angles[0];
		    
		    for (int i=1; i <4 ; i++)
		    {
			if ( angles[i] > phiMax ) phiMax = angles[i];
		    } // for i (angles)
		  }// else _zPositizion > 0
		    
		}// else not in active area

		//std::cout << "#phi_tangen_max "<< phi_tangent_max << std::endl;

		// Force local angle where the tangent touches the circle to phiMin -- phiMin+2pi
		// In case of negative half TPC min and max are exchanged since globalToLocal
		// flips the x axis (= angle)
		double phiTestMax = globalToLocal( sqrt(localOriginInGlobalCoordinates[0]*
							localOriginInGlobalCoordinates[0] 
							- localExtent[1] * localExtent[1]),
						   ( _zPosition > 0 ? phi_tangent_max 
						                    : phi_tangent_min    )  )[1];
		//std::cout << "#phiTestMax "<< phiTestMax << std::endl;
		// thest whether in active area
		while ( phiTestMax < localExtent[2] )
		     phiTestMax += 2*M_PI;
		while ( phiTestMax > localExtent[2] + 2*M_PI)
		    phiTestMax  -= 2*M_PI;


		// thest whether in active area
		if ( phiTestMax < localExtent[3] )
		{
		  // The phiTestMax is the maximum in local coordinates.
		  // For the negative half TPC it is the global minimum because the
		  // x coordinate is swapped
		  if ( _zPosition > 0 )
		  {
		    phiMax = phi_tangent_max;
		  }
		  else
		  {
		    phiMin = phi_tangent_min;
		  }
		}
		else // not in active area, test edges
		{
		  if ( _zPosition > 0 )
		  {
		    phiMax = angles[0];
		    
		    for (int i=1; i <4 ; i++)
		    {
			if ( angles[i] > phiMax ) phiMax = angles[i];
		    }
		  }
		  else
		  {
		    phiMin = angles[0];
		    
		    for (int i=1; i <4 ; i++)
		    {
			if ( angles[i] < phiMin ) phiMin = angles[i];
		    } // for i (angles)
		  }// else zPosition > 0 

		} // else not in active area


	    }
	    else // ( r < rMax )
	    {

		// There are may different cases ( in local coordinates ) // note r always < rMax in this section 
		
		// A: The active sector is < pi (sector 1 is active)
		// A1a. phi of global origin is in the active sector (1) and r <= rMin : phiMinG = (rMin,phiMin), phiMaxG = (rMin, phiMax)
		// A1b. phi of global origin is in the active sector (1) and rMin< r : phiMinG = 0, phiMaxG= 2 pi
		// A2. phi of the global origin is in sector 2 : phiMax is (rmax, phiMax), phiMin is min of (rMin, phiMax) and (rMin, phiMin)
		//            wrap to phiMax - 2pi first, phiMax is the fixed one in this case
		//     (( there is only a small triange near the edge for r > rMin where (rMin, phiMax) is phiMin, but it's difficult to calculate, 
		//        so we just take the min of both possibilities
		// A3. phi is in sector 3 : (rMax,phiMin), (rMax, phiMax)
		// A4. phi of the global origin is in sector 4 : phiMin is (rMax, phiMin), phiMax is max of (rMin, phiMax) and (rMin, phiMin)
		//            wrap to phiMin + 2pi first, phiMin is the fixed one in this case (see 2 for argumentation)
		
 /*
            /--------\
	   / \______/ \
          /  /\ 1  /\  \
	 /  /  \  /  \  \
         |  |   \/   |  |
         |  | 2 /\  4|  |
	 \  \  / 3\  /  /
          \  \/____\/  /
           \ /      \ /            
            \--------/
*/
		// wrap phi of global origin to phiMin -- phiMin + 2pi
		double phiGlobalOrigin = globalOriginInLocal[1];
		while ( phiGlobalOrigin < localExtent[2] )
		    phiGlobalOrigin += 2*M_PI;
		while ( phiGlobalOrigin > localExtent[2] + 2 *M_PI )
		    phiGlobalOrigin -= 2*M_PI;
	    
		//A
		if (localExtent[3] - localExtent[2] < M_PI)
		{
		    if ( phiGlobalOrigin < localExtent[3] ) // A1
		    {
			if ( globalOriginInLocal[0] <= localExtent[0] ) // A1a
			{
			    if ( _zPosition > 0 )
			    {
			      phiMin =  edges_global[0][1];
			      phiMax =  edges_global[1][1];
			    }
			    else //negative half TPC. Due to the flip in x min and max are exchanged
			    {
			      phiMin =  edges_global[1][1];
			      phiMax =  edges_global[0][1];
			    }

			    // due to the %2pi problem phiMax might be smaller than phiMin
			    // force phiMin to be smaller
			    while ( phiMin > phiMax)
				phiMin -= 2*M_PI;
			    
			    // force to +-2pi
			    while (phiMax > 2*M_PI)
			    {
				phiMin -= 2*M_PI;
				phiMax -= 2*M_PI;
			    }
			    while (phiMin < -2*M_PI)
			    {
				phiMin += 2*M_PI;
				phiMax += 2*M_PI;
			    }
			}
			else //A1b
			{
			    phiMin=0; phiMax=2*M_PI;
			}
		    }
		    else if ( phiGlobalOrigin < localExtent[2] + M_PI) // A2
		    {
			
		      // due to the many comparisons, which all change sign when 
		      // inverting x, the code unfortunately has to be duplicated :-(
		      if ( _zPosition > 0 )
		      {
			phiMax = edges_global[3][1];

			double phiMin1 = edges_global[0][1];
			while ( phiMin1 < phiMax - 2*M_PI )
			    phiMin1 += 2*M_PI;
			while ( phiMin1 > phiMax )
			    phiMin1 -= 2*M_PI;
			
			double phiMin2 = edges_global[1][1];
			while ( phiMin2 < phiMax - 2*M_PI )
			    phiMin2 += 2*M_PI;
			while ( phiMin2 > phiMax )
			    phiMin2 -= 2*M_PI;
			
			phiMin = (phiMin1 < phiMin2 ? phiMin1 : phiMin2);
		      }
		      else // negative half TPC
		      {
			phiMin = edges_global[3][1];

			double phiMax1 = edges_global[0][1];
			while ( phiMax1 > phiMin + 2*M_PI )
			    phiMax1 -= 2*M_PI;
			while ( phiMax1 < phiMin )
			    phiMax1 += 2*M_PI;

			double phiMax2 = edges_global[1][1];
			while ( phiMax2 > phiMin + 2*M_PI )
			    phiMax2 -= 2*M_PI;
			while ( phiMax2 < phiMin )
			    phiMax2 += 2*M_PI;
			
			phiMax = (phiMax1 > phiMax2 ? phiMax1 : phiMax2);
		      }
		    }
		    else if ( phiGlobalOrigin < localExtent[3] + M_PI) // A3
		    {
			if ( _zPosition > 0 )
			{
			    phiMin =  edges_global[2][1];
			    phiMax =  edges_global[3][1];			
			}
			else // negative half TPC
			{
			    phiMin =  edges_global[3][1];
			    phiMax =  edges_global[2][1];			
			}

			// due to the %2pi problem phiMax might be smaller than phiMin
			// force phiMin to be smaller
			while ( phiMin > phiMax)
			    phiMin -= 2*M_PI;
			
			// force to +-2pi
			while (phiMax > 2*M_PI)
			{
			    phiMin -= 2*M_PI;
			    phiMax -= 2*M_PI;
			}
			while (phiMin < -2*M_PI)
			{
			    phiMin += 2*M_PI;
			    phiMax += 2*M_PI;
			}
		    }
		    else // A4
		    {
		      if ( _zPosition > 0 )
		      {
			phiMin = edges_global[2][1];

			double phiMax1 = edges_global[0][1];
			while ( phiMax1 < phiMin )
			    phiMax1 += 2*M_PI;
			while ( phiMax1 > phiMin + 2*M_PI )
			    phiMax1 -= 2*M_PI;
			
			double phiMax2 = edges_global[1][1];
			while ( phiMax2 < phiMin )
			    phiMax2 += 2*M_PI;
			while ( phiMax2 > phiMin + 2*M_PI )
			    phiMax2 -= 2*M_PI;
			
			phiMax = (phiMax1 > phiMax2 ? phiMax1 : phiMax2);
		      }
		      else
		      {
			phiMax = edges_global[2][1];

			double phiMin1 = edges_global[0][1];
			while ( phiMin1 < phiMax - 2*M_PI )
			    phiMin1 += 2*M_PI;
			while ( phiMin1 > phiMax )
			    phiMin1 -= 2*M_PI;
			
			double phiMin2 = edges_global[1][1];
			while ( phiMin2 < phiMax - 2*M_PI )
			    phiMin2 += 2*M_PI;
			while ( phiMin2 > phiMax )
			    phiMin2 -= 2*M_PI;
			
			phiMin = (phiMin1 < phiMin2 ? phiMin1 : phiMin2);
		      }
		    }// scan sectors
		}
		else //active area > pi
		{// B
		    if ( phiGlobalOrigin > localExtent[3] ) // B4
		    {
		      if ( _zPosition > 0 )
		      {
			phiMin =  edges_global[2][1];
			phiMax =  edges_global[3][1];
		      }
		      else
		      {
			phiMin =  edges_global[3][1];
			phiMax =  edges_global[2][1];
		      }
			    // due to the %2pi problem phiMax might be smaller than phiMin
			    // force phiMin to be smaller
			    while ( phiMin > phiMax)
				phiMin -= 2*M_PI;
			    
			    // force to +-2pi
			    while (phiMax > 2*M_PI)
			    {
				phiMin -= 2*M_PI;
				phiMax -= 2*M_PI;
			    }
			    while (phiMin < -2*M_PI)
			    {
				phiMin += 2*M_PI;
				phiMax += 2*M_PI;
			    }
		    }
		    else
		    {
			if ( globalOriginInLocal[0] <= localExtent[0] ) // only perfrom further checks
			    // if outside active area
			{
			    if ( phiGlobalOrigin < localExtent[3] - M_PI ) // B1
			    {
			      if ( _zPosition > 0 )
			      {
				phiMin =  edges_global[0][1]; 
				phiMax =  edges_global[3][1];
			      }
			      else
			      {
				phiMin =  edges_global[3][1]; 
				phiMax =  edges_global[0][1];
			      }

				// due to the %2pi problem phiMax might be smaller than phiMin
				// force phiMin to be smaller
				while ( phiMin > phiMax)
				    phiMin -= 2*M_PI;

				// phiMax has to be more than pi larger than phiMin
				// there is a spechial case when there is no free line
				// of sight from the global origin to the outside of the
				// circle. In this case phiMax is a little bit more than
				// 2pi larger than phiMin, which in this case (%2pi) means
				// phiMax - phiMin < pi
				if ( phiMax - phiMin < M_PI )
				{
				    phiMin = 0;
				    phiMax = 2*M_PI;
				}
				else // force to +-2pi
				{
				    while (phiMax > 2*M_PI)
				    {
					phiMin -= 2*M_PI;
					phiMax -= 2*M_PI;
				    }
				    while (phiMin < -2*M_PI)
				    {
					phiMin += 2*M_PI;
					phiMax += 2*M_PI;
				    }
				}
			    }
			    else if ( phiGlobalOrigin < localExtent[2] + M_PI) // B2
			    {
			      if (_zPosition > 0 )
			      {
				phiMin =  edges_global[0][1]; 
				phiMax =  edges_global[1][1];
			      }
			      else
			      {
				phiMin =  edges_global[1][1]; 
				phiMax =  edges_global[0][1];
			      }
				while ( phiMin > phiMax)
				    phiMin -= 2*M_PI;

				 // force to +-2pi
				while (phiMax > 2*M_PI)
				{
				    phiMin -= 2*M_PI;
				    phiMax -= 2*M_PI;
				}
				while (phiMin < -2*M_PI)
				{
				    phiMin += 2*M_PI;
				    phiMax += 2*M_PI;
				}
			    }
			    else if ( phiGlobalOrigin < localExtent[3] + M_PI) // B3
			    {
			      if ( _zPosition > 0 )
			      {
				phiMin =  edges_global[2][1];
				phiMax =  edges_global[1][1];			
			      }
			      else
			      {
				phiMin =  edges_global[1][1];
				phiMax =  edges_global[2][1];			
			      }
				// due to the %2pi problem phiMax might be smaller than phiMin
				// force phiMin to be smaller
				while ( phiMin > phiMax)
				    phiMin -= 2*M_PI;

				// phiMax has to be more than pi larger than phiMin
				// there is a spechial case when there is no free line
				// of sight from the global origin to the outside of the
				// circle. In this case phiMax is a little bit more than
				// 2pi larger than phiMin, which in this case (%2pi) means
				// phiMax - phiMin < pi
				if ( phiMax - phiMin < M_PI )
				{
				    phiMin = 0;
				    phiMax = 2*M_PI;
				}
				else // force to +-2pi
				{
				    while (phiMax > 2*M_PI)
				    {
					phiMin -= 2*M_PI;
					phiMax -= 2*M_PI;
				    }
				    while (phiMin < -2*M_PI)
				    {
					phiMin += 2*M_PI;
					phiMax += 2*M_PI;
				    }
				}
			    }
			}
			else // in active area
			{
			    phiMin = 0;
			    phiMax = 2*M_PI;
			}
		    }// else (not in open sector

		} // else active area > pi

	    }// else (r < rMax)
	    

	    std::vector<double> globalExtent;
	    globalExtent.push_back(rMin);
	    globalExtent.push_back(rMax);
	    globalExtent.push_back(phiMin);
	    globalExtent.push_back(phiMax);

	    //std::cout << "#global extent "<< rMin <<"\t" << rMax <<"\t" 
	    //	      << phiMin <<"\t" << phiMax <<std::endl;
	
	    return globalExtent;
	}// else there is an offset
    }

    TPCModuleImpl::TPCModuleImpl(int moduleID, PadRowLayout2D *padRowLayout,int TPCCoordinateType,
				 double readoutFrequency)
    {
	  _moduleID = moduleID;
	  _readoutFrequency= readoutFrequency;
	  _padRowLayout= padRowLayout;
	  _momsCoordinateType =TPCCoordinateType;
	  _border= 0;
	  // set correct sizes for extent vectors, so it's save to use extent[i]
	  _planeExtent.resize(4);
	  _moduleExtent.resize(4);
	  _localModuleExtent.resize(4);

	  // as no offsets, angle and border are applied, all extents are identical
	  _planeExtent=_padRowLayout->getPlaneExtent();
	  _moduleExtent = _planeExtent; 
	  _localModuleExtent = _moduleExtent;

	  _offset[0]=0;
	  _offset[1]=0;
	  _offset_cartesian[0]=0;
	  _offset_cartesian[1]=0;
	  _zPosition=1.; // Set the z position to a positive value so no x-swapping
	                 // is done in the default case.
	                 // This value is not accessible from outside the class sice 
	                 // an exceptions is thrown if it has not been overwritten.
	  _angle =0 ;
	  _cos_angle = 1;
	  _sin_angle = 0;

	  _zPositionIsSet = false;

	  checkLocalIsGlobal();
    }

    TPCModuleImpl::TPCModuleImpl(const TPCModuleImpl &right)
    {
	copy_and_assign( right);
    }

    TPCModuleImpl & TPCModuleImpl::operator = (const TPCModuleImpl &right)
    {
	cleanup();
	copy_and_assign( right);
	return *this;
    }

    void TPCModuleImpl::copy_and_assign(const  TPCModuleImpl & right)
    {
	_planeExtent  = right._planeExtent;
	_moduleExtent = right._moduleExtent;
	_localModuleExtent = right._localModuleExtent;
	_readoutFrequency = right._readoutFrequency;
	_offset = right._offset;
	_offset_cartesian = right._offset_cartesian;
	_angle = right._angle;
	_cos_angle = right._cos_angle;
	_sin_angle = right._sin_angle;
	_momsCoordinateType = right._momsCoordinateType;
	_moduleID = right._moduleID;
	_border = right._border;
	_localIsGlobal = right._localIsGlobal;

	_zPosition = right._zPosition;
	_zPositionIsSet = right._zPositionIsSet;

	// test all knows possible instances of padRowLayout
	_padRowLayout = right._padRowLayout->clone();
    }


    TPCModuleImpl::~TPCModuleImpl() {
	cleanup();
    }

    PadRowLayout2D* TPCModuleImpl::clone() const
    {
	return new TPCModuleImpl(*this);
    }

    void TPCModuleImpl::cleanup(){
	delete _padRowLayout;
    }

    
    gear::Vector2D TPCModuleImpl::getPadCenter(int padIndex) const {
	Vector2D localCenter = _padRowLayout->getPadCenter(padIndex);
	return localToGlobal( localCenter[0],  localCenter[1] );
    }

  int TPCModuleImpl::getPadLayoutType() const 
  {
//      std::cerr << "FixedDiskLayoutBase::getPadLayoutType() : Warning: " <<std::endl
//		<< "  This is deprecated (ambiguous for polar coordinate systems)"<< std::endl
//		<< "  Please use getCoordinateType() or getPadLayoutImplType() "<< std::endl;
      return getCoordinateType();
  } 

  int TPCModuleImpl::getPadLayoutImplType() const 
  {
      return PadRowLayout2D::TPCMODULE;
  }

    int TPCModuleImpl::getCoordinateType() const
    {
	return _momsCoordinateType;
    }

    const std::vector<double>& TPCModuleImpl::getPlaneExtent()  const throw (gear::Exception, std::exception){
	return _planeExtent;	
    }

    int TPCModuleImpl::getNearestPad(double x, double y)  const{
	Vector2D temp = globalToLocal(x,y);
	return _padRowLayout->getNearestPad(temp[0],temp[1]);
    }

    bool TPCModuleImpl::isInsidePad(double c0, double c1, int padIndex)  const{
	Vector2D temp = globalToLocal(c0,c1);
	return _padRowLayout->isInsidePad(temp[0],temp[1],padIndex);
    }

    bool TPCModuleImpl::isInsidePad(double c0, double c1)  const{
	Vector2D temp = globalToLocal(c0,c1);

	switch (_padRowLayout->getCoordinateType())
	{
	    case PadRowLayout2D::CARTESIAN :
		// don't do anything
		break;
	    case PadRowLayout2D::POLAR :
		while ( temp[1] < _localModuleExtent[2] )
		    temp[1] += 2*M_PI;
		while ( temp[1] >= _localModuleExtent[2] + 2*M_PI )
		    temp[1] -= 2*M_PI;
	}

	int nearestPad = _padRowLayout->getNearestPad(temp[0],temp[1]);
	return _padRowLayout->isInsidePad(temp[0],temp[1],nearestPad);
    }

    bool TPCModuleImpl::isInsideModule(double c0, double c1) const{
	// first convert to local coordinates
	Vector2D localC = globalToLocal(c0, c1);

	// wrap angle to phiMin -- phiMin + 2pi for polar coordinates
	switch (_padRowLayout->getCoordinateType())
	{
	    case PadRowLayout2D::CARTESIAN :
		// don't do anything
		break;
	    case PadRowLayout2D::POLAR :
		while ( localC[1] < _localModuleExtent[2] )
		    localC[1] += 2*M_PI;
		while ( localC[1] >= _localModuleExtent[2] + 2*M_PI )
		    localC[1] -= 2*M_PI;
	}

	// if inside the local module extent it is inside the module
	if (localC[0] >=_localModuleExtent[0] && localC[0] <=_localModuleExtent[1] &&
	    localC[1] >=_localModuleExtent[2] && localC[1] <=_localModuleExtent[3] )
	{
	    return true;
	}
	else
	    return false;
    }

    double TPCModuleImpl::getDistanceToPad(double c0, double c1, int padIndex) const{
	Vector2D localCoordinates = globalToLocal(c0,c1);
	return _padRowLayout->getDistanceToPad(localCoordinates[0],localCoordinates[1], padIndex);

    } 

    double TPCModuleImpl::getDistanceToModule(double c0, double c1) const{
	// first convert to local coordinates
	Vector2D localC = globalToLocal(c0, c1);

	// wrap angle to phiMin -- phiMin + 2pi for polar coordinates
	switch (_padRowLayout->getCoordinateType())
	{
	    case PadRowLayout2D::CARTESIAN :
		// don't do anything
		break;
	    case PadRowLayout2D::POLAR :
		while ( localC[1] < _localModuleExtent[2] )
		    localC[1] += 2*M_PI;
		while ( localC[1] >= _localModuleExtent[2] + 2*M_PI )
		    localC[1] -= 2*M_PI;
	}

	// The easy part which works for both polar and cartesian coordinates.
	// If localC[1] is in the active rage (y positon or sector) it is the distance in localC[0]

	if (localC[1] >= _localModuleExtent[2] && localC[1] <= _localModuleExtent[3])
	{
	    if (localC[0] < _localModuleExtent[0]) 
	    {
		return (_localModuleExtent[0] - localC[0]);
	    }
	    else if (localC[0] <= _localModuleExtent[1]) // inside the actice area
	    {
		return 0.;
	    }
	    else
	    {
		return (localC[0] - _localModuleExtent[1]);
	    }
	}
	else switch (_padRowLayout->getCoordinateType())
	{
	    case  PadRowLayout2D::CARTESIAN :
		if (localC[1] < _localModuleExtent[2])
		{
		    if (localC[0] < _localModuleExtent[0]) 
		    {
			return sqrt( (_localModuleExtent[0] - localC[0]) * (_localModuleExtent[0] - localC[0]) +
				     (_localModuleExtent[2] - localC[1]) * (_localModuleExtent[2] - localC[1]) );
		    }
		    else if (localC[0] <= _localModuleExtent[1]) // inside the actice area
		    {
			return (_localModuleExtent[2] - localC[1]);
		    }
		    else
		    {
			return sqrt( (localC[0] - _localModuleExtent[1]) * (localC[0] - _localModuleExtent[1]) +
				     (_localModuleExtent[2] - localC[1]) * (_localModuleExtent[2] - localC[1]) );
		    }
		}
		else // (localC[1] > _localModuleExtent[3])
		    if (localC[0] < _localModuleExtent[0]) 
		    {
			return sqrt( (_localModuleExtent[0] - localC[0]) * (_localModuleExtent[0] - localC[0]) +
				     (localC[1] - _localModuleExtent[3]) * (localC[1] - _localModuleExtent[3]) );
		    }
		    else if (localC[0] <= _localModuleExtent[1]) // inside the actice area
		    {
			return (localC[1] - _localModuleExtent[3]);
		    }
		    else
		    {
			return sqrt( (localC[0] - _localModuleExtent[1]) * (localC[0] - _localModuleExtent[1]) +
				     (localC[1] - _localModuleExtent[3]) * (localC[1] - _localModuleExtent[3]) );
		    }
		break;
	    case PadRowLayout2D::POLAR :
	    {
		// calculate pca to straight line along radius with phi_min;
		
		// determine r_intersect for x = r_intersect * cos(_phiMin) = r* cos(phi) - m * sin(_phiMin)
		//                           y = r_intersect * sin(_phiMin) = r*sin(phi) + m * cos(_phiMin)
		// solve this to r_intersect and m
		double r_intersect_phiMin = localC[0] * (cos(localC[1])*cos(_localModuleExtent[2]) + sin(localC[1])*sin(_localModuleExtent[2]));
		double m_phiMin =  localC[0] * (cos(localC[1])*sin(_localModuleExtent[2]) - sin(localC[1])*cos(_localModuleExtent[2]));
		
		double distance_phiMin;
		if ( r_intersect_phiMin < _localModuleExtent[0] )
		{
		    // you can check this formula by calculaing sqrt( (x-x_min)^2 - (y-y_min)^2 )
		    // where x_min = r_min cos(phi_min) etc.
		    distance_phiMin = sqrt( localC[0]*localC[0] + _localModuleExtent[0]*_localModuleExtent[0] -2 * _localModuleExtent[0] * r_intersect_phiMin );
		}
		else
		    if ( r_intersect_phiMin > _localModuleExtent[1] ) 
		    {
			distance_phiMin = sqrt( localC[0]*localC[0] + _localModuleExtent[1]*_localModuleExtent[1] 
						-2 * _localModuleExtent[1] * r_intersect_phiMin );
		    }
		    else
			distance_phiMin =  fabs( m_phiMin );
		
		
		// now the same for _phiMax
		double r_intersect_phiMax = localC[0] * (cos(localC[1])*cos(_localModuleExtent[3]) + sin(localC[1])*sin(_localModuleExtent[3]));
		double m_phiMax =  localC[0] * (cos(localC[1])*sin(_localModuleExtent[3]) - sin(localC[1])*cos(_localModuleExtent[3]));
		
		double distance_phiMax;
		if ( r_intersect_phiMax < _localModuleExtent[0] )
		{
		    // you can check this formula by calculaing sqrt( (x-x_max)^2 - (y-y_max)^2 )
		    // where x_max = r_min cos(phi_max) etc.
		    distance_phiMax = sqrt( localC[0]*localC[0] + _localModuleExtent[0]*_localModuleExtent[0] -2 * _localModuleExtent[0] * r_intersect_phiMax );
		}
		else
		    if ( r_intersect_phiMax > _localModuleExtent[1] ) 
		    {
			distance_phiMax = sqrt( localC[0]*localC[0] + _localModuleExtent[1]*_localModuleExtent[1] -2 * _localModuleExtent[1] * r_intersect_phiMax );
		    }
		    else
			distance_phiMax =  fabs( m_phiMax );
		
		return (distance_phiMin < distance_phiMax ? distance_phiMin : distance_phiMax);
	    }
		break;
	    default:
		throw gear::Exception("TPCModuleImpl::getDistanceToModule: unkown coorinate type");			
	}
	return 0.;
    }

    bool TPCModuleImpl::isOverlapping(TPCModule * testThisModule) const {
	// I don't have the slightest idea how to test whether modules are overlapping
	throw gear::NotImplementedException("TPCModuleImpl::isOverlapping: This is currently not implemented.");
	return false;
    }

    void TPCModuleImpl::setBorderWidth(double border) {
	_border = border;
	// recalculate the plane extents
	convertLocalPlaneToGlobalPlaneExtend();
    }
 
    void TPCModuleImpl::setOffset(double x_r, double y_phi)
    {
	_offset[0] = x_r;
	_offset[1] = y_phi;

	switch (_momsCoordinateType)
	{
	    case PadRowLayout2D::POLAR :
	    {
		_offset_cartesian[0] = x_r * std::cos( y_phi );
		_offset_cartesian[1] = x_r * std::sin( y_phi );
	    }
	    break;
	    case  PadRowLayout2D::CARTESIAN :
	    {
		_offset_cartesian[0] = x_r;
		_offset_cartesian[1] = y_phi;
	    }
	    break;
	    default:
		throw gear::Exception("TPCModuleImpl::globalToLocal: unkown coorinate type");	
	}
	// recalculate the plane extents
	convertLocalPlaneToGlobalPlaneExtend();
	checkLocalIsGlobal();
    }

    void TPCModuleImpl::setZPosition(double z)
    {
      _zPosition = z;
      _zPositionIsSet = true;
    }

    void TPCModuleImpl::setAngle(double angle)
    {
	_angle= angle;
	_cos_angle =  std::cos( angle );
	_sin_angle =  std::sin( angle );
	// recalculate the plane extents
	convertLocalPlaneToGlobalPlaneExtend();
	checkLocalIsGlobal();
    }
    
    void TPCModuleImpl::setReadoutFrequency(double frequency) 
    {
//	std::cout << "TPCModuleImpl: Warning: "
//		  << "deprecated use of setReadoutFreuqency()." << std::endl
//		  << "   Please define readout freuqency in constructor!"
//		  << std::endl;
	
	_readoutFrequency = frequency;
    }

    void TPCModuleImpl::checkLocalIsGlobal()
    {
      if ( (_momsCoordinateType == _padRowLayout->getCoordinateType())
	   && (_angle == 0. ) 
	   && ( _offset[0] == 0. )
	   && ( _offset[1] == 0. ) 
	   && ( _zPosition > 0 ) ) // not >= 0, 0 (prototype) is negative half TPC
      {
	_localIsGlobal = true;
      }
      else
      {
	_localIsGlobal = false;
      }
    }

  double TPCModuleImpl::getZPosition() const throw (TPCModule::NoZPositionException)
  {
    if (!_zPositionIsSet) throw TPCModule::NoZPositionException();
    
    return _zPosition;
  }
       
}// namespace gear
