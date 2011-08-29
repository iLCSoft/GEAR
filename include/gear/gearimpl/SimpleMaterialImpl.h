// -*- C++ -*-
#ifndef GEAR_SimpleMaterialImpl_H
#define GEAR_SimpleMaterialImpl_H 1

#include "gear/SimpleMaterial.h"

namespace gear {

/** Implementation of gear::SimpleMaterial.
 *
 * @author F. Gaede, DESY
 * @version $Id: $
 */
  class SimpleMaterialImpl : public gear::SimpleMaterial {
    
  public: 

    /** Only c'tor specifying all data attributes */
    SimpleMaterialImpl(const std::string name, 
		       double A, 
		       double Z, 
		       double density,
		       double radLen, 
		       double intLen ) : 
      _name(name) ,
      _A(A),
      _Z(Z),
      _d(density),
      _rl( radLen ),
      _il( intLen ){ 
    }
    

    /// Destructor.
    virtual ~SimpleMaterialImpl() { /* nop */ }
    
    /** The material name.*/
    virtual const std::string & getName() const { return _name ; }
    
    /** The mass number of the (averaged) material.*/
    virtual double getA() const { return _A ; }
    
    /** The atomic number of the (averaged) material.*/
    virtual double getZ() const { return _Z ; }
    
    /** Density in kg/m^3.*/
    virtual double getDensity() const { return _d ; }
    
    /** Radiation length of material in mm.*/
    virtual double getRadLength() const  { return _rl ; }
    
    /** Interaction length of material in mm.*/
    virtual double getIntLength() const { return _il ; }

  protected:
    std::string _name ;
    double _A ;
    double _Z ;
    double _d ;
    double _rl ;
    double _il ;
    
  }; // class
  
} // namespace gear

#endif // ifndef GEAR_SimpleMaterialImpl_H 
