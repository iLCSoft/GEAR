
#include "MeasurementSurface.h"
#include "ICoordinateSystem.h"


namespace GearSurfaces {

  MeasurementSurface::~MeasurementSurface(){ 
    
    delete _coordinateSystem; 
    delete _boundary;
    
  }
  
  // only checks the boundary if a boundary is present
  bool MeasurementSurface::isLocalInBoundary( CLHEP::Hep3Vector local ){
    
    bool inbounds = true ; 
    
      if (_boundary) {
        inbounds =  _boundary->isInBoundary( local ) ;
      }

    return inbounds;
    
  }
  
  
  // only checks the boundary if a boundary is present
  bool MeasurementSurface::isGlobalInBoundary( CLHEP::Hep3Vector global ){
    
    bool inbounds = true ; 
    
    if (_boundary) {

      CLHEP::Hep3Vector local = _coordinateSystem->getLocalPoint( global );
      
      inbounds = this->isLocalInBoundary( local ) ;
      
    }
    
    return inbounds;

       
  }
  
  
  
} //end of GearSurfaces namespace 



