#ifndef GEAR_XMLHandlerMgr_H
#define GEAR_XMLHandlerMgr_H 1


#include <map>

#include "gearxml/XMLHandler.h"


namespace gear {
  
  typedef std::map< std::string, XMLHandler* >   XMLHandlerMap ;
  
  
  /** Manager for  XML handlers of GEAR classes.
   * 
   * @author F. Gaede, DESY
   * @version $Id$
   */
  
  class XMLHandlerMgr {
    
    
  public: 
    
    static XMLHandlerMgr* instance() ;
    
    
    /** XMLHandler for given type. 
     */
    const XMLHandler* getHandler( const std::string& type )  ;
    
    
    /** Register XMLHandler for type. 
     */
    void setHandler( const std::string& type , XMLHandler* handler )  ;
   
    // no need for copy-constructor or assignment operator
    // this is a sinleton class

    /// the destructor
    ~XMLHandlerMgr() ;

  protected:

    XMLHandlerMap _map{} ;

  private:
    XMLHandlerMgr() ;

  }; // class
  
} // namespace gear

#endif /* ifndef GEAR_XMLHandlerMgr_H */
