// -*- C++ _*_
#ifndef GEAR_FTDLAYERLAYOUT_H
#define GEAR_FTDLAYERLAYOUT_H 1

namespace gear 
{

/** Abstract description of layers in a FTD detector. <br>
 *  This assumes a symmetric layout of ladders (petals), arranged in layers (disk) 
 *  a z-position to IP.  <br>
 *  The sensitive area is assumed to be inside the ladders but can be positioned independently.
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
    
    /** The number of ladders in the layer layerIndex - layer indexing starts at 0
     *  for the layer closest to IP.  
     */
    virtual int getNLadders(int layerIndex) const = 0;
    
    /** The sensor type of the disk: pixel of micro-strip
    */
    virtual double getSensorType(int layerIndex) const = 0;
    
    /** Angular half-width of the petals of a layer
      */
    virtual double getPhiHalfDistance(int layerIndex) const = 0;
    
    /** Z-offset of the ladder structure
      */
    virtual double getZoffset(const int & layerIndex) const = 0;
    
    /** Angular distance of a petal
     */
    virtual double getPhi(const int & layerIndex) const = 0;
    
    
    /** Angle of rotation in the own plane of a petal
     */
    virtual double getAlpha(int layerIndex) const = 0;
    
    /** The position of the ladder in z direction in mm for ladders in layer layerIndex -
     *  layer indexing starting at 0 from the layer closest to IP.
     */
    virtual double getZposition(int layerIndex) const = 0;
    
    /** The R-min of the ladder in the XY-plane in mm for ladders
     */
    virtual double getLadderRinner(int layerIndex) const = 0;
    
    /** The radiation length in the support structure ladders of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to IP.
     */
    virtual double getLadderRadLength(int layerIndex) const = 0;
    
    /** The thickness in mm of the ladders in layerIndex - layer indexing starting at 0
     *  for the layer closest to IP.
     */
    virtual double getLadderThickness(int layerIndex) const = 0;
    
    /** The length (x-direction) of the smallest edge of the trapezoid (petal) 
     */
    virtual double getLadderLengthMin(int layerIndex) const = 0;
    
    /** The length (x-direction) of the largest edge of the trapezoid (petal) 
     */
    virtual double getLadderLengthMax(int layerIndex) const = 0;
    
    /** The width of the ladder in layer in mm for ladders in layer layerIndex -
     *  layer indexing starting at 0 from the layer closest to IP.
     */
    virtual double getLadderWidth(int layerIndex) const = 0;
    
    /** The R-min of the ladder in the XY-plane in mm for ladders
     */
    virtual double getSensitiveRinner(int layerIndex) const = 0;
    
    /** The radiation length in sensitive volumes in layer layerIndex - layer indexing starts at 0
     *  for the layer closest to IP.
     */
    virtual double getSensitiveRadLength(int layerIndex) const = 0;
    
    /** The thickness in mm of the sensitive area in ladders in layer layerIndex.
     */
    virtual double getSensitiveThickness(int layerIndex) const = 0;
    
    /** Same as getLadderLengthMin() except for the sensitive part of the ladder.
     * @see getLadderLengthMin
     */
    virtual double getSensitiveLengthMin(int layerIndex) const = 0;
    
    /** Same as getLadderLengthMax() except for the sensitive part of the ladder.
     * @see getLadderLengthMax
     */
    virtual double getSensitiveLengthMax(int layerIndex) const = 0;
    
    /** The width of the sensitive area in ladders in layer layerIndex in mm.
     */
    virtual double getSensitiveWidth(int layerIndex) const = 0;
    
    /** Azimuthal angle of the vector defined by the Z-axis to the j-petal x-positive, y-positive
     *  (edge side).
     *  phi smallest corresponds to the first ladder's closests to the positive X-axis.
     *  The layerIndex starts at 0 for the layer closest to IP.
     */
    virtual double getPhiStructure(const int & layerIndex, const int & ladderIndex, 
    		const bool & sensitive) const = 0;
    
    /** returns maximum radius for a given layer   */
    virtual double getMaxRadius(const int & layerIndex,const bool & sensitive=false ) const = 0;
    
    /** returns starting phi for first ladder in layer layerIndex (on side facing IP)
     */
    virtual double getStartPhi(const int & layerIndex, const int & ladderInd, 
    		const bool &sensitive=false ) const = 0;
    
    /** returns ending phi for first ladder in layer layerIndex (on side facing IP)
     */
    virtual double getEndPhi(const int & layerIndex,const int & ladderInd, 
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
