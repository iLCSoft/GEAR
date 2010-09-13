
#include "gearimpl/Util.h"
#include "gearxml/GearXML.h"
#include "gear/GearMgr.h"
#include "gear/GEAR.h"

#include "gear/GearDistanceProperties.h"
#include "gear/GearPointProperties.h"

#include <iostream>
#include <assert.h>

#include <exception>
#include <typeinfo>
#include <cstdlib>

#include <sstream>
#include <fstream>
#include <math.h>

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
    std::cout << " testMaterialMap:  Testprogram for ceating material map based on gdml file with GearDistanceProperties. " << std::endl 
	      << " usage:     testgear input.xml  " << std::endl ;
    exit(1) ;
   }
  
  std::string fileName( argv[1] ) ;
  
  GearXML gearXML( fileName ) ;
  
  GearMgr* gearMgr = gearXML.createGearMgr() ;
  
  // --- testing gear distance properties ---
  try{
   

    Vector3D initial, final;
    std::vector<std::string> matNames,volNames;
    initial[0] = 0.0;
    initial[1] = 0.0;
    initial[2] = 0.0;
    for(int r=200;r<400;r+=1)
      {
	for(int theta=1;theta<90;theta++)
	  {
	    final[0] = 0.0;
	    final[2] = r*cos(theta*M_PI/180);
	    final[1] = final[2]*tan(theta*M_PI/180);
	   
	   double  interactionLengthsFromIp = gearMgr->getDistanceProperties().getNIntlen( initial, final);     
	   double  radiationLengthsFromIp = gearMgr->getDistanceProperties().getNRadlen( initial, final);     
	   std::cout<<r<<" "<<theta<<" "<<interactionLengthsFromIp<<" "<<radiationLengthsFromIp<<std::endl;
	  }
      }
  }
  catch(NotImplementedException e){}
  
 

}


