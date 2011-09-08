// -*- C++ _*_
#ifndef GEAR_FTDLAYERLAYOUT_H
#define GEAR_FTDLAYERLAYOUT_H 1

namespace gear 
{

/** Abstract description of layers in a FTD detector. <br>
 *  This assumes a symmetric layout of petals (isocelete(?) trapezoids), arranged in layers (disk) 
 *  a z-position to IP.  <br>
 *  The sensitive area is assumed to be inside the petals but can be positioned independently.
 * 
 * @author J. Duarte Campderros, IFCA (Based at VXDLayerLayout)
 * @version $Id: 
 */

class FTDLayerLayout {

public:
    /** Destructor.
    */
    virtual ~FTDLayerLayout() { /* nop */ ; }
    
    /** The total number of layers. 
    */
    virtual int getNLayers() const = 0;
    
    /** The number of petals in the layer layerIndex - layer indexing starts at 0
     *  for the layer closest to IP.  
     */
    virtual int getNPetals(int layerIndex) const = 0;
    
    /** The sensor type of the disk: pixel of micro-strip
    */
    virtual double getSensorType(int layerIndex) const = 0;
    
    /** Angular half-width of the petals of a layer - half of the opening angle of the trapezoid
      */
    virtual double getPhiHalfDistance(int layerIndex) const = 0;
    

    /** Z-offset of the petals in a staggered setup - the z position of the 
     *  even numbered petals is zposition + zoffset and zposition - zoffset for the 
     *  odd numbered. 
     */
    virtual double getZoffset(const int & layerIndex) const = 0;
    

    /** Azimuthal angle of the first petal (angle between petal centroid and x-axis).
     */
    virtual double getPhi0(const int & layerIndex) const = 0;
    
    
    /** Angle of rotation in the own plane of a petal in a tilted configuration.
     */
    virtual double getAlpha(int layerIndex) const = 0;
    
  // description of the support petal  =========================================================================================
  // this is a trapezoidal frame really
  //  => we need a few additional parameters to desribe the 
  //     holes in petal ....

  
    /** The mean z position of the support  petal in layer layerIndex -
     *  layer indexing starting at 0 from the layer closest to IP.
     */
    virtual double getSupportZposition(int layerIndex) const = 0;
   

     /** The R-min of the support petals in the XY-plane in mm
     */
    virtual double getSupportRinner(int layerIndex) const = 0;
    
    /** The radiation length in the support structure petals of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to IP.
     */
    virtual double getSupportRadLength(int layerIndex) const = 0;
    
    /** The thickness in mm of the supports in layerIndex - layer indexing starting at 0
     *  for the layer closest to IP.
     */
    virtual double getSupportThickness(int layerIndex) const = 0;
    
    /** The length (x-direction) of the smallest edge of the trapezoid (support) 
     */
    virtual double getSupportLengthMin(int layerIndex) const = 0;
    
    /** The length (x-direction) of the largest edge of the trapezoid (support) 
     */
    virtual double getSupportLengthMax(int layerIndex) const = 0;
    
    /** The width of the support in layer in mm for petals in layer layerIndex -
     *  layer indexing starting at 0 from the layer closest to IP.
     */
    virtual double getSupportWidth(int layerIndex) const = 0;
    



   // description of the sensitive petal  =========================================================================================


    /** The mean z position of the sensitive  petal in layer layerIndex -
     *  layer indexing starting at 0 from the layer closest to IP.
     */
    virtual double getSensitiveZposition(int layerIndex) const = 0;

   /** The R-min of the petal in the XY-plane in mm for petals
     */
    virtual double getSensitiveRinner(int layerIndex) const = 0;
    
    /** The radiation length in sensitive volumes in layer layerIndex - layer indexing starts at 0
     *  for the layer closest to IP.
     */
    virtual double getSensitiveRadLength(int layerIndex) const = 0;
    
    /** The thickness in mm of the sensitive area in petals in layer layerIndex.
     */
    virtual double getSensitiveThickness(int layerIndex) const = 0;
    
    /** Same as getPetalLengthMin() except for the sensitive part of the petal.
     * @see getPetalLengthMin
     */
    virtual double getSensitiveLengthMin(int layerIndex) const = 0;
    
    /** Same as getPetalLengthMax() except for the sensitive part of the petal.
     * @see getPetalLengthMax
     */
    virtual double getSensitiveLengthMax(int layerIndex) const = 0;
    
    /** The width of the sensitive area in petals in layer layerIndex in mm.
     */
    virtual double getSensitiveWidth(int layerIndex) const = 0;
    



    /** Azimuthal angle of the vector defined by the Z-axis to the j-petal x-positive, y-positive
     *  (edge side).
     *  phi smallest corresponds to the first petal's closests to the positive X-axis.
     *  The layerIndex starts at 0 for the layer closest to IP.
     */
    virtual double getPhiStructure(const int & layerIndex, const int & petalIndex, 
    		const bool & sensitive) const = 0;
    
    /** returns maximum radius for a given layer   */
    virtual double getMaxRadius(const int & layerIndex,const bool & sensitive=false ) const = 0;
    
    /** returns starting phi for first petal in layer layerIndex (on side facing IP)
     */
    virtual double getStartPhi(const int & layerIndex, const int & petalInd, 
    		const bool &sensitive=false ) const = 0;
    
    /** returns ending phi for first petal in layer layerIndex (on side facing IP)
     */
    virtual double getEndPhi(const int & layerIndex,const int & petalInd, 
    		const bool & sensitive=false ) const = 0;
    
    /** returns thickness as viewed under the incidence angles phi and theta in 
     *  layer layerIndex.
     *  theta is the angle defined between the Z-axis (perpendicular to the petal plane)
     *  and the plane of the petal; phi is the incident angle defined in the ZX plane
     */
    virtual double getThicknessForAngle(const int & layerIndex, const double & tetha,
    		const double & phi, const bool & sensitive=false ) const = 0;
};

}   // namespace gear
#endif /* ifndef GEAR_FTDLAYERLAYOUT_H */
