#include "BoundaryRectangle.h"

#include "CLHEP/Vector/Rotation.h"

using namespace GearSurfaces;

bool BoundaryRectangle::isInBoundary( CLHEP::Hep3Vector local ) const {
  
  if( fabs( local.z() ) > _deltaW ) return false;
  

  
  // First we need to take into account, that the rectangle might be rotated
  // around w. So we transpose the local coordinates to the frame of the rectangle
  // If the rectangle is rotated by alpha, we need to rotate the coordinates by -alpha
  CLHEP::HepRotation R;
  R.rotateZ( -_alpha);
  local = R * local; //to rectangle system
  

  
  if( fabs( local.x() ) > _width/2. ) return false;
  if( fabs( local.y() ) > _length/2. ) return false;
  
  
  return true;
  
}