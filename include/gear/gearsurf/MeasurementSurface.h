#ifndef MEASUREMENTSURFACE_H
#define MEASUREMENTSURFACE_H

#include <vector>
#include "IBoundary.h"

/** Measurement Surface Class */
namespace gear{
  
  class ICoordinateSystem;
  
  class MeasurementSurface{
    
  public:
    
    MeasurementSurface( int ID, ICoordinateSystem* csys ): _ID( ID ), _coordinateSystem( csys ), _boundary(0) {}
    
    MeasurementSurface( int ID, ICoordinateSystem* csys ,IBoundary* boundary): _ID( ID ), _coordinateSystem( csys ), _boundary(boundary){}
    
    ~MeasurementSurface() ;
    
    int getID(){ return _ID; }
    ICoordinateSystem* getCoordinateSystem() const { return _coordinateSystem; } 
    
    
    /** Checks if a point in local coordinates is within the boundaries */
    bool isLocalInBoundary( CLHEP::Hep3Vector local ) const ;
    
    /** Checks if a point in global coordinates is within the boundaries */
    bool isGlobalInBoundary( CLHEP::Hep3Vector global ) const ;
    
  private:
    
    int _ID{};
    ICoordinateSystem* _coordinateSystem = nullptr ;
    
    IBoundary* _boundary = nullptr ;
    
    MeasurementSurface(const MeasurementSurface&){};   // copy constructor is private --> no cpoying allowed
    MeasurementSurface& operator= (MeasurementSurface const&); // assignment not allowed either
    
    
  };
  
  
} //end of gear namespace 


#endif
