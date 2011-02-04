
#include "gearxml/GearXML.h"
#include "gearxml/tinyxml.h"
#include "gearxml/XMLHandlerMgr.h"
#include "gearxml/GearParametersXML.h"
#include "gearxml/TPCParametersXML.h"
#include "gearxml/CalorimeterParametersXML.h"
#include "gearxml/VXDParametersXML.h"
#include "gearxml/ConstantBFieldXML.h"
#include "gearxml/SiPlanesParametersXML.h"

#include "gearimpl/GearMgrImpl.h"

#include "gear/GEAR.h"
#include "gear/CalorimeterParameters.h"
#include "gear/VXDParameters.h"
#include "gear/SiPlanesParameters.h"

// #ifdef GEAR_TGEO
// #include "geartgeo/TGeoGeometryInitializer.h"
// #include "geartgeo/TGeoGearPointProperties.h"
// #include "geartgeo/TGeoGearDistanceProperties.h"
// #endif
#include "gear/GearMaterialProperties.h"

//#include <algorithm>
#include <sstream>
#include <iostream>
#include <dlfcn.h>

namespace gear{


  GearXML::GearXML( const std::string& fileName ) : 
    _fileName( fileName ),
    _gearMgr(0) {

  } 

  GearXML::~GearXML(){
// cant delete this as GearXML is used as a factory by Marlin ....
//    if( _gearMgr != 0 ) 
//      delete _gearMgr ;
  }

