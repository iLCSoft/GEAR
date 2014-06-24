#include "gearxml/TrackerPlanesParametersXML.h"

#include "gearxml/XMLHandlerMgr.h"
#include "gearxml/GearParametersXML.h"

#include "gearxml/tinyxml.h"
#include "gearimpl/TrackerPlanesParametersImpl.h"

#include "gear/GearMgr.h"

#include <iostream>
#include <vector>
#include <string>

 
#include <cstring>

  /** Abstract description of layers in pixel beam telescope .
   *  This assumes a number of silicon layers, arranged perpendicular to the beam.
   *  @author I. Rubinskiy, DESY
   *  @version $Id: 
   */

using namespace gear;

namespace gear {

  TiXmlElement TrackerPlanesParametersXML::toXML( const GearParameters & parameters ) const {


    // check whether parameter is valid TrackerPlanesParameter
    const TrackerPlanesParameters* param = dynamic_cast<const TrackerPlanesParameters*> ( &parameters ) ;

    if( param == 0 ) {

      throw Exception( "TrackerPlanesParametersXML::toXML given parameters not of correct type. "
		       "needs to be gear::TrackerPlanesParameters." ) ;
    }

    // Set up Beam telescope  as Element
    TiXmlElement det("detector") ;

    TiXmlElement setup_id( "LayoutID" ) ;
    setup_id.SetAttribute("ID", param->getLayoutID()) ;
    det.InsertEndChild( setup_id ) ;


    TiXmlElement nplanes( "LayoutNumberOfLayers" ) ;
    nplanes.SetAttribute("number", param->getLayoutNumberOfLayers()) ;
    det.InsertEndChild( nplanes ) ;

    // layerLayout
    const TrackerPlanesLayerLayout& trackerplanesLayerLayout = param->getTrackerPlanesLayerLayout() ;

    TiXmlElement layers("layers") ;

    for( int i=0 ; i < trackerplanesLayerLayout.getNLayers() ; i++ ) {
      TiXmlElement xmlLayer("layer" ) ;

      TrackerPlanesLayerImpl* layer = const_cast<gear::TrackerPlanesLayerImpl*  > (trackerplanesLayerLayout.getLayer(i));
      if( layer == 0 ) continue;

      TrackerPlanesMaterialLayerImplVec& material = layer->getMaterialLayerVec();

      TrackerPlanesSensitiveLayerImplVec& sensitive = layer->getSensitiveLayerVec();

        for( int i=0 ; i < material.size() ; i++ ) {
           TrackerPlanesMaterialLayerImpl& ladder = material.at(i);

           TiXmlElement xmlLadder("ladder") ;
           xmlLadder.SetAttribute( "ID" , ladder.getID() ) ;
           xmlLadder.SetAttribute( "info" , ladder.getInfo() ) ;
           xmlLadder.SetDoubleAttribute( "positionX" , ladder.getPositionX() ) ;
           xmlLadder.SetDoubleAttribute( "positionY" , ladder.getPositionY() ) ;
           xmlLadder.SetDoubleAttribute( "positionZ" , ladder.getPositionZ() ) ;
           xmlLadder.SetDoubleAttribute( "rotationXY" , ladder.getRotationXY() ) ;
           xmlLadder.SetDoubleAttribute( "rotationZX" , ladder.getRotationZX() ) ;
           xmlLadder.SetDoubleAttribute( "rotationZY" , ladder.getRotationZY() ) ;
           xmlLadder.SetDoubleAttribute( "sizeX" , ladder.getSizeX() ) ;
           xmlLadder.SetDoubleAttribute( "sizeY" , ladder.getSizeY() ) ;
           xmlLadder.SetDoubleAttribute( "thickness" , ladder.getThickness() ) ;
           xmlLadder.SetDoubleAttribute( "radLength" , ladder.getRadLength() ) ;
     
           xmlLayer.InsertEndChild( xmlLadder ) ;
        }
      
 
        for( int i=0 ; i < sensitive.size() ; i++ ) {
           TrackerPlanesSensitiveLayerImpl& sensor = sensitive.at(i);
 
           TiXmlElement xmlSensor("sensitive" ) ;
           xmlSensor.SetAttribute( "ID" , sensor.getID() ) ;
           xmlSensor.SetAttribute( "info" , sensor.getInfo() ) ;
           xmlSensor.SetDoubleAttribute( "positionX" , sensor.getPositionX() ) ;
           xmlSensor.SetDoubleAttribute( "positionY" , sensor.getPositionY() ) ;
           xmlSensor.SetDoubleAttribute( "positionZ" , sensor.getPositionZ() ) ;
           xmlSensor.SetDoubleAttribute( "rotationXY" , sensor.getRotationXY() ) ;
           xmlSensor.SetDoubleAttribute( "rotationZX" , sensor.getRotationZX() ) ;
           xmlSensor.SetDoubleAttribute( "rotationZY" , sensor.getRotationZY() ) ;
           xmlSensor.SetDoubleAttribute( "sizeX" , sensor.getSizeX() ) ;
           xmlSensor.SetDoubleAttribute( "sizeY" , sensor.getSizeY() ) ;
           xmlSensor.SetDoubleAttribute( "thickness" , sensor.getThickness() ) ;
           xmlSensor.SetAttribute( "npixelX" , sensor.getNpixelX() ) ;
           xmlSensor.SetAttribute( "npixelY" , sensor.getNpixelY() ) ;
           xmlSensor.SetDoubleAttribute( "pitchX" , sensor.getPitchX() ) ;
           xmlSensor.SetDoubleAttribute( "pitchY" , sensor.getPitchY() ) ;
           xmlSensor.SetDoubleAttribute( "resolutionX" , sensor.getResolutionX() ) ;
           xmlSensor.SetDoubleAttribute( "resolutionY" , sensor.getResolutionY() ) ;
           xmlSensor.SetDoubleAttribute( "radLength" , sensor.getRadLength() ) ;
 
           xmlLayer.InsertEndChild( xmlSensor ) ;
       }

      det.InsertEndChild( xmlLayer ) ;
    }

    // Assemble Detector
    GearParametersXML::getXMLForParameters( &det , &parameters ) ;

    return det ;

  }

