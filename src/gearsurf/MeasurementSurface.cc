
#include "gearsurf/MeasurementSurface.h"
#include "gearsurf/ICoordinateSystem.h"


namespace gear {

  MeasurementSurface::~MeasurementSurface(){ 
    
    delete _coordinateSystem; 
    delete _boundary;
    
  }
  
  // only checks the boundary if a boundary is present
  bool MeasurementSurface::isLocalInBoundary( CLHEP::Hep3Vector local ) const{
    
    bool inbounds = true ; 
    
      if (_boundary) {
        inbounds =  _boundary->isInBoundary( local ) ;
      }

    return inbounds;
    
  }
  
  
  // only checks the boundary if a boundary is present
  bool MeasurementSurface::isGlobalInBoundary( CLHEP::Hep3Vector global ) const{
    
    bool inbounds = true ; 
    
    if (_boundary) {

      CLHEP::Hep3Vector local = _coordinateSystem->getLocalPoint( global );
      
      inbounds = this->isLocalInBoundary( local ) ;
      
    }
    
    return inbounds;

       
  }
  
  
  
} //end of gear namespace 



