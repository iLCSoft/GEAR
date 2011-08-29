// -*- C++ -*-
#ifndef GEAR_SimpleMaterial_H
#define GEAR_SimpleMaterial_H 1

#include <string>
#include <vector>

#include "GEAR.h"

namespace gear {

/** Abstract interface for a simple material description tjhat hold s the following properties:
 *  A, Z, density, radiation length and interaction length. This will be mainly used to describe 
 *  averaged materials in a simplified tracking geometry.
 * 
 *
 * @author F. Gaede, DESY
 * @version $Id: $
 */
  class SimpleMaterial {
    
  public: 
    /// Destructor.
    virtual ~SimpleMaterial() { /* nop */; }
    
    /** The material name.*/
    virtual const std::string & getName() const = 0;
    
    /** The mass number of the (averaged) material.*/
    virtual double getA() const = 0 ;

    /** The atomic number of the (averaged) material.*/
    virtual double getZ() const = 0 ;

    /** Density in kg/m^3.*/
    virtual double getDensity() const = 0 ;
    
    /** Radiation length of material in mm.*/
    virtual double getRadLength() const = 0 ;
    
    /** Interaction length of material in mm.*/
    virtual double getIntLength() const = 0;
    
}; // class

} // namespace gear

#endif // ifndef GEAR_SimpleMaterial_H 
