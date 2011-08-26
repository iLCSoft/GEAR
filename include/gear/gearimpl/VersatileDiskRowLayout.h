#ifndef VersatileDiskRowLayout_h
#define VersatileDiskRowLayout_h 1

#include <vector>
#include "gearimpl/FixedDiskLayoutBase.h"


namespace gear {

/** Implementation of PadRowLayout2D for a wedge shaped module.
 *  It is made up of individual rows. All pads within a row are the
 *  same. They are keystone shaped. The height and width can vary from row
 *  to row. Each row can have an offset in phi, so a staggering can be implemented.
 *
 *  The geometry is specified as frayed if the edge of a the first or last pad in
 *  a row is further away from the edge than half its height. In this case the 
 *  getNearestPad() function does not work reliably.
 * 
 *  @author M. Killenberg, University of Bonn
 *  @version $Id$
 */
class VersatileDiskRowLayout : public FixedDiskLayoutBase {
    
    public:
	
	/** Internal helper class for VersatileDiskRowLayout, describing one row */
	class Row
	{
	protected:
	    int _rowNumber; ///< the row number 
	    int _nPads ; ///< number of pads in the row
	    double _padPitch_mm; ///< the pad pitch in mm
	    double _padPitch; ///< the pad pitch in radians
	    double _padWidth_mm; ///< the pad width (without gap) in mm
	    double _padWidth; ///< the pad width (without gap) in radians
	    double _rowHeight; ///< the row height in mm 
	    double _padHeight; ///< the height of the pad (without gap)
	    double _offset_mm; ///< the offset (staggering) with respect to phi=0 in mm
	    double _offset; ///< the offset (staggering) with respect to phi=0 in radians, aka phiMin

	    double _rCentre; ///< radius at the pad centre

	    double _phiMax; ///< the maximum Angle of this row
	    double _rMin; ///< lower edge of the pad (incl. gap)
	    double _rMax; ///< upper edge of the pad (incl.gap)

	    double distanceToPhiMin(double r, double phi) const;
	    double distanceToPhiMax(double r, double phi) const;

	public:
	    Row(int rowNumber, int nPads, double rCentre, double padPitch_mm, double rowHeight,
		double padWidth_mm, double padHeight, double offset_mm);
	    int    getRowNumber()   const {return _rowNumber;}
	    int    getNPads()       const {return _nPads;}
	    double getPadPitch_mm() const {return _padPitch_mm;}
	    double getPadPitch()    const {return _padPitch;}
	    double getPadWidth_mm() const {return _padWidth_mm;}
	    double getPadWidth()    const {return _padWidth;}
	    double getRowHeight()   const {return _rowHeight;}
	    double getPadHeight()   const {return _padHeight;}
	    double getOffset_mm()   const {return _offset_mm;}
	    double getOffset()      const {return _offset;}
	    double getRCentre()     const {return _rCentre;}
	    double getPhiMin()      const {return _offset;}
	    double getPhiMax()      const {return _phiMax;}
	    double getRMin()        const {return _rMin;}
	    double getRMax()        const {return _rMax;}

	    /// Function to calculate closest distance to the row
	    double getDistanceToRow(double r, double phi) const;

	    /// Function to determine the nearest pad in the row.
	    int getNearestPad(double r, double phi) const;
	};
    

    protected:

	int _nPad ; ///< number of pads, keep for performance reasons
	std::vector<Row> _rows ; ///< vector with all the rows of the pad plane
	std::vector<double> _extent ; ///< the plane extent
	mutable std::vector< std::vector<int>* > _padIndices ; ///< vector for getPadsInRow

	/** Flag that describes if the geometry is frayed */
	bool _isFrayed ;

	/** function to copy all internal variables, incl. the objects
	 *  pointed to and owned by the VersatileDiskRowLayout.
	 *  Used by constructor and assigment operator to avoid code duplication
	 */
	void copy_and_assign(const  VersatileDiskRowLayout & );

	/** function to delete all the objects
	 *  pointed to and owned by the VersatileDiskRowLayout.
	 *  Used by desctructor and assigment operator to avoid code duplication
	 */
	void cleanup();
	
	
    public: 
	
	/** Construct the VersatileDiskRowLayout.  The only requred parameter is the minimal radius.
	 *  All other parameters are calculated when adding the pad rows.
	 */
	
	VersatileDiskRowLayout( double rMin ) ;
    
	/** The copy constructor.
	 *	Needed because _padIndices allocates memory dynamically
	 */
	VersatileDiskRowLayout( const VersatileDiskRowLayout &);
	
	/// The assignment operator
	VersatileDiskRowLayout & operator = ( const VersatileDiskRowLayout &);
	
	/// Destructor.
	virtual ~VersatileDiskRowLayout() ; 
	
	/* The clone function. Used to access the copy-constructor if this class via the
	 * acstract PadRowLayout2D interface.
	 */
	PadRowLayout2D* clone() const;
	
	/** Add 'repeat' rows with the given parameters.
	 */
	virtual void addRow(  int nPads , double padPitch , double rowHeight , 
			      double offset = 0., double padWidth =0., double padHeight = 0., 
			      int repeat = 1);
	
	/** The type of the row layout implementation:
	 *  PadRowLayout2D.FIXEDPADSIZEDISKLAYOUT
	 */
	virtual int getPadLayoutImplType() const { return PadRowLayout2D::VERSATILEDISKROWLAYOUT; } 

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
	
	/* The type of the row layout: PadRowLayout2D::POLAR.
	 */
	//
	// Already implemented in FixedDiskLayoutBase
	//virtual int getCoordinateType() const { return PadRowLayout2D::POLAR ; } 
	
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
	virtual double getRowHeight(int rowNumber) const;
	
	/** The height of the pad in mm.
	 */
	virtual double getPadHeight(int padIndex) const;
	
	/** The width of the pad in radians
	 */
	virtual double getPadWidth(int padIndex) const ;
	
	/** The pitch of the pad in radians 
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
	virtual int getRowNumber(int padIndex)  const;
	
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
	
	/** Get information whether geometry has large staggering */
	virtual bool isFrayed() const { return _isFrayed;}

	/** Get access to the individual rows */
	const std::vector<Row>& rows() const { return _rows ; }

    }; // class
    
} // namespace gear
#endif // ifndef VersatileDiskRowLayout_h
