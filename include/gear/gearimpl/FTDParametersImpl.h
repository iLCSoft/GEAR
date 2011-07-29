// -*- C++ -*-
#ifndef GEAR_FTDParametersImpl_H
#define GEAR_FTDParametersImpl_H 1

#include "gear/FTDParameters.h"
#include "gearimpl/GearParametersImpl.h"
#include "gearimpl/FTDLayerLayoutImpl.h"


namespace gear 
{
class FTDLayerLayout;
  /** Geometry properties of a FTD detector needed for reconstruction code. <br>
   *  <p>The FTD is assumed to consist of a number of disks (layers) located along the Z-axis. 
   *  Each disk consists of a number of trapezoid ladders (petals) that are uniformly distributed
   *  in a circle around the  Z-axis .<br>
   *  The sensitive volumes are rectangular pixels for the disks 1 and 2 covering the petal face
   *  nearest the IP; the remaining disks mount trapezoids covering both faces of the petal (we 
   *  assume 1 sensitive layer per face, in the current implementation there are two).
   *  The sensitive volumes can be placed relative to the (insensitive) ladders.
   *  @see addLayer .</p>
   *  <p>The shell is described by a cone for the inner part, inner radius min and inner radius max;
   *     a cylinder defined by a its radius is describing the shell farest from the Z-axis,
   *     and it's half length in z.</p>
   *
   *  @author J. Duarte, IFCA (based from VXDParametersImpl)
   *  @version $Id: 
   */
class FTDParametersImpl : public GearParametersImpl, public FTDParameters 
{
	public: 
		/** C'tor  
		 *  @param shellInnerRadiusMin  the minor Radius of the inner cone ftd' shell (in mm)
		 *  @param shellInnerRadiusMax  the great Radius of the inner cone ftd' shell (in mm)
		 *  @param shellOuterRadius     the radius of the outer cylinder ftd' shell (in mm)
		 *  @param shellHalfLength      the half length in z (in mm)
		 *  @param shellRadLength       the material property information about the shell's material radiation length
		 */
		FTDParametersImpl( double shellInnerRadiusMin, double shellInnerRadiusMax,
				double shellOuterRadius, double shellHalfLength, 
				double shellRadLength ) ;
		// Destructor.
		virtual ~FTDParametersImpl() { /* nop */; }
		
		// static constants enums( for the type of layout )
		static const int PIXEL   = 1 ;
		static const int STRIP   = 2 ;
		// for the sensitive strips, facing the IP or backing it
		static const int FACE    = 1;
		static const int BACK    = 2;

		/** adding a Layer to the vertex
		 *  one layer consists of a number of petals and sensitive areas (nLadders) that are
		 *  each Z-equidistant to IP (disk). The petals and sensitive areas are located in a plane XY
		 *  with the same distance to IP (zposition) with dimensions length (xMin and xMax, because 
		 *  are trapezoids),  width (perpendicular to connection IP-spacepoint) and thickness 
		 *  (extension to connection IP-spacepoint).
		 *
		 * @param nLadders   the number of ladders and sensitive areas inside the layer        
		 * @param sensorType the type of the sensors: PIXEL, STRIP
		 * @param phi        azimuthal angle of vector defined by the Z-axis to first petal
		 *                   x-positive, y-positive edge 
		 * @param alpha      angle of rotation of the petal in the turbine-blade design
		 * @param zposition  the centered position of the disk (layer) at Z-axis
		 * @param rInner     Distance from the z-axis to the closest point of the petal
		 *                   (sensitive)
		 * @param thickness  the thickness in mm of the ladder (ladderThickness) or the        
		 *                   sensitive area (sensitiveThickness) (Z-axis distances)                              
		 * @param lengthMin  the length (x-direction) in mm of the ladder (ladderLength)       
		 *                   or the sensitive area (sensitiveLength) in its shorter side                          
		 * @param lengthMax  the length (x-direction) in mm of the ladder (ladderLength)       
		 *                   or the sensitive area (sensitiveLength) in its longest side                          
		 * @param width      the width (y-direction) of the ladder (ladderWidth) or the                      
		 *                   sensitive area (sensitiveWidth) in mm      
		 * @param radLength  the radiation lenght of the ladder (ladderRadLength) or the       
		 *                   sensitive area (sensitiveRadLength) in mm                         
		 *  		     
		 */
		virtual void addLayer(int nLadders, int sensorType, double phi, double alpha, 
				double zposition,double zoffset,
				double ladderRinner, double ladderThickness,
				double ladderLengthMin, double ladderLengthMax,
				double ladderWidth, double ladderRadLength,
				double sensitiveRinner, double sensitiveThickness,
				double sensitiveLengthMin, double sensitiveLengthMax,
				double sensitiveWidth, double sensitiveRadLength )
		{
			_layer.addLayer( nLadders, sensorType, phi, alpha, 
					zposition, zoffset,
					ladderRinner,ladderThickness, 
					ladderLengthMin, ladderLengthMax,
					ladderWidth, ladderRadLength,
					sensitiveRinner,sensitiveThickness,
					sensitiveLengthMin, sensitiveLengthMax,
					sensitiveWidth, sensitiveRadLength ) ;
			return ;
		}
    

		/** Returns the layer layout in the Vertex */
		virtual const FTDLayerLayout & getFTDLayerLayout() const { return _layer ; }
		
		
		/** The half length (z) of the support shell in mm.
		 */
		virtual double getShellHalfLength() const { return _shellHalfLength ; }
		
		/** The inner minimum radius of the cone support shell in mm.
		 */
		virtual double getShellInnerRadiusMin() const { return _shellInnerRadiusMin ; }
		
