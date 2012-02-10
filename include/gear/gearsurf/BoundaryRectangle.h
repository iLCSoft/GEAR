#ifndef BOUNDARYRECTANGLE_h
#define BOUNDARYRECTANGLE_h

#include "IBoundary.h"

namespace gear {
  
  /** A class for boundaries of a rectangle.
   * The rectangle is assumed to be in the u,v plane (u,v being local coordinates).
   * So w should be relatively small to lie within the boundary.
   * The center of the coordinate system is assumed to be in the center of the rectangle.
   * A basic rectangle would have its legnth parallel with v and its width parallel with u.
   * However, the rectangle might be rotated as well around its center.
   */
  class BoundaryRectangle :public IBoundary{
    
  public:
    
    /** @param width the width of the rectangle: u must lie between +- width/2.
     * 
     * @param length the length of the rectange: v must lie between +- length/2
     * 
     * @param deltaW the maximum value of |w| to be within boundary
     * 
     * @param alpha how much the rectangle is rotated around the w axis. (counterclockwise as usual)
     */
    BoundaryRectangle( double width, double length, double deltaW, double alpha=0.): _width(width), _length(length), _deltaW(deltaW), _alpha(alpha){}
    
    
    /** @param local the local coordinates of a point to be checked
     * 
     * @return wether the point is inside the boundary
     */
    virtual bool isInBoundary( CLHEP::Hep3Vector local ) const;
    
    virtual ~BoundaryRectangle(){}
    
  private:
    
    double _width;
    double _length;
    double _deltaW;
    double _alpha;
    
  };
  
  
} // end of gear namespace 

#endif
