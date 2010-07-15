
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
  
  ///-----TPC----------------------------------------------------------------
  double L_tpc=0, Rtpc_i=0,Rtpc_o=0, Recal=0;
  try{
    const TPCParameters &tpcparams= gearMgr->getTPCParameters() ;
    //convert to cm for TGeo
    double r_in =  tpcparams.getDoubleVal("tpcInnerRadius")/10.;
    double wall_in = tpcparams.getDoubleVal("tpcInnerWallThickness")/10.;
    double wall_out = tpcparams.getDoubleVal("tpcOuterWallThickness")/10.;
    double r_out =  tpcparams.getDoubleVal("tpcOuterRadius")/10. ;  
    Rtpc_i=r_in-wall_in;
    Rtpc_o=r_out+wall_out;
  }
  catch( gear::UnknownParameterException& e ){
    std::cout << "  oops - no TPC available :( " << std::endl ;
  }
  try{
    const CalorimeterParameters &hcalparams= gearMgr->getHcalBarrelParameters() ;
    //convert to cm for TGeo
    L_tpc =  hcalparams.getDoubleVal("TPC_Ecal_Hcal_barrel_halfZ")/10.;
  }
  catch( gear::UnknownParameterException& e ){
    std::cout << "  oops - no TPC_Ecal_Hcal_barrel_halfZ available :( " << std::endl ;
  }
  try{
    const CalorimeterParameters &ecalparams= gearMgr->getEcalBarrelParameters() ;
    //convert to cm for TGeo
    Recal =  ecalparams.getExtent()[0]/10.;
    std::cout<<ecalparams.getExtent()[0]<<" "<<ecalparams.getExtent()[1]<<" "
	     <<ecalparams.getExtent()[1]<<" "<<ecalparams.getExtent()[2]<<std::endl;
  }
  catch( gear::UnknownParameterException& e ){
    std::cout << "  oops - no Ecal available :( " << std::endl ;
  }
  try{
    const CalorimeterParameters &ecalparams= gearMgr->getEcalEndcapParameters() ;
    //convert to cm for TGeo
    std::cout<<ecalparams.getExtent()[0]<<" "<<ecalparams.getExtent()[1]<<" "
	     <<ecalparams.getExtent()[1]<<" "<<ecalparams.getExtent()[2]<<std::endl;
  }
  catch( gear::UnknownParameterException& e ){
    std::cout << "  oops - no Ecal available :( " << std::endl ;
  }
  
  
  try{
    TGeoGearDistanceProperties &distProp = (TGeoGearDistanceProperties&)gearMgr->getDistanceProperties();
    std::vector<std::string> volNames;
    Vector3D initial, final;
   
    for(int theta=0;theta<90;theta++)
      {
	initial[0] = 0;
	initial[1] = Rtpc_i;
	initial[2] = Rtpc_i/tan(theta*M_PI/180);
	  
	final[0] = 0.0;
	final[1] = Rtpc_o;
	final[2] = Rtpc_o/tan(theta*M_PI/180);
	if(initial[2]>L_tpc)
	  continue;
	if(final[2]>L_tpc)
	  {
	    final[2]=L_tpc;
	    final[1]=L_tpc*tan(theta*M_PI/180);
	  }

	volNames = distProp.getVolumeNames(initial, final);
	double radLen=distProp.getNRadlen(initial, final);

	std::cout<<theta<<" "<<radLen<<std::endl;
// 	for(unsigned int i=0; i<volNames.size();i++)
// 	  std::cout <<"# "<<initial[2]<<" "<<final[2]<<" "<<volNames[i].c_str()<< std::endl;
// 	for(unsigned int i=0; i<volNames0.size();i++)
// 	  std::cout <<"# "<<startZero[2]<<" "<<final[2]<<" "<<volNames0[i].c_str()<< std::endl;
	
      }
  }
  catch(NotImplementedException e){}
  
} 


