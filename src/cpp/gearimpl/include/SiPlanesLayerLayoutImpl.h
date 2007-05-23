// -*- C++ -*-
#ifndef GEAR_SiPlanesLayerLayoutImpl_H
#define GEAR_SiPlanesLayerLayoutImpl_H 1

#include "gear/SiPlanesLayerLayout.h"
#include <vector>

namespace gear {

/** Abstract description of layers in beam telescope or FTD detector. <br>
 *  This assumes a number of silicon layers, arranged perpendicular to the beam <br>
 *  The sensitive area is assumed to be inside the layers but can be positioned independently.
 * 
 * @author T. Klimkovich, DESY
 * @version $Id: 
 */

class SiPlanesLayerLayoutImpl : public SiPlanesLayerLayout {

public: 
  
  /** Helper class for layer properties */
  struct Layer {
    int ID ;
    double PositionX ;
    double PositionY ;
    double PositionZ ;
    double SizeX ;
    double SizeY ;
    double Thickness ;
    double RadLength ;
  } ;

  typedef std::vector<Layer> LayerVec ;
  //  typedef Layer DUT ;
  typedef Layer DUT ;

  // Destructor.
  virtual ~SiPlanesLayerLayoutImpl() { /* nop */; }
  
  /** The total number of layers.
   */
  virtual int getNLayers() const { return _lVec.size() ; }
  
  /** The ID of the layer.
   */
  virtual int getID(int layerIndex) const { return _lVec.at( layerIndex ).ID  ; }

  /** The radiation length in the support structure of layer layerIndex - layer indexing starts at 0
   *  for the layer closest to the beam source
   */
  virtual double getLayerRadLength(int layerIndex) const { return _lVec.at( layerIndex ).RadLength  ; }
  
  /** The position of layer layerIndex
   */
  virtual double getLayerPositionX(int layerIndex) const { return _lVec.at( layerIndex ).PositionX  ; }
  virtual double getLayerPositionY(int layerIndex) const { return _lVec.at( layerIndex ).PositionY  ; }
  virtual double getLayerPositionZ(int layerIndex) const { return _lVec.at( layerIndex ).PositionZ  ; }

  /** The size in mm of layer layerIndex.
   */
  virtual double getLayerSizeX(int layerIndex) const { return _lVec.at( layerIndex ).SizeX  ; }
  virtual double getLayerSizeY(int layerIndex) const { return _lVec.at( layerIndex ).SizeY  ; }
  virtual double getLayerThickness(int layerIndex) const { return _lVec.at( layerIndex ).Thickness  ; }

  virtual double getSensitiveRadLength(int layerIndex) const { return _sVec.at( layerIndex ).RadLength  ; }

  /** The position of sensitive area in layer layerIndex
   */
  virtual double getSensitivePositionX(int layerIndex) const { return _sVec.at( layerIndex ).PositionX  ; }
  virtual double getSensitivePositionY(int layerIndex) const { return _sVec.at( layerIndex ).PositionY  ; }
  virtual double getSensitivePositionZ(int layerIndex) const { return _sVec.at( layerIndex ).PositionZ  ; }

  /** The size in mm of the sensitive area in layer layerIndex.
   */
  virtual double getSensitiveSizeX(int layerIndex) const { return _sVec.at( layerIndex ).SizeX  ; }
  virtual double getSensitiveSizeY(int layerIndex) const { return _sVec.at( layerIndex ).SizeY  ; }
  virtual double getSensitiveThickness(int layerIndex) const { return _sVec.at( layerIndex ).Thickness  ; }

  /** Add a new layer at the given positon
   */
  virtual void addLayer(int layerID,
                        double layerPositionX, double layerPositionY, double layerPositionZ,
			double layerSizeX, double layerSizeY, double layerThickness,
			double layerRadLength,
			// sensitive
			double sensitivePositionX, double sensitivePositionY, double sensitivePositionZ,
			double sensitiveSizeX, double sensitiveSizeY, double sensitiveThickness,
			double sensitiveRadLength);


  /** The ID of the DUT
   */
  virtual int getDUTID() const { return _lDut.ID  ; }

  /** The radiation length in the support structure of the DUT.
   */
  virtual double getDUTRadLength() const { return _lDut.RadLength  ; }
  
  /** The position of DUT.
   */
  virtual double getDUTPositionX() const { return _lDut.PositionX  ; }
  virtual double getDUTPositionY() const { return _lDut.PositionY  ; }
  virtual double getDUTPositionZ() const { return _lDut.PositionZ  ; }

  /** The size in mm of DUT.
   */
  virtual double getDUTSizeX() const { return _lDut.SizeX  ; }
  virtual double getDUTSizeY() const { return _lDut.SizeY  ; }
  virtual double getDUTThickness() const { return _lDut.Thickness  ; }

  /** The radiation length in the sensitive volume of the DUT.
   */
  virtual double getDUTSensitiveRadLength() const { return _sDut.RadLength  ; }

  /** The position of sensitive area of DUT.
   */
  virtual double getDUTSensitivePositionX() const { return _sDut.PositionX  ; }
  virtual double getDUTSensitivePositionY() const { return _sDut.PositionY  ; }
  virtual double getDUTSensitivePositionZ() const { return _sDut.PositionZ  ; }

  /** The size in mm of the sensitive area of DUT.
   */
  virtual double getDUTSensitiveSizeX() const { return _sDut.SizeX  ; }
  virtual double getDUTSensitiveSizeY() const { return _sDut.SizeY  ; }
  virtual double getDUTSensitiveThickness() const { return _sDut.Thickness  ; }

  virtual void addDUT(int dutID,
                        double dutPositionX, double dutPositionY, double dutPositionZ,
			double dutSizeX, double dutSizeY, double dutThickness,
			double dutRadLength,
			// sensitive
			double dutsensitivePositionX, double dutsensitivePositionY, double dutsensitivePositionZ,
			double dutsensitiveSizeX, double dutsensitiveSizeY, double dutsensitiveThickness,
			double dutsensitiveRadLength);
  
protected:

  // Layer
  LayerVec _lVec ;
  // Sensitive
  LayerVec _sVec ;

  // DUT plane
  DUT _lDut ;
  // Sensitive of DUT
  DUT _sDut ;

private:


}; // class
} // namespace gear
#endif /* ifndef GEAR_SIPLANESLAYERLAYOUT_H */
