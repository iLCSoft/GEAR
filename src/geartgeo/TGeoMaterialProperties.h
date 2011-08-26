// -*- C++ -*-
#ifndef GEAR_TGeoMaterialProperties_H
#define GEAR_TGeoMaterialProperties_H 1


#include "gear/GearMaterialProperties.h"

namespace gear {
  
  class  TGeoGeometryInitializer ;
  
  
  
  /** Concrete implemetation for TGeo.
   *
   * @author F. Gaede, DESY
   * @version $Id$
   */
  
  class TGeoMaterialProperties : public GearMaterialProperties {
    
  public: 
    
    /**C'tor. */
    TGeoMaterialProperties() ;

    /**D'tor. */
    ~TGeoMaterialProperties() ;
    
    /** Initialize a concrete implemetaiton of the detailed material properties - after the configuration.
     */
    void initialize()  ;
    
    /** Get an instance of the  GearPointProperties - 0 if not initialized.   
     */
    virtual GearPointProperties* getGearPointProperties() const ;
    
    /** Get an instance of the  GearDistanceProperties - 0 if not initialized.   
     */
    virtual GearDistanceProperties* getGearDistanceProperties() const ;
    
  private:
    TGeoGeometryInitializer* _tgeoInit ;
    
  }; // class
  
  
  /** Implementation of Factory 'singleton' class.
   */
  class TGeoMaterialPropertiesFactory : public GearMaterialPropertiesFactory {
    
  public:
    
    /** C'tor - will register with GearMaterialPropertiesFactory as the one instance.
     */
    TGeoMaterialPropertiesFactory() ;
    
    /**Create a new object of the TGeoMaterialProperties
     */
    TGeoMaterialProperties* create() const ;
    
  } ;

} // namespace gear
#endif /* ifndef GEAR_TGeoMaterialProperties_H */
