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
class vframe  // TO BE DEPRECATED...
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
		struct Layer 
		{
			int    nPetals ;    // Number of petals
			int    nSensors ;   // Number of sensors per petal
			bool   isDoubleSided; // if Petals have sensors on front AND back
			int    sensorType ; // The sensor type of the disk: pixel or strips
			                    // For future changes: could be extended the types
			                    // to Double Side Micro Strips, for instance...		                    
			double petalOpenningAngle ; // Angle defining the half-width of a petal
			double phi0 ;       // azimuthal angle  of the first petal (paralel to Y-axis,
			                    // and in the Y-positive direction)
			double alpha;       // Angle of the rotation of the petal in 
			                    // its own plane (turbine-blade design)
			double zposition;   // Z-position of the centroid of the layer  (disk).
			double zoffset;     // z-distance from the zposition to the centroid
			                    // of a petal (staggered design)
			double zsign0;      // Sign of the zoffset of the first petal --> FIXME: int
			double rInner;      // Distance from Z-axis to the closest point of a petal (or sensitive)
			double thickness ;  // thickness (petal or sensor) 
			double lengthMin ;  // smaller length of a trapezoid (petal or sensor)
			double lengthMax ;  // biggest length of a trapezoid (petal or sensor)
			double width ;      // y-distance (between the lengthMin line
			                    // and the lengthMax line) for petal or sensor
			double radLength ;  // Radiation length (petal or sensor)
		} ;
		
	
		typedef std::vector<Layer> LayerVec ;
		
		// Destructor.
		virtual ~FTDLayerLayoutImpl() { /* nop */; }
		
		/** The total number of layers (z-positives and z-negatives disks). */
		virtual int getNLayers() const { return _lVec.size() ; }
		
		/** The number of petals in the layer layerIndex - layer indexing starts at 0
		 *  for the layer closest to IP.  */
		virtual int getNPetals(int layerIndex) const { return _lVec.at( getEquivLayer(layerIndex) ).nPetals  ; }
		
		/** The number of sensors per petal on a specific layer - 
                 *  Sensor indexing is defined as follows: (illustrated here by a doublesided petal with 4 sensors)
                 *  it starts with number 1: the sensor on the side facing the IP (the front), that is 
                 *  farthest from the xy - origin. 
                 *  2 is the next sensor on the same side closer to the xy - origin.
                 *  3 is on the back ( the side not facing the IP ) and is back to back to sensor 1.
                 *  4 is then back to back to sensor 2. 
                 * 
                 *  If it were for exmaple 8 sensors on a doublesided petal, it would be (from outside to inside) 1,2,3,4 on the front
                 *  and 5,6,7,8 on the back.
                 * 
                 * If it were 3 sensors on a singlesided petal, it would be just 1,2,3 on the front.
                 */
                virtual int getNSensors(int layerIndex) const { return _lVec.at( getEquivLayer(layerIndex) ).nSensors; }
                
                /** Whether the petals on a layer have sensors in front and back
                 */
                virtual bool isDoubleSided(int layerIndex) const { return _lVec.at( getEquivLayer(layerIndex) ).isDoubleSided; }
		
		/** The sensor type of the disk: pixel or micro-strip
		 * The return value corresponds to the following values: 
		 *    gear::FTDParameters::PIXEL
		 *    gear::FTDParameters::STRIP
		*/
		virtual int getSensorType(int layerIndex) const { return _lVec.at(getEquivLayer(layerIndex)).sensorType; }

		/** Angular half-width of the petals of a layer. This angle define
		    the trapezoid.
		  */
		virtual double getPhiHalfDistance(int layerIndex) const 
		{ return _lVec.at( getEquivLayer(layerIndex) ).petalOpenningAngle  ; }

		/** Angular distance (XY plane) of a first petal (whose index is 0)
		 */
		virtual double getPhi0(const int & layerIndex) const { return _lVec.at(getEquivLayer(layerIndex)).phi0 ; }
		
		/** Angle of rotation in the own plane of a petal
		 */
		virtual double getAlpha(int layerIndex) const { return _lVec.at( getEquivLayer(layerIndex) ).alpha  ; }

		/** The z-position of the centroid of the disk structure
		  */
		//FIXME: TO BE DEPRECATED OR PRIVATE? All the positions having to 
		//       return this class is relative to the petals or
		//       sensors. to be checked
		virtual double getZposition(const int & layerIndex) const 
		{ return _lVec.at(getEquivLayer(layerIndex)).zposition ; }

		/** The z-offset of the support structure in a staggered 
		 *  the setup - the z position of the even numbered petals
		 *  is getZposition(i)+getZoffsetSign0(i)*getZoffset(i),
		 *  the odd numbered petals: getZposition(i)-getZoffsetSign0(i)*getZoffset(i),
		  */
		//FIXME: TO BE DEPRECATED OR PRIVATE?
		virtual double getZoffset(const int & layerIndex) const 
		{ return _lVec.at(getEquivLayer(layerIndex)).zoffset ; }
		
		/** The z-offset sign for the first petal (petalId=0)
		  */
		virtual double getZoffsetSign0(const int & layerIndex) const 
		{ return _lVec.at(getEquivLayer(layerIndex)).zsign0 ; }
	
		/** The position of the support in z direction in mm for the petalIndex petal
		    in layer layerIndex - layer indexing starting at 0 from the layer closest to IP.
		    Petal indexing starting at 0 for the petal placed in the X-axis and grows
		    with positive rotation around Z-axis.
		    The position is defined in the centroid point of the support.
		 */
		virtual double getSupportZposition(const int & layerIndex, const int & petalIndex) const;
		
		/** The R-min of the petals in the XY-plane in mm for supports
		 */
		virtual double getSupportRinner(int layerIndex) const 
		{ return _lVec.at( getEquivLayer(layerIndex) ).rInner  ; }
		
		/** The radiation length in the support structure supports of layer 
		 *  layerIndex - layer indexing starts at 0 for the layer closest to IP.
		 */
		virtual double getSupportRadLength(int layerIndex) const 
		{ return _lVec.at( getEquivLayer(layerIndex) ).radLength  ; }
	  
		/** The thickness in mm of the supports in layerIndex - layer indexing starting at 0
		 *  for the layer closest to IP.
		 */
		virtual double getSupportThickness(int layerIndex) const 
		{ return _lVec.at( getEquivLayer(layerIndex) ).thickness  ; }
		
		/** The length (x-direction) in mm for the smallest edge of the supports in 
		 *  layerIndex - layer indexing starting at 0 for the layer closest to IP
		 */
		// FIXME: Change Length <--> Width (I think is the usual notation)
		virtual double getSupportLengthMin(int layerIndex) const { return  _lVec.at( getEquivLayer(layerIndex) ).lengthMin  ; }
		
		/** The length (x-direction) in mm for the largest edge of the supports in
		 *  layerIndex - layer indexing starting at 0 for the layer closest to IP
		 */
		// FIXME: Change Length <--> Width (I think is the usual notation)
		virtual double getSupportLengthMax(int layerIndex) const { return  _lVec.at( getEquivLayer(layerIndex) ).lengthMax  ; }
	
	 	/** The width in mm for supports in layer layerIndex -
		 *  layer indexing starting at 0 from the layer closest to IP.
		 */
		// FIXME: Change Length <--> Width (I think is the usual notation)
		virtual double getSupportWidth(int layerIndex) const { return _lVec.at( getEquivLayer(layerIndex) ).width  ; }
	

                /** The position of the sensitive in z direction in mm for sensor sensorIndex of
                *  the petal support petalIndex in layer layerIndex -
                *  Layer indexing starting at 0 from the layer closest to IP.
                *  Petal indexing starting at 0 for the petal placed in the X-axis and grows
                *  with positive rotation around Z-axis.
                *  Sensor indexing is defined as follows: (illustrated here by a doublesided petal with 4 sensors)
                *  it starts with number 1: the sensor on the side facing the IP (the front), that is 
                *  farthest from the xy - origin. 
                *  2 is the next sensor on the same side closer to the xy - origin.
                *  3 is on the back ( the side not facing the IP ) and is back to back to sensor 1.
                *  4 is then back to back to sensor 2. 
                */
		virtual double getSensitiveZposition(const int & layerIndex, const int & petalIndex, 
				const int & sensorIndex) const;

		/** The R-min of the support in the XY-plane in mm for sensors
		 */
		virtual double getSensitiveRinner(int layerIndex) const 
		{ return _sVec.at( getEquivLayer(layerIndex) ).rInner  ; }

	  	/** The radiation length in sensitive volumes in layer layerIndex - layer indexing starts at 0
		 *  for the layer closest to IP.
		 */
		virtual double getSensitiveRadLength(int layerIndex) const { return _sVec.at( getEquivLayer(layerIndex) ).radLength  ; }
		
		/** The thickness in mm of the sensitive area in supports in layer layerIndex.
		 */
		virtual double getSensitiveThickness(int layerIndex) const { return _sVec.at( getEquivLayer(layerIndex) ).thickness  ; }
		
		/** Same as getSupportLengthMin() except for the sensitive part of the support.
		 * @see getSupportLengthMin
		 */
		virtual double getSensitiveLengthMin(int layerIndex) const { return _sVec.at( getEquivLayer(layerIndex) ).lengthMin  ; }
		
		/** Same as getSupportLengthMax() except for the sensitive part of the support.
		 * @see getSupportLengthMax
		 */
		// FIXME: Change Length <--> Width (I think is the usual notation)
		virtual double getSensitiveLengthMax(int layerIndex) const { return _sVec.at( getEquivLayer(layerIndex) ).lengthMax  ; }
		
		/** The width of the sensitive area in supports in layer layerIndex in mm.
		 */
		// FIXME: Change Length <--> Width (I think is the usual notation)
		virtual double getSensitiveWidth(int layerIndex) const { return _sVec.at( getEquivLayer(layerIndex) ).width  ; }
		
	
		/** Add a new layer at the given position
		 */
		virtual void addLayer(int nPetals, int nSensors, bool isDoubleSided ,int sensorType, double petalOpAngle, double phi0, double alpha, 
				      // common for support and sensitive
				      double zposition, double zoffset, double zsign0,
				      // support
				      //double supportZposlayer, 
				      double supportRinner, double supportThickness, 
				      double supportLengthMin, double supportLengthMax,
				      double supportWidth, 
				      double supportRadLength,
				      // sensitive
				      //double sensitiveZposlayer, 
				      double sensitiveRinner,	double sensitiveThickness,
				      double sensitiveLengthMin, double sensitiveLengthMax,
				      double sensitiveWidth, 
				      double sensitiveRadLength ) ;

	
    		/** Azimuthal angle of the petal petalIndex Centroid at layer layerIndex.
		 *  Layer indexing starting at 0 from the layer closest to IP.
		 *  Petal indexing starting at 0 for the petal placed in the X-axis and grows
		 *  with positive rotation around Z-axis.
		 */
		virtual double getPhiPetalCd(const int & layerIndex, const int & petalIndex) const;

		/** returns maximum radius for a given layer   */
		// FIXME: To be checked if necessary
		virtual double getMaxRadius(const int & layerIndex,const bool & sensitive=false ) const ; 
		
		/** returns starting phi for first petal in layer layerIndex (on side facing IP)
		 */
		// FIXME: To be checked if necessary
		virtual double getStartPhi(const int & layerIndex, const int & petalInd,const bool &sensitive=false ) const ;
	
		/** returns ending phi for first petal in layer layerIndex (on side facing IP)
		 */
		// FIXME: To be checked if necessary
		virtual double getEndPhi(const int & layerIndex,const int & petalInd, const bool & sensitive=false ) const ;
	
		/** returns thickness as viewed under the incidence angles phi and theta in 
		 *  layer layerIndex.
		 *  theta is the angle defined between the Z-axis (perpendicular to the petal plane)
		 *  and the plane of the petal; phi is the incident angle defined in the ZX plane
		 */
		// FIXME: To be checked if necessary
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
		std::vector<vframe> _eL; //FIXME: TO BE DEPRECATED
		std::vector<vframe> _eS; //FIXME: TO BE DEPRECATED
		
	private:
		// Internal function to convert the raw layerIndex (which goes from 0 to 2N-1,
		// being N=total number of positive disk, to the identifier which corresponds
		// with the z-positive disks symmetric to it.
		// The positives disks have defined the raw index as 0,N-1
		// The negatives disks have defined the raw index as N,2N-1
		// So, the returned index will be the same for positives disks
		// and the correspondent value between 0,N-1 for the negative ones.
		int getEquivLayer(const int & layerIndex) const;

		// Extraction of the left side reference frame
		std::vector<vframe> getframe( const Layer & layer ); //FIXME: TO BE DEPRECATED
		
}; // class
} // namespace gear
#endif /* ifndef GEAR_FTDLAYERLAYOUT_H */
