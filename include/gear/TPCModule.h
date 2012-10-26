// =========================================================================
// Since the removal of aid header files are modified directly (Killenberg 2010/07/06)
// =========================================================================
#ifndef GEAR_TPCMODULE_H
#define GEAR_TPCMODULE_H 1

#include <vector>

#include "GEAR.h"
#include "gear/GearParameters.h"
#include "gear/PadRowLayout2D.h"

namespace gear {

/** A wrapper Class for PadRowLayout2D which converts 
 * between the actual pad layouts local coodinate system and the TPCs global
 * coordinate systems.
 * Adds some module appropriate functionality.
 * 
 * New in v00-15: The z position of the module can be set to distinguish the two end plates.
 * \attention For the "negative" half TPC (the half with drift direction in negative z direction)
 * the x coodinate of global system is mirrored. The reason is that in case of two identical end plates,
 * the one mounted on the negative side is rotated by 180 degrees around the y axis, which changes the
 * z coordinate (drift direction) and the x coordinate.
 * The prototype case, which is described by zPosition = 0, also has the drift direction in negative z direction.
 *
 * original author F. Gaede, DESY 
 * @author M. Killenberg, (Bonn)  S. Turnbull, (Saclay/Carleton)
 */

class TPCModule : virtual public GearParameters, virtual public PadRowLayout2D {

public: 

    /** An exception that is special for the TPCModule. It is thrown if getZPosition() is
     *  called without calling setZPosition() before because there is no useful default value.
     */
    class NoZPositionException : public Exception{

    public:
      NoZPositionException(){
	message = "gear::TPCModule::NoZPositionException: Set the z position of a module before calling getZPosition(). There is no useful default value.";
      }
      virtual ~NoZPositionException() throw() { /*no_op*/; }
    };


    /// Destructor.
    virtual ~TPCModule() { /* nop */; }

    /** The type of the row layout: PadRowLayout2D.CARTESIAN or
     *  PadRowLayout2D.POLAR.
     */
    virtual int getPadLayoutType() const = 0;

    /** The shape of the pads, one of PadRowLayout2D.RECTANGLE (Keystone)
     * , PadRowLayout2D.DIAMOND, PadRowLayout2D.HEXAGON, 
     * PadRowLayout2D.CHEVRON
     */ 
    virtual int getPadShape() const = 0;

    /** The total number of pads on this module.
     */
    virtual int getNPads() const = 0;

    /** The number of rows on this module.
     */
    virtual int getNRows() const = 0;

    /** The row height in mm.
     */
    virtual double getRowHeight(int rowNumber) const = 0;

    /** The width of the pad at padIndex in mm (CARTESIAN) or radians (POLAR). 
     */
    virtual double getPadWidth(int padIndex) const = 0;

    /** The center of the pad in global 2D coordinates, (x,y) or (r,phi).
     */
    virtual Vector2D getPadCenter(int padIndex) const = 0;

    /** The height of the pad in mm.
     */
    virtual double getPadHeight(int padIndex) const = 0;

    /** Indices of all pads in row \c rowNumber (row indices start from 0 at the
     * bottom (CARTESIAN) or at the center (POLAR)). 
     */
    virtual const std::vector<int>  & getPadsInRow(int rowNumber) const = 0;

    /** Inherited from PadRowLayout2D. It gives the minimal set of global coodinates that
     *  contains the pad plane. In general this does not mean that the global plane extent
     *  if fully covered with avtive area, but only that there is no active area outside
     *  the plane extent.
     */
    virtual const std::vector<double>  & getPlaneExtent() const throw (Exception, std::exception )  = 0;

    /** Maximal extent of the sensitive plane, defined relative to global 
     *  origin - [xmin,xmax,ymin,ymax] CARTESIAN or 
     *  [rmin,rmax,phimin,phimax] POLAR, may contain dead space due to
     *  conversion from local to global coordinate system.
     *
     *  The sensitive plane / amplification plane is the pad plane extended by a border (see
     *  getBorderWidth()).
     *  This allows the gas amplification structure to be larger than
     *  the pad plane, which in the case of a resistive coating
     *  will also produce signal on the pad plane for electrons arriving 
     *  within this border.
    
     *  Note: Amplification planes may overlap, 
     *  pad planes may not. 
     */
    virtual const std::vector<double>  & getModuleExtent() const = 0;

    /** The module extent in local coordinates. This is the (local) pad plane,
     * enlarged by the border. As the plane extent in global coordinates does
     * not correspond to the actual shape, this is the only way to describe
     * the module extent without dead space (everything in the module extent
     * is considerred active, for instance due to a resistive coating which
     * causes signals even if there is no pad at the specific position).
     * \attention Returns LOCAL coordinates!   
     */
    virtual const std::vector<double>  & getLocalModuleExtent() const = 0;

    /** The number of the row within this module that contains 
     *  the pad at padIndex - numbering starts at lowest r/y.
     */
    virtual int getRowNumber(int padIndex) const = 0;

    /** The pad number (column) within the row  - numbering starts
     * at lowest phi/x.
     */
    virtual int getPadNumber(int padIndex) const = 0;

