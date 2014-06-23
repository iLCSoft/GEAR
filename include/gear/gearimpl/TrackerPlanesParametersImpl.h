// -*- C++ -*-
#ifndef GEAR_TrackerPlanesParametersImpl_H
#define GEAR_TrackerPlanesParametersImpl_H 1

#include "gear/TrackerPlanesParameters.h"
#include "gearimpl/GearParametersImpl.h"
#include "gearimpl/TrackerPlanesLayerLayoutImpl.h"


namespace gear {

class TrackerPlanesLayerLayout;

  /** Abstract description of layers in pixel beam telescope.
   *  This assumes a number of silicon layers, arranged perpendicular to the beam.
   *  @author I. Rubinskiy, DESY
   *  @version $Id: 
   */
class TrackerPlanesParametersImpl : public GearParametersImpl, public TrackerPlanesParameters {

public: 
  /** C'tor  
   *  @param layoutID             ID of TrackerPlanes detector setup   
   *  @param layoutType           the type of the TrackerPlanes detector
   *  @param layoutNumberOfLayers         the number of Si planes
   */
  TrackerPlanesParametersImpl(int layoutID, int layoutType, int layoutNumberOfLayers) ;

  // Destructor.
  virtual ~TrackerPlanesParametersImpl() { /* nop */; }
  

  /** Adding a Layer to the TrackerPlanes detector (without parameters for layer rotation)
   **/
  virtual void addLayer( TrackerPlanesLayerImpl* _layer ){

     _layerLayout.addLayer( _layer ); // pointer as input
  }
 
 
      
  /** Returns the layer layout of TrackerPlanes detector 
   */
  virtual const TrackerPlanesLayerLayout & getTrackerPlanesLayerLayout() const { return _layerLayout ; }
  

  /** Returns the ID of TrackerPlanes detector setup
   */

  virtual int getLayoutID() const { return _layoutID ; }

  /** Returns the type of TrackerPlanes detector
   */
  virtual int getLayoutType() const { return _layoutType ; }

  /** Returns the number of Si planes
   */
  virtual int getLayoutNumberOfLayers() const { return _layoutNumberOfLayers ; }
  
protected:
  
  TrackerPlanesLayerLayoutImpl _layerLayout ;
  
  int _layoutID;

  int _layoutType ;
  
  int _layoutNumberOfLayers ;

private:

}; // class

} // namespace gear

#endif /* #ifndef GEAR_TrackerPlanesParametersImpl_H */