  void GearXML::createXMLFile( GearMgr* mgr, const std::string& fileName ) {

    if( mgr == 0 ){
      throw Exception("GearXML::createXMLFile: GearMgr dosn't exist");
    }

    TiXmlDocument doc( fileName )  ;

    TiXmlElement root("gear") ;

    TiXmlElement detectors("detectors") ;

    TiXmlElement global("global") ;
    

    std::string detName("Unknown") ;

    try{   detName = mgr->getDetectorName()  ;
    }
    catch( UnknownParameterException ){}

    global.SetAttribute( "detectorName" , detName ) ;
    
    root.InsertEndChild( global ) ;
    

    TiXmlComment rootComment ;
    rootComment.SetValue( "Gear XML file automatically created with GearXML::createXMLFile ...."  ) ;
    root.InsertEndChild ( rootComment) ;

    // --- the BField ------------

    try{
      
      ConstantBFieldXML handler ;  //FIXME : need full field map ...

      TiXmlElement field = handler.toXML(  mgr->getBField()  )  ;

      root.InsertEndChild( field ) ;
 
    }
    catch( UnknownParameterException& e){
    }



    // --------- add TPC parameters -------------------

    try{
      
      TPCParametersXML handler ;

      TiXmlElement detector = handler.toXML( mgr->getTPCParameters() )  ;
      
      detector.SetAttribute( "name" , "TPC" ) ;
      detector.SetAttribute( "geartype" , GEAR::TPCPARAMETERS ) ;

      detectors.InsertEndChild( detector ) ;

    }
    catch( UnknownParameterException& e){
    }


	// --------- add EcalBarrel parameters -------------------
    try{
      
      CalorimeterParametersXML handler ;

      TiXmlElement detector = handler.toXML( mgr->getEcalBarrelParameters() )  ;
      
      detector.SetAttribute( "name" , "EcalBarrel" ) ;
      detector.SetAttribute( "geartype" , GEAR::CALORIMETERPARAMETERS ) ;

      detectors.InsertEndChild( detector ) ;

    }
    catch( UnknownParameterException& e){
    }

    // --------- add EcalEndcap parameters -------------------
    try{
      
      CalorimeterParametersXML handler ;

      TiXmlElement detector = handler.toXML( mgr->getEcalEndcapParameters() )  ;
      
      detector.SetAttribute( "name" , "EcalEndcap" ) ;
      detector.SetAttribute( "geartype" , GEAR::CALORIMETERPARAMETERS ) ;

      detectors.InsertEndChild( detector ) ;

    }
    catch( UnknownParameterException& e){
    }

    // --------- add EcalPlug parameters -------------------
    try{
      
      CalorimeterParametersXML handler ;

      TiXmlElement detector = handler.toXML( mgr->getEcalPlugParameters() )  ;
      
      detector.SetAttribute( "name" , "EcalPlug" ) ;
      detector.SetAttribute( "geartype" , GEAR::CALORIMETERPARAMETERS ) ;

      detectors.InsertEndChild( detector ) ;

    }
    catch( UnknownParameterException& e){
    }

   // --------- add YokeBarrel parameters -------------------
    try{
      
      CalorimeterParametersXML handler ;

      TiXmlElement detector = handler.toXML( mgr->getYokeBarrelParameters() )  ;
      
      detector.SetAttribute( "name" , "YokeBarrel" ) ;
      detector.SetAttribute( "geartype" , GEAR::CALORIMETERPARAMETERS ) ;

      detectors.InsertEndChild( detector ) ;

    }
    catch( UnknownParameterException& e){
    }

    // --------- add YokeEndcap parameters -------------------
    try{
      
      CalorimeterParametersXML handler ;

      TiXmlElement detector = handler.toXML( mgr->getYokeEndcapParameters() )  ;
      
      detector.SetAttribute( "name" , "YokeEndcap" ) ;
      detector.SetAttribute( "geartype" , GEAR::CALORIMETERPARAMETERS ) ;

      detectors.InsertEndChild( detector ) ;

    }
    catch( UnknownParameterException& e){
    }

    // --------- add YokePlug parameters -------------------
    try{
      
      CalorimeterParametersXML handler ;

      TiXmlElement detector = handler.toXML( mgr->getYokePlugParameters() )  ;
      
      detector.SetAttribute( "name" , "YokePlug" ) ;
      detector.SetAttribute( "geartype" , GEAR::CALORIMETERPARAMETERS ) ;

      detectors.InsertEndChild( detector ) ;

    }
    catch( UnknownParameterException& e){
    }

    // --------- add HcalBarrel parameters -------------------
    try{
      
      CalorimeterParametersXML handler ;

      TiXmlElement detector = handler.toXML( mgr->getHcalBarrelParameters() )  ;
      
      detector.SetAttribute( "name" , "HcalBarrel" ) ;
      detector.SetAttribute( "geartype" , GEAR::CALORIMETERPARAMETERS ) ;

      detectors.InsertEndChild( detector ) ;

    }
    catch( UnknownParameterException& e){
    }

    // --------- add HcalEndcap parameters -------------------
    try{
      
      CalorimeterParametersXML handler ;

      TiXmlElement detector = handler.toXML( mgr->getHcalEndcapParameters() )  ;
      
      detector.SetAttribute( "name" , "HcalEndcap" ) ;
      detector.SetAttribute( "geartype" , GEAR::CALORIMETERPARAMETERS ) ;

      detectors.InsertEndChild( detector ) ;

    }
    catch( UnknownParameterException& e){
    }

    // --------- add HcalRing parameters -------------------
    try{
      
      CalorimeterParametersXML handler ;

      TiXmlElement detector = handler.toXML( mgr->getHcalRingParameters() )  ;
      
      detector.SetAttribute( "name" , "HcalRing" ) ;
      detector.SetAttribute( "geartype" , GEAR::CALORIMETERPARAMETERS ) ;

      detectors.InsertEndChild( detector ) ;

    }
    catch( UnknownParameterException& e){
    }

    // --------- add Lcal parameters -------------------
    try{
      
      CalorimeterParametersXML handler ;

      TiXmlElement detector = handler.toXML( mgr->getLcalParameters() )  ;
      
      detector.SetAttribute( "name" , "Lcal" ) ;
      detector.SetAttribute( "geartype" , GEAR::CALORIMETERPARAMETERS ) ;

      detectors.InsertEndChild( detector ) ;

    }
    catch( UnknownParameterException& e){
    }

    // --------- add LHcal parameters -------------------
    try{
      
      CalorimeterParametersXML handler ;

      TiXmlElement detector = handler.toXML( mgr->getLHcalParameters() )  ;
      
      detector.SetAttribute( "name" , "LHcal" ) ;
      detector.SetAttribute( "geartype" , GEAR::CALORIMETERPARAMETERS ) ;

      detectors.InsertEndChild( detector ) ;

    }
    catch( UnknownParameterException& e){
    }

    // --------- add BeamCal parameters -------------------
    try{
      
      CalorimeterParametersXML handler ;

      TiXmlElement detector = handler.toXML( mgr->getBeamCalParameters() )  ;
      
      detector.SetAttribute( "name" , "BeamCal" ) ;
      detector.SetAttribute( "geartype" , GEAR::CALORIMETERPARAMETERS ) ;

      detectors.InsertEndChild( detector ) ;

    }
    catch( UnknownParameterException& e){
    }



    // ------- add VXD parameters ----------------------------
    try{

      VXDParametersXML handler ;

      TiXmlElement detector = handler.toXML( mgr->getVXDParameters() ) ;

      // debugging
      //std::cout << "VXD called." << std::endl ;

      detector.SetAttribute( "name" , "VXD" ) ;
      detector.SetAttribute( "geartype" , GEAR::VXDPARAMETERS ) ;
      
      detectors.InsertEndChild( detector ) ;
    }
    catch( UnknownParameterException& e) {
    }
    
    // ------- add SiPlanes parameters ----------------------------
    try{

      SiPlanesParametersXML handler ;

      TiXmlElement detector = handler.toXML( mgr->getSiPlanesParameters() ) ;

      // debugging
      //      std::cout << "SiPlanes called." << std::endl ;

      detector.SetAttribute( "name" , "SiPlanes" ) ;
      detector.SetAttribute( "geartype" , GEAR::SIPLANESPARAMETERS ) ;
      detectors.InsertEndChild( detector ) ;
    }
    catch( UnknownParameterException& e) {
    }
    


    // ------- generic/user detector parameters -----------

    const std::vector<std::string>& keys = mgr->getGearParameterKeys() ;


    for( unsigned int i=0; i<keys.size(); i++ ){


      GearParametersXML handler ;

      TiXmlElement detector = handler.toXML( mgr->getGearParameters( keys[i] ) )  ;
      
      detector.SetAttribute( "name" , keys[i] ) ;
      detector.SetAttribute( "geartype" , GEAR::GEARPARAMETERS ) ;

      detectors.InsertEndChild( detector ) ;

    }



    // ---- now put everything together -------------

    root.InsertEndChild ( detectors ) ;

    doc.InsertEndChild ( root ) ;

    doc.SaveFile() ;

  }