  GearParameters* TrackerPlanesParametersXML::fromXML( const TiXmlElement* xmlElement, GearMgr* gearMgr) const {

    // setup ID

    const TiXmlElement* trackerplanesID = xmlElement->FirstChildElement( "LayoutID" ) ;
    int setupID = atoi( getXMLAttribute( trackerplanesID , "ID" ).c_str() ) ;

    int intType = 0;
 
    const TiXmlElement* trackerplanesNumber = xmlElement->FirstChildElement( "LayoutNumberOfLayers" ) ;
    int nplanes = atoi( getXMLAttribute( trackerplanesNumber , "number" ).c_str() ) ;


    // create TrackerPlanesParameters
    TrackerPlanesParametersImpl* trackerplanesParam = new TrackerPlanesParametersImpl( setupID, intType , nplanes) ;

    // layers
    const TiXmlNode* xmlLayers = xmlElement->FirstChildElement( "layers" ) ;

    const TiXmlNode* xmlLayer = 0 ;
    while( ( xmlLayer = xmlLayers->IterateChildren( "layer" , xmlLayer ) ) != 0 ) {

            TrackerPlanesLayerImpl* layerImpl = new TrackerPlanesLayerImpl() ;

            int layerID = atoi( getXMLAttribute( xmlLayer, "ID" ).c_str() ) ;
	    std::cout << " layer ID " << layerID << std::endl;
            layerImpl->setID( layerID ) ; 

	    std::string infoLayer = getOptionalXMLAttribute( xmlLayer, "info", "" ).c_str()  ;
	    std::cout << " layer Info " << infoLayer << std::endl;
            layerImpl->setInfo( infoLayer ) ; 

            const TiXmlNode* xmlLad = 0 ;
            while( ( xmlLad = xmlLayer->IterateChildren( "ladder" , xmlLad ) ) != 0 ) {

		    int lID        = atoi( getXMLAttribute( xmlLad , "ID" ).c_str() ) ;
		    std::string lInfo = getOptionalXMLAttribute( xmlLad, "info", "" ).c_str()  ;
		    double lPosX   = atof( getOptionalXMLAttribute( xmlLad , "positionX", "0."  ).c_str() ) ;
		    double lPosY   = atof( getOptionalXMLAttribute( xmlLad , "positionY", "0."  ).c_str() ) ;
		    double lPosZ   = atof( getOptionalXMLAttribute( xmlLad , "positionZ", "0."  ).c_str() ) ;
		    double lRotXY  = atof( getOptionalXMLAttribute( xmlLad , "rotationXY", "0." ).c_str() ) ;
		    double lRotZX  = atof( getOptionalXMLAttribute( xmlLad , "rotationZX", "0." ).c_str() ) ;  
		    double lRotZY  = atof( getOptionalXMLAttribute( xmlLad , "rotationZY", "0." ).c_str() ) ;
		    double lSizX   = atof( getOptionalXMLAttribute( xmlLad , "sizeX", "1."  ).c_str() ) ;
		    double lSizY   = atof( getOptionalXMLAttribute( xmlLad , "sizeY", "1."  ).c_str() ) ;
		    double lThick  = atof( getOptionalXMLAttribute( xmlLad , "thickness", "0.0" ).c_str() ) ;
		    double lRadLen = atof( getOptionalXMLAttribute( xmlLad , "radLength", "0.0" ).c_str() ) ;

		    std::cout << " material layer ID " << lID << std::endl;

         	    layerImpl->addMaterialLayer( lID, lInfo, lPosX, lPosY, lPosZ, lRotXY, lRotZX, lRotZY, lSizX, lSizY, lThick, lRadLen);                  
	    }

            const TiXmlNode* xmlSen = 0 ;
            while( ( xmlSen = xmlLayer->IterateChildren( "sensitive" , xmlSen ) ) != 0 ) {

		    int sID        = atoi( getXMLAttribute( xmlSen , "ID" ).c_str() ) ;
		    std::string sInfo = getOptionalXMLAttribute( xmlSen, "info", "" ).c_str()  ;
		    double sPosX   = atof( getOptionalXMLAttribute( xmlSen , "positionX", "0." ).c_str() ) ;
		    double sPosY   = atof( getOptionalXMLAttribute( xmlSen , "positionY", "0." ).c_str() ) ;
		    double sPosZ   = atof( getOptionalXMLAttribute( xmlSen , "positionZ", "0." ).c_str() ) ;
		    double sRotXY  = atof( getOptionalXMLAttribute( xmlSen , "rotationXY", "0." ).c_str() ) ;
		    double sRotZX  = atof( getOptionalXMLAttribute( xmlSen , "rotationZX", "0." ).c_str() ) ;  
		    double sRotZY  = atof( getOptionalXMLAttribute( xmlSen , "rotationZY", "0." ).c_str() ) ;
		    double sSizX   = atof( getOptionalXMLAttribute( xmlSen , "sizeX", "1." ).c_str() ) ;
		    double sSizY   = atof( getOptionalXMLAttribute( xmlSen , "sizeY", "1." ).c_str() ) ;
		    double sThick  = atof( getOptionalXMLAttribute( xmlSen , "thickness", "0." ).c_str() ) ;
		    double sRadLen = atof( getOptionalXMLAttribute( xmlSen , "radLength", "0." ).c_str() ) ;

		    int sNPixX     = atoi(getOptionalXMLAttribute( xmlSen , "npixelX", "1" ).c_str() ) ;
		    int sNPixY     = atoi(getOptionalXMLAttribute( xmlSen , "npixelY", "1" ).c_str() ) ;
		    double sPitX   = atof(getOptionalXMLAttribute( xmlSen , "pitchX", "0." ).c_str() ) ;
		    double sPitY   = atof(getOptionalXMLAttribute( xmlSen , "pitchY", "0." ).c_str() ) ;
		    double sResolX = atof(getOptionalXMLAttribute( xmlSen , "resolutionX", "0." ).c_str() ) ;
		    double sResolY = atof(getOptionalXMLAttribute( xmlSen , "resolutionY", "0." ).c_str() ) ;
		    std::cout << " sensitive layer ID " << sID << std::endl;

	       	    layerImpl->addSensitiveLayer( sID, sInfo, sPosX, sPosY, sPosZ, sRotXY, sRotZX, sRotZY, sSizX, sSizY, sThick, sRadLen, sNPixX, sNPixY, sPitX, sPitY, sResolX, sResolY);
	    }  

            trackerplanesParam->addLayer( layerImpl ) ; // ? many scattering ladders and multi sensitive scructure -> double layer?

    } // end loop



    // now read the generic parameters
    GearParametersXML::setParametersFromXML( xmlElement, trackerplanesParam  ) ;
    
    // ------- add to GearMgr ----
    if( gearMgr != 0 ) {
      
      gearMgr->setTrackerPlanesParameters( trackerplanesParam ) ;

    }

    return trackerplanesParam ;

  } // fromXML

} // namespace
  
    
