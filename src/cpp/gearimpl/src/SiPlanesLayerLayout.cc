#include "gearimpl/SiPlanesLayerLayoutImpl.h"
#include <math.h>

namespace gear{

  void SiPlanesLayerLayoutImpl::addLayer(
			      // layer
			      int layerID,	 
			      double layerPositionX, double layerPositionY, double layerPositionZ,
			      double layerSizeX, double layerSizeY, double layerThickness,
			      double layerRadLength,
			      // sensitive
			      double sensitivePositionX, double sensitivePositionY, double sensitivePositionZ,
			      double sensitiveSizeX, double sensitiveSizeY, double sensitiveThickness,
			      double sensitiveRadLength)
  {

    Layer lL, sL ;
    lL.ID = layerID ;
    lL.PositionX  = layerPositionX ;
    lL.PositionY  = layerPositionY ;
    lL.PositionZ  = layerPositionZ ;
    lL.SizeX  = layerSizeX ;
    lL.SizeY  = layerSizeY ;
    lL.Thickness  = layerThickness ;
    lL.RadLength = layerRadLength ;

    sL.ID = layerID ;
    sL.PositionX  = sensitivePositionX ;
    sL.PositionY  = sensitivePositionY ;
    sL.PositionZ  = sensitivePositionZ ;
    sL.SizeX  = sensitiveSizeX ;
    sL.SizeY  = sensitiveSizeY ;
    sL.Thickness  = sensitiveThickness ;
    sL.RadLength = sensitiveRadLength ;

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
			      double dutsensitivePositionX, double dutsensitivePositionY, double dutsensitivePositionZ,
			      double dutsensitiveSizeX, double dutsensitiveSizeY, double dutsensitiveThickness,
			      double dutsensitiveRadLength)
  {

    Layer lDUT, sDUT ;
    lDUT.ID = dutID ;
    lDUT.PositionX  = dutPositionX ;
    lDUT.PositionY  = dutPositionY ;
    lDUT.PositionZ  = dutPositionZ ;
    lDUT.SizeX  = dutSizeX ;
    lDUT.SizeY  = dutSizeY ;
    lDUT.Thickness  = dutThickness ;
    lDUT.RadLength = dutRadLength ;

    sDUT.ID = dutID ;
    sDUT.PositionX  = dutsensitivePositionX ;
    sDUT.PositionY  = dutsensitivePositionY ;
    sDUT.PositionZ  = dutsensitivePositionZ ;
    sDUT.SizeX  = dutsensitiveSizeX ;
    sDUT.SizeY  = dutsensitiveSizeY ;
    sDUT.Thickness  = dutsensitiveThickness ;
    sDUT.RadLength = dutsensitiveRadLength ;

    _lDut = lDUT ;
    _sDut = sDUT  ;

  }

} //namespace
