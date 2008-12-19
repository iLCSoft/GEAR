#ifndef FixedPadAngleDiskLayout_h
#define FixedPadAngleDiskLayout_h 1

#include <vector>
#include "gearimpl/FixedDiskLayoutBase.h"


namespace gear {

/** Implementation of PadRowLayout2D for a disk with fixed angled keystone pads.
 *  The number of pads is distrubuted on a wedge shaped segment starting 
 *  with the right edge of the first pad in the row at phiMin to the left edge
 *  of the last pad in the row at phiMax.
 *  All rows have the same height, starting at rmin for the lower edge of the
 *  first row to rmax for the upper edge of the last row. The row height is 
 *  calculated from the number of pads.
 *
 *  Row height and pad height are identical, and pad gap is 0
 *  (the only thing that matters is the effective pitch).
 * 
 * @author M. Killenberg, University of Bonn
 * @version $Id: FixedPadAngleDiskLayout.h,v 1.3 2008-12-19 13:52:34 gaede Exp $
 */
  class FixedPadAngleDiskLayout : public FixedDiskLayoutBase {
    
  public:


  protected:
    double _rMin ;
    double _rMax ;
    int _nRow ;
    double _rowHeight ;
    double _phiMin;
    double _phiMax;
//    double _padWidth ;
//    double _padHeight ;
//    double _padGap ;
    double _padAngle;
    int _nPadsInRow;
    int _nPad ;

    std::vector<double> _extent ;
    mutable std::vector< std::vector<int>* > _padIndices ;

  /** Function to copy all internal variables, incl.\ the objects
   *  pointed to and owned by the FixedPadAngleDiskLayout.
   *
   *  Used by constructor and assignment operator to avoid code duplication.
   */
  void copy_and_assign(const  FixedPadAngleDiskLayout & );

  /** Function to delete all the objects
   *  pointed to and owned by the FixedPadAngleDiskLayout.
   *  Used by destructor and assignment operator to avoid code duplication.
   */
  void cleanup();
    

  public: 

    /** Construct the FixedPadAngleDiskLayout from the given parameters rMin, rMax,
     *  nRow, phiMin, phiMax, and nPadsInRow.
     */
    
  FixedPadAngleDiskLayout( double   rMin, double   rMax, int nRow,
			   double phiMin, double phiMax, int nPadsInRow ) ;

    /** The copy constructor.
     *	Needed because _padIndices allocates memory dynamically
     */
    FixedPadAngleDiskLayout( const FixedPadAngleDiskLayout &);
    
    /// The assignment operator
    FixedPadAngleDiskLayout & operator = ( const FixedPadAngleDiskLayout &);

    /// Destructor.
    virtual ~FixedPadAngleDiskLayout() ; 
    
    /* The clone function. Used to access the copy-constructor if this class via the
     * acstract PadRowLayout2D interface.
     */
    PadRowLayout2D* clone() const;


    /** The gap width in mm that was given in the C'tor. Always 0 in this implementation. */
    virtual double getPadGap() const { return 0 ; } 

    /** The fixed width of the pads in mm. 
     */
    virtual double getFixedPadAngle() const { return _padAngle ; }
    

    /** The type of the row layout implementation:
     *  PadRowLayout2D.FIXEDPADANGLEDISKLAYOUT
     */
    virtual int getPadLayoutImplType() const { return PadRowLayout2D::FIXEDPADANGLEDISKLAYOUT; } 

    /* The type of the row layout: PadRowLayout2D::POLAR.
     */
    //
    // Already implemented in FixedDiskLayoutBase
    //virtual int getCoordinateType() const { return PadRowLayout2D::POLAR ; } 
    //virtual int getPadLayoutType() const;
    
    /* The shape of the pads: PadRowLayout2D::RECTANGLE (i.e.\ keystone).
     */ 
    //
    // Already implemented in FixedDiskLayoutBase
    //virtual int getPadShape() const { return PadRowLayout2D::RECTANGLE ; }
    
    /** The total number of pads in the TPC.
     */
    virtual int getNPads() const { return _nPad ; }
    
    /** The number of rows.
     */
    virtual int getNRows() const { return _nRow ; }
    
    /** The row height in mm.
     */
    virtual double getRowHeight(int rowNumber) const { return _rowHeight ; }

    /** The height of the pad in mm.
     */
    virtual double getPadHeight(int padIndex) const { return _rowHeight ; }

    /** The width of the pad in radians NB! 
     */
    virtual double getPadWidth(int padIndex) const { return _padAngle;}

    /** The center of the pad in 2d coordinates (r,phi).
     */
    virtual Vector2D getPadCenter(int padIndex)  const;

    /** Get the (fixed) number of pads per row.
     *  Just returns the value set in the constructor.
     */
    virtual int getNPadsInRow() const {return _nPadsInRow; }

    /** Indices of all pads in row rowNumber (row indices start at rMin). 
     */
    virtual const std::vector<int>& getPadsInRow(int rowNumber) const ;

    /** Extent of the sensitive plane [rmin,rmax,phimin,phimax].
     */
    virtual const std::vector<double>& getPlaneExtent()  const { return _extent ; }

    /* The number of the row that contains the pad at padIndex.
     *  Numbering starts at rMin and phiMin.
     */
    //
    // Already implemented in FixedDiskLayoutBase
    //virtual int getRowNumber(int padIndex)  const { return ( 0xffff0000 & padIndex ) >> 16 ; } 

    /* The pad number (column) within the row  - numbering starts at phi/x =.
     */
    //
    // Already implemented in FixedDiskLayoutBase
    //virtual int getPadNumber(int padIndex) const { return ( 0x0000ffff & padIndex ) ; }

    /** Create a padIndex for the given row and pad ( column ) number 
     */
    virtual int getPadIndex(int rowNum, int padNum)  const;

    /** The index of the pad nearest to the given point in 2d coordinates (r, phi).
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

    /* Returns the closest distance to the edge of the pad
     */
        //
    // Already implemented in FixedDiskLayoutBase
    //virtual double getDistanceToPad(double c0, double c1, int padIndex) const;
    
  }; // class

} // namespace gear
#endif // ifndef FixedPadAngleDiskLayout_h
