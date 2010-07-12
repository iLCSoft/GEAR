
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
    //TGeoGearPointProperties &pointProp = (TGeoGearPointProperties&)gearMgr->getPointProperties();

    Vector3D initial, final;
    std::vector<std::string> matNames,volNames;
    initial[0] = 0.0;
    initial[1] = 0.0;
    initial[2] = 0.0;
    double R=1000;
    for(int theta=0;theta<90;theta++)
      {
	final[0] = 0.0;
	final[1] = R*sin(theta*3.14/180);
	final[2] = R*cos(theta*3.14/180);
	//std::cout<<"# "<<final[1]<<" "<<final[2]<<" "<<pointProp.getListOfLogicalVolumes(initial)[0]<<std::endl;
	double radLen=distProp.getNRadlen(initial, final);
	matNames = distProp.getMaterialNames(initial, final);
	volNames = distProp.getVolumeNames(initial, final);
	std::cout << theta<<" "<<radLen<< std::endl;
	//for(unsigned int i=0; i<matNames.size();i++)
	//  std::cout <<"#"<< volNames[i].c_str()<<" "<<matNames[i].c_str() << std::endl;
      }
  }
  catch(NotImplementedException e){}
  
 

}


