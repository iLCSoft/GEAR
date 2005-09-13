#include "gearxml/XMLHandlerMgr.h"

#include "gearxml/GearParametersXML.h"
#include "gearxml/TPCParametersXML.h"
#include "gearxml/CalorimeterParametersXML.h"
#include "gearxml/tinyxml.h"

#include "gear/GEAR.h"


#include <string> 
#include <sstream> 

namespace gear{
  
  XMLHandlerMgr* XMLHandlerMgr::_me  = 0 ;
  
  
  
  XMLHandlerMgr* XMLHandlerMgr::instance() {
    
    if( _me == 0 ) {
      _me = new XMLHandlerMgr ;
    }
    return _me ;
  }
  

  XMLHandlerMgr::XMLHandlerMgr() {
   
    _map[ GEAR::GEARPARAMETERS ] = new GearParametersXML ;
    _map[ GEAR::TPCPARAMETERS ]  = new TPCParametersXML ;
    _map[ GEAR::CALORIMETERPARAMETERS ]  = new CalorimeterParametersXML ;

  }
    
  
  const XMLHandler* XMLHandlerMgr::getHandler( const std::string& type ) {

    return _map[ type ] ;
  }
  
  
  const void XMLHandlerMgr::setHandler( const std::string& type , XMLHandler* handler ) {

    XMLHandlerMap::iterator it = _map.find( type ) ;
    
    if( it != _map.end()  ) {
      
      delete it->second ;
      
      it->second = handler ;
      
    } else {
      
      _map.insert( std::make_pair( type , handler )  ) ;
    }
    
  }
  
  




  // helper function (declared in XMLHandler.h )

  std::string getXMLAttribute(const TiXmlNode* node , const std::string& name ) {
    
    const TiXmlElement* el = node->ToElement() ;
    if( el == 0 ) 
      throw ParseException("XMLParser::getAttribute not an XMLElement " ) ;
    
    const char* at = el->Attribute( name ) ;
    
    if( at == 0 ){
      
      std::stringstream str ;
      str  << "XMLParser::getAttribute missing attribute \"" << name 
	   << "\" in element <" << el->Value() << "/> " ;
      throw ParseException( str.str() ) ;
    }

    return std::string( at )  ;

  }


  // helper function (declared in XMLHandler.h )

  std::string getChildElementValue(const TiXmlNode* node , const std::string& name ) {
    
    const TiXmlElement* el = node->ToElement() ;
    if( el == 0 ) 
      throw ParseException("XMLParser::getAttribute not an XMLElement " ) ;
    
    const TiXmlElement* cE = el->FirstChildElement( name );
    
    if( cE == 0 ) {
      
      std::stringstream str ;
      str  << "XMLParser::getChildElementValue missing element \"" << name 
	   << "\" in element <" << el->Value() << "/> " ;
      
      throw ParseException( str.str() ) ;
    }

    return getXMLAttribute( cE , "value" )   ;

  }
  
}
