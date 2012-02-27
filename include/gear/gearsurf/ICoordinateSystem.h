#ifndef ICOORDINATESYSTEM_h
#define ICOORDINATESYSTEM_h

#include "CLHEP/Vector/ThreeVector.h"

namespace gear{
  
  /** An abstract base class for coordinate systems
   */
  class ICoordinateSystem{
    
    
  public:
    
    virtual ~ICoordinateSystem() { /* no-op */ }
    
    /** @return the local coordinates of the point */
    virtual CLHEP::Hep3Vector getLocalPoint( CLHEP::Hep3Vector globalPoint )const = 0;
    
    /** @return the global coordinates of the point */
    virtual CLHEP::Hep3Vector getGlobalPoint( CLHEP::Hep3Vector localPoint )const = 0;
    
    /** @return the global coordinates of the origin of the coordinate system */
    virtual CLHEP::Hep3Vector const* getOrigin() const = 0;
    
    
    
  };
  
} // end of gear namespace


#endif

