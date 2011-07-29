// -*- C++ _*_
#ifndef GEAR_FTDPARAMETERS_H
#define GEAR_FTDPARAMETERS_H 1

#include "GEAR.h"
#include "gear/GearParameters.h"

namespace gear 
{

class FTDLayerLayout;

/** Geometry properties of a FTD detector needed for reconstruction code. <br>
 *  <p>The FTD is assumed to consist of a number of disks (layers) located along the Z-axis. 
 *  Each disk consists of a number of trapezoid ladders (petals) that are uniformly distributed
 *  in a circle around the  Z-axis .<br>
 *  The sensitive volumes are rectangular pixels for the disks 1 and 2 covering the petal face
 *  nearest the IP; the remaining disks mount trapezoids covering both faces of the petal (we 
 *  assume 1 sensitive layer per face; in the current implementation there are two).
 *  The sensitive volumes can be placed relative to the (insensitive) ladders.
 *  @see addLayer .</p>
 *  <p>The shell is described by a cone for the inner part; inner radius min and inner radius max;
 *     a cylinder defined by a its radius is describing the shell farest from the Z-axis;
 *     and it's half length in z.</p>
 *
 *  @author J. Duarte; IFCA (based from VXDParametersImpl)
 *  @version $Id: 
 */
	
class FTDParameters : virtual public GearParameters
{   
  
public:
    /// Destructor.
    virtual ~FTDParameters() { /* nop */; }

    static const int PIXEL    = 1;
    static const int STRIP    = 2;
   
    static const int FACE    = 1;
    static const int BACK    = 2;

    /** The layer layout in the Vertex */
    virtual const FTDLayerLayout& getFTDLayerLayout() const  = 0;
    
    /** The half length (z) of the support shell in mm.
     */
    virtual double getShellHalfLength() const  = 0;
   
    /** The inner minimum radius of the cone support shell in mm.
    */
    virtual double getShellInnerRadiusMin() const = 0;
	
    /** The inner maximum radius of the cone support shell in mm.
    */
    virtual double getShellInnerRadiusMax() const = 0;
    
    /** The outer radius of the support shell in mm.
    */
    virtual double getShellOuterRadius() const = 0;
    
    /** The radiation length in the support shell
    */
    virtual double getShellRadLength() const = 0;
         
    /** returns the layerIndex which correspond to the layer where the point
    * is. If the point is not inside any layer returns -1
    */
    virtual int getLayerIndex( const Vector3D & p ) const = 0;
    
    /** returns the ladder Index which correspond to the layer where the point
    * is. If the point is not inside any layer returns -1
    */
    virtual int getLadderIndex( const Vector3D & p ) const = 0;
    
    /** returns the layerIndex which correspond to the layer where the point
    * is. If the point is not inside any layer returns -1
    */
    virtual int getSensitiveIndex( const Vector3D & p ) const = 0;
};

}   // namespace gear
#endif /* ifndef GEAR_FTDPARAMETERS_H */
