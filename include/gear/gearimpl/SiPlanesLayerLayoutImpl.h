// -*- C++ -*-
#ifndef GEAR_SiPlanesLayerLayoutImpl_H
#define GEAR_SiPlanesLayerLayoutImpl_H 1

#include "gear/SiPlanesLayerLayout.h"
#include <vector>

namespace gear {

/** Abstract description of layers in pixel beam telescope. <br>
 *  This assumes a number of silicon layers arranged perpendicular to the beam.
 * 
 * @author T. Klimkovich, DESY
 * @author I. Rubinskiy,  DESY
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
    double RotationXY;
    double RotationZX;
    double RotationZY;
    double SizeX ;
    double SizeY ;
    double Thickness ;
    double RadLength ;
    
     /** safe default contructor **/
  Layer() : ID(0), PositionX(0.), PositionY(0.), PositionZ(0.), RotationXY(0.), RotationZX(0.), RotationZY(0.),  SizeX(0.), SizeY(0.), Thickness(0.), RadLength(0.) { ; } ;
  } ;

  struct SensLayer {
    int ID ;
    double PositionX ;
    double PositionY ;
    double PositionZ ;
    double SizeX ;
    double SizeY ;
    double Thickness ;
    int NpixelX;
    int NpixelY;
    double PitchX;
    double PitchY;
    double Resolution;
    double Rotation1;
    double Rotation2;
    double Rotation3;
    double Rotation4;
    double RadLength ;
  } ;

  typedef std::vector<Layer> LayerVec ;
  typedef std::vector<SensLayer> SensLayerVec ;
  typedef Layer DUT ;
  typedef SensLayer SensDUT ;

  // Destructor.
  virtual ~SiPlanesLayerLayoutImpl() { /* nop */; }
  
  virtual int getNLayers() const { return _lVec.size() ; }
  
  virtual int getID(int layerIndex) const { return _lVec.at( layerIndex ).ID  ; }

  virtual double getLayerRadLength(int layerIndex) const { return _lVec.at( layerIndex ).RadLength  ; }
   
  virtual double getLayerPositionX(int layerIndex) const { return _lVec.at( layerIndex ).PositionX  ; }
  virtual double getLayerPositionY(int layerIndex) const { return _lVec.at( layerIndex ).PositionY  ; }
  virtual double getLayerPositionZ(int layerIndex) const { return _lVec.at( layerIndex ).PositionZ  ; }

  virtual double getLayerRotationXY(int layerIndex) const { return _lVec.at( layerIndex ).RotationXY  ; }
  virtual double getLayerRotationZX(int layerIndex) const { return _lVec.at( layerIndex ).RotationZX  ; }
  virtual double getLayerRotationZY(int layerIndex) const { return _lVec.at( layerIndex ).RotationZY  ; }

  virtual double getLayerSizeX(int layerIndex) const { return _lVec.at( layerIndex ).SizeX  ; }
  virtual double getLayerSizeY(int layerIndex) const { return _lVec.at( layerIndex ).SizeY  ; }
  virtual double getLayerThickness(int layerIndex) const { return _lVec.at( layerIndex ).Thickness  ; }

  virtual int getSensitiveID(int layerIndex) const { return _sVec.at( layerIndex ).ID  ; }

  virtual double getSensitiveRadLength(int layerIndex) const { return _sVec.at( layerIndex ).RadLength  ; }

  virtual double getSensitivePositionX(int layerIndex) const { return _sVec.at( layerIndex ).PositionX  ; }
  virtual double getSensitivePositionY(int layerIndex) const { return _sVec.at( layerIndex ).PositionY  ; }
  virtual double getSensitivePositionZ(int layerIndex) const { return _sVec.at( layerIndex ).PositionZ  ; }

  virtual double getSensitiveSizeX(int layerIndex) const { return _sVec.at( layerIndex ).SizeX  ; }
  virtual double getSensitiveSizeY(int layerIndex) const { return _sVec.at( layerIndex ).SizeY  ; }
  virtual double getSensitiveThickness(int layerIndex) const { return _sVec.at( layerIndex ).Thickness  ; }

  virtual int getSensitiveNpixelX(int layerIndex) const { return _sVec.at( layerIndex ).NpixelX  ; }
  virtual int getSensitiveNpixelY(int layerIndex) const { return _sVec.at( layerIndex ).NpixelY  ; }

  virtual double getSensitiveResolution(int layerIndex) const { return _sVec.at( layerIndex ).Resolution  ; }

  virtual double getSensitivePitchX(int layerIndex) const { return _sVec.at( layerIndex ).PitchX  ; }
  virtual double getSensitivePitchY(int layerIndex) const { return _sVec.at( layerIndex ).PitchY  ; }

  virtual double getSensitiveRotation1(int layerIndex) const { return _sVec.at( layerIndex ).Rotation1  ; }
  virtual double getSensitiveRotation2(int layerIndex) const { return _sVec.at( layerIndex ).Rotation2  ; }
  virtual double getSensitiveRotation3(int layerIndex) const { return _sVec.at( layerIndex ).Rotation3  ; }
  virtual double getSensitiveRotation4(int layerIndex) const { return _sVec.at( layerIndex ).Rotation4  ; }

  /** Add a new layer at the given position
   */
  virtual void addLayer(int layerID,
                        double layerPositionX, double layerPositionY, double layerPositionZ,
                        double layerRotationXY, double layerRotationZX, double layerRotationZY,
			double layerSizeX, double layerSizeY, double layerThickness,
			double layerRadLength,
			// sensitive
			int sensitiveID,
			double sensitivePositionX, double sensitivePositionY, double sensitivePositionZ,
			double sensitiveSizeX, double sensitiveSizeY, double sensitiveThickness,
			int sensitiveNpixelX, int sensitiveNpixelY,
			double sensitivePitchX,double sensitivePitchY,
			double sensitiveResolution,
			double Rotation1,
			double Rotation2,
			double Rotation3,
			double Rotation4,
			double sensitiveRadLength);


  // the DUT

  virtual int getDUTID() const { return _lDut.ID  ; }

  virtual double getDUTRadLength() const { return _lDut.RadLength  ; }

  virtual double getDUTPositionX() const { return _lDut.PositionX  ; }
  virtual double getDUTPositionY() const { return _lDut.PositionY  ; }
  virtual double getDUTPositionZ() const { return _lDut.PositionZ  ; }

  virtual double getDUTSizeX() const { return _lDut.SizeX  ; }
  virtual double getDUTSizeY() const { return _lDut.SizeY  ; }
  virtual double getDUTThickness() const { return _lDut.Thickness  ; }

  virtual int getDUTSensitiveID() const { return _sDut.ID  ; }

  virtual double getDUTSensitiveRadLength() const { return _sDut.RadLength  ; }

  virtual double getDUTSensitivePositionX() const { return _sDut.PositionX  ; }
  virtual double getDUTSensitivePositionY() const { return _sDut.PositionY  ; }
  virtual double getDUTSensitivePositionZ() const { return _sDut.PositionZ  ; }

  virtual double getDUTSensitiveSizeX() const { return _sDut.SizeX  ; }
  virtual double getDUTSensitiveSizeY() const { return _sDut.SizeY  ; }
  virtual double getDUTSensitiveThickness() const { return _sDut.Thickness  ; }

  virtual int getDUTSensitiveNpixelX() const { return _sDut.NpixelX  ; }
  virtual int getDUTSensitiveNpixelY() const { return _sDut.NpixelY  ; }

  virtual double getDUTSensitivePitchX() const { return _sDut.PitchX  ; }
  virtual double getDUTSensitivePitchY() const { return _sDut.PitchY  ; }

  virtual double getDUTSensitiveResolution() const { return _sDut.Resolution  ; }

  virtual double getDUTSensitiveRotation1() const { return _sDut.Rotation1  ; }
  virtual double getDUTSensitiveRotation2() const { return _sDut.Rotation2  ; }
  virtual double getDUTSensitiveRotation3() const { return _sDut.Rotation3  ; }
  virtual double getDUTSensitiveRotation4() const { return _sDut.Rotation4  ; }

  /** Add a DUT at the given position
   */

  virtual void addDUT(int dutID,
                      double dutPositionX, double dutPositionY, double dutPositionZ,
		      double dutSizeX, double dutSizeY, double dutThickness,
		      double dutRadLength,
		      // sensitive
		      int dutsensitiveID,
		      double dutsensitivePositionX, double dutsensitivePositionY, double dutsensitivePositionZ,
		      double dutsensitiveSizeX, double dutsensitiveSizeY, double dutsensitiveThickness,
		      int dutsensitiveNpixelX, int dutsensitiveNpixelY,
		      double dutsensitivePitchX,double dutsensitivePitchY,
		      double dutsensitiveResolution,
		      double dutsensitiveRotation1,
		      double dutsensitiveRotation2,
		      double dutsensitiveRotation3,
		      double dutsensitiveRotation4,
		      double dutsensitiveRadLength);
  
protected:

  typedef double MyMatrix[2][2];
  
  // Layer
  LayerVec _lVec ;
  // Sensitive layer
  SensLayerVec _sVec ;

  // DUT plane
  DUT _lDut ;
  // Sensitive of DUT
  SensDUT _sDut ;

private:


}; // class
} // namespace gear
#endif /* ifndef GEAR_SIPLANESLAYERLAYOUT_H */
