// -*- C++ -*-
#ifndef GEAR_GEAR_H
#define GEAR_GEAR_H 1

#include <map>
#include <vector>
#include <string>
#include <exception> 

#include "gear/GearConfig.h"

// version macros are now defined with cmake
//#define GEAR_MAJOR_VERSION 0
//#define GEAR_MINOR_VERSION 14
//#define GEAR_VERSION_GE( MAJV , MINV )  ( (  GEAR_MAJOR_VERSION  > MAJV ) || ( (GEAR_MAJOR_VERSION==MAJV) && ( GEAR_MINOR_VERSION >= MINV ) ) )

#include "gearimpl/Vectors.h"


namespace gear {


  typedef Vector2D Point2D ;
  typedef Vector3D Point3D ;
  
  typedef std::vector<int> IntVec ;
  typedef std::vector<double> DoubleVec ;
  typedef std::vector<std::string> StringVec ;
  
  
  /** 64 bit integer,e.g.to be used for cellids **/
   typedef long long long64 ;
   // FIXME: needs to be only long on 64bit machines ->  flags??


  /**Base exception class for GEAR - all other exceptions extend this.
   * @author gaede
   * @version $Id$
   */

  class Exception : public std::exception {

    
  protected:
    std::string message{} ;
    
    Exception(){  /*no_op*/ ; } 
    
  public: 
    virtual ~Exception() throw() { /*no_op*/; } 
    
    Exception( const std::string& text ){
      message = "gear::Exception: " + text ;
    }

    virtual const char* what() const  throw() { return  message.c_str() ; } 

  };


  /**ParseException used for parse errors, e.g. when reading the xml file.
   * @author gaede
   * @version $Id$
   */
  class ParseException : public Exception{
    
  protected:
    ParseException() {  /*no_op*/ ; } 
  public: 
    virtual ~ParseException() throw() { /*no_op*/; } 

    ParseException( std::string text ){
      message = "gear::ParseException: " + text ;
    }
  }; 

  /**NotImplementedException used for features that are not implemented.
   * @author gaede
   * @version $Id$
   */
  class NotImplementedException : public Exception{

  protected:
    NotImplementedException() {  /*no_op*/ ; } 

  public: 
    NotImplementedException( std::string text ){
      message = "gear::NotImplementedException: " + text ;
    }
    virtual ~NotImplementedException() throw() { /*no_op*/; } 

  }; 

  /** UnknownParameterException 
   *  call Processor::end().
   * @author gaede
   * @version $Id$
   */
   class UnknownParameterException : public Exception{

  protected:
    UnknownParameterException() {  /*no_op*/ ; } 

  public: 
    UnknownParameterException( std::string text ){
      message = "gear::UnknownParameterException: " + text ;
    }
    virtual ~UnknownParameterException() throw() { /*no_op*/; } 

  }; 

  /** OutsideGeometryException used if user asks for info from geometry tree
   * where no node is defined.
   *  call Processor::end().
   * @author muennich
   * @version $Id$
   */
   class OutsideGeometryException : public Exception{

  protected:
    OutsideGeometryException() {  /*no_op*/ ; } 

  public: 
    OutsideGeometryException( std::string text ){
      message = "gear::OutsideGeometryException: " + text ;
    }
    virtual ~OutsideGeometryException() throw() { /*no_op*/; } 

  }; 


  /** Helper struct for decoding a sensor ID */
  struct SensorID{
    SensorID() : layer(0), side(0), module(0), sensor(0) {}
    int layer ;
    int side ;
    int module ;
    int sensor ;
  } ;
  

}

namespace gear {

// namespace
/** Global constants used in GEAR
 */
class GEAR {

public: 
    /// Destructor.
    virtual ~GEAR() { /* nop */; }



  public:
   static const char*  GEARPARAMETERS  ;
   static const char*  TPCPARAMETERS  ;
   static const char*  CALORIMETERPARAMETERS  ;
   static const char*  ZPLANARPARAMETERS  ;
   static const char*  FTDPARAMETERS  ;
   static const char*  SIPLANESPARAMETERS  ;
   static const char*  TRACKERPLANESPARAMETERS  ;


}; // class
} // namespace gear
#endif /* ifndef GEAR_GEAR_H */
