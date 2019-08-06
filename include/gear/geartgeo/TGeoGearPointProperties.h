#ifndef GEAR_TGEOGEARPOINTPROPERTIES_H
#define GEAR_TGEOGEARPOINTPROPERTIES_H 1

#include <string>
#include <vector>

#include "gear/GEAR.h"
#include "gear/GearPointProperties.h"

//#include "TGeoManager.h"
class TGeoManager;

namespace gear {

/** TGeo implementation of the abstract interface that returns the (material) 
 *  properties of a given point in world coordinates.  
 * @author A. Muennich, CERN
 * @version $Id$
 */
class TGeoGearPointProperties: public GearPointProperties {

public:
  TGeoGearPointProperties(const TGeoGearPointProperties&) = delete ;
  TGeoGearPointProperties& operator=(const TGeoGearPointProperties&) = delete ;

    TGeoGearPointProperties(TGeoManager *geoMgr);

    /// Destructor.
    virtual ~TGeoGearPointProperties() { /* nop */; }

    //vec getPosition returns the current position (if ID is known)
    /** The cellID of the the sensitive detector at pos.
     */
    virtual long64 getCellID(const Vector3D & pos) const ;

    // 	bool getMaterial  (true if defined material, false if problem)
    /** Name of material at pos.  
     */
    virtual const std::string & getMaterialName(const Vector3D & pos) const ;

    /** Density in kg/m^3 at pos.  
     */
    virtual double getDensity(const Vector3D & pos) const ;

//     /** Name of material at pos.  
//      */
//     public double getState( const Vector3D& pos) const throws NotImplementedException ;
    /** Temperature in K of material at pos.  
     */
    virtual double getTemperature(const Vector3D & pos) const ;

    /** Pressure in P at pos.  
     */
    virtual double getPressure(const Vector3D & pos) const ;

    /** Radiation length of material in mm at pos.  
     */
    virtual double getRadlen(const Vector3D & pos) const ;

    /** Interaction length of material in mm at pos.  
     */
    virtual double getIntlen(const Vector3D & pos) const ;

    /** Position in local coordinate 
     */
    virtual Vector3D getLocalPosition(const Vector3D & pos) const ;

    /** The magnetic field vector at pos in [Tesla].  
     */
    virtual Vector3D getB(const Vector3D & pos) const ;

    /** The electric field vector at pos in [V/m].  
     */
    virtual Vector3D getE(const Vector3D & pos) const ;

    /** Names of (geant4) logical volumes in heirarchy starting at given pos ending with the world volume. 
     */
    virtual std::vector<std::string>  getListOfLogicalVolumes(const Vector3D & pos) const ;

    /** Names of (geant4) physical volumes in heirarchy starting at given pos ending with the world volume. 
     */
    virtual std::vector<std::string>  getListOfPhysicalVolumes(const Vector3D & pos) const ;

    /** Names of (geant4) region that contains the given pos.
     */
    virtual std::string getRegion(const Vector3D & pos) const ;

    /** True if region that contains pos is defined as a tracker.
     */
    virtual bool isTracker(const Vector3D & pos) const ;

    /** True if region that contains pos is defined as a calorimeter.
     */
    virtual bool isCalorimeter(const Vector3D & pos) const ;

  protected:
    TGeoManager *_tgeomanager = nullptr ;
    
};//class
} // namespace gear
#endif /* ifndef GEAR_TGeoGEARPOINTPROPERTIES_H */
