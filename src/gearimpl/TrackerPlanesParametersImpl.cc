#include "gearimpl/TrackerPlanesParametersImpl.h"
#include <math.h>
#include <iostream>

namespace gear{


  TrackerPlanesParametersImpl::TrackerPlanesParametersImpl
  (int layoutID, int layoutType, int layoutNumberOfLayers ) :
    _layoutID( layoutID ),
    _layoutType( layoutType ) ,
    _layoutNumberOfLayers( layoutNumberOfLayers ) {}
 
} // namespace



