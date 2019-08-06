// -*- C++ -*-
#ifndef GEAR_GEARPOINTPROPERTIES_H
#define GEAR_GEARPOINTPROPERTIES_H 1

#include <string>
#include <vector>

#include "GEAR.h"

namespace gear {

/** Abstract interface for a class that returns the (material) properties of a given point in
 *  in world coordinates. No assumption is made on the tool used to implement the functionality.
 * 
 *  Based on ideas discussed at the 2004 Argonne Simulation Workshop as summarized by T.Behnke.
 *
 * @author F. Gaede, DESY
 * @version $Id$
 */
class GearPointProperties {

public: 
    /// Destructor.
    virtual ~GearPointProperties() { /* nop */; }

    //vec getPosition returns the current position (if ID is known)
    /** The cellID of the the sensitive detector at pos.
     */
    virtual long64 getCellID(const Vector3D & pos) const   = 0;

    // 	bool getMaterial  (true if defined material, false if problem)
    /** Name of material at pos.  
     */
    virtual const std::string & getMaterialName(const Vector3D & pos) const   = 0;

    /** Density in kg/m^3 at pos.  
     */
    virtual double getDensity(const Vector3D & pos) const   = 0;

//     /** Name of material at pos.  
//      */
//     public double getState( const Vector3D& pos) const throws NotImplementedException ;
    /** Temperature in K of material at pos.  
     */
    virtual double getTemperature(const Vector3D & pos) const   = 0;

    /** Pressure in P at pos.  
     */
    virtual double getPressure(const Vector3D & pos) const   = 0;

    /** Radiation length of material in mm at pos.  
     */
    virtual double getRadlen(const Vector3D & pos) const   = 0;

    /** Interaction length of material in mm at pos.  
     */
    virtual double getIntlen(const Vector3D & pos) const   = 0;

    /** Position in local coordinate 
     */
    virtual Vector3D getLocalPosition(const Vector3D & pos) const   = 0;

    /** The magnetic field vector at pos in [Tesla].  
     */
    virtual Vector3D getB(const Vector3D & pos) const   = 0;

    /** The electric field vector at pos in [V/m].  
     */
    virtual Vector3D getE(const Vector3D & pos) const   = 0;

    /** Names of (geant4) logical volumes in heirarchy starting at given pos ending with the world volume. 
     */
    virtual std::vector<std::string>  getListOfLogicalVolumes(const Vector3D & pos) const   = 0;

    /** Names of (geant4) physical volumes in heirarchy starting at given pos ending with the world volume. 
     */
    virtual std::vector<std::string>  getListOfPhysicalVolumes(const Vector3D & pos) const   = 0;

    /** Names of (geant4) region that contains the given pos.
     */
    virtual std::string getRegion(const Vector3D & pos) const   = 0;

    /** True if region that contains pos is defined as a tracker.
     */
    virtual bool isTracker(const Vector3D & pos) const   = 0;

    /** True if region that contains pos is defined as a calorimeter.
     */
    virtual bool isCalorimeter(const Vector3D & pos) const   = 0;
}; // class
} // namespace gear
#endif /* ifndef GEAR_GEARPOINTPROPERTIES_H */
