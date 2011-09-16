/* C++ */
#include "gearxml/FTDParametersXML.h"

#include "gearxml/XMLHandlerMgr.h"
#include "gearxml/GearParametersXML.h"

#include "gearxml/tinyxml.h"
#include "gearimpl/FTDParametersImpl.h"

#include "gear/GearMgr.h"

#include <vector>
#include <string>

namespace gear 
{
	TiXmlElement FTDParametersXML::toXML( const GearParameters & parameters ) const 
	{
		//std::cout << "FTDParameters::toXML called" << std::endl ; //debug
		
		// check whether parameter is valid FTDParameter
		const FTDParameters* param = dynamic_cast<const FTDParameters*> ( &parameters ) ;
		
		if( param == 0 ) 
		{
			throw Exception( "FTDParametersXML::toXML given parameters not of correct type. "
					"needs to be gear::FTDParameters." ) ;
		}
		// Set up FTD-Detector as Element
		TiXmlElement det("detector") ;
		
		//std::cout << "FTDParameters::toXML strType == '" << strType << "'"<< std::endl ; // debug

		// layerLayout
		const FTDLayerLayout & ftdLayers = param->getFTDLayerLayout() ;
		TiXmlElement layers("layers") ;
		
		for( int i=0 ; i < ftdLayers.getNLayers() ; i++ ) 
		{
			//std::cout << "                     layer #petals " << ftdLayers.getNPetals( i ) << std::endl ; //debug			
			TiXmlElement layer("layer" ) ;
			layer.SetAttribute( "nPetals" , ftdLayers.getNPetals(i) ) ;
			std::string strType;
			switch( (int)ftdLayers.getSensorType(i) ) 
			{
				case ( FTDParameters::PIXEL ) :
					strType = "PIXEL";
					break ;
				case ( FTDParameters::STRIP ) :
					strType = "STRIP" ;
					break ;
				default :
					strType = "Unknown" ;
			}
			layer.SetAttribute( "sensorType", strType );
			layer.SetDoubleAttribute( "petalOpenningAngle" , ftdLayers.getPhiHalfDistance(i) ) ;
			layer.SetDoubleAttribute( "phi0" , ftdLayers.getPhi0(i) ) ;
			layer.SetDoubleAttribute( "alpha" , ftdLayers.getAlpha(i) ) ;
			layer.SetDoubleAttribute("zoffset", ftdLayers.getZoffset(i) );
			layer.SetDoubleAttribute("zsign0", ftdLayers.getZoffsetSign0(i) );
			layer.SetDoubleAttribute( "zposition" , ftdLayers.getZposition(i) ) ;
			
			TiXmlElement support("support") ;
			//support.SetDoubleAttribute( "zposition" , ftdLayers.getSupportZposition(i) ) ;
			support.SetDoubleAttribute( "thickness" , ftdLayers.getSupportThickness(i) ) ;
			support.SetDoubleAttribute( "width" , ftdLayers.getSupportWidth( i ) ) ; 
			support.SetDoubleAttribute( "lengthMin" , ftdLayers.getSupportLengthMin(i) ) ;
			support.SetDoubleAttribute( "lengthMax" , ftdLayers.getSupportLengthMax(i) ) ;
			support.SetDoubleAttribute( "rInner" , ftdLayers.getSupportRinner( i ) ) ;
			support.SetDoubleAttribute( "radLength" , ftdLayers.getSupportRadLength( i ) ) ;
			
			TiXmlElement sens("sensitive" ) ;
			//sens.SetDoubleAttribute( "zposition" , ftdLayers.getSensitiveZposition(i) ) ;
			sens.SetDoubleAttribute( "thickness" , ftdLayers.getSensitiveThickness( i ) ) ;
			sens.SetDoubleAttribute( "width" , ftdLayers.getSensitiveWidth( i ) ) ;
			sens.SetDoubleAttribute( "lengthMin" , ftdLayers.getSensitiveLengthMin( i ) ) ;
			sens.SetDoubleAttribute( "lengthMax" , ftdLayers.getSensitiveLengthMax( i ) ) ;
			sens.SetDoubleAttribute( "rInner" , ftdLayers.getSensitiveRinner( i ) ) ;
			sens.SetDoubleAttribute( "radLength" , ftdLayers.getSensitiveRadLength( i ) ) ;
			
			// assemble layer
			layer.InsertEndChild(support);
			layer.InsertEndChild(sens) ;
			layers.InsertEndChild(layer) ;
		}
		// assemble layers
		det.InsertEndChild(layers) ;
		
		// Assemble Detector
		GearParametersXML::getXMLForParameters( &det , &parameters ) ;
		
		return det ;
	}

