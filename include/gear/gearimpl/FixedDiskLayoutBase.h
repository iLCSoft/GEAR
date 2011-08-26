#ifndef FixedDiskLayoutBase_h
#define FixedDiskLayoutBase_h 1

#include "gear/PadRowLayout2D.h"


namespace gear {


/** Base class for circular PadRowLayout2D implementations.
 *
 *  It implements the functionality common for all FixedPad***DiskLayout 
 *  classes to avoid code duplication (non-trivial distance calculations etc).
  * 
 * @author M. Killenberg, University of Bonn
 * @version $Id$
 */
  class FixedDiskLayoutBase : public PadRowLayout2D {
    
  public: 


    /** \deprecated{
     *  As there now is more than one implementation for each coordinate type
     *  this is not sufficient to distinguish the implementations any more.
     *  Use getPadLayoutImplType() to get the implementation type.
     *  Use getCoordinateType() to get the type of the coordinate system.
     *  
     *  For backward compatibility this function returns
     *  PadRowLayout2D.CARTESIAN, i. e. getCoordinateType().}
     */
     virtual int getPadLayoutType() const; 

    /** The type of the row layouts coordinate system:
      *  PadRowLayout2D.POLAR
      */
    virtual int getCoordinateType() const { return PadRowLayout2D::POLAR ; } 
    
    /** The shape of the pads: PadRowLayout2D::RECTANGLE (i.e.\ keystone).
     */ 
    virtual int getPadShape() const { return PadRowLayout2D::RECTANGLE ; }
    
    /** The number of the row that contains the pad at padIndex.
     *  Row numbering starts at rMin.
     */
    virtual int getRowNumber(int padIndex)  const { return ( 0xffff0000 & padIndex ) >> 16 ; } 

    /** The pad number (column) within the row.
     *  Pad numbering within the row starts with 0 at phiMin.
     */
    virtual int getPadNumber(int padIndex) const { return ( 0x0000ffff & padIndex ) ; }

    /** Returns the closest distance to the edge (outer border) of the pad.
     */
    virtual double getDistanceToPad(double c0, double c1, int padIndex) const;
    
  }; // class

} // namespace gear
#endif // ifndef FixedPadAngleDiskLayout_h
