// -*- C++ -*-
#ifndef GEAR_SIPLANESLAYERLAYOUT_H
#define GEAR_SIPLANESLAYERLAYOUT_H 1

namespace gear {

/**Abstract description of layers in a pixel beam telescope.
 * @author T. Klimkovich, DESY
 * @author I. Rubinskiy,  DESY
 * @version $Id: 
 */
class SiPlanesLayerLayout  {

public: 
    /// Destructor.
    virtual ~SiPlanesLayerLayout() { /* nop */; }

    /** The total number of layers. */

    virtual int getNLayers() const = 0;

     /** Layer ID of nonsensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source.*/
    virtual int getID(int layerIndex) const = 0;

   /** The radiation length of nonsensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source.
     */
    virtual double getLayerRadLength(int layerIndex) const = 0;

    /** The radiation length of sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source.
     */
    virtual double getSensitiveRadLength(int layerIndex) const = 0;

    /** x position of the center of nonsensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source.
     */
    virtual double getLayerPositionX(int layerIndex) const = 0;

    /** y position of the center of nonsensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source.
     */
    virtual double getLayerPositionY(int layerIndex) const = 0;

    /** z position of the center of nonsensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */
    virtual double getLayerPositionZ(int layerIndex) const = 0;

    /** rotation angles according to Euler implementation scheme
     * (in XY plane -> around axis Z: gamma )
     */
    virtual double getLayerRotationXY(int layerIndex) const = 0;
    
    /** rotation angles according to Euler implmentation scheme
     * (in ZX plane -> around axis Y: beta  )
     */
    virtual double getLayerRotationZX(int layerIndex) const = 0;
  
    /** rotation angles according to Euler implmentation scheme
     *  (in ZY plane -> around axis X: alfa  )
     */
    virtual double getLayerRotationZY(int layerIndex) const = 0;

    virtual void setLayerRadLength(int layerIndex, double value)    = 0;
    virtual void setLayerPositionX(int layerIndex, double value)    = 0;
    virtual void setLayerPositionY(int layerIndex, double value)    = 0;
    virtual void setLayerPositionZ(int layerIndex, double value)    = 0;
    virtual void setLayerRotationXY(int layerIndex, double value)   = 0;
    virtual void setLayerRotationZX(int layerIndex, double value)   = 0;
    virtual void setLayerRotationZY(int layerIndex, double value)   = 0;
    virtual void setLayerSizeX(int layerIndex, double value)        = 0;
    virtual void setLayerSizeY(int layerIndex, double value)        = 0;
    virtual void setLayerThickness(int layerIndex, double value)    = 0;


    /** Size in x direction of nonsensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */	
    virtual double getLayerSizeX(int layerIndex) const = 0;

    /** Size in y direction of nonsensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */	
    virtual double getLayerSizeY(int layerIndex) const = 0;

    /** Thickness of nonsensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */	
    virtual double getLayerThickness(int layerIndex) const = 0;

    /** ID of sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source.*/

    virtual int getSensitiveID(int layerIndex) const = 0;

    /** x position of the center of sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source.
     */
    virtual double getSensitivePositionX(int layerIndex) const = 0;

    /** y position of the center of sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source.
     */
    virtual double getSensitivePositionY(int layerIndex) const = 0;

    /** z position of the center of sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source.
     */
    virtual double getSensitivePositionZ(int layerIndex) const = 0;

    /** rotation angles according to Euler implementation scheme
     * (in XY plane -> around axis Z: gamma )
     */
    virtual double getSensitiveRotationXY(int layerIndex) const = 0;
    
    /** rotation angles according to Euler implmentation scheme
     * (in ZX plane -> around axis Y: beta  )
     */
    virtual double getSensitiveRotationZX(int layerIndex) const = 0;
  
    /** rotation angles according to Euler implmentation scheme
     *  (in ZY plane -> around axis X: alfa  )
     */
    virtual double getSensitiveRotationZY(int layerIndex) const = 0;


    /** Size in x direction of sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */	
    virtual double getSensitiveSizeX(int layerIndex) const = 0;

    /** Size in y direction of sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */	
    virtual double getSensitiveSizeY(int layerIndex) const = 0;

    /** Thickness of sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */	
    virtual double getSensitiveThickness(int layerIndex) const = 0;

    /** Number of pixels in x direction in sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */
    virtual int getSensitiveNpixelX(int layerIndex) const = 0;

    /** Number of pixels in y direction in sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */
    virtual int getSensitiveNpixelY(int layerIndex) const = 0;

    /** Pitch size in x direction in sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */
    virtual double getSensitivePitchX(int layerIndex) const = 0;