	GearParameters* FTDParametersXML::fromXML( const TiXmlElement* xmlElement, GearMgr* gearMgr) const 
	{
		
		

		// create FTDParameters
       	        FTDParametersImpl* ftdParam = new FTDParametersImpl() ;
		
		// layers
		const TiXmlNode* xmlLayers = xmlElement->FirstChildElement( "layers" ) ;
		const TiXmlNode* xmlLayer = 0 ;
		
		while( ( xmlLayer = xmlLayers->IterateChildren( "layer" , xmlLayer ) ) != 0 ) 
		{
			int nPetals    = atoi( getXMLAttribute( xmlLayer , "nPetals" ).c_str() );
			std::string type= getXMLAttribute( xmlLayer, "sensorType" );
			int sensorType;
			if(  type == "PIXEL"  ) 
			{
				sensorType = FTDParameters::PIXEL ;
			}
			else if( type =="STRIP"  ) 
			{
				sensorType = FTDParameters::STRIP;
			}
			else
			{	  
				throw Exception( "FTDParametersXML::fromXML technology of sensorType not known: " + type + 
						" - Needs to be 'PIXEL' or 'STRIP'." ) ;
			}
			double petalOpAngle = atof( getXMLAttribute( xmlLayer , "petalOpenningAngle"     ).c_str() ); 
			double phi0      = atof( getXMLAttribute( xmlLayer , "phi0"     ).c_str() ); 
			double alpha     = atof( getXMLAttribute( xmlLayer , "alpha"     ).c_str() ); 
			double zposition = atof( getXMLAttribute( xmlLayer , "zposition"     ).c_str() ); 
			double zoffset   = atof( getXMLAttribute( xmlLayer , "zoffset"     ).c_str() ); 
			double zsign0   = atof( getXMLAttribute( xmlLayer , "zsign0"     ).c_str() ); 
			
			const TiXmlNode* xmlLad = xmlLayer->FirstChildElement( "support" ) ;
			const TiXmlNode* xmlSen = xmlLayer->FirstChildElement( "sensitive" ) ;
			
			//double lzposition = atof( getXMLAttribute( xmlLad , "zposition"     ).c_str() ); 
			double lThick     = atof( getXMLAttribute( xmlLad , "thickness" ).c_str() );
			double lWidth     = atof( getXMLAttribute( xmlLad , "width"     ).c_str() ) ;
			double lLengthMin = atof( getXMLAttribute( xmlLad , "lengthMin"    ).c_str() ) ;
			double lLengthMax = atof( getXMLAttribute( xmlLad , "lengthMax"    ).c_str() ) ;
			double lRinner    = atof( getXMLAttribute( xmlLad , "rInner"    ).c_str() ) ;
			double lRadLen    = atof( getXMLAttribute( xmlLad , "radLength" ).c_str() ) ;
			
			//double szposition = atof( getXMLAttribute( xmlSen ,"zposition"     ).c_str() ); 
			double sThick     = atof( getXMLAttribute( xmlSen , "thickness" ).c_str() ) ;
			double sWidth     = atof( getXMLAttribute( xmlSen , "width"     ).c_str() ) ;
			double sLengthMin = atof( getXMLAttribute( xmlSen , "lengthMin"    ).c_str() ) ;
			double sLengthMax = atof( getXMLAttribute( xmlSen , "lengthMax"    ).c_str() ) ;
			double sRinner    = atof( getXMLAttribute( xmlSen , "rInner"    ).c_str() ) ;
			double sRadLen = atof( getXMLAttribute( xmlSen , "radLength" ).c_str() ) ;
			
			ftdParam->addLayer( nPetals, sensorType,  petalOpAngle, phi0 ,alpha, zposition, zoffset, zsign0,
					//lzposition,
					lRinner, lThick, lLengthMin, lLengthMax, lWidth, lRadLen,
					//szposition,
					sRinner, sThick, sLengthMin, sLengthMax, sWidth, sRadLen ) ;
		} // end loop
		
		// ------- add to GearMgr ----
		if( gearMgr != 0 ) 
		{
			gearMgr->setFTDParameters( ftdParam ) ;
			
		}
		
		return ftdParam ;
	} // fromXML

} // namespace
  
    
