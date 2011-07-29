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
		
		// shell
		// std::cout << "FTDParameters::toXML shell      " << std::endl ; // debug
		//     std::cout << "                     halflength " << param->getShellHalfLength() << std::endl ; //debug
		//     std::cout << "                            gap " << param ->getShellGap() << std::endl ; // debug
		//     std::cout << "                         innerR " << param->getShellInnerRadius() << std::endl ; //debug
		//     std::cout << "                         outerR " << param->getShellOuterRadius() << std::endl ; //debug
		//     std::cout << "                         radLen " << param->getShellRadLength() << std::endl ; //debug
		
		TiXmlElement shell( "shell" ) ;
		shell.SetDoubleAttribute( "halfLength" , param->getShellHalfLength() ) ;
		shell.SetDoubleAttribute( "innerRadiusMin" , param->getShellInnerRadiusMin() ) ;
		shell.SetDoubleAttribute( "innerRadiusMax" , param->getShellInnerRadiusMax() ) ;
		shell.SetDoubleAttribute( "outerRadius" , param->getShellOuterRadius() ) ;
		shell.SetDoubleAttribute( "radLength" , param->getShellRadLength() ) ;
		det.InsertEndChild( shell ) ;
    
		// layerLayout
		const FTDLayerLayout & ftdLayers = param->getFTDLayerLayout() ;
		TiXmlElement layers("layers") ;
		
		for( int i=0 ; i < ftdLayers.getNLayers() ; i++ ) 
		{
			//std::cout << "                     layer #ladders " << ftdLayers.getNLadders( i ) << std::endl ; //debug			
			TiXmlElement layer("layer" ) ;
			layer.SetAttribute( "nLadders" , ftdLayers.getNLadders(i) ) ;
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
			layer.SetDoubleAttribute( "phi" , ftdLayers.getPhi(i) ) ;
			layer.SetDoubleAttribute( "alpha" , ftdLayers.getAlpha(i) ) ;
			layer.SetDoubleAttribute("zposition", ftdLayers.getZposition(i) );
			layer.SetDoubleAttribute("zoffset", ftdLayers.getZoffset(i) );
			
			TiXmlElement ladder("ladder") ;
			ladder.SetDoubleAttribute( "thickness" , ftdLayers.getLadderThickness(i) ) ;
			ladder.SetDoubleAttribute( "width" , ftdLayers.getLadderWidth( i ) ) ; 
			ladder.SetDoubleAttribute( "lengthMin" , ftdLayers.getLadderLengthMin(i) ) ;
			ladder.SetDoubleAttribute( "lengthMax" , ftdLayers.getLadderLengthMax(i) ) ;
			ladder.SetDoubleAttribute( "rInner" , ftdLayers.getLadderRinner( i ) ) ;
			ladder.SetDoubleAttribute( "radLength" , ftdLayers.getLadderRadLength( i ) ) ;
			
			TiXmlElement sens("sensitive" ) ;
			sens.SetDoubleAttribute( "thickness" , ftdLayers.getSensitiveThickness( i ) ) ;
			sens.SetDoubleAttribute( "width" , ftdLayers.getSensitiveWidth( i ) ) ;
			sens.SetDoubleAttribute( "lengthMin" , ftdLayers.getSensitiveLengthMin( i ) ) ;
			sens.SetDoubleAttribute( "lengthMax" , ftdLayers.getSensitiveLengthMax( i ) ) ;
			sens.SetDoubleAttribute( "rInner" , ftdLayers.getSensitiveRinner( i ) ) ;
			sens.SetDoubleAttribute( "radLength" , ftdLayers.getSensitiveRadLength( i ) ) ;
			
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

	GearParameters* FTDParametersXML::fromXML( const TiXmlElement* xmlElement, GearMgr* gearMgr) const 
	{
		
		
		const TiXmlElement* shell = xmlElement->FirstChildElement( "shell" ) ;
		
		double shellHalfLength = atof( getXMLAttribute( shell , "halfLength" ) .c_str() ) ;
		double shellInnerRMin = atof( getXMLAttribute( shell , "innerRadiusMin" ) .c_str() ) ;
		double shellInnerRMax = atof( getXMLAttribute( shell , "innerRadiusMax" ) .c_str() ) ;
		double shellOuterR = atof( getXMLAttribute( shell , "outerRadius" ) .c_str() ) ;
		double shellRadLen = atof( getXMLAttribute( shell , "radLength" ) .c_str() ) ;

		// create FTDParameters
		FTDParametersImpl* ftdParam = new FTDParametersImpl( shellInnerRMin ,
				shellInnerRMax, shellOuterR , shellHalfLength, shellRadLen ) ;
		
		// layers
		const TiXmlNode* xmlLayers = xmlElement->FirstChildElement( "layers" ) ;
		const TiXmlNode* xmlLayer = 0 ;
		
		while( ( xmlLayer = xmlLayers->IterateChildren( "layer" , xmlLayer ) ) != 0 ) 
		{
			int nLadders    = atoi( getXMLAttribute( xmlLayer , "nLadders" ).c_str() );
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
			double phi      = atof( getXMLAttribute( xmlLayer , "phi"     ).c_str() ); 
			double alpha    = atof( getXMLAttribute( xmlLayer , "alpha"     ).c_str() ); 
			double zposition= atof( getXMLAttribute( xmlLayer , "zposition"     ).c_str() ); 
			double zoffset  = atof( getXMLAttribute( xmlLayer , "zoffset"     ).c_str() ); 
			
			const TiXmlNode* xmlLad = xmlLayer->FirstChildElement( "ladder" ) ;
			const TiXmlNode* xmlSen = xmlLayer->FirstChildElement( "sensitive" ) ;
			
			double lThick     = atof( getXMLAttribute( xmlLad , "thickness" ).c_str() );
			double lWidth     = atof( getXMLAttribute( xmlLad , "width"     ).c_str() ) ;
			double lLengthMin = atof( getXMLAttribute( xmlLad , "lengthMin"    ).c_str() ) ;
			double lLengthMax = atof( getXMLAttribute( xmlLad , "lengthMax"    ).c_str() ) ;
			double lRinner    = atof( getXMLAttribute( xmlLad , "rInner"    ).c_str() ) ;
			double lRadLen    = atof( getXMLAttribute( xmlLad , "radLength" ).c_str() ) ;
			
			double sThick     = atof( getXMLAttribute( xmlSen , "thickness" ).c_str() ) ;
			double sWidth     = atof( getXMLAttribute( xmlSen , "width"     ).c_str() ) ;
			double sLengthMin = atof( getXMLAttribute( xmlSen , "lengthMin"    ).c_str() ) ;
			double sLengthMax = atof( getXMLAttribute( xmlSen , "lengthMax"    ).c_str() ) ;
			double sRinner    = atof( getXMLAttribute( xmlSen , "rInner"    ).c_str() ) ;
			double sRadLen = atof( getXMLAttribute( xmlSen , "radLength" ).c_str() ) ;
			
			ftdParam->addLayer( nLadders, sensorType, phi,alpha,zposition,zoffset,
					lRinner, lThick, lLengthMin, lLengthMax, lWidth, lRadLen,
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
  
    
