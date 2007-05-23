#include "gearxml/SiPlanesParametersXML.h"

#include "gearxml/XMLHandlerMgr.h"
#include "gearxml/GearParametersXML.h"

#include "gearxml/tinyxml.h"
#include "gearimpl/SiPlanesParametersImpl.h"

#include "gear/GearMgr.h"

#include <vector>
#include <string>

namespace gear {

  TiXmlElement SiPlanesParametersXML::toXML( const GearParameters & parameters ) const {

   std::cout << "SiPlanesParameters::toXML called" << std::endl ; //debug

    // check whether parameter is valid SiPlanesParameter
    const SiPlanesParameters* param = dynamic_cast<const SiPlanesParameters*> ( &parameters ) ;

    if( param == 0 ) {

      throw Exception( "SiPlanesParametersXML::toXML given parameters not of correct type. "
		       "needs to be gear::SiPlanesParameters." ) ;
    }

    // Set up Beam TelescopeWithDUT or TelescopeWithoutDUT as Element
    TiXmlElement det("detector") ;

    //type
    TiXmlElement type( "siplanesType" ) ;
    std::string strType ;
    switch( param->getSiPlanesType() ) {
    case ( SiPlanesParameters::TelescopeWithDUT ) :
      strType = "TelescopeWithDUT" ;
      break ;
    case ( SiPlanesParameters::TelescopeWithoutDUT ) :
      strType = "TelescopeWithoutDUT" ;
      break ;
    default :
      strType = "Unknown" ;
    }

    type.SetAttribute( "type", strType ) ;
    det.InsertEndChild( type ) ;

    //    std::cout << "SiPlanesParameters::toXML strType == '" << strType << "'"<< std::endl ; // debug

    //    std::cout << " N of planes = " << param->getSiPlanesNumber() << std::endl ; //debug

    TiXmlElement nplanes( "siplanesNumber" ) ;
    nplanes.SetAttribute("siplanesNumber", param->getSiPlanesNumber()) ;
    det.InsertEndChild( nplanes ) ;

    // layerLayout
    const SiPlanesLayerLayout& siplanesLayers = param->getSiPlanesLayerLayout() ;

    if (SiPlanesParameters::TelescopeWithDUT){

    TiXmlElement dut("dut" ) ;
    dut.SetAttribute( "dut_ID" , siplanesLayers.getDUTID() ) ;

    //    std::cout << " DUT ID = " << siplanesLayers.getDUTID() << std::endl ; //debug

    TiXmlElement dutladder("dut_ladder") ;
    dutladder.SetDoubleAttribute( "positionX" , siplanesLayers.getDUTPositionX() ) ;
    dutladder.SetDoubleAttribute( "positionY" , siplanesLayers.getDUTPositionY() ) ;
    dutladder.SetDoubleAttribute( "positionZ" , siplanesLayers.getDUTPositionZ() ) ;
    dutladder.SetDoubleAttribute( "sizeX" , siplanesLayers.getDUTSizeX() ) ;
    dutladder.SetDoubleAttribute( "sizeY" , siplanesLayers.getDUTSizeY() ) ;
    dutladder.SetDoubleAttribute( "thickness" , siplanesLayers.getDUTThickness() ) ;
    dutladder.SetDoubleAttribute( "radLength" , siplanesLayers.getDUTRadLength() ) ;
    
    TiXmlElement dutsens("dut_sensitive" ) ;
    dutsens.SetDoubleAttribute( "positionX" , siplanesLayers.getDUTSensitivePositionX() ) ;
    dutsens.SetDoubleAttribute( "positionY" , siplanesLayers.getDUTSensitivePositionY() ) ;
    dutsens.SetDoubleAttribute( "positionZ" , siplanesLayers.getDUTSensitivePositionZ() ) ;
    dutsens.SetDoubleAttribute( "sizeX" , siplanesLayers.getDUTSensitiveSizeX() ) ;
    dutsens.SetDoubleAttribute( "sizeY" , siplanesLayers.getDUTSensitiveSizeY() ) ;
    dutsens.SetDoubleAttribute( "thickness" , siplanesLayers.getDUTSensitiveThickness() ) ;
    dutsens.SetDoubleAttribute( "radLength" , siplanesLayers.getDUTSensitiveRadLength() ) ;
    
    // assemble dut
    dut.InsertEndChild(dutladder) ;
    dut.InsertEndChild(dutsens) ;
    det.InsertEndChild(dut) ;

    }

    TiXmlElement layers("layers") ;

    for( int i=0 ; i < siplanesLayers.getNLayers() ; i++ ) {

       //      std::cout << " layer " <<  i  << std::endl ; //debug
      
      TiXmlElement layer("layer" ) ;
      layer.SetAttribute( "ID" , siplanesLayers.getID( i ) ) ;

      TiXmlElement ladder("ladder") ;
      ladder.SetDoubleAttribute( "positionX" , siplanesLayers.getLayerPositionX( i ) ) ;
      ladder.SetDoubleAttribute( "positionY" , siplanesLayers.getLayerPositionY( i ) ) ;
      ladder.SetDoubleAttribute( "positionZ" , siplanesLayers.getLayerPositionZ( i ) ) ;
      ladder.SetDoubleAttribute( "sizeX" , siplanesLayers.getLayerSizeX( i ) ) ;
      ladder.SetDoubleAttribute( "sizeY" , siplanesLayers.getLayerSizeY( i ) ) ;
      ladder.SetDoubleAttribute( "thickness" , siplanesLayers.getLayerThickness( i ) ) ;
      ladder.SetDoubleAttribute( "radLength" , siplanesLayers.getLayerRadLength( i ) ) ;
      
      TiXmlElement sens("sensitive" ) ;
      sens.SetDoubleAttribute( "positionX" , siplanesLayers.getSensitivePositionX( i ) ) ;
      sens.SetDoubleAttribute( "positionY" , siplanesLayers.getSensitivePositionY( i ) ) ;
      sens.SetDoubleAttribute( "positionZ" , siplanesLayers.getSensitivePositionZ( i ) ) ;
      sens.SetDoubleAttribute( "sizeX" , siplanesLayers.getSensitiveSizeX( i ) ) ;
      sens.SetDoubleAttribute( "sizeY" , siplanesLayers.getSensitiveSizeY( i ) ) ;
      sens.SetDoubleAttribute( "thickness" , siplanesLayers.getSensitiveThickness( i ) ) ;
      sens.SetDoubleAttribute( "radLength" , siplanesLayers.getSensitiveRadLength( i ) ) ;

      // assemble layer
      layer.InsertEndChild(ladder) ;
      layer.InsertEndChild(sens) ;
      layers.InsertEndChild(layer);
      
    }
    
    det.InsertEndChild(layers) ;

    // Assemble Detector
    GearParametersXML::getXMLForParameters( &det , &parameters ) ;

    return det ;

  }

