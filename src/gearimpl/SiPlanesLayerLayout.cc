#include "gearimpl/SiPlanesLayerLayoutImpl.h"
#include <math.h>

/** Abstract description of layers in pixel beam telescope with or without DUT.
   *  This assumes a number of silicon layers, arranged perpendicular to the beam.
   *  @author T Klimkovich, DESY
   *  @author I. Rubinskiy, DESY
   *  @version $Id: 
   */

namespace gear{

  void SiPlanesLayerLayoutImpl::addLayer( // old interface method // overloaded to reuse new one with 5 more parameters.
			      // layer
			      int layerID,	 
			      double layerPositionX, double layerPositionY, double layerPositionZ,
			      // no rotations of the support ladder neither
			      double layerSizeX, double layerSizeY, double layerThickness,
			      double layerRadLength,
			      // sensitive
			      int sensitiveID,
			      double sensitivePositionX, double sensitivePositionY, double sensitivePositionZ,
			      // no sensitive layer Rotations previously, as the sensitive layer was bound to the ladder ("layer" here)
			      double sensitiveSizeX, double sensitiveSizeY, double sensitiveThickness,
			      int sensitiveNpixelX, int sensitiveNpixelY,
			      double sensitivePitchX,double sensitivePitchY,
			      double sensitiveResolution,
			      // no resolution X  
			      // and Y
			      double sensitiveRotation1,
			      double sensitiveRotation2,
			      double sensitiveRotation3,
			      double sensitiveRotation4,
			      double sensitiveRadLength )
  {
     addLayer( layerID, layerPositionX, layerPositionY, layerPositionZ,
                        0., 0., 0.,
               	        layerSizeX, layerSizeY, layerThickness,
		        layerRadLength,
		        // sensitive
		        sensitiveID,
		        sensitivePositionX, sensitivePositionY, sensitivePositionZ,
		        0., 0., 0., // pass zeros as these fields were never present in the old interface
		        sensitiveSizeX, sensitiveSizeY, sensitiveThickness,
		        sensitiveNpixelX, sensitiveNpixelY,
		        sensitivePitchX,sensitivePitchY,
		        sensitiveResolution,
		        0., // pass zeros as X and Y resolution fields 
		        0., // were not present in the old interface
		        sensitiveRotation1,
		        sensitiveRotation2,
		        sensitiveRotation3,
		        sensitiveRotation4,
  		        sensitiveRadLength );

  }

  void SiPlanesLayerLayoutImpl::addLayer(
			      // layer
			      int layerID,	 
			      double layerPositionX, double layerPositionY, double layerPositionZ,
			      double layerRotationXY, double layerRotationZX, double layerRotationZY,
			      double layerSizeX, double layerSizeY, double layerThickness,
			      double layerRadLength,
			      // sensitive
			      int sensitiveID,
			      double sensitivePositionX, double sensitivePositionY, double sensitivePositionZ,
			      double sensitiveRotationXY, double sensitiveRotationZX, double sensitiveRotationZY,
			      double sensitiveSizeX, double sensitiveSizeY, double sensitiveThickness,
			      int sensitiveNpixelX, int sensitiveNpixelY,
			      double sensitivePitchX,double sensitivePitchY,
			      double sensitiveResolution,
			      double sensitiveResolutionX,
			      double sensitiveResolutionY,
			      double sensitiveRotation1,
			      double sensitiveRotation2,
			      double sensitiveRotation3,
			      double sensitiveRotation4,
			      double sensitiveRadLength )
  {

    Layer lL ;
    SensLayer sL ;
    lL.ID          = layerID ;
    lL.PositionX   = layerPositionX ;
    lL.PositionY   = layerPositionY ;
    lL.PositionZ   = layerPositionZ ;
    lL.RotationXY  = layerRotationXY;
    lL.RotationZX  = layerRotationZX;
    lL.RotationZY  = layerRotationZY;
    lL.SizeX       = layerSizeX ;
    lL.SizeY       = layerSizeY ;
    lL.Thickness   = layerThickness ;
    lL.RadLength   = layerRadLength ;

    sL.ID          = sensitiveID ;
    sL.PositionX   = sensitivePositionX ;
    sL.PositionY   = sensitivePositionY ;
    sL.PositionZ   = sensitivePositionZ ;
    sL.RotationXY  = sensitiveRotationXY;
    sL.RotationZX  = sensitiveRotationZX;
    sL.RotationZY  = sensitiveRotationZY;
    sL.SizeX       = sensitiveSizeX ;
    sL.SizeY       = sensitiveSizeY ;
    sL.Thickness   = sensitiveThickness ;
    sL.NpixelX     = sensitiveNpixelX;
    sL.NpixelY     = sensitiveNpixelY;
    sL.PitchX      = sensitivePitchX;
    sL.PitchY      = sensitivePitchY;
    sL.Resolution  = sensitiveResolution,
    sL.ResolutionX = sensitiveResolutionX,
    sL.ResolutionY = sensitiveResolutionY,
    sL.Rotation1   = sensitiveRotation1;
    sL.Rotation2   = sensitiveRotation2;
    sL.Rotation3   = sensitiveRotation3;
    sL.Rotation4   = sensitiveRotation4;
    sL.RadLength   = sensitiveRadLength ;

    _lVec.push_back( lL ) ;
    _sVec.push_back( sL ) ;

  }

  void SiPlanesLayerLayoutImpl::addDUT(
			      // layer
			      int dutID,	 
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
			      double dutsensitiveRadLength)
  {

    Layer lDUT ;
    SensLayer sDUT ;
    lDUT.ID         = dutID ;
    lDUT.PositionX  = dutPositionX ;
    lDUT.PositionY  = dutPositionY ;
    lDUT.PositionZ  = dutPositionZ ;
    lDUT.SizeX      = dutSizeX ;
    lDUT.SizeY      = dutSizeY ;
    lDUT.Thickness  = dutThickness ;
    lDUT.RadLength  = dutRadLength ;

    sDUT.ID         = dutsensitiveID ;
    sDUT.PositionX  = dutsensitivePositionX ;
    sDUT.PositionY  = dutsensitivePositionY ;
    sDUT.PositionZ  = dutsensitivePositionZ ;
    sDUT.SizeX      = dutsensitiveSizeX ;
    sDUT.SizeY      = dutsensitiveSizeY ;
    sDUT.Thickness  = dutsensitiveThickness ;
    sDUT.NpixelX    = dutsensitiveNpixelX;
    sDUT.NpixelY    = dutsensitiveNpixelY;
    sDUT.PitchX     = dutsensitivePitchX;
    sDUT.PitchY     = dutsensitivePitchY;
    sDUT.Resolution = dutsensitiveResolution;
    sDUT.Rotation1  = dutsensitiveRotation1;
    sDUT.Rotation2  = dutsensitiveRotation2;
    sDUT.Rotation3  = dutsensitiveRotation3;
    sDUT.Rotation4  = dutsensitiveRotation4;
    sDUT.RadLength  = dutsensitiveRadLength ;

    _lDut = lDUT ;
    _sDut = sDUT  ;

  }

} //namespace
