
#include "gearimpl/Util.h"
#include "gearxml/GearXML.h"
#include "gear/GearMgr.h"
#include "gear/GEAR.h"

#include "geartgeo/TGeoGearDistanceProperties.h"
#include "geartgeo/TGeoGearPointProperties.h"

#include <iostream>
#include <assert.h>

#include <exception>
#include <typeinfo>
#include <cstdlib>

#include <sstream>
#include <fstream>

using namespace gear ;


void gear_unexpected(){

  try {

    throw ;

  } catch( std::exception& e) {

    std::cout << " A runtime error has occured : "
              << e.what()
              << std::endl
              << " the program will have to be terminated - sorry." << std::endl ;
    exit(1) ;
  }
}


/** Testprogram for gear classes.
 * 
 */

int main(int argc, char**argv){
  
  
  std::set_unexpected( gear_unexpected ) ;
  std::set_terminate( gear_unexpected ) ;
  
  if( argc < 2 ) {
    std::cout << " testgear:  Testprogram for gear classes. " << std::endl 
	      << " usage:     testgear input.xml  " << std::endl ;
    exit(1) ;
   }
  
  std::string fileName( argv[1] ) ;
  
  GearXML gearXML( fileName ) ;
  
  GearMgr* gearMgr = gearXML.createGearMgr() ;
  
  
  //std::cout << " testgear - instantiated  GearMgr from file " << fileName 
  //	    << std::endl ;

  // --- testing geartgeo ---
  try{
    TGeoGearDistanceProperties &distProp = (TGeoGearDistanceProperties&)gearMgr->getDistanceProperties();
    TGeoGearPointProperties &pointProp = (TGeoGearPointProperties&)gearMgr->getPointProperties();
    
    Vector3D initial, final;
    std::vector<std::string> matNames,volNames;
    initial[0] = 0.0;
    initial[1] = 0.0;
    initial[2] = 0.0;
    
    final[0] = 0.0;
    final[1] = 0.0;
    final[2] = 5000.0;
    
    std::cout<<"Top logical volume containing point (0,0,0): "<<pointProp.getListOfLogicalVolumes(initial)[0]<<std::endl;
    double radLen=distProp.getNRadlen(initial, final);
    std::cout<<"Radiation length between (0,0,0) and (50,50,50): "<<radLen<<std::endl;
    matNames = distProp.getMaterialNames(initial, final);
    volNames = distProp.getVolumeNames(initial, final);
   
    std::cout<<"Volumes and their material between (0,0,0) and (50,50,50): "<<std::endl;
    for(unsigned int i=0; i<volNames.size();i++)
      std::cout <<i<<" "<< volNames[i]<<" "<<matNames[i] << std::endl;
      
  }
  catch(NotImplementedException e){}
  
 

}


