#ifndef GEAR_CGAGEARDISTANCEPROPERTIES_H
#define GEAR_CGAGEARDISTANCEPROPERTIES_H 1

#include <string>
#include <vector>

#include "gear/GEAR.h"
#include "gear/GearDistanceProperties.h"

namespace gear {

/** CGA Implementation of the abstract interface that returns the (material) 
 *  properties along a given distance between two points in in world 
 *  coordinates. 
 * @author G. Musat, Ecole Polytechnique
 * @version $Id: CGAGearDistanceProperties.h,v 1.1 2006-05-29 13:03:18 musat Exp $
 */
class CGAGearDistanceProperties: public GearDistanceProperties {

public: 
    CGAGearDistanceProperties(std::string steer, std::string model, 
			std::string setup, std::string host, std::string user, 
			std::string password);

    /// Destructor.
    virtual ~CGAGearDistanceProperties() { /* nop */; }

    /** List of matrial names along the distance between [p0,p1] .
     */
    virtual const std::vector<std::string> & getMaterialNames(const Point3D & p0, const Point3D & p1) const throw (NotImplementedException, std::exception );

    /** List of matrial thicknesses in mm along the distance between [p0,p1] - runs parallel to the array
     *  returned by  getMaterialNames().
     */
    virtual const std::vector<double>  & getMaterialThicknesses(const Point3D & p0, const Point3D & p1) const throw (NotImplementedException, std::exception );

    /** The number of radiation lengths along the distance between [p0,p1] .
     */
    virtual double getNRadlen(const Point3D & p0, const Point3D & p1) const throw (NotImplementedException, std::exception );

    /** The number of interaction lengths along the distance between [p0,p1] .
     */
    virtual double getNIntlen(const Point3D & p0, const Point3D & p1) const throw (NotImplementedException, std::exception );

    /** The integrated magnetic field along  the distance between [p0,p1] in Tesla*mm.  
     */
    virtual double getBdL(const Point3D & p0, const Point3D & p1) const throw (NotImplementedException, std::exception );

    /** The integrated electric field along  the distance between [p0,p1] in  mVolt.  
     */
    virtual double getEdL(const Point3D & p0, const Point3D & p1) const throw (NotImplementedException, std::exception );

protected:
    void beamOn(const Point3D & p0, const Point3D & p1) const;

	
}; // class
} // namespace gear
#endif /* ifndef GEAR_CGAGEARDISTANCEPROPERTIES_H */
