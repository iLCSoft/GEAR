// -*- C++ -*-
#ifndef GEAR_TPCPARAMETERS_H
#define GEAR_TPCPARAMETERS_H 1

#include <vector>

#include "gear/GearParameters.h"
#include "gear/PadRowLayout2D.h"
#include "gearimpl/GlobalPadIndex.h"

namespace gear {

class TPCModule;

/** Proposal for an abstract interface that defines the geometry properties of
 * a TPC like detector needed for reconstruction code. <br>
 * This object contains one or more modules, these modules define the
 * sensitive region<br>
 * The original TPCParameters class was based on ideas by Dean Karlen.
 * 
 * @author F. Gaede (DESY), M. Killenberg (Bonn)  S. Turnbull (Saclay/Carleton)
 * @version $Id: TPCParameters.aid,v 1.2 2008-12-19 13:52:34 gaede Exp $
 */
class TPCParameters : virtual public GearParameters {

public: 
    /// Destructor.
    virtual ~TPCParameters() { /* nop */; }

    /** Returns module with given moduleID. ModuleID is NOT the position
     *  within the modules vector and may be larger than (nModules-1)! It is the 
     *  global ID (potentially across several end plates), stored as CellID1 
     *  in the lcio data classes.
     */
    virtual const TPCModule & getModule(int ID) const = 0;

    /** Returns number of modules in this TPC (endplate).
     */
    virtual int getNModules() const = 0;

    /** Returns nearest module to given coordinates (2D).
     *  In case of a full TPC with two end caps the first matching module is returned,
     *  although a module from the other end plate might be closer in 3D.
     *  Use the 3D version of getNearestModule in this case 
     *  to get the module from the correct half TPC.
     */
    virtual const TPCModule & getNearestModule(double c0, double c1) const = 0;

    /** Returns nearest module to given coordinates (3D).
     *  The z coordinate is only used to determine whether the positive or negative 
     *  half TPC is to be used.
     */
    virtual const TPCModule & getNearestModule(double c0, double c1, double z) const = 0;

    /** The maximum drift length in the TPC in mm.
     */
    virtual double getMaxDriftLength() const = 0;

    /** True if coordinate (c0,c1) is within any module.
     *  This may or may not be
     *  on a pad, since with resitive films being on the film is enough
     *  to generate signal on pads (see TPCModule::getBorderWidth() ).
     *  This is the 2D version. Use isInsideModule(double c0, double c1, double z)
     *  to limit to correct end plate in case there are two.
     */
    virtual bool isInsideModule(double c0, double c1) const = 0;

    /** First determines the correct end plate from the z coordinate and then
     *  calls the same function as the 2D version on the modules from the correct 
     *  end cap.
     */
    virtual bool isInsideModule(double c0, double c1, double z) const = 0;

    /** True if coordinate (c0,c1) is within any pad, on any module.
     *  This is the 2D version. Use isInsideModule(double c0, double c1, double z)
     *  to limit to correct end plate in case there are two.
     */
    virtual bool isInsidePad(double c0, double c1) const = 0;

    /** First determines the correct end plate from the z coordinate and then
     *  calls the same function as the 2D version on the modules from the correct 
     *  end cap.
     */
    virtual bool isInsidePad(double c0, double c1, double z) const = 0;

   /** Returns globalPadindex Object for nearest pad to given coordinates (2D).
     *  In case of a full TPC with two end caps the first matching module ID is returned,
     *  although a module from the other end plate might be closer in 3D.
     *  Use the 3D version of getNearestPad in this case 
     *  to get the pad from a module from the correct half TPC.
     */
    virtual GlobalPadIndex getNearestPad(double c0, double c1) const = 0;

    /** Returns globalPadindex Object for nearest pad to given coordinates (3D).
     *  The z coordinate is only used to determine whether the positive or negative 
     *  half TPC is to be used.
     */	
    virtual GlobalPadIndex getNearestPad(double c0, double c1, double z) const = 0;

    /** Extent of the sensitive plane - [xmin,xmax,ymin,ymax] CARTESIAN or 
     *	[rmin,rmax,phimin,phimax] POLAR. These are the outermost coordinates
     *  in the respective coordinate system. It does not mean that the complete
     *  plane is sensitive as there might be dead space between the modules.
     */
    virtual const std::vector<double>  & getPlaneExtent() const = 0;

    /** Returns coordinate type as an int (see PadRowLayout2D::CARTESIAN,
     *  PadRowLayout2D::POLAR)
     */
    virtual int getCoordinateType() const = 0;

    /** Returns vector of all modules in this TPC (endplate).
    */
    virtual const std::vector<TPCModule *>  & getModules() const = 0;

    /** Kept for backward compatibility.
     *  In case there is only one module, which has no angle and no
     *  offset wrt. the gobal coordinate systen, it gives back the 
     *  PadRowLayout2D of this module.
     *  Otherwise it throws a gear::Exception.
     *
     *  \deprecated{Please use getModule(moduleID) instead.
     *  The TPCModule is an implementation of PadRowLayout2D}
     */
    virtual const PadRowLayout2D & getPadLayout() const = 0;

    /** The electron drift velocity in the TPC in mm/s.
     *  Kept for backwards compatibility. 
     *
     *  \deprecated{This should come from  conditions data.}
     */
    virtual double getDriftVelocity() const = 0;

    /** Kept for backward compatibility.
     *  In case there is only one module it gives back the 
     *  redaout frequency of this module.
     *  Otherwise it throws a gear::Exception.
     *  
     *  \deprecated{Use TPCModule::getReadoutFrequency() instead.}
     */
    virtual double getReadoutFrequency() const = 0;
}; // class
} // namespace gear
#endif /* ifndef GEAR_TPCPARAMETERS_H */
