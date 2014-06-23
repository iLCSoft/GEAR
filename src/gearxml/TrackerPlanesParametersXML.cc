#include "gearxml/TelPlanesParametersXML.h"

#include "gearxml/XMLHandlerMgr.h"
#include "gearxml/GearParametersXML.h"

#include "gearxml/tinyxml.h"
#include "gearimpl/TelPlanesParametersImpl.h"

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

namespace gear {

  TiXmlElement TelPlanesParametersXML::toXML( const GearParameters & parameters ) const {


    // check whether parameter is valid TelPlanesParameter
    const TelPlanesParameters* param = dynamic_cast<const TelPlanesParameters*> ( &parameters ) ;

    if( param == 0 ) {

      throw Exception( "TelPlanesParametersXML::toXML given parameters not of correct type. "
		       "needs to be gear::TelPlanesParameters." ) ;
    }

    // Set up Beam telescope  as Element
    TiXmlElement det("detector") ;

    TiXmlElement setup_id( "telplanesID" ) ;
    setup_id.SetAttribute("ID", param->getTelPlanesID()) ;
    det.InsertEndChild( setup_id ) ;


    TiXmlElement nplanes( "telplanesNumber" ) ;
    nplanes.SetAttribute("number", param->getTelPlanesNumber()) ;
    det.InsertEndChild( nplanes ) ;

    // layerLayout
    const TelPlanesLayerLayout& telplanesLayers = param->getTelPlanesLayerLayout() ;

    TiXmlElement layers("layers") ;

    for( int i=0 ; i < telplanesLayers.getNLayers() ; i++ ) {
      TiXmlElement layer("layer" ) ;
      det.InsertEndChild( layers ) ;
    }

    // Assemble Detector
    GearParametersXML::getXMLForParameters( &det , &parameters ) ;

    return det ;

  }

  GearParameters* TelPlanesParametersXML::fromXML( const TiXmlElement* xmlElement, GearMgr* gearMgr) const {

    // setup ID

    const TiXmlElement* telplanesID = xmlElement->FirstChildElement( "telplanesID" ) ;
    int setupID = atoi( getXMLAttribute( telplanesID , "ID" ).c_str() ) ;

    int intType = 0;
 
    const TiXmlElement* telplanesNumber = xmlElement->FirstChildElement( "telplanesNumber" ) ;
    int nplanes = atoi( getXMLAttribute( telplanesNumber , "number" ).c_str() ) ;


    // create TelPlanesParameters
    TelPlanesParametersImpl* telplanesParam = new TelPlanesParametersImpl( setupID, intType , nplanes) ;

    // layers
    const TiXmlNode* xmlLayers = xmlElement->FirstChildElement( "layers" ) ;

    const TiXmlNode* xmlLayer = 0 ;
    while( ( xmlLayer = xmlLayers->IterateChildren( "layer" , xmlLayer ) ) != 0 ) {

            TelPlanesLayerImpl* layerImpl = new TelPlanesLayerImpl() ;

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

            telplanesParam->addLayer( layerImpl ) ; // ? many scattering ladders and multi sensitive scructure -> double layer?

    } // end loop



    // now read the generic parameters
    GearParametersXML::setParametersFromXML( xmlElement, telplanesParam  ) ;
    
    // ------- add to GearMgr ----
    if( gearMgr != 0 ) {
      
      gearMgr->setTelPlanesParameters( telplanesParam ) ;

    }

    return telplanesParam ;

  } // fromXML

} // namespace
  
    
