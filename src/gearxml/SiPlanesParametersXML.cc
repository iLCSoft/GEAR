#include "gearxml/SiPlanesParametersXML.h"

#include "gearxml/XMLHandlerMgr.h"
#include "gearxml/GearParametersXML.h"

#include "tinyxml/tinyxml.h"
#include "gearimpl/SiPlanesParametersImpl.h"

#include "gear/GearMgr.h"

#include <vector>
#include <string>

 
#include <cstring>

  /** Abstract description of layers in pixel beam telescope with or without DUT.
   *  This assumes a number of silicon layers, arranged perpendicular to the beam.
   *  @author T Klimkovich, DESY
   *  @author I. Rubinskiy, DESY
   *  @version $Id: 
   */

namespace gear {

  TiXmlElement SiPlanesParametersXML::toXML( const GearParameters & parameters ) const {


    // check whether parameter is valid SiPlanesParameter
    const SiPlanesParameters* param = dynamic_cast<const SiPlanesParameters*> ( &parameters ) ;

    if( param == 0 ) {

      throw Exception( "SiPlanesParametersXML::toXML given parameters not of correct type. "
		       "needs to be gear::SiPlanesParameters." ) ;
    }

    // Set up Beam TelescopeWithDUT or TelescopeWithoutDUT as Element
    TiXmlElement det("detector") ;

    TiXmlElement setup_id( "siplanesID" ) ;
    setup_id.SetAttribute("ID", param->getSiPlanesID()) ;
    det.InsertEndChild( setup_id ) ;

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



    TiXmlElement nplanes( "siplanesNumber" ) ;
    nplanes.SetAttribute("number", param->getSiPlanesNumber()) ;
    det.InsertEndChild( nplanes ) ;

    // layerLayout
    const SiPlanesLayerLayout& siplanesLayers = param->getSiPlanesLayerLayout() ;

    if (strType == "TelescopeWithDUT"){
      
      TiXmlElement dut("dut" ) ;
      
      TiXmlElement dutladder("dut_ladder") ;
      dutladder.SetAttribute( "ID" , siplanesLayers.getDUTID() ) ;
      dutladder.SetDoubleAttribute( "positionX" , siplanesLayers.getDUTPositionX() ) ;
      dutladder.SetDoubleAttribute( "positionY" , siplanesLayers.getDUTPositionY() ) ;
      dutladder.SetDoubleAttribute( "positionZ" , siplanesLayers.getDUTPositionZ() ) ;
      dutladder.SetDoubleAttribute( "sizeX" , siplanesLayers.getDUTSizeX() ) ;
      dutladder.SetDoubleAttribute( "sizeY" , siplanesLayers.getDUTSizeY() ) ;
      dutladder.SetDoubleAttribute( "thickness" , siplanesLayers.getDUTThickness() ) ;
      dutladder.SetDoubleAttribute( "radLength" , siplanesLayers.getDUTRadLength() ) ;
      
      TiXmlElement dutsens("dut_sensitive" ) ;
      dutsens.SetAttribute( "ID" , siplanesLayers.getDUTSensitiveID() ) ;
      dutsens.SetDoubleAttribute( "positionX" , siplanesLayers.getDUTSensitivePositionX() ) ;
      dutsens.SetDoubleAttribute( "positionY" , siplanesLayers.getDUTSensitivePositionY() ) ;
      dutsens.SetDoubleAttribute( "positionZ" , siplanesLayers.getDUTSensitivePositionZ() ) ;
      dutsens.SetDoubleAttribute( "sizeX" , siplanesLayers.getDUTSensitiveSizeX() ) ;
      dutsens.SetDoubleAttribute( "sizeY" , siplanesLayers.getDUTSensitiveSizeY() ) ;
      dutsens.SetDoubleAttribute( "thickness" , siplanesLayers.getDUTSensitiveThickness() ) ;
      dutsens.SetAttribute( "npixelX" , siplanesLayers.getDUTSensitiveNpixelX() ) ;
      dutsens.SetAttribute( "npixelY" , siplanesLayers.getDUTSensitiveNpixelY() ) ;
      dutsens.SetDoubleAttribute( "pitchX" , siplanesLayers.getDUTSensitivePitchX() ) ;
      dutsens.SetDoubleAttribute( "pitchY" , siplanesLayers.getDUTSensitivePitchY() ) ;
      dutsens.SetDoubleAttribute( "resolution" , siplanesLayers.getDUTSensitiveResolution() ) ;
      dutsens.SetDoubleAttribute( "rotation1" , siplanesLayers.getDUTSensitiveRotation1() ) ;
      dutsens.SetDoubleAttribute( "rotation2" , siplanesLayers.getDUTSensitiveRotation2() ) ;
      dutsens.SetDoubleAttribute( "rotation3" , siplanesLayers.getDUTSensitiveRotation3() ) ;
      dutsens.SetDoubleAttribute( "rotation4" , siplanesLayers.getDUTSensitiveRotation4() ) ;
      dutsens.SetDoubleAttribute( "radLength" , siplanesLayers.getDUTSensitiveRadLength() ) ;
      
      // assemble dut
      dut.InsertEndChild(dutladder) ;
      dut.InsertEndChild(dutsens) ;
      det.InsertEndChild(dut) ;
      
    }
    
    TiXmlElement layers("layers") ;

    for( int i=0 ; i < siplanesLayers.getNLayers() ; i++ ) {

      
      TiXmlElement layer("layer" ) ;

      TiXmlElement ladder("ladder") ;
      ladder.SetAttribute( "ID" , siplanesLayers.getID( i ) ) ;
      ladder.SetDoubleAttribute( "positionX" , siplanesLayers.getLayerPositionX( i ) ) ;
      ladder.SetDoubleAttribute( "positionY" , siplanesLayers.getLayerPositionY( i ) ) ;
      ladder.SetDoubleAttribute( "positionZ" , siplanesLayers.getLayerPositionZ( i ) ) ;
      ladder.SetDoubleAttribute( "rotationXY" , siplanesLayers.getLayerRotationXY( i ) ) ;
      ladder.SetDoubleAttribute( "rotationZX" , siplanesLayers.getLayerRotationZX( i ) ) ;
      ladder.SetDoubleAttribute( "rotationZY" , siplanesLayers.getLayerRotationZY( i ) ) ;
      ladder.SetDoubleAttribute( "sizeX" , siplanesLayers.getLayerSizeX( i ) ) ;
      ladder.SetDoubleAttribute( "sizeY" , siplanesLayers.getLayerSizeY( i ) ) ;
      ladder.SetDoubleAttribute( "thickness" , siplanesLayers.getLayerThickness( i ) ) ;
      ladder.SetDoubleAttribute( "radLength" , siplanesLayers.getLayerRadLength( i ) ) ;
      
      TiXmlElement sens("sensitive" ) ;
      sens.SetAttribute( "ID" , siplanesLayers.getSensitiveID( i ) ) ;
      sens.SetDoubleAttribute( "positionX" , siplanesLayers.getSensitivePositionX( i ) ) ;
      sens.SetDoubleAttribute( "positionY" , siplanesLayers.getSensitivePositionY( i ) ) ;
      sens.SetDoubleAttribute( "positionZ" , siplanesLayers.getSensitivePositionZ( i ) ) ;
      sens.SetDoubleAttribute( "sizeX" , siplanesLayers.getSensitiveSizeX( i ) ) ;
      sens.SetDoubleAttribute( "sizeY" , siplanesLayers.getSensitiveSizeY( i ) ) ;
      sens.SetDoubleAttribute( "thickness" , siplanesLayers.getSensitiveThickness( i ) ) ;
      sens.SetAttribute( "npixelX" , siplanesLayers.getSensitiveNpixelX( i ) ) ;
      sens.SetAttribute( "npixelY" , siplanesLayers.getSensitiveNpixelY( i ) ) ;
      sens.SetDoubleAttribute( "pitchX" , siplanesLayers.getSensitivePitchX( i ) ) ;
      sens.SetDoubleAttribute( "pitchY" , siplanesLayers.getSensitivePitchY( i ) ) ;
      sens.SetDoubleAttribute( "resolution" , siplanesLayers.getSensitiveResolution( i ) ) ;
      sens.SetDoubleAttribute( "rotation1" , siplanesLayers.getSensitiveRotation1( i ) ) ;
      sens.SetDoubleAttribute( "rotation2" , siplanesLayers.getSensitiveRotation2( i ) ) ;
      sens.SetDoubleAttribute( "rotation3" , siplanesLayers.getSensitiveRotation3( i ) ) ;
      sens.SetDoubleAttribute( "rotation4" , siplanesLayers.getSensitiveRotation4( i ) ) ;
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

    // setup ID

    const TiXmlElement* siplanesID = xmlElement->FirstChildElement( "siplanesID" ) ;
    int setupID = atoi( getXMLAttribute( siplanesID , "ID" ).c_str() ) ;


    // type

    const TiXmlElement* siplanesType = xmlElement->FirstChildElement( "siplanesType" ) ;
    int intType = 0 ;
    std::string strType = getXMLAttribute( siplanesType , "type" ) ;
    

    if( strType == "TelescopeWithDUT"  ) {

      intType = SiPlanesParameters::TelescopeWithDUT ;

    } else if( strType == "TelescopeWithoutDUT" ) {

      intType = SiPlanesParameters::TelescopeWithoutDUT ;

    } else {
      throw Exception( "SiPlanesParametersXML::fromXML type of SiPlanes not known."
		       "Needs to be 'TelescopeWithDUT' or 'TelescopeWithoutDUT'." ) ;
    }

    // number of telescope planes

    const TiXmlElement* siplanesNumber = xmlElement->FirstChildElement( "siplanesNumber" ) ;
    int nplanes = atoi( getXMLAttribute( siplanesNumber , "number" ).c_str() ) ;


    // create SiPlanesParameters
    SiPlanesParametersImpl* siplanesParam = new SiPlanesParametersImpl( setupID, intType , nplanes) ;

    if (intType == SiPlanesParameters::TelescopeWithDUT) {
      const TiXmlNode* xmlDUT = xmlElement->FirstChildElement( "dut" ) ;
      const TiXmlNode* xmlDUTLad = xmlDUT->FirstChildElement( "dut_ladder" ) ;
      const TiXmlNode* xmlDUTSen = xmlDUT->FirstChildElement( "dut_sensitive" ) ;
      
      int ldutID = atoi(getXMLAttribute( xmlDUTLad , "ID" ).c_str() );
      double ldutPosX   = atof(getXMLAttribute( xmlDUTLad , "positionX" ).c_str() ) ;
      double ldutPosY   = atof(getXMLAttribute( xmlDUTLad , "positionY" ).c_str() ) ;
      double ldutPosZ   = atof(getXMLAttribute( xmlDUTLad , "positionZ" ).c_str() ) ;
      double ldutSizX   = atof(getXMLAttribute( xmlDUTLad , "sizeX" ).c_str() ) ;
      double ldutSizY   = atof(getXMLAttribute( xmlDUTLad , "sizeY" ).c_str() ) ;
      double ldutThick   = atof(getXMLAttribute( xmlDUTLad , "thickness" ).c_str() ) ;
      double ldutRadLen = atof(getXMLAttribute( xmlDUTLad , "radLength" ).c_str() ) ;
      
      int sdutID = atoi(getXMLAttribute( xmlDUTSen , "ID" ).c_str() );
      double sdutPosX   = atof(getXMLAttribute( xmlDUTSen , "positionX" ).c_str() ) ;
      double sdutPosY   = atof(getXMLAttribute( xmlDUTSen , "positionY" ).c_str() ) ;
      double sdutPosZ   = atof(getXMLAttribute( xmlDUTSen , "positionZ" ).c_str() ) ;
      double sdutSizX   = atof(getXMLAttribute( xmlDUTSen , "sizeX" ).c_str() ) ;
      double sdutSizY   = atof(getXMLAttribute( xmlDUTSen , "sizeY" ).c_str() ) ;
      double sdutThick   = atof(getXMLAttribute( xmlDUTSen , "thickness" ).c_str() ) ;
      int sdutNPixX   = atoi(getXMLAttribute( xmlDUTSen , "npixelX" ).c_str() ) ;
      int sdutNPixY   = atoi(getXMLAttribute( xmlDUTSen , "npixelY" ).c_str() ) ;
      double sdutPitX   = atof(getXMLAttribute( xmlDUTSen , "pitchX" ).c_str() ) ;
      double sdutPitY   = atof(getXMLAttribute( xmlDUTSen , "pitchY" ).c_str() ) ;
      double sdutResol   = atof(getXMLAttribute( xmlDUTSen , "resolution" ).c_str() ) ;
      double sdutRotat1 =  atof(getOptionalXMLAttribute( xmlDUTSen , "rotation1", "1.0" ).c_str() ) ;
      double sdutRotat2 =  atof(getOptionalXMLAttribute( xmlDUTSen , "rotation2", "0.0" ).c_str() ) ;
      double sdutRotat3 =  atof(getOptionalXMLAttribute( xmlDUTSen , "rotation3", "0.0" ).c_str() ) ;
      double sdutRotat4 =  atof(getOptionalXMLAttribute( xmlDUTSen , "rotation4", "1.0" ).c_str() ) ;
      double sdutRadLen = atof(getXMLAttribute( xmlDUTSen , "radLength" ).c_str() ) ;
    
      siplanesParam->addDUT(ldutID, ldutPosX, ldutPosY, ldutPosZ, ldutSizX, ldutSizY, ldutThick, ldutRadLen, sdutID, sdutPosX, sdutPosY, sdutPosZ, sdutSizX, sdutSizY, sdutThick, sdutNPixX, sdutNPixY, sdutPitX, sdutPitY, sdutResol, sdutRotat1, sdutRotat2,sdutRotat3,sdutRotat4, sdutRadLen) ;
    }

    // layers
    const TiXmlNode* xmlLayers = xmlElement->FirstChildElement( "layers" ) ;

    const TiXmlNode* xmlLayer = 0 ;
    while( ( xmlLayer = xmlLayers->IterateChildren( "layer" , xmlLayer ) ) != 0 ) {

    const TiXmlNode* xmlLad = xmlLayer->FirstChildElement( "ladder" ) ;
    const TiXmlNode* xmlSen = xmlLayer->FirstChildElement( "sensitive" ) ;
    
    int lID = atoi( getXMLAttribute( xmlLad , "ID" ).c_str() ) ;
    double lPosX   = atof( getXMLAttribute( xmlLad , "positionX" ).c_str() ) ;
    double lPosY   = atof( getXMLAttribute( xmlLad , "positionY" ).c_str() ) ;
    double lPosZ   = atof( getXMLAttribute( xmlLad , "positionZ" ).c_str() ) ;
    double lRotXY  = atof( getOptionalXMLAttribute( xmlLad , "rotationXY", "0." ).c_str() ) ;
    double lRotZX  = atof( getOptionalXMLAttribute( xmlLad , "rotationZX", "0." ).c_str() ) ;  
    double lRotZY  = atof( getOptionalXMLAttribute( xmlLad , "rotationZY", "0." ).c_str() ) ;
    double lSizX   = atof( getXMLAttribute( xmlLad , "sizeX" ).c_str() ) ;
    double lSizY   = atof( getXMLAttribute( xmlLad , "sizeY" ).c_str() ) ;
    double lThick   = atof( getXMLAttribute( xmlLad , "thickness" ).c_str() ) ;
    double lRadLen = atof( getXMLAttribute( xmlLad , "radLength" ).c_str() ) ;

    int sID = atoi( getXMLAttribute( xmlSen , "ID" ).c_str() ) ;
    double sPosX   = atof( getXMLAttribute( xmlSen , "positionX" ).c_str() ) ;
    double sPosY   = atof( getXMLAttribute( xmlSen , "positionY" ).c_str() ) ;
    double sPosZ   = atof( getXMLAttribute( xmlSen , "positionZ" ).c_str() ) ;
    double sRotXY  = atof( getOptionalXMLAttribute( xmlSen , "rotationXY", "0." ).c_str() ) ;
    double sRotZX  = atof( getOptionalXMLAttribute( xmlSen , "rotationZX", "0." ).c_str() ) ;  
    double sRotZY  = atof( getOptionalXMLAttribute( xmlSen , "rotationZY", "0." ).c_str() ) ;
    double sSizX   = atof( getXMLAttribute( xmlSen , "sizeX" ).c_str() ) ;
    double sSizY   = atof( getXMLAttribute( xmlSen , "sizeY" ).c_str() ) ;
    double sThick   = atof( getXMLAttribute( xmlSen , "thickness" ).c_str() ) ;
    int sNPixX   = atoi(getXMLAttribute( xmlSen , "npixelX" ).c_str() ) ;
    int sNPixY   = atoi(getXMLAttribute( xmlSen , "npixelY" ).c_str() ) ;
    double sPitX   = atof(getXMLAttribute( xmlSen , "pitchX" ).c_str() ) ;
    double sPitY   = atof(getXMLAttribute( xmlSen , "pitchY" ).c_str() ) ;
    double sResol   = atof(getXMLAttribute( xmlSen , "resolution" ).c_str() ) ;
    double sResolX   = atof(getOptionalXMLAttribute( xmlSen , "resolutionX", "-1.0" ).c_str() ) ;
    double sResolY   = atof(getOptionalXMLAttribute( xmlSen , "resolutionY"," -1.0" ).c_str() ) ;
    if( sResolX < 0 ) sResolX = sResol;
    if( sResolY < 0 ) sResolY = sResol;
    double sRotat1 = atof(getOptionalXMLAttribute( xmlSen , "rotation1", "1.0" ).c_str() ) ;
    double sRotat2 = atof(getOptionalXMLAttribute( xmlSen , "rotation2", "0.0" ).c_str() ) ;
    double sRotat3 = atof(getOptionalXMLAttribute( xmlSen , "rotation3", "0.0" ).c_str() ) ;
    double sRotat4 = atof(getOptionalXMLAttribute( xmlSen , "rotation4", "1.0" ).c_str() ) ;
    double sRadLen = atof( getXMLAttribute( xmlSen , "radLength" ).c_str() ) ;
    
    siplanesParam->addLayer(lID, lPosX, lPosY, lPosZ, lRotXY, lRotZX, lRotZY, lSizX, lSizY, lThick, lRadLen, 
                            sID, sPosX, sPosY, sPosZ, sRotXY, sRotZX, sRotZY, sSizX, sSizY, sThick, sNPixX, sNPixY, sPitX, sPitY,  sResol,  sResolX, sResolY, sRotat1, sRotat2, sRotat3, sRotat4, sRadLen) ;

    } // end loop



    // now read the generic parameters
    GearParametersXML::setParametersFromXML( xmlElement, siplanesParam  ) ;
    
    // ------- add to GearMgr ----
    if( gearMgr != 0 ) {
      
      gearMgr->setSiPlanesParameters( siplanesParam ) ;

    }

    return siplanesParam ;

  } // fromXML

} // namespace
  
    
