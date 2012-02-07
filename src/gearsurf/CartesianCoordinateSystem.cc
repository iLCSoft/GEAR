#include "CartesianCoordinateSystem.h"

namespace GearSurfaces{
  
  CLHEP::Hep3Vector CartesianCoordinateSystem::getLocalPoint( CLHEP::Hep3Vector globalPoint ){
    
    // First we get us a new origin via translation
    CLHEP::Hep3Vector x = globalPoint  - _T;
    
    // Then we do the rotation
    CLHEP::HepRotation R_inv = _R.inverse();
    
    return R_inv*x;   
    
  }
  
  CLHEP::Hep3Vector CartesianCoordinateSystem::getGlobalPoint( CLHEP::Hep3Vector localPoint ){
    
    
    // The point in global coordinates is the place of the origin in gloabl coordinates (=T) plus
    // the local coordinates rotated into the global ones   
    return _T + _R*localPoint;
    
  }
  
} //end of GearSurfaces namespace 



