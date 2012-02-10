#ifndef IBOUNDARY_h
#define IBOUNDARY_h

#include "CLHEP/Vector/ThreeVector.h"
  
  namespace gear {
    
    
    class IBoundary{
      
    public:
      
      /** @param local the local coordinates of a point to be checked
       * 
       * @return wether the point is inside the boundary
       */
      virtual bool isInBoundary( CLHEP::Hep3Vector local ) const = 0;
      
      virtual ~IBoundary(){ /* no_op */ };
      
    };
    
    
  } // end of gear namespace 

#endif

