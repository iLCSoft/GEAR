#include "gearimpl/TrackerPlanesLayerLayoutImpl.h"
#include <math.h>

/** Abstract description of layers in pixel beam telescope with or without DUT.
   *  This assumes a number of silicon layers, arranged perpendicular to the beam.
   *  @author I. Rubinskiy, DESY
   *  @version $Id: 
   */

namespace gear{

  void TrackerPlanesLayerImpl::addMaterialLayer(
			      // layer
			      int id, std::string infostr,
			      double PositionX, double PositionY, double PositionZ,
			      double RotationXY, double RotationZX, double RotationZY,
			      double SizeX, double SizeY, double Thickness, double RadLength 
                              )
  {

    TrackerPlanesMaterialLayerImpl lL ;
    lL.setID         ( id        );
    lL.setInfo       ( infostr   );
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

  void TrackerPlanesLayerImpl::addSensitiveLayer(
			      int id, std::string infostr,
			      double PositionX, double PositionY, double PositionZ,
			      double RotationXY, double RotationZX, double RotationZY,
			      double SizeX, double SizeY, double Thickness, double RadLength,
			      int NpixelX, int NpixelY, double PitchX,double PitchY, double ResolutionX, double ResolutionY
                              )
  {

    TrackerPlanesSensitiveLayerImpl sL ;
    sL.setID         ( id        );
    sL.setInfo       ( infostr   );
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
    sL.setResolutionY ( ResolutionY );

    _sensitiveVec.push_back( sL ) ;

  }

} //namespace
