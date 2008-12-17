#ifndef GEAR_TPCModuleXML_H
#define GEAR_TPCModuleXML_H 1


#include "gearxml/XMLHandler.h"
#include "gear/TPCModule.h"
 
//#include <string>
//#include <map>

namespace gear {
  
   
  /** Abstract XML handler for TPCModuleXML.
   * 
   * @author Martin Killenberg, University of Bonn
   * @version $Id: TPCModuleXML.h,v 1.1 2008-12-17 09:39:46 engels Exp $
   */
  class TPCModuleXML {
    
  public: 
    
      virtual ~TPCModuleXML(){}
    /** Creates an XML node for the given  TPCModule.
     */
      virtual TiXmlElement toXML( const TPCModule* layout ) const ;
    
    
    /** Creates the appropriate TPCModule class from the given
     *  XML element (node) 
     */
    virtual TPCModule* fromXML( const TiXmlElement* moduleElement ,
				const TiXmlElement* defaultModuleElement,
				int tpcCoordinateType, int moduleID) const ;

  }; // class
} // namespace gear

#endif /* ifndef GEAR_TPCModuleXML_H */
