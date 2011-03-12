// -*- C++ -*-
#ifndef GEAR_PADROWLAYOUT2D_H
#define GEAR_PADROWLAYOUT2D_H 1

#include <vector>

#include "GEAR.h"

namespace gear {

/** Abstract description of a planar subdetector with pads (cells) 
 *  that are positioned in rows (circular or rectangular). 
 *  This can be used, e.g. to describe the layout of the GEMs in 
 *  a TPC or the FTD disks. The pads are indexed by row and column numbers
 *  in one 32-bit word.
 * 
 * @author F. Gaede, DESY
 * @version $Id: PadRowLayout2D.aid,v 1.5 2009-02-27 09:00:50 gaede Exp $
 */
class PadRowLayout2D {

public: 
    /// Destructor.
    virtual ~PadRowLayout2D() { /* nop */; }

    // Static constants (enums) for the type of the row layout 

    static const int RECTANGULARPADROWLAYOUT = 1 ;
    static const int FIXEDPADSIZEDISKLAYOUT = 2 ;
    static const int FIXEDPADANGLEDISKLAYOUT = 3 ;
    static const int VERSATILEDISKROWLAYOUT = 4 ;
    static const int TPCMODULE = 100 ;

    static const int CARTESIAN = 1 ;
    static const int POLAR = 2 ;

    static const int RECTANGLE = 1 ;
    static const int DIAMOND = 2 ;
    static const int HEXAGON = 3 ;
    static const int CHEVRON = 4 ;
    
    /** Returns a copy (clone) of this class. As PadRowLayout2D is an abstract
     *  interface with several implementation, this is used get a copy of the
     *  actual implementation without having to know what the actual
     *	implementation is.
     */
    virtual PadRowLayout2D * clone() const = 0;

    /** \deprecated{
     *  As there now is more than one implementation for each coordinate type
     *  this is not sufficient to distinguish the implementations any more.
     *  Use getPadLayoutImplType() to get the implementation type.
     *  Use getCoordinateType() to get the type of the coordinate system.
     *  
     *  For backward compatibility this function returns
     *  PadRowLayout2D.CARTESIAN or PadRowLayout2D.POLAR, i. e. getCoordinateType().}
     */
    virtual int getPadLayoutType() const = 0;

    /** The type of the row layout implementation:
     *  PadRowLayout2D.RECTANGULARPADROWLAYOUT,
     *  PadRowLayout2D.FIXEDPADSIZEDISKLAYOUT, 
     *  PadRowLayout2D.FIXEDPADANGLEDISKLAYOUT
     *  or PadRowLayout2D.TPCMODULE.
     */
    virtual int getPadLayoutImplType() const = 0;

    /** The type of the row layouts coordinate system:
      *  PadRowLayout2D.CARTESIAN or
      *  PadRowLayout2D.POLAR.
      */
    virtual int getCoordinateType() const = 0;

    /** The shape of the pads, one of PadRowLayout2D.RECTANGLE (Keystone), PadRowLayout2D.DIAMOND, 
     *	PadRowLayout2D.HEXAGON, PadRowLayout2D.CHEVRON
     */ 
    virtual int getPadShape() const = 0;

    /** The total number of pads.
     */
    virtual int getNPads() const = 0;

    /** The number of rows.
     */
    virtual int getNRows() const = 0;

    /** The row height in mm.
     */
    virtual double getRowHeight(int rowNumber) const = 0;

    /** The width of the pad at padIndex in mm (CARTESIAN) or radians (POLAR). 
     *  This is only the metal of the pad. Use getPadPitch() to get the
     *  effective pitch.
     */
    virtual double getPadWidth(int padIndex) const = 0;

    /** The pitch (i. e. pad width plus gap)
     *  of the pad at padIndex in mm (CARTESIAN) or radians (POLAR). 
     */
    virtual double getPadPitch(int padIndex) const = 0;

    /** The center of the pad in 2d coordinates, (x,y) or (r,phi).
     */
    virtual Vector2D getPadCenter(int padIndex) const = 0;

    /** The height of the pad in mm.
     */
    virtual double getPadHeight(int padIndex) const = 0;

    /** Indices of all pads in row rowNumber (row indices start from 0 at the
     * bottom (CARTESIAN) or at the center (POLAR)). 
     */
    virtual const std::vector<int>  & getPadsInRow(int rowNumber) const = 0;

    /** Extent of the sensitive plane - [xmin,xmax,ymin,ymax] CARTESIAN or 
     *	[rmin,rmax,phimin,phimax] POLAR.
     */
    virtual const std::vector<double>  & getPlaneExtent() const = 0;

    /** The number of the row that contains the pad at padIndex - numbering starts at r/y==0.
     */
    virtual int getRowNumber(int padIndex) const = 0;

    /** The pad number (column) within the row  - numbering starts at phi/x == 0.
     */
    virtual int getPadNumber(int padIndex) const = 0;

    /** Create a padIndex for the given row and pad ( column ) number 
     */
    virtual int getPadIndex(int rowNum, int padNum) const = 0;

    /** The index of the pad nearest to the given point in 2d coordinates
     * (x,y,) or (r,phi).
     */
    virtual int getNearestPad(double c0, double c1) const = 0;

    /** The index of the right neighbour pad.
     */
    virtual int getRightNeighbour(int padIndex) const = 0;

    /** The index of the left neighbour pad.
     */
    virtual int getLeftNeighbour(int padIndex) const = 0;

    /** True if coordinate (c0,c1) is within the given pad.
     */
    virtual bool isInsidePad(double c0, double c1, int padIndex) const = 0;

    /** True if coordinate (c0,c1) is within any pad.
     */
    virtual bool isInsidePad(double c0, double c1) const = 0;

    /** Returns the closest distance to the edge (outer border) of the pad.
     *  For coordinates inside the pad it returns 0.
     *  ATTENTION: This gives the distance to the effective pad, which is
     *  the relevant one, and not the distance to the metal. I. e. it uses
     *  the pad pitch and the row height instead of pad width and pad height. 
     */
    virtual double getDistanceToPad(double c0, double c1, int padIndex) const = 0;
}; // class
} // namespace gear
#endif /* ifndef GEAR_PADROWLAYOUT2D_H */