  GearMgr* GearXML::createGearMgr() {

    if( _gearMgr != 0 ){
      
      return _gearMgr ;
      
    } else {
      
      _gearMgr = new GearMgrImpl ;
    }
    
    
    // parse the XML file
    
    //    TiXmlDocument* doc = new TiXmlDocument ;
    TiXmlDocument doc ;
    //    TiXmlDocument* doc = &xmldoc ;

    bool loadOkay = doc.LoadFile( _fileName  ) ;
    
    if( !loadOkay ) {

      std::stringstream str ;
      
      str  << "GearXML::createGearMgr error in file [" << _fileName 
	   << ", row: " << doc.ErrorRow() << ", col: " << doc.ErrorCol() << "] : "
	   << doc.ErrorDesc() ;
      
      throw ParseException( str.str() ) ;
    }
    
//     TiXmlHandle docHandle( &doc );
    
    TiXmlElement* root = doc.RootElement();

    if( root == 0 ){
      throw ParseException( std::string( "GearXML::createGearMgr : no root tag found in  ") 
			   + _fileName  ) ;
    }
    

    TiXmlNode* global =  root->FirstChild("global") ;
    if( global != 0 ){
      std::string detName  =  getXMLAttribute( global, "detectorName" )  ;
      _gearMgr->setDetectorName( detName  ) ;
    }    
    
    TiXmlNode* detectors = root->FirstChild("detectors")  ;
    if( detectors == 0 ){
      throw ParseException( std::string( "GearXML::createGearMgr : no detectors tag found in  ") 
			   + _fileName  ) ;
    }
    
//     // --- the BField ------------
    TiXmlNode* field = root->FirstChild("BField")  ;
    if( field != 0 ){
      ConstantBFieldXML handler ;
      handler.fromXML( field->ToElement() , _gearMgr ) ; 
    }

//-----GDML File ---------------
    TiXmlNode* gdmlfile = root->FirstChild("GDMLFile")  ;
    
    if( gdmlfile != 0 ){
      
      std::string gdmlname  =  getXMLAttribute( gdmlfile, "name" )  ;
      
      
      // TGeoGeometryInitializer *tgeoini=TGeoGeometryInitializer::getTGeoGeometryInitializer(gdmlname);
      // TGeoGearPointProperties *tgeopoint=new TGeoGearPointProperties(tgeoini->getGeoManager());
      // TGeoGearDistanceProperties *tgeodist=new TGeoGearDistanceProperties(tgeoini->getGeoManager());
      
#ifdef __APPLE__
      std::string libName =  "libgeartgeo.dylib" ;
#else
      std::string libName =  "libgeartgeo.so" ;
#endif
      
      
      // now we need to dynamically load the libgearTGeo ....
      void* libPointer  = dlopen( libName.c_str() , RTLD_LAZY | RTLD_GLOBAL) ;
      
      if(  libPointer != 0 && GearMaterialPropertiesFactory::instance() != 0 ) {
	
	GearMaterialProperties* mProp     = GearMaterialPropertiesFactory::instance()->create() ;
	mProp->config()["GDMLFileName"] = gdmlname ;
	mProp->initialize() ;
	
	GearPointProperties*    tgeopoint = mProp->getGearPointProperties() ;
	GearDistanceProperties* tgeodist  = mProp->getGearDistanceProperties() ;
	
	_gearMgr->setPointProperties( tgeopoint );
	_gearMgr->setDistanceProperties( tgeodist );
	
      } else {
	
	std::cout << " WARNING: GearXML::createGearMgr: GDML file specified "  << std::endl  
		  << "   but failed to load library  "  <<  libName  << " ! "  << std::endl  
		  << "   Check your dynamic library search path ($(DY)LD_LIBRARY_PATH )... "  << std::endl  
		  <<   std::endl ;
	
      }
    }


    TiXmlNode* det = 0 ;
    while( ( det = detectors->IterateChildren( "detector", det ) )  != 0  ){
      
      std::string name  =  getXMLAttribute( det, "name" )  ;

      std::string type("UNKOWN") ;

      try {

	type  =  getXMLAttribute( det, "geartype" )  ;

// 	std::cout << "GearXML::createGearMgr: reading detector " << name 
// 		  << " with \"geartype\" " << type << std::endl ;


      } catch( ParseException& e){

	std::cout << "GearXML::createGearMgr: igoring detector " << name 
		  << " with missing attribute \"geartype\" " << std::endl ;

	continue ;
      }
	
      const XMLHandler* handler = XMLHandlerMgr::instance()->getHandler( type ) ;
      
      if( handler == 0 ) {
	
	throw ParseException( std::string( "GearXML::createGearMgr : unknown geartype \"")  
			      + type + "\" in  file " + _fileName  ) ;
      }
      
      //       GearParameters* gp = 

      handler->fromXML( det->ToElement() , _gearMgr ) ; 

    }
    
    return _gearMgr ;
  }
  
  

}
