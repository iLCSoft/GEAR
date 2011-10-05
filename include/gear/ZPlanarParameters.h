
// -*- C++ -*-
#ifndef GEAR_ZPlanarParameters_H
#define GEAR_ZPlanarParameters_H 1

#include "GEAR.h"
#include "gear/GearParameters.h"

namespace gear {

class ZPlanarLayerLayout;

  /** Geometry properties of a vertex detector needed for reconstruction code. <br>
   *  <p>The vertex is assumed to consist of a number of layers. Each layer consists of
   *  a number of rectangular ladders that are uniformly distributed in a circle around the IP.<br>
   *  The sensitive volumes can be placed relative to the (insensitive) ladders.
   *  @see addLayer .</p>
   *  <p>The shell is described by outer and inner radius and it's half length in z.</p>
   *  <p>The gap is assumed to be symetrical around z==0 and goes through all layers and ladders.</p>
   *  
   *
   *  @author R. Lippe, DESY
   *  @version $Id: 
   */
	
class ZPlanarParameters : virtual public GearParameters {

public: 
    /// Destructor.
    virtual ~ZPlanarParameters() { /* nop */; }

  // static constants (enums for the type of the detetctor (layout etc) )

    static const int CCD    = 1 ;
    static const int CMOS   = 2 ;
    static const int HYBRID = 3 ;

    /** The layer layout in the detector */
   virtual const ZPlanarLayerLayout & getZPlanarLayerLayout() const = 0;

   /** fix for backward compatibility */  
    inline const ZPlanarLayerLayout& getVXDLayerLayout() const { return getZPlanarLayerLayout() ; } 
  
    /** The type of detector - to be defined by users...
    */
    virtual int getType() const = 0;

    /** For backward compatibility: The type of detector: ZPlanarParameters.CCD, ZPlanarParameters.CMOS or
     *  ZPlanarParameters.HYBRID.
     */
    virtual int getVXDType() const { return getType() ; } 

    /** The half length (z) of the support shell in mm (w/o gap).
     */
    virtual double getShellHalfLength() const = 0;

    /** The length of the gap in mm (gap position at z=0)
     */
    virtual double getShellGap() const = 0;

    /** The inner radius of the support shell in mm.
     */
    virtual double getShellInnerRadius() const = 0;

    /** The outer radius of the support shell in mm.
     */
    virtual double getShellOuterRadius() const = 0;

    /** The radiation length in the support shell
     */
    virtual double getShellRadLength() const = 0;

    /** returns whether a point is inside a ladder 
     */
  virtual bool isPointInLadder(Vector3D p ) const = 0;

  /** returns wheter a point is inside a sensitive volume- if sensorID != 0 the sensorID is returned in case the point
   *  is inside the sensitive
   */
  virtual bool isPointInSensitive(Vector3D p, SensorID* sensorID=0 ) const = 0;

    /** returns vector from point to nearest ladder
     */
    virtual Vector3D distanceToNearestLadder(Vector3D p) const = 0;

    /** returns vector from point to nearest sensitive volume
     */
    virtual Vector3D distanceToNearestSensitive(Vector3D p) const = 0;

    /** returns the first point where a given straight line
     *  (parameters point p and direction v)  crosses a ladder
     *  (0,0,0) is returned if no intersection can be found.
     *  @param p  a point on the line. This will be the starting point.
     *  @param v  a vector along the line    
     */	
    virtual Vector3D intersectionLadder(Vector3D p, Vector3D v) const = 0;

    /** returns the first point where a given strainght line
     *  (parameters point p and direction v)  crosses a sensitive volume
     *  (0,0,0) is returned if no intersection can be found.
     *  @param p  a point on the line. This will be the starting point.
     *  @param v  a vector along the line
     */	
    virtual Vector3D intersectionSensitive(Vector3D p, Vector3D v) const = 0;
}; // class
} // namespace gear
#endif /* ifndef GEAR_ZPlanarParameters_H */
