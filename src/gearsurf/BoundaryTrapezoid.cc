#include "gearsurf/BoundaryTrapezoid.h"

#include "CLHEP/Vector/Rotation.h"

using namespace GearSurfaces;

bool BoundaryTrapezoid::isInBoundary( CLHEP::Hep3Vector local ) const {
  
  if( fabs( local.z() ) > _deltaW ) return false;
  
  // First we need to take into account, that the trapezoid might be rotated
  // around w. So we transpose the local coordinates to the frame of the rectangle
  // If the trapezoid is rotated by alpha, we need to rotate the coordinates by -alpha
  CLHEP::HepRotation R;
  R.rotateZ( -_alpha);
  local = R * local; //to rectangle system
  
  if( fabs( local.y() ) > _height/2. ) return false;
  
  // Calculate the width of the trapezoid at the given height
  double width = (_baseBottom + _baseTop)/2.  + local.y() * (_baseTop - _baseBottom) / _height;
  if( fabs( local.x() ) > width/2. ) return false;
  
  
  return true;
  
} //end of GearSurfaces namespace 
