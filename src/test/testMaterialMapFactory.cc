
#include "gearimpl/Util.h"
#include "gearxml/GearXML.h"
#include "gear/GearMgr.h"
#include "gear/GEAR.h"

#include <iostream>
#include <assert.h>

#include <exception>
#include <typeinfo>
#include <cstdlib>

#include <sstream>
#include <fstream>
#include <math.h>

#include "gear/geartgeo/MaterialMapFactory.h"

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


/** Program to check the functionality of the MaterialMapFactory
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
  
  try{  
    MaterialMapFactory *myMapFactory= new MaterialMapFactory(gearMgr);
    
    MaterialMap *myMap=myMapFactory->createMaterialMap(-200,200, 10,-200., 200,10 ,-400, 400, 10, 0);
  
    for(int r=0;r<20;r++)
      {
	double x=-200+200*drand48();
	double y=-200+200*drand48();
	double z=-400*drand48();
	//double  interactionLengthsFromIp = myMap->getInteractionLength( r,theta*M_PI/180. ,0);     
	double  radiationLengthsFromIp = myMap->getRadiationLength(x,y,z);     
	std::cout<<x<<" "<<y<<" "<<z<<" "<<radiationLengthsFromIp<<std::endl;
      }
     
  }
  catch(NotImplementedException e){}
  
} 


