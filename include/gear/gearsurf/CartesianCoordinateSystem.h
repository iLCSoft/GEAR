#ifndef CARTESIANCOORDINATESYSTEM_h
#define CARTESIANCOORDINATESYSTEM_h

#include "ICoordinateSystem.h"
#include "CLHEP/Vector/Rotation.h"


namespace gear{
  
  /** Cartesian coordinate system class */
  class CartesianCoordinateSystem : public ICoordinateSystem{
    
    
  public:
    
    CartesianCoordinateSystem( CLHEP::Hep3Vector T , CLHEP::HepRotation R ): _T(T), _R(R){}
    
    /** @return the local coordinates of the point */
    virtual CLHEP::Hep3Vector getLocalPoint( CLHEP::Hep3Vector globalPoint );
    
    /** @return the global coordinates of the point */
    virtual CLHEP::Hep3Vector getGlobalPoint( CLHEP::Hep3Vector localPoint );
    
    /** @return the global coordinates of the origin of the coordinate system */
    virtual CLHEP::Hep3Vector getOrigin(){ return _T; }
    
    /** @return a rotation Matrix. local = R*global (after the translation)
     */
    CLHEP::HepRotation getR(){ return _R;}
    
    /** @return the local x-axis in global coordinates */
    CLHEP::Hep3Vector getLocalXAxis(){ return _R * CLHEP::Hep3Vector(1,0,0); }
    CLHEP::Hep3Vector getLocalYAxis(){ return _R * CLHEP::Hep3Vector(0,1,0); }
    CLHEP::Hep3Vector getLocalZAxis(){ return _R * CLHEP::Hep3Vector(0,0,1); }
    
  private:
    
    /** The translation vector (= the origin of the Coordinate System ) */
    CLHEP::Hep3Vector _T;
    
    /** Rotation Matrix
     * Definition: global = R* local
     */
    CLHEP::HepRotation _R;
    
  };
  
} // end of gear namespace

#endif

