#include "gearxml/CalorimeterParametersXML.h"

#include "gearxml/XMLHandlerMgr.h"
#include "gearxml/GearParametersXML.h"

#include "gearxml/tinyxml.h"
#include "gearimpl/CalorimeterParametersImpl.h"
#include "gearimpl/FixedPadSizeDiskLayout.h"
#include "gear/GearMgr.h"

#include <vector>
#include <algorithm>
#include <sstream>


namespace gear {


  TiXmlElement CalorimeterParametersXML::toXML( GearParameters* parameters ) const {

    
    // FIXME: needs to be implemented .....
  }
  
  
  GearParameters* CalorimeterParametersXML::fromXML( const TiXmlElement* xmlElement, GearMgr* gearMgr) const {
    
    
    // ---- layout -----

    const TiXmlElement* layout = xmlElement->FirstChildElement( "layout" ) ;
 
    if( layout == 0 ) {
      
      std::stringstream str ;
      str  << "XMLParser::getChildElementValue missing element \"layout\" " 
	   << " in element <" << xmlElement->Value() << "/> " ;
      
      throw ParseException( str.str() ) ;
    }

    std::string layoutType = getXMLAttribute( layout , "type" )   ;

    std::transform( layoutType.begin() , layoutType.end() , layoutType.begin(),  tolower ) ;
  
    if( layoutType != "barrel" &&  layoutType != "endcap" ) {

      throw ParseException( "CalorimeterParametersXML::toXML: currently only barrel and endcap type layouts implemented ! " ) ;
    }


    // ---- dimensions -----
    const TiXmlElement* dimensions = xmlElement->FirstChildElement( "dimensions" ) ;
 
    if( dimensions == 0 ) {
      
      std::stringstream str ;
      str  << "XMLParser::getChildElementValue missing element \"dimensions\" " 
	   << " in element <" << xmlElement->Value() << "/> " ;
      
      throw ParseException( str.str() ) ;
    }

    int    symmetry   =  atoi(  getXMLAttribute( layout , "symmetry" ) .c_str() ) ;
    double phi0       =  atof(  getXMLAttribute( layout , "phi0" ) .c_str() ) ;

    CalorimeterParametersImpl* caloParams = 0 ;
    
    double firstLayerDistance = 0. ;
    bool isBarrelCalorimeter = false ;

    if( layoutType == "barrel" ) {

      isBarrelCalorimeter = true ;

      // barrel dimensions defined through inner_r and outer_z
      double rMin = atof(  getXMLAttribute( dimensions , "inner_r" ) .c_str() ) ;
      double zMax = atof(  getXMLAttribute( dimensions , "outer_z" ) .c_str() ) ;
      
      caloParams = new CalorimeterParametersImpl( rMin, zMax,  symmetry , phi0 ) ;

      firstLayerDistance = rMin ;

    } else if( layoutType == "endcap" ) { 
      
      // endcap dimensions defined through inner_r, outer_r  and inner_z
      double rMin = atof(  getXMLAttribute( dimensions , "inner_r" ) .c_str() ) ;
      double rMax = atof(  getXMLAttribute( dimensions , "outer_r" ) .c_str() ) ;
      double zMin = atof(  getXMLAttribute( dimensions , "inner_z" ) .c_str() ) ;
      
      caloParams = new CalorimeterParametersImpl( rMin, rMax, zMin,  symmetry , phi0 ) ;

      firstLayerDistance = zMin ;

    }

    // now we can read the generic parameters
    GearParametersXML::setParametersFromXML( xmlElement, caloParams  ) ;    


    // ----- layers ------------
    int layerId = 0 ;

    const TiXmlNode* layer = 0 ;
    while( ( layer = xmlElement->IterateChildren( "layer", layer ) )  != 0  ){

      int    nLayer            = atoi(  getXMLAttribute( layer , "repeat" ) .c_str() ) ;
      double thickness         = atof(  getXMLAttribute( layer , "thickness" ) .c_str() ) ;
      double absorberThickness = atof(  getXMLAttribute( layer , "absorberThickness" ) .c_str() ) ;
      double cellSize0         = atof(  getXMLAttribute( layer , "cellSize0" ) .c_str() ) ;
      double cellSize1         = atof(  getXMLAttribute( layer , "cellSize1" ) .c_str() ) ;
      
      for(int i=0 ; i < nLayer ; ++i ) {

	if( layerId == 0 )
	  caloParams->layerLayout().positionLayer( firstLayerDistance , thickness, 
						   cellSize0, cellSize1, absorberThickness ) ;
	else
	  caloParams->layerLayout().addLayer( thickness, 
					      cellSize0, cellSize1, absorberThickness ) ;

	++ layerId ;
      }
    }



    //--------- add to proper section in GearMgr ----
    if( gearMgr != 0 ) {

      std::string caloName = getXMLAttribute( xmlElement , "name" ) ;

      bool isEcal = ( caloName[0] == 'E' || caloName[0] == 'e' ) ; 

      if( isEcal ){

	if( isBarrelCalorimeter )
	  gearMgr->setEcalBarrelParameters( caloParams ) ;
	else
	  gearMgr->setEcalEndcapParameters( caloParams ) ;

      } else {

	if( isBarrelCalorimeter )
	  gearMgr->setHcalBarrelParameters( caloParams ) ;
	else
	  gearMgr->setHcalEndcapParameters( caloParams ) ;

      }
    }
    
    return caloParams ;
  }


} // namespace
