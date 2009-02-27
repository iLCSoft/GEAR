#include "gearxml/TPCModuleXML.h"
#include "gearimpl/TPCModuleImpl.h"

#include "gearxml/XMLHandler.h"
#include "gearxml/PadRowLayout2DXML.h"

#include "gearxml/tinyxml.h"
#include "gear/GearMgr.h"

#include "gear/PadRowLayout2D.h"

#include <sstream>
#include <typeinfo>

namespace gear {
  

TiXmlElement TPCModuleXML::toXML( const TPCModule* module ) const 
{
    TiXmlElement tpcModuleXML( "module" );
    
    TiXmlElement moduleIDElement("moduleID");
    moduleIDElement.SetAttribute( "value", module->getModuleID() );
    tpcModuleXML.InsertEndChild(moduleIDElement);
    
    TiXmlElement readoutFrequencyElement("readoutFrequency");
    readoutFrequencyElement.SetDoubleAttribute( "value", module->getReadoutFrequency() );
    tpcModuleXML.InsertEndChild(readoutFrequencyElement);

    // cast to TPCModuleImpl so we can directly access the pad layout
    // TPCModuleImpl is a friend of TPCModule so we can access the private variables
    PadRowLayout2DXML* layoutXML = PadRowLayout2DXML::getHandler( & module->getLocalPadLayout() ) ;
    if( layoutXML == 0 ) 
    {
      throw ParseException( "TPCModuleXML::toXML: no handler for " + std::string(typeid(& module->getLocalPadLayout()).name()) + " found !" ) ;
    }
    tpcModuleXML.InsertEndChild( layoutXML->toXML(  & module->getLocalPadLayout() ) );
    

    TiXmlElement offsetElement("offset");
    offsetElement.SetDoubleAttribute( "x_r", module->getOffset()[0] );
    offsetElement.SetDoubleAttribute( "r_phi", module->getOffset()[1] );
    tpcModuleXML.InsertEndChild(offsetElement);

    TiXmlElement angleElement("angle");
    angleElement.SetDoubleAttribute( "value", module->getAngle() );
    tpcModuleXML.InsertEndChild(angleElement);

    TiXmlElement borderWidthElement("enlargeActiveAreaBy");
    borderWidthElement.SetDoubleAttribute( "value", module->getBorderWidth() );
    tpcModuleXML.InsertEndChild(borderWidthElement);

    return tpcModuleXML ;
}
    
    
TPCModule* TPCModuleXML::fromXML( const TiXmlElement* moduleElement ,
				  const TiXmlElement* defaultModuleElement,
				  int tpcCoordinateType, int moduleID) const
{
    // the values which have to be there: moduleID, readoutFrequency and the PadRowLayout2D
    
    const TiXmlElement* moduleIDElement = moduleElement->FirstChildElement( "moduleID" );
    if ( ( moduleIDElement == 0 ) && defaultModuleElement) // tag has not been found
    {   //try to find it in the defaultModuleElement
	moduleIDElement = defaultModuleElement->FirstChildElement( "moduleID" );
    }
    if ( moduleIDElement != 0 ) // tag has been found
	{
	    moduleID = atoi( getXMLAttribute( moduleIDElement , "value" ).c_str() );
	}
    // if the module index has not been found at all it's ok, there is a default value
    
    
    // the readout frequency
    const TiXmlElement* readoutFrequencyElement = moduleElement->FirstChildElement( "readoutFrequency" );
    if ( ( readoutFrequencyElement == 0 ) && defaultModuleElement) // tag has not been found
    {
	//try to find it in the defaultModuleElement
	readoutFrequencyElement = defaultModuleElement->FirstChildElement( "readoutFrequency" );
	}
    if ( readoutFrequencyElement == 0 ) // tag still has not been found
    {
	throw ParseException("TPCModuleXML: No readoutFrequency in TPCModule" ) ;
    }
    double 	readoutFrequency = atoi( getXMLAttribute( readoutFrequencyElement , "value" ).c_str() );
    
    // get the pad layout
    const TiXmlElement* padLayoutElement = moduleElement->FirstChildElement( "PadRowLayout2D" );
    if( ( padLayoutElement == 0 ) && defaultModuleElement) // tag has not been found
    {
	//try to find it in the defaultModuleElement
	padLayoutElement = defaultModuleElement->FirstChildElement( "PadRowLayout2D" );
    }
    if( padLayoutElement == 0 ) // tag still has not been found
    {
	throw ParseException( "TPCModuleXML: No PadRowLayout2D in TPCModule" );
    }
    
    std::string layoutType = getXMLAttribute( padLayoutElement , "type" )   ;
    
    PadRowLayout2DXML* padLayoutXML = PadRowLayout2DXML::getHandler( layoutType ) ;
    if( padLayoutXML == 0 )
    {
	throw ParseException( "TPCParametersXML::fromXML: no handler for " + layoutType + " found !" ) ;
    }
    
    PadRowLayout2D* padLayout = padLayoutXML->fromXML( padLayoutElement ) ;
    
    // now everything is there to create the module instance
    
    TPCModuleImpl *tpcModule = new TPCModuleImpl( moduleID, 
						  padLayout,
						  tpcCoordinateType,
						  readoutFrequency);
    
    // now search for the additional parameters: offset, angle, enlargeActiveAreaBy
    
    // offset
    const TiXmlElement* offsetElement = moduleElement->FirstChildElement( "offset" );
    if ( ( offsetElement == 0 ) && defaultModuleElement) // tag has not been found
    {   //try to find it in the defaultModuleElement
	offsetElement = defaultModuleElement->FirstChildElement( "offset" );
    }
    if ( offsetElement != 0 ) // tag has been found
    {
	tpcModule->setOffset ( atof( getXMLAttribute( offsetElement , "x_r"   ).c_str() ),
			       atof( getXMLAttribute( offsetElement , "y_phi" ).c_str() ) );
    }
    
	// angle 
    const TiXmlElement* angleElement = moduleElement->FirstChildElement( "angle" );
    if ( ( angleElement == 0 ) && defaultModuleElement )// tag has not been found
    {   //try to find it in the defaultModuleElement
	angleElement = defaultModuleElement->FirstChildElement( "angle" );
    }
    if ( angleElement != 0 ) // tag has been found
    {
	tpcModule->setAngle ( atof( getXMLAttribute( angleElement , "value"   ).c_str() ) );
    }
    
    // enlargeActiveAreaBy
    const TiXmlElement* borderWidthElement = moduleElement->FirstChildElement( "enlargeActiveAreaBy" );
    if ( ( borderWidthElement == 0 ) && defaultModuleElement) // tag has not been found
    {   //try to find it in the defaultModuleElement
	borderWidthElement = defaultModuleElement->FirstChildElement( "enlargeActiveAreaBy" );
    }
    if ( borderWidthElement != 0 ) // tag has been found
    {
	    tpcModule->setBorderWidth ( atof( getXMLAttribute( borderWidthElement , "value"  ).c_str() ) );
    }

    return tpcModule;
}// TPCModuleXML::fromXML 

}//namespace gear