    /** Create a padIndex for the given row and pad ( column ) number 
     */
    virtual int getPadIndex(int rowNum, int padNum) const = 0;

    /** The index of the pad nearest to the given point in global 2D 
     * coordinates, (x,y,) or (r,phi).
     */
    virtual int getNearestPad(double c0, double c1) const = 0;

    /** The index of the right neighbour pad.
     */
    virtual int getRightNeighbour(int padIndex) const = 0;

    /** The index of the left neighbour pad.
     */
    virtual int getLeftNeighbour(int padIndex) const = 0;

    /** True if global coordinate (c0,c1) is within the given pad.
     */
    virtual bool isInsidePad(double c0, double c1, int padIndex) const = 0;

    /** True if global coordinate (c0,c1) is within any pad.
     */
    virtual bool isInsidePad(double c0, double c1) const = 0;

    /** True if global coordinate (c0,c1) is within this modules
     * area of amplification/interest.  i.e. the pad plane plus the
     * surrounding border (see getBorderWidth() ).
     */
    virtual bool isInsideModule(double c0, double c1) const = 0;

    /** The readout frequency in Hz.
     */
    virtual double getReadoutFrequency() const = 0;

    /** Returns the offest of Modules origin from the global origin
     *  A vector from Global (0,0) to module (0,0)
     */
    virtual const Vector2D & getOffset() const = 0;

    /** Returns the z position of the module. This is the position of the 
     *  anode surface terminating the drift volume. For prototypes this will
     *  typically be 0 so z is propotional to the drift distance.
     *  For a full detector positive and negative values correspond to modules
     *  on the first and second end cap, resepctively.
     *  It throws a gear::TPCModule::NoZPositionException in case setZPosition has not 
     *  been called before.
     */
     virtual double getZPosition() const throw (TPCModule::NoZPositionException) = 0;

    /** Returns the rotation of the module, in Rads, with respect 
     *  to the modules internal origin.
     */
    virtual double getAngle() const = 0;

    /** Returns the distance from a global coodinate (c0,c1), to a given
     *  pad's nearest boundery;   (c0,c1,index) 
     */
    virtual double getDistanceToPad(double c0, double c1, int index) const = 0;

    /** Returns distastance from a global coodinate (c0,c1), to the 
     *  module's nearest boundery;   (c0,c1) 
     */
    virtual double getDistanceToModule(double c0, double c1) const = 0;

     /** Returns module ID
     */
    virtual int getModuleID() const = 0;

    /**  Returns True if this and The given module * overlap pad regions
     *   Note: overlaping sensitive regions is ok, Just no two pads with
     * 	 shared physical space.
     */
    virtual bool isOverlapping(TPCModule * testThisModule) const = 0;

    /** Returns the amount by which the pad plane has been
     *  extended to produce the module plane.
     *
     *  In cartesian coordinates the plane is extended in x and y 
     *  by this ammount on all sides. 
     *
     *  In polar coordinates the 
     *  r-coordinate is extended by the borderwidth, in \f$\varphi\f$  it is 
     *  extended by an angle corresponding to \c borderWidth/rMin,
     *  so that the border is at least boderWidth at the inner radius
     *  and somewhat wider for larger  r.
     */
    virtual double getBorderWidth() const = 0;

    /** Returns the TPCs coordinate type. Note that this is NOT the 
     *  coordinate type of the (local) pad plane on the module but 
     *  of the end plate this module is mounted in.
     */
    virtual int getTPCCoordinateType() const = 0;

    /** Returns a reference to the instance of the underlaying pad layout.
     * 	\attention This pad layout has it's own, local coordinate system!
     *  Usually it is not necessary to deal with the pad layout directly.
     *  It is preferred to use the modules memberfunctions, since the module
     *  is an implementation of  PadRowLayout2D which is implemented such that
     *  all coordinates are delivered in global coordinates.
     *  However there are cases it can be handy to have the local coordinates 
     *  available, for instance for debugging or to perform the coordinate
     *  transformations on the graphics processor for an event display.
     */   
    virtual const PadRowLayout2D & getLocalPadLayout() const = 0;

    /** Returns the local coordinates for a point in global coordinates.
     *  It performs a complete coordinate transformation, applying the translation, 
     *  rotation and coordinate conversion to the coordinate type of the local
     *  pad layout. Use getLocalPadLayout()->getCoordinateType() to interpret the
     *  return value correctly.
     *
     *	Usally you don't need this function since all member functions of the module
     *  use global coordinates only.
     */
    virtual Vector2D globalToLocal(double c0, double c1) const = 0;

    /** Returns the global coordinates for a point in local coordinates.
     *  It performs a complete coordinate transformation, applying the coordinate conversion
     *  from the coordinate type of the local pad layout to the global coordinate type,
     *  rotation and translation.
     *  Use getLocalPadLayout()->getCoordinateType() to produce the input in the correct
     *  coordinate type.
     *
     *	Usally you don't need this fuction since all member functions of the module
     *  use global coordinates only.
     */
    virtual Vector2D localToGlobal(double c0, double c1) const = 0;

    
}; // class
} // namespace gear
#endif /* ifndef GEAR_TPCMODULE_H */
