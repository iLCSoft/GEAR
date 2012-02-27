#include "gearsurf/CartesianCoordinateSystem.h"

namespace gear{
  
  CLHEP::Hep3Vector CartesianCoordinateSystem::getLocalPoint( CLHEP::Hep3Vector globalPoint ) const {
    
    // First we get us a new origin via translation
    CLHEP::Hep3Vector x = globalPoint  - _T;
  
    // Then we do the rotation
    return _R_inv*x;   
    
  }
  
  CLHEP::Hep3Vector CartesianCoordinateSystem::getGlobalPoint( CLHEP::Hep3Vector localPoint ) const {
    
    
    // The point in global coordinates is the place of the origin in gloabl coordinates (=T) plus
    // the local coordinates rotated into the global ones   
    return _T + _R*localPoint;
    
  }
  
} //end of gear namespace 



