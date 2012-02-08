#ifndef ICOORDINATESYSTEM_h
#define ICOORDINATESYSTEM_h

#include "CLHEP/Vector/ThreeVector.h"

namespace GearSurfaces{
  
  /** An abstract base class for coordinate systems
   */
  class ICoordinateSystem{
    
    
  public:
    
    virtual ~ICoordinateSystem() { /* no-op */ }
    
    /** @return the local coordinates of the point */
    virtual CLHEP::Hep3Vector getLocalPoint( CLHEP::Hep3Vector globalPoint ) = 0;
    
    /** @return the global coordinates of the point */
    virtual CLHEP::Hep3Vector getGlobalPoint( CLHEP::Hep3Vector localPoint ) = 0;
    
    /** @return the global coordinates of the origin of the coordinate system */
    virtual CLHEP::Hep3Vector getOrigin() = 0;
    
    
    
  };
  
} // end of GearSurfaces namespace


#endif

