// -*- C++ -*-
#ifndef GEAR_GearMaterialProperties_H
#define GEAR_GearMaterialProperties_H 1

#include <map>
#include <string>

namespace gear {
  
  class GearPointProperties ;
  class GearDistanceProperties ;
  
  typedef std::map< std::string, std::string > ConfigMap ;

  /** Abstract base class for implemenations of the GearPointPtoperties and GearDistanceProperties.
   *  Will allow to have concrete implementations as plugin libraries.
   *
   * @author F. Gaede, DESY
   * @version $Id:$
   */
  
  class GearMaterialProperties {
    
  public: 
    
    virtual ~GearMaterialProperties() {} 

    /** Use to configure an instance of the material properties, e.g. 
     *    gMP.config()["name"] =  "ILD_01" ;  <br>
     *  Actual configuration parameter names depend on the implementation.
     */ 
    ConfigMap& config() { return _cfg ; } 

    /** Initialize a concrete implemetaiton of the detailed material properties - after the configuration.
     */
    virtual void initialize() = 0 ;

    /** Get an instance of the  GearPointProperties - 0 if not initialized.   
     */
    virtual GearPointProperties* getGearPointProperties() const  = 0;
    
    /** Get an instance of the  GearDistanceProperties - 0 if not initialized.   
     */
    virtual GearDistanceProperties* getGearDistanceProperties() const  = 0;
    
    
  protected:
    ConfigMap _cfg  ; 


  }; // class

  
  /** Factory singleton class - plugin libraries will have to implement a concrete factory and register it on loading.
   */
  class GearMaterialPropertiesFactory{
    
  public:

    virtual ~GearMaterialPropertiesFactory() {} 


   /**Create a new object of the GearMaterialProperties
     */
    virtual GearMaterialProperties* create() const = 0 ;


    /** The instance.
     */
    static const GearMaterialPropertiesFactory* instance(){
      return me() ;
    }

  protected:
    //static void register(  GearMaterialPropertiesFactory* impl) { _me = impl ; } 

    static GearMaterialPropertiesFactory*& me() {
      static GearMaterialPropertiesFactory* _me ;
      return _me ;
    }
    
  } ;
} // namespace gear
#endif /* ifndef GEAR_GearMaterialProperties_H */
