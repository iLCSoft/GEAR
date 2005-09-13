
#include "gearxml/GearXML.h"
#include "gearxml/tinyxml.h"
#include "gearxml/XMLHandlerMgr.h"

#include "gearimpl/GearMgrImpl.h"


//#include <algorithm>
#include <sstream>

namespace gear{


  GearXML::GearXML( const std::string& fileName ) : 
    _fileName( fileName ),
    _gearMgr(0) {

  } 


  GearMgr* GearXML::createGearMgr() {

    if( _gearMgr != 0 ){
      
      return _gearMgr ;
      
    } else {
      
      _gearMgr = new GearMgrImpl ;
    }
    
    
    // parse the XML file
    
    TiXmlDocument* doc = new TiXmlDocument ;
    bool loadOkay = doc->LoadFile( _fileName  ) ;
    
    if( !loadOkay ) {

      std::stringstream str ;
      
      str  << "GearXML::createGearMgr error in file [" << _fileName 
	   << ", row: " << doc->ErrorRow() << ", col: " << doc->ErrorCol() << "] : "
	   << doc->ErrorDesc() ;
      
      throw ParseException( str.str() ) ;
    }
    
    TiXmlHandle docHandle( doc );
    
    TiXmlElement* root = doc->RootElement();

    if( root == 0 ){
      throw ParseException( std::string( "GearXML::createGearMgr : no root tag found in  ") 
			   + _fileName  ) ;
    }
    
    TiXmlNode* detectors = root->FirstChild("detectors")  ;
    if( detectors == 0 ){
      throw ParseException( std::string( "GearXML::createGearMgr : no detectors tag found in  ") 
			   + _fileName  ) ;
    }

    
    TiXmlNode* det = 0 ;
    while( ( det = detectors->IterateChildren( "detector", det ) )  != 0  ){
      
      std::string name  =  getXMLAttribute( det, "name" )  ;

      std::string type("UNKOWN") ;

      try {

	type  =  getXMLAttribute( det, "geartype" )  ;

      } catch( ParseException& e){

	std::cout << "GearXML::createGearMgr: igoring detector " << name 
		  << " with missing attribute \"geartype\" " << std::endl ;

	continue ;
      }
	
      const XMLHandler* handler = XMLHandlerMgr::instance()->getHandler( type ) ;
      
      if( handler == 0 ) {
	
	throw ParseException( std::string( "GearXML::createGearMgr : unknown geartype \"")  
			      + type + "\" in  file " + _fileName  ) ;
      }
      
      //       GearParameters* gp = 

      handler->fromXML( det->ToElement() , _gearMgr ) ; 

    }
    
    return _gearMgr ;
  }
  
  

}
