#include "gearxml/TPCParametersXML.h"

#include "gearxml/XMLHandlerMgr.h"
#include "gearxml/GearParametersXML.h"

#include "gearxml/tinyxml.h"
#include "gearimpl/TPCParametersImpl.h"
#include "gearimpl/FixedPadSizeDiskLayout.h"
#include "gear/GearMgr.h"

#include <vector>
#include <algorithm>
#include <sstream>


namespace gear {


  TiXmlElement TPCParametersXML::toXML( const GearParameters & parameters ) const {

    TiXmlElement detector("detector" ) ;


    // FIXME: needs to be implemented .....


    GearParametersXML::getXMLForParameters( &detector , &parameters ) ;

    return detector ;
 }
  
  
  GearParameters* TPCParametersXML::fromXML( const TiXmlElement* xmlElement, GearMgr* gearMgr) const {
    
    
    TPCParametersImpl* tpcParams = new TPCParametersImpl ;
    
    // first we read the generic parameters
    GearParametersXML::setParametersFromXML( xmlElement, tpcParams  ) ;
    
    tpcParams->setMaxDriftLength(   atof(  getChildElementValue( xmlElement, "maxDriftLength"   ).c_str() )) ;
    tpcParams->setDriftVelocity(    atof(  getChildElementValue( xmlElement, "driftVelocity"    ).c_str() )) ;
    tpcParams->setReadoutFrequency( atof(  getChildElementValue( xmlElement, "readoutFrequency" ).c_str() )) ;
    

    // get the pad layout
    const TiXmlElement* layout = xmlElement->FirstChildElement( "PadRowLayout2D" ) ;

    if( layout == 0 ) {
      
      std::stringstream str ;
      str  << "XMLParser::getChildElementValue missing element \"PadRowLayout2D\" " 
	   << " in element <" << xmlElement->Value() << "/> " ;
      
      throw ParseException( str.str() ) ;
    }

    std::string layoutType = getXMLAttribute( layout , "type" )   ;
  
    // FIXME: this needs to be implemented more generically for other types of PadRowLayout !
    if( layoutType != "FixedPadSizeDiskLayout" ) {

      throw ParseException( "TPCParametersXML::toXML: currently only FixedPadSizeDiskLayout implemented ! " ) ;
    }

//     <PadRowLayout2D  type="FixedPadSizeDiskLayout" rMin="320.0" rMax="1680.0" padHeight="6.0" padWidth="2.0" />

    double rMin      =  atof(  getXMLAttribute( layout , "rMin" ) .c_str() ) ;
    double rMax      =  atof(  getXMLAttribute( layout , "rMax" ) .c_str() ) ;
    double padHeight =  atof(  getXMLAttribute( layout , "padHeight" ) .c_str() ) ;
    double padWidth  =  atof(  getXMLAttribute( layout , "padWidth" ) .c_str() ) ;
    int    maxRow    =  atoi(  getXMLAttribute( layout , "maxRow" ) .c_str() ) ;
    double padGap    =  atof(  getXMLAttribute( layout , "padGap" ) .c_str() ) ;


    PadRowLayout2D* dLayout = 
      new FixedPadSizeDiskLayout( rMin, rMax, padHeight, padWidth, maxRow, padGap ) ;

    tpcParams->setPadLayout( dLayout ) ;


   if( gearMgr != 0 ) {

      gearMgr->setTPCParameters( tpcParams ) ;
    }
    

    return tpcParams ;
  }


} // namespace
