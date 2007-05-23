// -*- C++ -*-
#ifndef GEAR_SiPlanesParametersImpl_H
#define GEAR_SiPlanesParametersImpl_H 1

#include "gear/SiPlanesParameters.h"
#include "gearimpl/GearParametersImpl.h"
#include "gearimpl/SiPlanesLayerLayoutImpl.h"


namespace gear {

class SiPlanesLayerLayout;

  /** Abstract description of layers in beam telescope with or without DUT. <br>
   *  This assumes a number of silicon layers, arranged perpendicular to the beam <br>
   *  @author T Klimkovich, DESY
   *  @version $Id: 
   */
class SiPlanesParametersImpl : public GearParametersImpl, public SiPlanesParameters {

public: 
  /** C'tor  
   *  @param siplanesType           the type of the Si Planes detector: Telescope, FTD
   *  @param siplanesNumber         the number of Si planes
   */
  SiPlanesParametersImpl( int siplanesType, int siplanesNumber) ;

  // Destructor.
  virtual ~SiPlanesParametersImpl() { /* nop */; }
  
  // static constants enums( for the type of layout )
  
  static const int TelescopeWithDUT    = 1 ;
  static const int TelescopeWithoutDUT = 2 ;

  /** adding a Layer to the Si planes
   *
   * @param PositionX
   * @param PositionY
   * @param PositionZ   the length (mm) of the straigth line between the moddle of the DUT and spacepoint to 
   *                   layer (layerDistance) or sensitive Area (sensitiveDistance)     
   * @param SizeX
   * @param SizeY
   * @param Thickness  the thickness in mm of the ladder (ladderThickness) or the        
   *                   sensitive area (sensitiveThickness)                     
   * @param RadLenght  the radiation lenght of the layer (layerRadLength) or the       
   *                   sensitive area (sensitiveRadLength) in mm                         
   */
  virtual void addLayer(int layerID,
                        double layerPositionX, double layerPositionY, double layerPositionZ,
			double layerSizeX, double layerSizeY, double layerThickness,
			double layerRadLength,
			// sensitive
			double sensitivePositionX, double sensitivePositionY, double sensitivePositionZ,
			double sensitiveSizeX, double sensitiveSizeY, double sensitiveThickness,
			double sensitiveRadLength)
  {
    _layer.addLayer( layerID,
                     layerPositionX, layerPositionY, layerPositionZ,
                     layerSizeX, layerSizeY, layerThickness,
		     layerRadLength,
                     sensitivePositionX, sensitivePositionY, sensitivePositionZ,
		     sensitiveSizeX, sensitiveSizeY, sensitiveThickness,
		     sensitiveRadLength ) ;
    return ;
  }
  
  virtual void addDUT(int dutID,
                        double dutPositionX, double dutPositionY, double dutPositionZ,
			double dutSizeX, double dutSizeY, double dutThickness,
			double dutRadLength,
			// sensitive
			double dutsensitivePositionX, double dutsensitivePositionY, double dutsensitivePositionZ,
			double dutsensitiveSizeX, double dutsensitiveSizeY, double dutsensitiveThickness,
			double dutsensitiveRadLength)
  {
    _layer.addDUT( dutID,
                     dutPositionX, dutPositionY, dutPositionZ,
                     dutSizeX, dutSizeY, dutThickness,
		     dutRadLength,
                     dutsensitivePositionX, dutsensitivePositionY, dutsensitivePositionZ,
		     dutsensitiveSizeX, dutsensitiveSizeY, dutsensitiveThickness,
		     dutsensitiveRadLength ) ;
    return ;
  }
  
      
  /** Returns the layer layout in the SiPlanes detector 
   */
  virtual const SiPlanesLayerLayout & getSiPlanesLayerLayout() const { return _layer ; }
  
  /** The type of SiPlanes detector: SiPlanesParametersImpl.TelescopeWithDUT 
   *  or SiPlanesParametersImpl.TelescopeWithoutDUT
   */
  virtual int getSiPlanesType() const { return _siplanesType ; }

  /** Number of Si planes
   */
  virtual int getSiPlanesNumber() const { return _siplanesNumber ; }
  
protected:
  
  SiPlanesLayerLayoutImpl _layer ;
  
  int _siplanesType ;
  
  int _siplanesNumber ;

private:

}; // class

} // namespace gear

#endif /* ifndef GEAR_SIPLANESPARAMETERS_H */
