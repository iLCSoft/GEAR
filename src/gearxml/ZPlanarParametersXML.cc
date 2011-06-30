#include "gearxml/ZPlanarParametersXML.h"

#include "gearxml/XMLHandlerMgr.h"
#include "gearxml/GearParametersXML.h"

#include "gearxml/tinyxml.h"
#include "gearimpl/ZPlanarParametersImpl.h"

#include "gear/GearMgr.h"

#include <vector>
#include <string>
#include <algorithm>

namespace gear {

  TiXmlElement ZPlanarParametersXML::toXML( const GearParameters & parameters ) const {

    //std::cout << "ZPlanarParameters::toXML called" << std::endl ; //debug

    // check whether parameter is a valid ZPlanarParameter
    const ZPlanarParameters* param = dynamic_cast<const ZPlanarParameters*> ( &parameters ) ;
    
    if( param == 0 ) {

      throw Exception( "ZPlanarParametersXML::toXML given parameters not of correct type. "
		       "needs to be gear::ZPlanarParameters." ) ;
    }

    // Set up VXD-Detector as Element
    TiXmlElement det("detector") ;

    //type
    TiXmlElement type( "type" ) ;
    std::string strType ;
    switch( param->getType() ) {

    case ( ZPlanarParameters::CCD ) :
      strType = "CCD" ;
      break ;

    case ( ZPlanarParameters::CMOS ) :
      strType = "CMOS" ;
      break ;

    case ( ZPlanarParameters::HYBRID ) :
      strType = "HYBRID" ;
      break ;

    default :
      strType = "Unknown" ;

    }

    type.SetAttribute( "technology", strType ) ;
    
    det.InsertEndChild( type ) ;
    
    //std::cout << "ZPlanarParameters::toXML strType == '" << strType << "'"<< std::endl ; // debug
    
    // shell
    // std::cout << "ZPlanarParameters::toXML shell      " << std::endl ; // debug
    //     std::cout << "                     halflength " << param->getShellHalfLength() << std::endl ; //debug
    //     std::cout << "                            gap " << param ->getShellGap() << std::endl ; // debug
    //     std::cout << "                         innerR " << param->getShellInnerRadius() << std::endl ; //debug
    //     std::cout << "                         outerR " << param->getShellOuterRadius() << std::endl ; //debug
    //     std::cout << "                         radLen " << param->getShellRadLength() << std::endl ; //debug
    
    TiXmlElement shell( "shell" ) ;
    shell.SetDoubleAttribute( "halfLength" , param->getShellHalfLength() ) ;
    shell.SetDoubleAttribute( "gap" , param->getShellGap() ) ;
    shell.SetDoubleAttribute( "innerRadius" , param->getShellInnerRadius() ) ;
    shell.SetDoubleAttribute( "outerRadius" , param->getShellOuterRadius() ) ;
    shell.SetDoubleAttribute( "radLength" , param->getShellRadLength() ) ;
    det.InsertEndChild( shell ) ;
    
    // layerLayout
    const ZPlanarLayerLayout& layout = param->getZPlanarLayerLayout() ;
    TiXmlElement layers("layers") ;

    for( int i=0 ; i < layout.getNLayers() ; i++ ) {

      //std::cout << "                     layer #ladders " << layout.getNLadders( i ) << std::endl ; //debug
      
      TiXmlElement layer("layer" ) ;
      layer.SetAttribute( "nLadders" , layout.getNLadders( i ) ) ;
      layer.SetDoubleAttribute( "phi0" , layout.getPhi0( i ) ) ;
      
      TiXmlElement ladder("ladder") ;
      ladder.SetDoubleAttribute( "distance" , layout.getLadderDistance( i ) ) ;
      ladder.SetDoubleAttribute( "thickness" , layout.getLadderThickness( i ) ) ;
      ladder.SetDoubleAttribute( "width" , layout.getLadderWidth( i ) ) ;
      ladder.SetDoubleAttribute( "length" , layout.getLadderLength( i ) ) ;
      ladder.SetDoubleAttribute( "offset" , layout.getLadderOffset( i ) ) ;
      ladder.SetDoubleAttribute( "radLength" , layout.getLadderRadLength( i ) ) ;
      
      TiXmlElement sens("sensitive" ) ;
      sens.SetDoubleAttribute( "distance" , layout.getSensitiveDistance( i ) ) ;
      sens.SetDoubleAttribute( "thickness" , layout.getSensitiveThickness( i ) ) ;
      sens.SetDoubleAttribute( "width" , layout.getSensitiveWidth( i ) ) ;
      sens.SetDoubleAttribute( "length" , layout.getSensitiveLength( i ) ) ;
      sens.SetDoubleAttribute( "offset" , layout.getSensitiveOffset( i ) ) ;
      sens.SetDoubleAttribute( "radLength" , layout.getSensitiveRadLength( i ) ) ;

      // assemble layer
      layer.InsertEndChild(ladder);
      layer.InsertEndChild(sens) ;
      layers.InsertEndChild(layer) ;
      
    }

    // assemble layers
    det.InsertEndChild(layers) ;

    // Assemble Detector
    GearParametersXML::getXMLForParameters( &det , &parameters ) ;

    return det ;

  }

