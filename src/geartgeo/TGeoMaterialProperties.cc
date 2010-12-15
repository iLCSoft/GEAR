
#include "TGeoMaterialProperties.h"


#include "geartgeo/TGeoGeometryInitializer.h"
#include "geartgeo/TGeoGearPointProperties.h"
#include "geartgeo/TGeoGearDistanceProperties.h"

namespace gear{


 
  TGeoMaterialProperties::TGeoMaterialProperties() : _tgeoInit(0) {
  }

  TGeoMaterialProperties::~TGeoMaterialProperties() {

    delete _tgeoInit ;
  }
  
  void  TGeoMaterialProperties::initialize() {
    
    std::string gdmlFileName = _cfg[ "GDMLFileName" ] ;
    
    _tgeoInit = new TGeoGeometryInitializer( gdmlFileName ) ;
      
  }
  
  GearPointProperties*  TGeoMaterialProperties::getGearPointProperties() const {

    return ( _tgeoInit ?  new TGeoGearPointProperties( _tgeoInit->getGeoManager() ) : 0 ) ;
  }
  
  GearDistanceProperties* TGeoMaterialProperties::getGearDistanceProperties() const {
    
    return ( _tgeoInit ?  new TGeoGearDistanceProperties( _tgeoInit->getGeoManager() ) : 0 )  ;
  }
  
  
  // have a global instance of the factory:
  TGeoMaterialPropertiesFactory _global_TGeoMaterialPropertiesFactory ;


  TGeoMaterialPropertiesFactory::TGeoMaterialPropertiesFactory() {

    std::cout << " TGeoMaterialPropertiesFactory::TGeoMaterialPropertiesFactory() +++++++++++++++++ " 
	      << GearMaterialPropertiesFactory::me() 
	      << std::endl ;

    GearMaterialPropertiesFactory::me() = this ;
  }
    
  TGeoMaterialProperties* TGeoMaterialPropertiesFactory::create() const {

    return new TGeoMaterialProperties ;
  }
  

}