    /** Pitch size in y direction in sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */
    virtual double getSensitivePitchY(int layerIndex) const = 0;

    /** Intrinsic resolution of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */
    virtual double getSensitiveResolution(int layerIndex) const = 0;
 
    /** Intrinsic resolution of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */
    virtual double getSensitiveResolutionX(int layerIndex) const = 0;

    /** Intrinsic resolution of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */
    virtual double getSensitiveResolutionY(int layerIndex) const = 0;


    /** First element (cos(theta)) of rotation matrix of sensitive volume 
     *  of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */
    virtual double getSensitiveRotation1(int layerIndex) const = 0;

    /** Second element (-sin(theta)) of rotation matrix of sensitive volume 
     *  of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */
    virtual double getSensitiveRotation2(int layerIndex) const = 0;

    /** Third element (sin(theta)) of rotation matrix of sensitive volume 
     *  of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */
    virtual double getSensitiveRotation3(int layerIndex) const = 0;

    /** Fourth element (cos(theta)) of rotation matrix of sensitive volume 
     *  of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */

    virtual double getSensitiveRotation4(int layerIndex) const = 0;

     /** ID of nonsensitive volume of the DUT.*/
    virtual int getDUTID() const = 0;

   /** The radiation length of nonsensitive volume of the DUT.
     */
    virtual double getDUTRadLength() const = 0;

    /** The radiation length of sensitive volume of the DUT.
     */
    virtual double getDUTSensitiveRadLength() const = 0;

    /** x position of the center of nonsensitive volume of the DUT. */ 
    virtual double getDUTPositionX() const = 0;

    /** y position of the center of nonsensitive volume of the DUT. */ 
    virtual double getDUTPositionY() const = 0;

    /** z position of the center of nonsensitive volume of the DUT. */ 
    virtual double getDUTPositionZ() const = 0;

    /** Size in x direction of nonsensitive volume of the DUT. */ 
    virtual double getDUTSizeX() const = 0;

    /** Size in y direction of nonsensitive volume of the DUT. */ 
    virtual double getDUTSizeY() const = 0;

    /** Thickness of nonsensitive layer of the DUT. */ 
    virtual double getDUTThickness() const = 0;

    /** ID of sensitive volume of the DUT. */
    virtual int getDUTSensitiveID() const = 0;

    /** x position of the center of sensitive volume of the DUT. */ 
    virtual double getDUTSensitivePositionX() const = 0;

    /** y position of the center of sensitive volume of the DUT. */ 
    virtual double getDUTSensitivePositionY() const = 0;

    /** z position of the center of sensitive volume of the DUT. */ 
    virtual double getDUTSensitivePositionZ() const = 0;

    /** Size in x direction of sensitive volume of the DUT. */ 
    virtual double getDUTSensitiveSizeX() const = 0;

    /** Size in y direction of sensitive volume of the DUT. */ 
    virtual double getDUTSensitiveSizeY() const = 0;

    /** Thickness of sensitive volume of the DUT. */ 
    virtual double getDUTSensitiveThickness() const = 0;

    /** Number of pixels in x direction of sensitive volume of the DUT. */ 
    virtual int getDUTSensitiveNpixelX() const = 0;

    /** Number of pixels in y direction of sensitive volume of the DUT. */ 
    virtual int getDUTSensitiveNpixelY() const = 0;

    /** Pitch size in x direction of sensitive volume of the DUT. */ 
    virtual double getDUTSensitivePitchX() const = 0;

    /** Pitch size in y direction of sensitive volume of the DUT. */ 
    virtual double getDUTSensitivePitchY() const = 0;

    /** Intrinsic resolution of the DUT, not relevant. Here just for comleteness. */ 
    virtual double getDUTSensitiveResolution() const = 0;

    /** First element (cos(theta)) of rotation matrix of sensitive volume 
     * of the DUT.
     */
    virtual double getDUTSensitiveRotation1() const = 0;

    /** Second element (-sin(theta)) of rotation matrix of sensitive volume 
     * of the DUT.
     */
    virtual double getDUTSensitiveRotation2() const = 0;

    /** Third element (sin(theta)) of rotation matrix of sensitive volume 
     * of the DUT.
     */
    virtual double getDUTSensitiveRotation3() const = 0;

    /** Fourth element (cos(theta)) of rotation matrix of sensitive volume 
     * of the DUT.
     */
    virtual double getDUTSensitiveRotation4() const = 0;
}; // class
} // namespace gear
#endif /* ifndef GEAR_SIPLANESLAYERLAYOUT_H */