  GearParameters* SiPlanesParametersXML::fromXML( const TiXmlElement* xmlElement, GearMgr* gearMgr) const {

    // type

    const TiXmlElement* siplanesType = xmlElement->FirstChildElement( "siplanesType" ) ;
    int intType = 0 ;
    const char* strType = getXMLAttribute( siplanesType , "type" ) .c_str() ;

    std::cout << "SiPlanesParameters::fromXML siplanesType == '" << strType << "'" << std::endl ; // debug

    if( !std::strcmp( strType , "TelescopeWithDUT" ) ) {
      intType = SiPlanesParameters::TelescopeWithDUT ;
     }
    if( !std::strcmp( strType ,"TelescopeWithoutDUT" ) ) {
      intType = SiPlanesParameters::TelescopeWithoutDUT ;
    }
    if( std::strcmp( strType , "TelescopeWithDUT" ) && std::strcmp( strType ,"TelescopeWithoutDUT" ) ) {
      throw Exception( "SiPlanesParametersXML::fromXML type of SiPlanes not known."
		       "Needs to be 'TelescopeWithDUT' or 'TelescopeWithoutDUT'." ) ;
    }

    const TiXmlElement* siplanesNumber = xmlElement->FirstChildElement( "siplanesNumber" ) ;
    int nplanes = atoi( getXMLAttribute( siplanesNumber , "siplanesNumber" ).c_str() ) ;

    //    std::cout << "SiPlanesParameters::fromXML siplanesNumber == " << nplanes << std::endl ; // debug

    // create SiPlanesParameters
    SiPlanesParametersImpl* siplanesParam = new SiPlanesParametersImpl( intType , nplanes) ;

    const TiXmlNode* xmlDUT = xmlElement->FirstChildElement( "dut" ) ;
    //    xmlDUT = xmlLayers->IterateChildren( "dut" , xmlDUT );

    int dutID = atoi(getXMLAttribute( xmlDUT , "dut_ID" ).c_str() );

    const TiXmlNode* xmlDUTLad = xmlDUT->FirstChildElement( "dut_ladder" ) ;
    const TiXmlNode* xmlDUTSen = xmlDUT->FirstChildElement( "dut_sensitive" ) ;

    double ldutPosX   = atof(getXMLAttribute( xmlDUTLad , "positionX" ).c_str() ) ;
    double ldutPosY   = atof(getXMLAttribute( xmlDUTLad , "positionY" ).c_str() ) ;
    double ldutPosZ   = atof(getXMLAttribute( xmlDUTLad , "positionZ" ).c_str() ) ;
    double ldutSizX   = atof(getXMLAttribute( xmlDUTLad , "sizeX" ).c_str() ) ;
    double ldutSizY   = atof(getXMLAttribute( xmlDUTLad , "sizeY" ).c_str() ) ;
    double ldutThick   = atof(getXMLAttribute( xmlDUTLad , "thickness" ).c_str() ) ;
    double ldutRadLen = atof(getXMLAttribute( xmlDUTLad , "radLength" ).c_str() ) ;

    double sdutPosX   = atof(getXMLAttribute( xmlDUTSen , "positionX" ).c_str() ) ;
    double sdutPosY   = atof(getXMLAttribute( xmlDUTSen , "positionY" ).c_str() ) ;
    double sdutPosZ   = atof(getXMLAttribute( xmlDUTSen , "positionZ" ).c_str() ) ;
    double sdutSizX   = atof(getXMLAttribute( xmlDUTSen , "sizeX" ).c_str() ) ;
    double sdutSizY   = atof(getXMLAttribute( xmlDUTSen , "sizeY" ).c_str() ) ;
    double sdutThick   = atof(getXMLAttribute( xmlDUTSen , "thickness" ).c_str() ) ;
    double sdutRadLen = atof(getXMLAttribute( xmlDUTSen , "radLength" ).c_str() ) ;
    
    siplanesParam->addDUT(dutID, ldutPosX, ldutPosY, ldutPosZ, ldutSizX, ldutSizY, ldutThick, ldutRadLen, sdutPosX, sdutPosY, sdutPosZ, sdutSizX, sdutSizY, sdutThick, sdutRadLen) ;

    // layers
    const TiXmlNode* xmlLayers = xmlElement->FirstChildElement( "layers" ) ;

    const TiXmlNode* xmlLayer = 0 ;
    while( ( xmlLayer = xmlLayers->IterateChildren( "layer" , xmlLayer ) ) != 0 ) {

    int layerID = atoi( getXMLAttribute( xmlLayer , "ID" ).c_str() ) ;

    const TiXmlNode* xmlLad = xmlLayer->FirstChildElement( "ladder" ) ;
    const TiXmlNode* xmlSen = xmlLayer->FirstChildElement( "sensitive" ) ;
    
    double lPosX   = atof( getXMLAttribute( xmlLad , "positionX" ).c_str() ) ;
    double lPosY   = atof( getXMLAttribute( xmlLad , "positionY" ).c_str() ) ;
    double lPosZ   = atof( getXMLAttribute( xmlLad , "positionZ" ).c_str() ) ;
    double lSizX   = atof( getXMLAttribute( xmlLad , "sizeX" ).c_str() ) ;
    double lSizY   = atof( getXMLAttribute( xmlLad , "sizeY" ).c_str() ) ;
    double lThick   = atof( getXMLAttribute( xmlLad , "thickness" ).c_str() ) ;
    double lRadLen = atof( getXMLAttribute( xmlLad , "radLength" ).c_str() ) ;

    double sPosX   = atof( getXMLAttribute( xmlSen , "positionX" ).c_str() ) ;
    double sPosY   = atof( getXMLAttribute( xmlSen , "positionY" ).c_str() ) ;
    double sPosZ   = atof( getXMLAttribute( xmlSen , "positionZ" ).c_str() ) ;
    double sSizX   = atof( getXMLAttribute( xmlSen , "sizeX" ).c_str() ) ;
    double sSizY   = atof( getXMLAttribute( xmlSen , "sizeY" ).c_str() ) ;
    double sThick   = atof( getXMLAttribute( xmlSen , "thickness" ).c_str() ) ;
    double sRadLen = atof( getXMLAttribute( xmlSen , "radLength" ).c_str() ) ;
    
    siplanesParam->addLayer(layerID, lPosX, lPosY, lPosZ, lSizX, lSizY, lThick, lRadLen, sPosX, sPosY, sPosZ, sSizX, sSizY, sThick, sRadLen) ;

    } // end loop


    // ------- add to GearMgr ----
    if( gearMgr != 0 ) {
      
      gearMgr->setSiPlanesParameters( siplanesParam ) ;

    }

    return siplanesParam ;

  } // fromXML

} // namespace
  
    
