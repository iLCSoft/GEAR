// -*- C++ -*-
#ifndef GEAR_GEARPARAMETERS_H
#define GEAR_GEARPARAMETERS_H 1

#include <string>
#include <vector>

#include "GEAR.h"

namespace gear {

/** Abstract interface for a set of parameters that can be used to describe the 
 *  geometrical properties of a specific subdetector type for reconstruction.
 *  It basically allows to retrieve named parameters of type int, string and double
 *  and arrays/vectors of these types. 
 *
 * @author F. Gaede, DESY
 * @version $Id$
 */
class GearParameters {

public: 
    /// Destructor.
    virtual ~GearParameters() { /* nop */; }

    /** Integer value for key.     
     *  @throws UnknownParameterException
     */
    virtual int getIntVal(const std::string & key) const   = 0;

    /** Double value for key      
     *  @throws UnknownParameterException
     */
    virtual double getDoubleVal(const std::string & key) const   = 0;

    /** String value for key      
     *  @throws UnknownParameterException
     */
    virtual const std::string & getStringVal(const std::string & key) const   = 0;

    /** Integer values for key      
     *  @throws UnknownParameterException
     */
    virtual const std::vector<int>  & getIntVals(const std::string & key) const   = 0;

    /** Double values for key      
     *  @throws UnknownParameterException
     */
    virtual const std::vector<double>  & getDoubleVals(const std::string & key) const   = 0;

    /** String values for key      
     *  @throws UnknownParameterException
     */
    virtual const std::vector<std::string>  & getStringVals(const std::string & key) const   = 0;

    /** All keys of int variables. */ 
    virtual const std::vector<std::string>  & getIntKeys() const = 0;

    /** All keys of double variables. */ 
    virtual const std::vector<std::string>  & getDoubleKeys() const = 0;

    /** All keys of string variables. */ 
    virtual const std::vector<std::string>  & getStringKeys() const = 0;

    /** All keys of IntVec variables. */ 
    virtual const std::vector<std::string>  & getIntVecKeys() const = 0;

    /** All keys of DoubleVec variables. */ 
    virtual const std::vector<std::string>  & getDoubleVecKeys() const = 0;

    /** All keys of StringVec variables. */ 
    virtual const std::vector<std::string>  & getStringVecKeys() const = 0;
}; // class
} // namespace gear
#endif /* ifndef GEAR_GEARPARAMETERS_H */
