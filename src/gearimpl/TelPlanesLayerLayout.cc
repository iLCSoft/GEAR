#include "gearimpl/TelPlanesLayerLayoutImpl.h"
#include <math.h>

/** Abstract description of layers in pixel beam telescope with or without DUT.
   *  This assumes a number of silicon layers, arranged perpendicular to the beam.
   *  @author I. Rubinskiy, DESY
   *  @version $Id: 
   */

namespace gear{

  void TelPlanesLayerImpl::addMaterialLayer(
			      // layer
			      int ID, std::string info,
			      double PositionX, double PositionY, double PositionZ,
			      double RotationXY, double RotationZX, double RotationZY,
			      double SizeX, double SizeY, double Thickness, double RadLength 
                              )
  {

    TelPlanesMaterialLayerImpl lL ;
    lL.setID         ( ID        );
    lL.setInfo       ( info      );
    lL.setPositionX  ( PositionX );
    lL.setPositionY  ( PositionY );
    lL.setPositionZ  ( PositionZ );
    lL.setRotationXY ( RotationXY);
    lL.setRotationZX ( RotationZX);
    lL.setRotationZY ( RotationZY);
    lL.setSizeX      ( SizeX     );
    lL.setSizeY      ( SizeY     );
    lL.setThickness  ( Thickness );
    lL.setRadLength  ( RadLength );
 
    _materialVec.push_back( lL ) ;
  }

  void TelPlanesLayerImpl::addSensitiveLayer(
			      int ID, std::string info,
			      double PositionX, double PositionY, double PositionZ,
			      double RotationXY, double RotationZX, double RotationZY,
			      double SizeX, double SizeY, double Thickness, double RadLength,
			      int NpixelX, int NpixelY, double PitchX,double PitchY, double ResolutionX, double ResolutionY
                              )
  {

    TelPlanesSensitiveLayerImpl sL ;
    sL.setID         ( ID        );
    sL.setInfo       ( info      );
    sL.setPositionX  ( PositionX );
    sL.setPositionY  ( PositionY );
    sL.setPositionZ  ( PositionZ );
    sL.setRotationXY ( RotationXY);
    sL.setRotationZX ( RotationZX);
    sL.setRotationZY ( RotationZY);
    sL.setSizeX      ( SizeX     );
    sL.setSizeY      ( SizeY     );
    sL.setThickness  ( Thickness );
    sL.setRadLength  ( RadLength );

    sL.setNpixelX    ( NpixelX   );
    sL.setNpixelY    ( NpixelY   );
    sL.setPitchX     ( PitchX    );
    sL.setPitchY     ( PitchY    );
    sL.setResolutionX ( ResolutionX );
    sL.setResolutionY ( ResolutionX );

    _sensitiveVec.push_back( sL ) ;

  }

} //namespace
