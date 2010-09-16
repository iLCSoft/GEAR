#ifndef FixedPadSizeDiskLayout_h
#define FixedPadSizeDiskLayout_h 1

#include <vector>
#include "gearimpl/FixedDiskLayoutBase.h"

#include <cmath>

namespace gear {



/** Implementation of PadRowLayout2D for a disk with fixed sized keystone pads.
 *  The pads with the given height and width are distributed in symmetrical rings
 *  starting with the pad number 0 above the x-axis so that the pad 0 is fully contained
 *  in the first quadrant (x>0,y>0 ) and symmetyrical to the last pad right below the the x-axis.
 *  The pad width is the width alog the ring's circumference at the middle of the row
 *  (through the pad center).
 *
 * @author F. Gaede, DESY
 * @version $Id: FixedPadSizeDiskLayout.h,v 1.6 2009-02-27 09:00:50 gaede Exp $
 */
  class FixedPadSizeDiskLayout : public FixedDiskLayoutBase {

  public:

    /** Internal helper class for FixedPadSizeDiskLayout */
    struct Row{
      int NPad ;
      double RCenter ;
      double PhiPad ;
    } ;


  protected:
    double _rMin ;
    double _rMax ;
    double _phiMax ;
    double _rowHeight ;
    double _padWidth ;
    double _padHeight ;
    double _padGap ;
    int _nRow ;
    int _nPad ;
    std::vector<Row> _rows ;
    std::vector<double> _extent ;
    mutable std::vector< std::vector<int>* > _padIndices ;

  /** function to copy all internal variables, incl. the objects
   *  pointed to and owned by the FixedPadSizeDiskLayout.
   *  Used by constructor and assigment operator to avoid code duplication
   */
  void copy_and_assign(const  FixedPadSizeDiskLayout & );

  /** function to delete all the objects
   *  pointed to and owned by the FixedPadSizeDiskLayout.
   *  Used by desctructor and assigment operator to avoid code duplication
   */
  void cleanup();


  public:

    /** Construct the FixedPadSizeDiskLayout from the given parameters rMin, rMax,
     *  padHeight and PadWidth where the maximum number of pads are fitted to the plane.
     *  @param nRow :  limits the number of rows - if given
     *  @param padGap : additional gap between pads in r-phi in mm
     */

    FixedPadSizeDiskLayout( double rMin, double rMax, double padHeight, double PadWidth,
			    int nRow=0 ,
			    double padGap=0.,
			    double phiMax=2*M_PI) ;

    /** The copy constructor.
     *	Needed because _padIndices allocates memory dynamically
     */
    FixedPadSizeDiskLayout( const FixedPadSizeDiskLayout &);

    /// The assignment operator
    FixedPadSizeDiskLayout & operator = ( const FixedPadSizeDiskLayout &);

    /// Destructor.
    virtual ~FixedPadSizeDiskLayout() ;

    /* The clone function. Used to access the copy-constructor if this class via the
     * acstract PadRowLayout2D interface.
     */
    PadRowLayout2D* clone() const;


    /** The gap width in mm that was given in the C'tor. */
    virtual double getPadGap() const { return _padGap ; }

    /** The fixed width of the pads in mm.
     */
    virtual double getFixedPadWidth() const { return _padWidth ; }


    /** The type of the row layout implementation:
     *  PadRowLayout2D.FIXEDPADSIZEDISKLAYOUT
     */
    virtual int getPadLayoutImplType() const { return PadRowLayout2D::FIXEDPADSIZEDISKLAYOUT; }

    /* The type of the row layout: PadRowLayout2D::POLAR.
     */
    //
    // Already implemented in FixedDiskLayoutBase
    //virtual int getCoordinateType() const { return PadRowLayout2D::POLAR ; }
    //virtual int getPadLayoutType() const;

    /** The shape of the pads: PadRowLayout2D::RECTANGLE (i.e. keystone).
     */
    virtual int getPadShape() const { return PadRowLayout2D::RECTANGLE ; }

    /** The total number of pads in the TPC.
     */
    virtual int getNPads() const { return _nPad ; }

    /** The number of rows.
     */
    virtual int getNRows() const ;

    /** The row height in mm.
     */
    virtual double getRowHeight(int rowNumber) const { return _rowHeight ; }

    /** The height of the pad in mm.
     */
    virtual double getPadHeight(int padIndex) const { return _padHeight ; }

    /** The width of the pad in radians (only the metal)
     */
    virtual double getPadWidth(int padIndex) const ;

    /** The pitch of the pad in radians (metal + gap)
     */
    virtual double getPadPitch(int padIndex) const ;

    /** The center of the pad in 2d coordinates, (x,y) or (r,phi).
     */
    virtual Vector2D getPadCenter(int padIndex)  const;


    /** Indices of all pads in row rowNumber (row indices start from 0 at the
     * bottom (CARTESIAN) or at the center (POLAR)).
     */
    virtual const std::vector<int>& getPadsInRow(int rowNumber) const ;

    /** Extent of the sensitive plane - [xmin,xmax,ymin,ymax] CARTESIAN or
     *	[rmin,rmax,phimin,phimax] POLAR.
     */
    virtual const std::vector<double>& getPlaneExtent()  const { return _extent ; }

    /** The number of the row that contains the pad at padIndex - numbering starts at r/y==0.
     */
    virtual int getRowNumber(int padIndex)  const { return ( 0xffff0000 & padIndex ) >> 16 ; }

    /** The pad number (column) within the row  - numbering starts at phi/x =.
     */
    virtual int getPadNumber(int padIndex) const { return ( 0x0000ffff & padIndex ) ; }

    /** Create a padIndex for the given row and pad ( column ) number
     */
    virtual int getPadIndex(int rowNum, int padNum)  const;

    /** The index of the pad nearest to the given point in 2d coordinates
     * (x,y,) or (r,phi).
     */
    virtual int getNearestPad(double c0, double c1)  const;

    /** The index of the right neighbour pad.
     */
    virtual int getRightNeighbour(int padIndex)  const;

    /** The index of the left neighbour pad.
     */
    virtual int getLeftNeighbour(int padIndex)  const;

    /** True if coordinate (c0,c1) is within the given pad.
     */
    virtual bool isInsidePad(double c0, double c1, int padIndex)  const;

    /** True if coordinate (c0,c1) is within any pad.
     */
    virtual bool isInsidePad(double c0, double c1)  const;

  }; // class

} // namespace gear
#endif // ifndef FixedPadSizeDiskLayout_h
