#include "gearimpl/SiPlanesParametersImpl.h"
#include <math.h>
#include <iostream>

#define _EPSILON 0.0001

namespace gear{


  SiPlanesParametersImpl::SiPlanesParametersImpl
  ( int siplanesType, int siplanesNumber) :
    _siplanesType( siplanesType ) ,
    _siplanesNumber( siplanesNumber ) {}
 
} // namespace



