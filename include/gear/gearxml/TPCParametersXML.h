#ifndef GEAR_TPCParametersXML_H
#define GEAR_TPCParametersXML_H 1


#include "gearxml/XMLHandler.h"
#include "gearxml/TPCModuleXML.h"
#include "gear/TPCParameters.h"
 
//#include <string>
//#include <map>

namespace gear {
  
   
  /** Abstract XML handler for TPCParameters.
   *  This is the modular version.
   * 
   * @author Martin Killenberg, University of Bonn
   * @version $Id$
   */
    class TPCParametersXML : public XMLHandler {
    
  public: 
    
    /** Creates an XML node for the given  TPCParameters.
     */
    virtual TiXmlElement toXML( const GearParameters & modularTPC ) const ;
    
    
    /** Creates the appropriate TPCParameters class from the given
     *  XML element (node) 
     */
    virtual GearParameters* fromXML( const TiXmlElement* xmlElement, GearMgr* gearMgr = 0) const ;

  private:
    /** An instance of the parser for modules
     */
    TPCModuleXML _tpcModuleXML;

  }; // class
} // namespace gear

#endif /* ifndef GEAR_TPCParametersXML_H */
