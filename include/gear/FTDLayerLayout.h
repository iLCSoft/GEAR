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
    
    /** The z-position of the centroid of the disk structure
     */
    //FIXME: TO BE DEPRECATED OR PRIVATE? All the positions having to 
    //       return this class is relative to the petals or
    //       sensors. to be checked
    virtual double getZposition(const int & layerIndex) const = 0;

    /** Z-offset of the petals in a staggered setup - the z position of the 
     *  even numbered petals is getZposition(i)+getZoffsetSign0(i)*getZoffset(i),
     *  the odd numbered petals: getZposition(i)-getZoffsetSign0(i)*getZoffset(i),
     */
    // FIXME: TO BE DEPRECATED OR PRIVATE? To be checked
    virtual double getZoffset(const int & layerIndex) const = 0;
    
    /** Z-offset sign of the petal 0 in a staggered setup - the z position of the 
     *  petals is zposition + zsign0^{petalIndex}*zoffset 
     */
    virtual double getZoffsetSign0(const int & layerIndex) const = 0;
    

    /** Azimuthal angle of the first petal (angle between petal centroid and x-axis
      * for the petal indexed as 0).
     */
    virtual double getPhi0(const int & layerIndex) const = 0;
    
    
    /** Angle of rotation in the own plane of a petal in a tilted configuration.
     */
    virtual double getAlpha(int layerIndex) const = 0;
    
  // description of the support petal  =========================================================================================
  // this is a trapezoidal frame really
  //  => we need a few additional parameters to desribe the 
  //     holes in petal ....
  

  
    /** The position of the support in z direction in mm for the petalIndex petal
      * in layer layerIndex - layer indexing starting at 0 from the layer closest to IP.
      * Petal indexing starting at 0 for the petal placed in the y-axis and grows
      * with positive rotation around Z-axis.
      * The position is defined in the centroid point of the support.
     */
    virtual double getSupportZposition(const int & layerIndex,const int & petalIndex) const = 0;
   

     /** The R-min of the support petals in the XY-plane in mm, for the layer layerIndex - layer
       * indexing starts at 0 for the layer closest to IP
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
    
    /** The length (x-direction) of the smallest edge of the trapezoid support  in
      * layer layerIndex - layer indexing starting at 0 for the layer closest to IP
     */ 
    // FIXME: Change Length <--> Width (I think is the usual notation)
    virtual double getSupportLengthMin(int layerIndex) const = 0;
    
    /** The length (x-direction) of the largest edge of the trapezoid support in
      * layer layerIndex - layer indexing starting at 0 for the layer closest to IP
      */
    virtual double getSupportLengthMax(int layerIndex) const = 0;
    
    /** The width of the support in layer in mm for petals in layer layerIndex -
     *  layer indexing starting at 0 from the layer closest to IP.
     */
    virtual double getSupportWidth(int layerIndex) const = 0;
    



   // description of the sensitive petal  =========================================================================================


    /** The position of the sensitive in z direction in mm for sensor sensorIndex of
     *  the petal support petalIndex in layer layerIndex -
     *  layer indexing starting at 0 from the layer closest to IP.
     *  Petal indexing starting at 0 for the petal placed in the y-axis and grows
     *  with positive rotation around Z-axis.
     *  Sensor indexing is defined from Y-axis higher values, 1 (UP) 2 (DOWN) for the 
     *  sensor facing the IP;  and 3 (UP), 4 (DOWN) for the sensor backing the IP.
     *  The z-position is defined in the centroid point of the sensor.
     */
    virtual double getSensitiveZposition(const int & layerIndex, const int & petalIndex, const int & sensorId) const = 0;

   /** The R-min of the petal in the XY-plane in mm for sensors in layer layerIndex - layer
     * indexing starts at 0 for the layer closest to IP.
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
    



    /** Azimuthal angle of the petal petalIndex Centroid at layer layerIndex.
     *  Layer indexing starting at 0 from the layer closest to IP.
     *  Petal indexing starting at 0 for the petal placed in the y-axis and grows
     *  with positive rotation around Z-axis.
     */
    virtual double getPhiPetalCd(const int & layerIndex, const int & petalIndex) const = 0;
    
    /** returns maximum radius for a given layer   */
    //FIXME: To be checked if necessary
    virtual double getMaxRadius(const int & layerIndex,const bool & sensitive=false ) const = 0;
    
    /** returns starting phi for first petal in layer layerIndex (on side facing IP)
     */
    //FIXME: To be checked if necessary
    virtual double getStartPhi(const int & layerIndex, const int & petalInd, 
    		const bool &sensitive=false ) const = 0;
    
    /** returns ending phi for first petal in layer layerIndex (on side facing IP)
     */
    //FIXME: To be checked if necessary
    virtual double getEndPhi(const int & layerIndex,const int & petalInd, 
    		const bool & sensitive=false ) const = 0;
    
    /** returns thickness as viewed under the incidence angles phi and theta in 
     *  layer layerIndex.
     *  theta is the angle defined between the Z-axis (perpendicular to the petal plane)
     *  and the plane of the petal; phi is the incident angle defined in the ZX plane
     */
    //FIXME: To be checked if necessary
    virtual double getThicknessForAngle(const int & layerIndex, const double & tetha,
    		const double & phi, const bool & sensitive=false ) const = 0;
};

}   // namespace gear
#endif /* ifndef GEAR_FTDLAYERLAYOUT_H */
