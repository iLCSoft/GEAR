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
	throw ParseException ("XML TPCParametersXML::toXML given parameter not of correct type. " 
			      "Needs to be a gear::TPCParameter");


    // append data to PadRowLayout2D
    TiXmlElement modularTPCXML("detector");    
    modularTPCXML.SetAttribute("geartype","TPCParameters"); 

    TiXmlElement maxDriftLengthElement("maxDriftLength");
    maxDriftLengthElement.SetDoubleAttribute( "value", mTPC->getMaxDriftLength() );
    modularTPCXML.InsertEndChild(maxDriftLengthElement);

    TiXmlElement driftVelocityElement("driftVelocity");
    driftVelocityElement.SetDoubleAttribute("value", mTPC->getDriftVelocity());
    modularTPCXML.InsertEndChild(driftVelocityElement);

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

    // Write all other parameters to detecotor as attributes
    GearParametersXML::getXMLForParameters( &modularTPCXML ,  mTPC ) ;

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
//	std::cout << "TPCParametersXML::fromXML : "
//		  << "No coordinate type given for TPCParameters,"
//		  << " switching to old, non-modular syntax (deprecated)" << std::endl;
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
	
    
    TPCParametersImpl* modularTPC = 0;

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

	// we can only create the module here because now we know the type of coordinate system
	modularTPC =  new TPCParametersImpl( maxDriftLength , dLayout->getCoordinateType() ) ;
    
	modularTPC->setPadLayout( dLayout ) ;
    
	// the module has to be set first, now the frequency can be set
	modularTPC->setDriftVelocity(    atof(  getChildElementValue( xmlElement, "driftVelocity"    ).c_str() )) ;
	modularTPC->setReadoutFrequency( atof(  getChildElementValue( xmlElement, "readoutFrequency" ).c_str() )) ;

    }
    else // new syntax with modules
    {
        // create the modular TPC
        modularTPC = new TPCParametersImpl( maxDriftLength ,coordinateType ) ;

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
	    
	    // Now loop the content of the modules section. It might be comments, "module" or
	    // "default" elements.
	    const TiXmlNode * moduleNode = 0;
	    
	    while( ( moduleNode = modulesElement->IterateChildren( moduleNode ) ) != 0  )
	    {
		//std::cout << "GEARDEBUG: " << "looping modules "<<  std::endl;

	        // modulesElement (with s ) is for a block of (all) modules
	        // moduleElement (without s ) is for a single module
	        const TiXmlElement* moduleElement = dynamic_cast<const TiXmlElement*>(moduleNode);
		
		// Check if the dynamic cast succeeded. This is not the case for comments.
		// In this case just continue with the next node.
		if (moduleElement==0) continue;

		// Check if it is a new default module
                if( std::string(moduleElement->Value()) == std::string("default") )
                {
                    defaultModuleElement = moduleElement;
                    continue;
                } 
 
		// Now the element should be a module. If not give a warning and ignore it.
		if ( std::string(moduleElement->Value()) != std::string("module") )
		{
		    std::cout << "GEAR::TPCParametersXML: WARNING " 
			      << " Unknown tag " << moduleNode->Value() << " in <modules> section."
			      << std::endl;
		    continue;
		}

		// finaly create the module from the xml element and add it to the TPC
		modularTPC->addModule (_tpcModuleXML.fromXML(moduleElement,
							     defaultModuleElement, 
							     coordinateType,
							     moduleIDStartCount +  moduleIDOffset++ ) );
	    }
	} 
	
    } // else (oldsyntax)

    // now read the generic parameters
    GearParametersXML::setParametersFromXML( xmlElement, modularTPC  ) ;

    if( gearMgr != 0 )
    {
	gearMgr->setTPCParameters( modularTPC ) ;
    }
	
    //std::cout << "GEARDEBUG: " << "TPCParametersXML: vector size is "<< modularTPC->getModules().size() << std::endl;
    return modularTPC ;
 }// TPCParametersXML::fromXML 
    
}//namespace gear

    