		/** The inner maximum radius of the cone support shell in mm.
		 */
		virtual double getShellInnerRadiusMax() const { return _shellInnerRadiusMax ; }
		
		/** The outer radius of the support shell in mm.
		 */
		virtual double getShellOuterRadius() const { return _shellOuterRadius ; }
		
		/** The radiation length in the support shell
		 */
		virtual double getShellRadLength() const { return _shellRadLength ; }
		
		/** returns whether a point is inside a ladder
		 */
		virtual bool isPointInLadder(const Vector3D & p) const { return isPointInFTD( p, false ); }
		
		 /** returns whether a point is inside a sensitive volume
		  */
		virtual bool isPointInSensitive(const Vector3D & p) const {  return isPointInFTD( p, true ) ; }
		
		/** returns vector from given point p to nearest ladder
		 */
//		virtual Vector3D distanceToNearestLadder(Vector3D p) const { return distanceToNearestFTD( p, false ) ; }
	
		/** returns vector from given point p to nearest sensitive volume
		 */
//		virtual Vector3D distanceToNearestSensitive(Vector3D p) const {	return distanceToNearestFTD( p, true ) ;}
		
		/** returns the first point where a given strainght line
		 *  (parameters point p and direction v)  crosses a ladder
		 */
//		virtual Vector3D intersectionLadder( Vector3D p, Vector3D v ) const 
//		{
//			return intersectionFTD( p, v, false ) ;
//		}
		
		/** returns the first point where a given strainght line
		 *  (parameters point p and direction v)  crosses a sensitive volume
		 */
//		virtual Vector3D intersectionSensitive( Vector3D p, Vector3D v ) const 
//		{
//			return intersectionFTD( p, v, true ) ;
//		}

		/** returns the layerIndex which correspond to the layer where the point
		  * is. If the point is not inside any layer returns -1
		  */
		virtual int getLayerIndex( const Vector3D & p ) const;
		
		/** returns the ladder Index which correspond to the layer where the point
		  * is. If the point is not inside any layer returns -1
		  */
		virtual int getLadderIndex( const Vector3D & p ) const { return getPetalIndex(p); }
		
		/** returns the layerIndex which correspond to the layer where the point
		  * is. If the point is not inside any layer returns -1
		  */
		virtual int getSensitiveIndex( const Vector3D & p ) const;

	protected:
		
		FTDLayerLayoutImpl _layer ;
		
		int _ftdType ;
		
		double _shellInnerRadiusMin;
		double _shellInnerRadiusMax;
		double _shellOuterRadius ;
		double _shellHalfLength ;
  		double _shellGap ;
		double _shellRadLength ;
		
	private:
		
		/** returns if a point is in ladder (sensitive == false) or in sensitive (sensitive == true)
		 */
		bool isPointInFTD(const Vector3D & p ,const bool & sensitive = false) const ;

		/** returns the petal number corresponding to a given point. Return -1 if the point is outside
		  * the ladder. If sensitive=true, uses the geometry of the sensitive to perform the result
		  */
		int getPetalIndex(const Vector3D & p, const bool & sensitive = false) const;
		
		/** returns distance to nearest ladder (sensitive == false) or nearest sensitiv (sensitive == true)
		 */
//   		Vector3D distanceToNearestFTD(Vector3D p, bool sensitive = false) const ;
		
		/** returns vector to from point to closest point in described plane
		 *  r as spacepoint vector
		 *  n as normal vector
		 *  u and v as in-plane vectors
		 *  minU and maxU as min/max sizes of vector u still in plane
		 *  minV and maxV as min/max sizes of vector v still in plane
		 */
		Vector3D distanceToPlane(Vector3D p, Vector3D r, Vector3D n, Vector3D u, 
				Vector3D v, float minU, float maxU, float minV, float maxV) const ;
		
		/** returns the first point on the ftd, where it intersects with a given straight
		  line (parameters point p and direction v)
		 */     
//		Vector3D intersectionFTD( Vector3D p, Vector3D v, bool sensitive = false) const ;
		
		/** returns the intersection point of a plane and a straight line
		 */
		Vector3D planeLineIntersection( Vector3D r, Vector3D n, Vector3D linePoint, Vector3D lineDir) const ;
		
		/** returns the confiningRatio of a point p in a plane when the plane (r,n) is reduced to a finite square
		 *  extensions in direction u (minU to maxU) and in direction v (minV to maxV)
		 */
		double confiningRatio( Vector3D p , Vector3D r, Vector3D n, Vector3D u, Vector3D v, 
				float minU, float maxU, float minV, float maxV ) const ;
		
		/** corrects the vector vPlane into the given borders, given by
		 *  direction u (minU to maxU) and in direction v (minV to maxV)
		 *  the corrected Vector is returned
		 */
		Vector3D correctToBorderPoint( Vector3D vPlane , Vector3D u, Vector3D v,
				float minU, float maxU, float minV, float maxV ) const ;
		
		bool isEqual( double valueOne , double valueTwo ) const ;
		
		bool isEqual( Vector3D p1 , Vector3D p2 ) const ;
		
		bool differsLess( double valueOne , double valueTwo ) const ;
		
		
		/** returns Phi corrected to Range -M_PI/2 to +M_PI/2
		 */
		double correctPhiRange( double Phi ) const ;
		
		/** returns Phi for a point   */
		double getPhiPoint( Vector3D p ) const ;
		
}; // class

} // namespace gear

#endif /* ifndef GEAR_FTDPARAMETERS_H */
