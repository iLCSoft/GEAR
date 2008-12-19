#include "gearxml/TPCParametersXML.h"
#include "gearimpl/TPCParametersImpl.h"

#include "gearxml/XMLHandler.h"
#include "gearxml/GearParametersXML.h"
#include "gearxml/PadRowLayout2DXML.h"

#include "gearxml/tinyxml.h"
#include "gear/GearMgr.h"
//modulesElement
#include "gear/PadRowLayout2D.h"

#include <sstream>

namespace gear {
  

  TiXmlElement TPCParametersXML::toXML( const GearParameters& modularTPC ) const {

      const TPCParameters *mTPC = dynamic_cast<const TPCParameters *>(&modularTPC);
      if (!mTPC)
	  throw ParseException("TPCParametersXML::toXML : Wrong GearParameters type !");

    // append data to PadRowLayout2D
    TiXmlElement modularTPCXML("detector");    
    modularTPCXML.SetAttribute("geartype","TPCParameters"); 

    TiXmlElement maxDriftLengthElement("maxDriftLength");
    maxDriftLengthElement.SetDoubleAttribute( "value", mTPC->getMaxDriftLength() );
    modularTPCXML.InsertEndChild(maxDriftLengthElement);

    TiXmlElement coordinateTypeElement("coordinateType");
    switch( mTPC->getCoordinateType() )
    {
	case PadRowLayout2D::CARTESIAN : 
	    coordinateTypeElement.SetAttribute( "value", "cartesian" ); break;
	case PadRowLayout2D::POLAR : 
	    coordinateTypeElement.SetAttribute( "value", "polar" ); break;
	default:
	    throw ParseException("TPCParametersXML::toXML : Unknown coordinateType !");
    }
    modularTPCXML.InsertEndChild(coordinateTypeElement);
    
//    TiXmlElement nModulesElement("numberOfModules");
//    maxDriftLengthElement.SetDoubleAttribute( "value", mTPC->getNModules() );
//    modularTPCXML.InsertEndChild(nModulesElement);

    TiXmlElement modulesElement("modules");
    
    const std::vector<TPCModule *> modules = mTPC->getModules();
    for ( std::vector<TPCModule *>::const_iterator moduleIter = modules.begin();
	  moduleIter < modules.end(); moduleIter++)
    {
	modulesElement.InsertEndChild( (_tpcModuleXML.toXML( *moduleIter ) ) );
    }

    modularTPCXML.InsertEndChild( modulesElement );

    return modularTPCXML ;
  }
  
    
  GearParameters* TPCParametersXML::fromXML( const TiXmlElement* xmlElement,
						    GearMgr* gearMgr ) const
  {
      // debug information
      //std::cout << "GEARDEBUG: " << "This is TPCParametersXML::fromXML" << std::endl;

      // a flag whether to use old or new syntax
      bool oldsyntax=false;

    double maxDriftLength =  atof(  getChildElementValue( xmlElement , "maxDriftLength" ) .c_str() ) ;
    int coordinateType;
    std::string deaultString;
    std::string typeString = getOptionalChildElementValue( xmlElement , "coordinateType" , deaultString);
    if ( typeString.empty() )
    {
	std::cout << "TPCParametersXML::fromXML : "
		  << "No coordinate type given for TPCParameters,"
		  << " switching to old, non-modular syntax (deprecated)" << std::endl;
	oldsyntax = true;
	coordinateType = PadRowLayout2D::CARTESIAN;
    }
    else
    if ( typeString==std::string("cartesian") )
	coordinateType = PadRowLayout2D::CARTESIAN;
    else
    if ( typeString==std::string("polar") )
	coordinateType = PadRowLayout2D::POLAR;
    else
	throw ParseException("TPCParametersXML::fromXML : Unknown coordinateType !");
	
    
    TPCParametersImpl* modularTPC = new TPCParametersImpl( maxDriftLength ,coordinateType ) ;

    // first we read the generic parameters
    GearParametersXML::setParametersFromXML( xmlElement, modularTPC  ) ;

    // check for old syntax
    if (oldsyntax)
    {
	// get the pad layout
	const TiXmlElement* layout = xmlElement->FirstChildElement( "PadRowLayout2D" ) ;
	
	if( layout == 0 ) {
	    
	    std::stringstream str ;
	    str  << "XMLParser::getChildElementValue missing element \"PadRowLayout2D\" " 
		 << " in element <" << xmlElement->Value() << "/> " ;
	    
	    throw ParseException( str.str() ) ;
	}
	
	std::string layoutType = getXMLAttribute( layout , "type" )   ;
	
	PadRowLayout2DXML* layoutXML = PadRowLayout2DXML::getHandler( layoutType ) ;
	
	if( layoutXML == 0 ) {
	    
	    throw ParseException( "TPCParametersXML::fromXML: no handler for " + layoutType + " found !" ) ;
	}
	
	PadRowLayout2D* dLayout = layoutXML->fromXML( layout ) ;
    
	modularTPC->setPadLayout( dLayout ) ;
    
	// the module has to be set first, now the frequency can be set
	modularTPC->setDriftVelocity(    atof(  getChildElementValue( xmlElement, "driftVelocity"    ).c_str() )) ;
	modularTPC->setReadoutFrequency( atof(  getChildElementValue( xmlElement, "readoutFrequency" ).c_str() )) ;

    }
    else // new syntax with modules
    {
	// try to find a default module
//	const TiXmlElement* defaultModuleElement = xmlElement->FirstChildElement("default");
//	// no need to check whether the default module was found, the module parser can run without
//	std::cout << "GEARDEBUG: " << "defaultModuleElement = "<<defaultModuleElement<<  std::endl;
	
	// loop the modules section (there might be mor than one)
	const TiXmlElement* modulesElement=0;
	while( ( modulesElement =dynamic_cast<const TiXmlElement*>(
		     xmlElement->IterateChildren( "modules", modulesElement ) )  )  != 0  )
	{
	    //std::cout << "GEARDEBUG: " << "TPCParametersXML::fromXML : found modules section" << std::endl;
	    
	    int moduleIDOffset=0;
	    int moduleIDStartCount = atoi(  getOptionalXMLAttribute( modulesElement,
								     "moduleIDStartCount" ,
								     "0"            ) .c_str() ) ;
	    //std::cout << "GEARDEBUG: " << "TPCParametersXML::fromXML : moduleIDStartCount = " 
	    //      <<moduleIDStartCount<< std::endl;

	    // try to find a default module for this section
	    const TiXmlElement* defaultModuleElement = modulesElement->FirstChildElement("default");
	    // no need to check whether the default module was found, the module parser can run without
	    //std::cout << "GEARDEBUG: " << "defaultModuleElement = "<<defaultModuleElement<<  std::endl;
	    
	    // now loop all modules
	    
	    // modulesElement (with s ) is for a block of (all) modules
	    // moduleElement (without s ) is for a single module
	    const TiXmlElement* moduleElement =0;
	    
	    while( ( moduleElement = dynamic_cast<const TiXmlElement*>(
			 modulesElement->IterateChildren( "module", moduleElement ) )  ) != 0  )
	    {
		//std::cout << "GEARDEBUG: " << "looping modules "<<  std::endl;
		
		modularTPC->addModule (_tpcModuleXML.fromXML(moduleElement,
							     defaultModuleElement, 
							     coordinateType,
							     moduleIDStartCount +  moduleIDOffset++ ) );
	    }
	} 
	
    } // else (oldsyntax)

    if( gearMgr != 0 )
    {
	gearMgr->setTPCParameters( modularTPC ) ;
    }
	
    //std::cout << "GEARDEBUG: " << "TPCParametersXML: vector size is "<< modularTPC->getModules().size() << std::endl;
    return modularTPC ;
 }// TPCParametersXML::fromXML 
    
}//namespace gear

    
