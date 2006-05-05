#ifndef GEAR_Vectors_H
#define GEAR_Vectors_H 1

#include <stdexcept> 

namespace gear {


  /** Trivial N-dimensional vector. 
   *  Access to coordinates is provided through the [] operator.
   *  Could be replaced or make use of a  a more elaborate implementation, 
   *  e.g. from CLHEP or root::MathCore in the future.
   * 
   * @author F. Gaede, DESY
   * @version $Id: Vectors.h,v 1.1 2006-05-05 12:59:26 gaede Exp $
   */
  
  template <int N, typename float_t=double>
  class VectorND_T {
    
  public: 

    VectorND_T() {} ;

    /** The i-th coordinate */
    inline float_t& operator[](unsigned i) {  
      
      if( i > N-1 ) throw std::out_of_range( "VectorND_T::operator[]" ) ;
      
      return _c[i]  ; 
    } 
    
  protected:
    
    float_t _c[N] ; 
    
  }; // class
  

  /** Specialization for 2D */
  template <typename float_t=double>
  struct Vector2D_T : public VectorND_T<2,float_t> {
    Vector2D_T() { _c[0] = 0. ; _c[1] = 0. ; }
    Vector2D_T( float_t c0, float_t c1 ) { _c[0] = c0 ; _c[1] = c1 ; } 
  } ;

  /** Specialization for 3D */
  template <typename float_t=double>
  struct Vector3D_T : public VectorND_T<3,float_t> {

    Vector3D_T() { _c[0] = 0. ; _c[1] = 0. ; _c[2] = 0. ;}
    Vector3D_T( float_t c0, float_t c1, float_t c2 ) { _c[0] = c0 ; _c[1] = c1 ; _c[2] = c2 ; } 
  } ;

  typedef Vector2D_T<double> Vector2D ;

  typedef Vector3D_T<double> Vector3D ;


} // namespace gear

#endif /* ifndef GEAR_Vectors_H */

