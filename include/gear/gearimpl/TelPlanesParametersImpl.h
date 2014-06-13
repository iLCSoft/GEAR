// -*- C++ -*-
#ifndef GEAR_TelPlanesParametersImpl_H
#define GEAR_TelPlanesParametersImpl_H 1

#include "gear/TelPlanesParameters.h"
#include "gearimpl/GearParametersImpl.h"
#include "gearimpl/TelPlanesLayerLayoutImpl.h"


namespace gear {

class TelPlanesLayerLayout;

  /** Abstract description of layers in pixel beam telescope.
   *  This assumes a number of silicon layers, arranged perpendicular to the beam.
   *  @author I. Rubinskiy, DESY
   *  @version $Id: 
   */
class TelPlanesParametersImpl : public GearParametersImpl, public TelPlanesParameters {

public: 
  /** C'tor  
   *  @param siplanesID             ID of TelPlanes detector setup   
   *  @param siplanesType           the type of the TelPlanes detector
   *  @param siplanesNumber         the number of Si planes
   */
  TelPlanesParametersImpl(int siplanesID, int siplanesType, int siplanesNumber) ;

  // Destructor.
  virtual ~TelPlanesParametersImpl() { /* nop */; }
  

  /** Adding a Layer to the TelPlanes detector (without parameters for layer rotation)
   **/
  virtual void addLayer( TelPlanesLayerImpl* _layer ){

     _layerLayout.addLayer( _layer ); // pointer as input
  }
 
 
      
  /** Returns the layer layout of TelPlanes detector 
   */
  virtual const TelPlanesLayerLayout & getTelPlanesLayerLayout() const { return _layerLayout ; }
  

  /** Returns the ID of TelPlanes detector setup
   */

  virtual int getTelPlanesID() const { return _siplanesID ; }

  /** Returns the type of TelPlanes detector
   */
  virtual int getTelPlanesType() const { return _siplanesType ; }

  /** Returns the number of Si planes
   */
  virtual int getTelPlanesNumber() const { return _siplanesNumber ; }
  
protected:
  
  TelPlanesLayerLayoutImpl _layerLayout ;
  
  int _siplanesID;

  int _siplanesType ;
  
  int _siplanesNumber ;

private:

}; // class

} // namespace gear

#endif /* ifndef GEAR_SIPLANESPARAMETERS_H */
