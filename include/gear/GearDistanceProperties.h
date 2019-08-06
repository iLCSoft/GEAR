// -*- C++ -*-
#ifndef GEAR_GEARDISTANCEPROPERTIES_H
#define GEAR_GEARDISTANCEPROPERTIES_H 1

#include <string>
#include <vector>

#include "GEAR.h"

namespace gear {
  
  /** Abstract interface for a class that returns the (material) properties along a given distance 
   *  between two points in in world coordinates. 
   *  No assumption is made on the tool used to implement the functionality.
   * 
   *  Based on ideas discussed at the 2004 Argonne Simulation Workshop as summarized by T.Behnke.
   *
   * @author F. Gaede, DESY
   * @version $Id$
   */
  class GearDistanceProperties {
    
  public: 
    /// Destructor.
    virtual ~GearDistanceProperties() { /* nop */; }
    
    /** List of material names along the distance between [p0,p1] - WARNING: this method returns a reference to a vector that will
     *  change after the next call to this method with different arguments or if the GearDistanceProperties object goes out of scope.
     *  It is the users responsibility to make a copy of that vector if needed.
     */
    virtual const std::vector<std::string>&  getMaterialNames(const Vector3D & p0, const Vector3D & p1) const   = 0;
    

    /** List of matrial thicknesses in mm along the distance between [p0,p1] - runs parallel to the array
     *  returned by  getMaterialNames() - WARNING: this method returns a reference to a vector that will
     *  change after the next call to this method with different arguments or if the GearDistanceProperties object goes out of scope.
     *  It is the users responsibility to make a copy of that vector if needed.
     */
    virtual const std::vector<double>&  getMaterialThicknesses(const Vector3D & p0, const Vector3D & p1) const   = 0;



    /** The number of radiation lengths along the distance between [p0,p1] .
     */
    virtual double getNRadlen(const Vector3D & p0, const Vector3D & p1) const   = 0;

    /** The number of interaction lengths along the distance between [p0,p1] .
     */
    virtual double getNIntlen(const Vector3D & p0, const Vector3D & p1) const   = 0;

    /** The integrated magnetic field along  the distance between [p0,p1] in Tesla*mm.  
     */
    virtual double getBdL(const Vector3D & p0, const Vector3D & p1) const   = 0;

    /** The integrated electric field along  the distance between [p0,p1] in  mVolt.  
     */
    virtual double getEdL(const Vector3D & p0, const Vector3D & p1) const   = 0;
}; // class
} // namespace gear
#endif /* ifndef GEAR_GEARDISTANCEPROPERTIES_H */
