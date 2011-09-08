// -*- C++ -*-
#ifndef GEAR_FTDLayerLayoutImpl_H
#define GEAR_FTDLayerLayoutImpl_H 1

#include "gear/FTDLayerLayout.h"
#include <vector>
#include <math.h>

namespace gear 
{
	/** Abstract description of layers in a FTD detector. <br>
	 *  This assumes a symmetric layout of supports (petals), arranged in layers (disks) 
	 *  placed at z-position.  <br>
	 *  The sensitive area is assumed to be inside the petals but can be positioned independently.
	 * 
	 * @author J. Duarte Campderros, IFCA (Based at VXDLayerLayout)
	 * @version $Id: 
 */

// Helper class: Components x,y,z of a vector which defines
//               a reference frame (its module is 1), why not
//               usign Vector3D (CLHEP or GEAR)?... by the moment	
class vframe
{
	public:
		vframe(): x(0),y(0),z(0) { ; }
		vframe(const double & xval, const double & yval, const double & zval):
			x(xval),y(yval),z(zval) { ; }
		~vframe() { ; }

		double x;
		double y;
		double z;
};

class FTDLayerLayoutImpl : public FTDLayerLayout 
{
	friend class FTDParametersImpl ;
	
	public: 
		/** Helper class for layer properties */
		// FIXME: Still missing the strips pitch, distance from the edge,..
	        //        or whatever parameter needed to characterize the strips
	        //        
		struct Layer 
		{
			int    nPetals ;   // Number of petals
			int    sensorType ; // The sensor type of the disk: pixel or strips
			//double pitch;
			//double sensoroffset;
			double phi ;        // Angle defining the half-width of a petal
			double phi0 ;       // azimuthal angle  of the first petal
			double alpha;       // Angle of the rotation of the petal in 
			                    // its own plane (turbine-blade like)
			double zposition;   // Z-position (centered) of the layer
			double zoffset;     // z-distance from the zposition to the center
			                    // of a petal (sensitive) (a la CMS design)
			double rInner;      // Distance from Z-axis to closest point of a petal (sensitive)
			double thickness ;  // thickness
			double lengthMin ;  // smaller length of a trapezoid
			double lengthMax ;  // biggest length of a trapezoid
			double width ;      // y-distance (between the lengthMin line
			                    // and the lengthMax line)
			double radLength ;  // Radiation length
		} ;
		
	
		typedef std::vector<Layer> LayerVec ;
		
		// Destructor.
		virtual ~FTDLayerLayoutImpl() { /* nop */; }
		
		/** The total number of layers. */
		virtual int getNLayers() const { return _lVec.size() ; }
		
		/** The number of petals in the layer layerIndex - layer indexing starts at 0
		 *  for the layer closest to IP.  */
		virtual int getNPetals(int layerIndex) const { return _lVec.at( layerIndex ).nPetals  ; }
		
		/** The sensor type of the disk: pixel of micro-strip
		  */
		virtual double getSensorType(int layerIndex) const { return _lVec.at( layerIndex).sensorType; }

		/** Angular half-width of the petals of a layer
		  */
		virtual double getPhiHalfDistance(int layerIndex) const { return _lVec.at( layerIndex ).phi  ; }
		
		/** Angular distance of a petal 
		 */
		virtual double getPhi0(const int & layerIndex) const { return _lVec.at(layerIndex).phi0 ; }
		
		/** Angle of rotation in the own plane of a petal
		 */
		virtual double getAlpha(int layerIndex) const { return _lVec.at( layerIndex ).alpha  ; }

		/** The z-offset of the support structure
		  */
		virtual double getZoffset(const int & layerIndex) const { return _lVec.at(layerIndex).zoffset ; }
	
		/** The position of the support in z direction in mm for petals in layer layerIndex -
		 *  layer indexing starting at 0 from the layer closest to IP.
		 */
		virtual double getSupportZposition(int layerIndex) const { return _lVec.at( layerIndex ).zposition  ; }
		
		/** The R-min of the petal in the XY-plane in mm for supports
		 */
		virtual double getSupportRinner(int layerIndex) const { return _lVec.at( layerIndex ).rInner  ; }
		
		/** The radiation length in the support structure supports of layer layerIndex - layer indexing starts at 0
		 *  for the layer closest to IP.
		 */
		virtual double getSupportRadLength(int layerIndex) const { return _lVec.at( layerIndex ).radLength  ; }
	  
		/** The thickness in mm of the supports in layerIndex - layer indexing starting at 0
		 *  for the layer closest to IP.
		 */
		virtual double getSupportThickness(int layerIndex) const { return _lVec.at( layerIndex ).thickness  ; }
		
		/** The length (x-direction) of the smallest edge of the trapezoid (petal) 
		 */
		virtual double getSupportLengthMin(int layerIndex) const { return  _lVec.at( layerIndex ).lengthMin  ; }
		