  GearParameters* ZPlanarParametersXML::fromXML( const TiXmlElement* xmlElement, GearMgr* gearMgr) const {

    // type

    const TiXmlElement* typeEle = 0 ;

    typeEle = xmlElement->FirstChildElement( "type" ) ;
      
    if( typeEle == 0 )  // old file with VXDParameters 
      typeEle = xmlElement->FirstChildElement( "vxdType" ) ;
    

    int intType = 0 ;
    
    std::string type =  getXMLAttribute( typeEle , "technology" )  ; 

	//const char* strType = type.c_str() ;

    //std::cout << "ZPlanarParameters::fromXML vxdType == '" << strType << "'" 
	//		<< " string: " << type << std::endl ; // debug

    if(  type == "CCD"  ) {
      intType = ZPlanarParameters::CCD ;
     }
	else if( type =="CMOS"  ) {
      intType = ZPlanarParameters::CMOS ;
    }
	else if( type == "HYBRID"  ) {
      intType = ZPlanarParameters::HYBRID ;
    }
	else{
	  throw Exception( "ZPlanarParametersXML::fromXML technology of vxdType not known: " + type + 
	                   " - Needs to be 'CCD', 'CMOS' or 'HYBRID'." ) ;
    }

    const TiXmlElement* shell = xmlElement->FirstChildElement( "shell" ) ;

    double shellHalfLength = atof( getXMLAttribute( shell , "halfLength" ) .c_str() ) ;
    double shellGap = atof( getXMLAttribute( shell , "gap" ) . c_str() ) ;
    double shellInnerR = atof( getXMLAttribute( shell , "innerRadius" ) .c_str() ) ;
    double shellOuterR = atof( getXMLAttribute( shell , "outerRadius" ) .c_str() ) ;
    double shellRadLen = atof( getXMLAttribute( shell , "radLength" ) .c_str() ) ;

    // create VDXParameters
    ZPlanarParametersImpl* param = new ZPlanarParametersImpl( intType , shellInnerR , shellOuterR , shellHalfLength , shellGap , shellRadLen ) ;

    // layers
    const TiXmlNode* xmlLayers = xmlElement->FirstChildElement( "layers" ) ;
    const TiXmlNode* xmlLayer = 0 ;
    
    while( ( xmlLayer = xmlLayers->IterateChildren( "layer" , xmlLayer ) ) != 0 ) {
      
      int nLadders = atoi( getXMLAttribute( xmlLayer , "nLadders" ).c_str() ) ;
      double phi0  = atof( getXMLAttribute( xmlLayer , "phi0"     ).c_str() ) ;

      const TiXmlNode* xmlLad = xmlLayer->FirstChildElement( "ladder" ) ;
      const TiXmlNode* xmlSen = xmlLayer->FirstChildElement( "sensitive" ) ;

      double lDist   = atof( getXMLAttribute( xmlLad , "distance"  ).c_str() ) ;
      double lThick  = atof( getXMLAttribute( xmlLad , "thickness" ).c_str() ) ;
      double lWidth  = atof( getXMLAttribute( xmlLad , "width"     ).c_str() ) ;
      double lLength = atof( getXMLAttribute( xmlLad , "length"    ).c_str() ) ;
      double lOffset = atof( getXMLAttribute( xmlLad , "offset"    ).c_str() ) ;
      double lRadLen = atof( getXMLAttribute( xmlLad , "radLength" ).c_str() ) ;
    
      double sDist   = atof( getXMLAttribute( xmlSen , "distance"  ).c_str() ) ;
      double sThick  = atof( getXMLAttribute( xmlSen , "thickness" ).c_str() ) ;
      double sWidth  = atof( getXMLAttribute( xmlSen , "width"     ).c_str() ) ;
      double sLength = atof( getXMLAttribute( xmlSen , "length"    ).c_str() ) ;
      double sOffset = atof( getXMLAttribute( xmlSen , "offset"    ).c_str() ) ;
      double sRadLen = atof( getXMLAttribute( xmlSen , "radLength" ).c_str() ) ;

      param->addLayer( nLadders, phi0 ,
			  lDist, lOffset, lThick, lLength, lWidth, lRadLen,
			  sDist, sOffset, sThick, sLength, sWidth, sRadLen ) ;

    } // end loop

      
    //--------- add to proper section in GearMgr ----
    if( gearMgr != 0 ) {
      
      std::string detName = getXMLAttribute( xmlElement , "name" ) ;
      

      std::transform( detName.begin() , detName.end() , 
		      detName.begin(), tolower ) ;
      
      
      if( detName.find("vxd") != std::string::npos ){
	
	gearMgr->setVXDParameters( param ) ;	  
      }
      else if(  detName.find("sit") != std::string::npos ){
	
	gearMgr->setSITParameters( param ) ;	  
      }
      else if(  detName.find("set") != std::string::npos ){
	
	gearMgr->setSETParameters( param ) ;	  
      }
      
    }

    return param ;

  } // fromXML

} // namespace
  
    
