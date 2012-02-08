#ifndef BOUNDARYTRAPEZOID_h
#define BOUNDARYTRAPEZOID_h

#include "IBoundary.h"

  
  namespace GearSurfaces {
    
    /** A class for boundaries of a trapzoid.
     * The trapezoid is assumed to be in the u,v plane (u,v being local coordinates).
     * So w should be relatively small to lie within the boundary.
     * The center of the coordinate system is assumed to be in the center of the trapezoid.
     * Also the trapezoid is supposed to be symmetric around its height.
     * A basic trapezoid would have its bases parallel with v and its height parallel with u.
     * The trapezoid might be rotated around its center.
     */
    class BoundaryTrapezoid :public IBoundary {
      
    public:
      
      /** @param baseBottom the length of the bottom base
       * 
       * @param baseTop the length of the rectange: v must lie between +- length/2
       * 
       * @param height the height of the trapezoid
       * 
       * @param deltaW the maximum value of |w| to be within boundary
       * 
       * @param alpha how much the trapezoid is rotated around the w axis. (counterclockwise as usual)
       */
      BoundaryTrapezoid( double baseBottom, double baseTop, double height, double deltaW, double alpha):
          _baseBottom(baseBottom), _baseTop(baseTop), _height(height), _deltaW(deltaW), _alpha(alpha){}
      
      
      /** @param local the local coordinates of a point to be checked
       * 
       * @return wether the point is inside the boundary
       */
      virtual bool isInBoundary( CLHEP::Hep3Vector local ) const;
      
      virtual ~BoundaryTrapezoid(){}
      
    private:
      
      double _baseBottom;
      double _baseTop;
      double _height;
      double _deltaW;
      double _alpha;
      
    };
    
      
} //end of GearSurfaces namespace 



#endif
