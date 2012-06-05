
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


/** Program uses geartgeo and geometry info in gear file to print
 *  all materials on a straight line between two points.
 *
 */

int main(int argc, char**argv){
  
  
  std::set_unexpected( gear_unexpected ) ;
  std::set_terminate( gear_unexpected ) ;
  
  if( argc != 8 ) {
    std::cout << " printMaterials:   print all materials between two points given in [mm]: " << std::endl 
	      << " usage:   \n" 
	      << "   printMaterials gearFile.xml x0 y0 z0 x1 y1 z1 " << std::endl ;
    exit(1) ;
   }
  
  //----------------------------------
  std::stringstream ss ;
  ss << argv[2] << " " << argv[3]  << " " << argv[4]  << " " << argv[5]  << " " << argv[6]  << " " << argv[7]  ;
  float x0,y0,z0,x1,y1,z1 ;
  
  ss >> x0 >> y0 >> z0 >> x1 >> y1 >> z1 ;
  
  if( !ss){
    std::cout << " *** Number Format Error: " <<  ss.str() << std::endl; 
    exit(1) ;
  }
  //-----------------------------------


  std::string fileName( argv[1] ) ;
  
  GearXML gearXML( fileName ) ;
  
  GearMgr* gearMgr = gearXML.createGearMgr() ;

  
  TGeoGearDistanceProperties &distProp = (TGeoGearDistanceProperties&)gearMgr->getDistanceProperties() ;
      
  //===========================================================


  
  gear::Vector3D p0( x0 , y0 , z0 ) ;
  
  gear::Vector3D p1( x1 , y1 , z1 ) ;

  std::cout << " ############################################################ "
	    << "    materials between the two points :  \n " 
	    <<    p0 
	    <<    p1 
	    << " ############################################################# " 
	    << std::endl ;
  
  //convert all lengths to cm for TGeo
  p0 = .1 * p0 ;
  p1 = .1 * p1 ;

  gear::Vector3D dir =  p1 - p0 ;
  dir = ( 1. / dir.r() )  * dir ;

  std::vector<std::string> names =  distProp.getMaterialNames( p0 , p1 ) ;
      
  std::vector<double> thicks =  distProp.getMaterialThicknesses( p0 , p1 ) ;
      
  double lambda = 0. ;

    
  for(unsigned j=0 ; j < names.size() ; ++j){
    
    //gear::Vector3D prev = p0 + lambda * dir ;
    gear::Vector3D prev = p0 ;

    lambda +=  thicks[j]  ;
    
    gear::Vector3D next = p0 + lambda * dir ;

    double rL =  distProp.getNRadlen( prev, next )  ;

    //    std::cout << "   " <<  names[j] << " -  " << thicks[j] * 10.  <<  " "  <<  rL  <<  std::endl ;  
    //    printf( "%25s  %1.8e  %1.8e \n" ,  names[j].c_str() , thicks[j] * 10. , rL   ) ;

    printf( "%25s  %1.8e  %1.8e   %1.8e   %1.8e   %1.8e   %1.8e   %1.8e   %1.8e \n" ,  names[j].c_str() , thicks[j] * 10. , rL ,
	    prev[0]*10.,  prev[1]*10.,  prev[2]*10.,  next[0]*10.,  next[1]*10.,  next[2]*10. ) ;
 
  }
} 