		/** The length (x-direction) of the largest edge of the trapezoid (petal) 
		 */
		virtual double getSupportLengthMax(int layerIndex) const { return  _lVec.at( layerIndex ).lengthMax  ; }
	
	 	/** The width of the support in layer in mm for supports in layer layerIndex -
		 *  layer indexing starting at 0 from the layer closest to IP.
		 */
		virtual double getSupportWidth(int layerIndex) const { return _lVec.at( layerIndex ).width  ; }
	

		/** The position of the sensitive in z direction in mm for petals in layer layerIndex -
		 *  layer indexing starting at 0 from the layer closest to IP.
		 */
		virtual double getSensitiveZposition(int layerIndex) const { return _sVec.at( layerIndex ).zposition  ; }

		/** The R-min of the support in the XY-plane in mm for supports
		 */
		virtual double getSensitiveRinner(int layerIndex) const { return _sVec.at( layerIndex ).rInner  ; }

	  	/** The radiation length in sensitive volumes in layer layerIndex - layer indexing starts at 0
		 *  for the layer closest to IP.
		 */
		virtual double getSensitiveRadLength(int layerIndex) const { return _sVec.at( layerIndex ).radLength  ; }
		
		/** The thickness in mm of the sensitive area in supports in layer layerIndex.
		 */
		virtual double getSensitiveThickness(int layerIndex) const { return _sVec.at( layerIndex ).thickness  ; }
		
		/** Same as getSupportLengthMin() except for the sensitive part of the support.
		 * @see getSupportLengthMin
		 */
		virtual double getSensitiveLengthMin(int layerIndex) const { return _sVec.at( layerIndex ).lengthMin  ; }
		
		/** Same as getSupportLengthMax() except for the sensitive part of the support.
		 * @see getSupportLengthMax
		 */
		virtual double getSensitiveLengthMax(int layerIndex) const { return _sVec.at( layerIndex ).lengthMax  ; }
		
		/** The width of the sensitive area in supports in layer layerIndex in mm.
		 */
		virtual double getSensitiveWidth(int layerIndex) const { return _sVec.at( layerIndex ).width  ; }
		
	
		/** Add a new layer at the given position
		 */
		virtual void addLayer(int nPetals, int sensorType, double phi, double phi0, double alpha, 
				      // common for support and sensitive
				      double zoffset,
				      // support
				      double supportZposlayer, 
				      double supportRinner, double supportThickness, 
				      double supportLengthMin, double supportLengthMax,
				      double supportWidth, 
				      double supportRadLength,
				      // sensitive
				      double sensitiveZposlayer, 
				      double sensitiveRinner,	double sensitiveThickness,
				      double sensitiveLengthMin, double sensitiveLengthMax,
				      double sensitiveWidth, 
				      double sensitiveRadLength ) ;

	
		/** Azimuthal angle of the vector defined by the Z-axis to the j-petal x-positive, y-positive
		 *  (edge side).
		 *  phi smallest corresponds to the first petal's closests to the positive X-axis.
		 *  The layerIndex starts at 0 for the layer closest to IP.
		 */
		virtual double getPhiStructure(const int & layerIndex, const int & petalIndex, const bool & sensitive) const;

		/** returns maximum radius for a given layer   */
		virtual double getMaxRadius(const int & layerIndex,const bool & sensitive=false ) const ; 
		
		/** returns starting phi for first petal in layer layerIndex (on side facing IP)
		 */
		virtual double getStartPhi(const int & layerIndex, const int & petalInd,const bool &sensitive=false ) const ;
	
		/** returns ending phi for first petal in layer layerIndex (on side facing IP)
		 */
		virtual double getEndPhi(const int & layerIndex,const int & petalInd, const bool & sensitive=false ) const ;
	
		/** returns thickness as viewed under the incidence angles phi and theta in 
		 *  layer layerIndex.
		 *  theta is the angle defined between the Z-axis (perpendicular to the petal plane)
		 *  and the plane of the petal; phi is the incident angle defined in the ZX plane
		 */
		virtual double getThicknessForAngle(const int & layerIndex, const double & tetha,
				const double & phi, const bool & sensitive=false ) const ;
	
	
	protected:
		// Support
		LayerVec _lVec ;
		// Sensitive
		LayerVec _sVec ;

		// Reference frame of the petal (left side ).  
		// The frame is defined in the way of that all the points inside the 
		// trapezoid are defined positives. 
		std::vector<vframe> _eL;
		std::vector<vframe> _eS;
		
	private:
		// Extraction of the left side reference frame
		std::vector<vframe> getframe( const Layer & layer );
		
}; // class
} // namespace gear
#endif /* ifndef GEAR_FTDLAYERLAYOUT_H */
