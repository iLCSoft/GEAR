#include "gearxml/SimpleMaterialXML.h"

#include "gear/GearMgr.h"
#include "gearimpl/SimpleMaterialImpl.h"

#include "gearxml/tinyxml.h"
#include "gearxml/XMLHandler.h"

#include <string>
#include <sstream>

namespace gear {
  
  TiXmlElement SimpleMaterialXML::toXML( const SimpleMaterial& material ) const {
    
    //std::cout << "SimpleMaterial::toXML called" << std::endl ; //debug
    
    TiXmlElement matElem("material") ;
    
    matElem.SetAttribute( "name", material.getName()  ) ;
    matElem.SetDoubleAttribute( "A", material.getA()  ) ;
    matElem.SetDoubleAttribute( "Z", material.getZ()  ) ;
    matElem.SetDoubleAttribute( "density", material.getDensity()  ) ;
    matElem.SetDoubleAttribute( "radLength", material.getRadLength()  ) ;
    matElem.SetDoubleAttribute( "intLength", material.getIntLength()  ) ;
    
    return matElem ;
  }
  
  

  SimpleMaterial* SimpleMaterialXML::fromXML( const TiXmlElement* xmlElement, GearMgr* gearMgr) const {



    std::string name =  getXMLAttribute( xmlElement , "name" )  ; 
    

    //std::cout << "  SimpleMaterialXML::fromXML( " << name << " ... ) " << std::endl ;

    std::stringstream ss ; 
    double dummy ;

    ss << getXMLAttribute( xmlElement , "A" ) ;  ss >> dummy ; ss.clear() ;
    double A = dummy ;

    ss << getXMLAttribute( xmlElement , "Z" ) ;  ss >> dummy ; ss.clear() ;
    double Z = dummy ;

    ss << getXMLAttribute( xmlElement , "density" ) ;  ss >> dummy ; ss.clear() ;
    double d = dummy ;

    ss << getXMLAttribute( xmlElement , "radLength" ) ;  ss >> dummy ; ss.clear() ;
    double rL = dummy ;

    ss << getXMLAttribute( xmlElement , "intLength" ) ;  ss >> dummy ; ss.clear() ;
    double iL = dummy ;

    SimpleMaterialImpl* sm = new SimpleMaterialImpl( name , A , Z , d , rL , iL  ) ;

    //--------- add to proper section in GearMgr ----
    if( gearMgr != 0 ) {
      
      gearMgr->registerSimpleMaterial( sm ) ;
    }

    return sm  ;

  } // fromXML

} // namespace
  
    
