
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


/** Program uses geartgeo and geometry info in gear file
 *  to compute material budget for everything before the TPC,
 *  the TPC alone and everything up to the ecal front.
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
  double L_tpc=0, Rtpc_i=0,Rtpc_o=0, Recal_o=0,Recal_i=0, Zecal=0;
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
    const CalorimeterParameters &ecalparamsB= gearMgr->getEcalBarrelParameters() ;
    //convert to cm for TGeo
    Recal_o =  ecalparamsB.getExtent()[0]/10.;
  }
  catch( gear::UnknownParameterException& e ){
    std::cout << "  oops - no Ecal barrel available :( " << std::endl ;
  }
  try{
    const CalorimeterParameters &ecalparamsEC= gearMgr->getEcalEndcapParameters() ;
    //convert to cm for TGeo
    Zecal=ecalparamsEC.getExtent()[2]/10;
  }
  catch( gear::UnknownParameterException& e ){
    std::cout << "  oops - no Ecal endcap available :( " << std::endl ;
  }
  try{
    const CalorimeterParameters &ecalparamsPl= gearMgr->getEcalPlugParameters() ;
    //convert to cm for TGeo
    Recal_i=ecalparamsPl.getExtent()[0]/10;
  }
  catch( gear::UnknownParameterException& e ){
    std::cout << "  oops - no Ecal endcap available :( " << std::endl ;
  }
  
  try{
    TGeoGearDistanceProperties &distProp = (TGeoGearDistanceProperties&)gearMgr->getDistanceProperties();
    Vector3D tpc_initial, tpc_final;
    Vector3D all_initial, all_final;
    Vector3D bf_initial, bf_final;
    double tpc_radLen=0, all_radLen=0, bf_radLen=0;
    std::cout<<"# theta      before TPC      TPC        up to ECAL"<<std::endl;
    for(int theta=0;theta<90;theta++)
      {
	//all up to ECAL front
	all_initial[0] = 0;
	all_initial[1] = 0;
	all_initial[2] = 0;
	all_final[0] = 0.0;
	all_final[1] = Recal_o;
	all_final[2] = Recal_o/tan(theta*M_PI/180);
	if(all_final[2]>Zecal)
	  {
	    all_final[2]=Zecal;
	    all_final[1]=Zecal*tan(theta*M_PI/180);
	  }
	if(all_final[1]<Recal_i)
	  all_radLen=0;
	else
	  all_radLen=distProp.getNRadlen(all_initial, all_final);
	//TPC only
	tpc_initial[0] = 0;
	tpc_initial[1] = Rtpc_i;
	tpc_initial[2] = Rtpc_i/tan(theta*M_PI/180);
	  
	tpc_final[0] = 0.0;
	tpc_final[1] = Rtpc_o;
	tpc_final[2] = Rtpc_o/tan(theta*M_PI/180);
	if(tpc_final[2]>L_tpc)
	  {
	    tpc_final[2]=L_tpc;
	    tpc_final[1]=L_tpc*tan(theta*M_PI/180);
	  }
	if(tpc_initial[2]>L_tpc)
	  tpc_radLen=0;
	else
	  tpc_radLen=distProp.getNRadlen(tpc_initial, tpc_final);
	
	//before TPC
	bf_initial[0] = 0;
	bf_initial[1] = 0;
	bf_initial[2] = 0;
	bf_final[0] = 0.0;
	bf_final[1] = tpc_initial[1];
	bf_final[2] = tpc_initial[2];
	if(bf_final[2]>L_tpc)
	  bf_radLen=0;
	else
	  bf_radLen=distProp.getNRadlen(bf_initial, bf_final);

	std::cout<<theta<<" "<<bf_radLen<<" "<<tpc_radLen<<" "<<all_radLen<<std::endl;


	
      }
  }
  catch(NotImplementedException e){}
  
} 


