#ifndef GEAR_TGeoGEARDISTANCEPROPERTIES_H
#define GEAR_TGeoGEARDISTANCEPROPERTIES_H 1

#include <string>
#include <vector>

#include "gear/GEAR.h"
#include "gear/GearDistanceProperties.h"

//#include "TGeoManager.h"
class TGeoManager;

namespace gear {

/** TGeo Implementation of the abstract interface that returns the (material) 
 *  properties along a given distance between two points in in world 
 *  coordinates. 
 * @author A. Muennich, CERN
 * @version $Id$
 */
class TGeoGearDistanceProperties: public GearDistanceProperties {

public:

    TGeoGearDistanceProperties( const TGeoGearDistanceProperties&) = delete ;
    TGeoGearDistanceProperties& operator=( const TGeoGearDistanceProperties&) = delete ;

    TGeoGearDistanceProperties(TGeoManager *geoMgr);

    /// Destructor.
    virtual ~TGeoGearDistanceProperties() { /* nop */; }

    /** List of matrial names along the distance between [p0,p1]- WARNING: this method returns a reference to a vector that will
     *  change after the next call to this method with different arguments or if the GearDistanceProperties object goes out of scope.
     *  It is the users responsibility to make a copy of that vector if needed.
     */
    virtual const std::vector<std::string>&  getMaterialNames(const Vector3D & p0, const Vector3D & p1) const throw (NotImplementedException, std::exception );

 /** List of matrial thicknesses in mm along the distance between [p0,p1] - runs parallel to the array
     *  returned by  getMaterialNames() - WARNING: this method returns a reference to a vector that will
     *  change after the next call to this method with different arguments or if the GearDistanceProperties object goes out of scope.
     *  It is the users responsibility to make a copy of that vector if needed.
     */
    virtual const std::vector<double>&  getMaterialThicknesses(const Vector3D & p0, const Vector3D & p1) const throw (NotImplementedException, std::exception );

    /** List of traversed volumes by name.
     */
   virtual const std::vector<std::string>  getVolumeNames(const Vector3D & p0, const Vector3D & p1) const throw (NotImplementedException, std::exception );

    /** The number of radiation lengths along the distance between [p0,p1] .
     */
    virtual double getNRadlen(const Vector3D & p0, const Vector3D & p1) const throw (NotImplementedException, std::exception );

    /** The number of interaction lengths along the distance between [p0,p1] .
     */
    virtual double getNIntlen(const Vector3D & p0, const Vector3D & p1) const throw (NotImplementedException, std::exception );

    /** The integrated magnetic field along  the distance between [p0,p1] in Tesla*mm.  
     */
    virtual double getBdL(const Vector3D & p0, const Vector3D & p1) const throw (NotImplementedException, std::exception );

    /** The integrated electric field along  the distance between [p0,p1] in  mVolt.  
     */
    virtual double getEdL(const Vector3D & p0, const Vector3D & p1) const throw (NotImplementedException, std::exception );

protected:
    void beamOn(const Vector3D & p0, const Vector3D & p1)const throw (OutsideGeometryException, std::exception );
    TGeoManager *_tgeomanager = nullptr ;
    //two points to keep track of what has already been called and in memory at the moment
    mutable Vector3D _p0{};
    mutable Vector3D _p1{};
    //containers for data evaluated during tracking in beamOn
    //they have to be mutable in order to be changed by beamOn
    //beamOn is const otherwise it could not be called,
    //but should still be able to change the private variables
    mutable std::vector<std::string> _volNames{};
    mutable std::vector<std::string> _matNames{};
    mutable std::vector<double> _distance{};
    mutable std::vector<double> _intLen{};
    mutable std::vector<double> _radLen{};
	
}; // class
} // namespace gear
#endif /* ifndef GEAR_TGeoGEARDISTANCEPROPERTIES_H */
