
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



Vector3D pointOnCylinder( double theta, double r, double z, double phi=0.0) ;


/** Program uses geartgeo and geometry info in gear file to print integrated material budget for 
 *  points on cylinders that are inside the SIT, inside the TPC and inside the Ecal.
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

  //convert all lengths to cm for TGeo
  
  const TPCParameters &tpcparams= gearMgr->getTPCParameters() ;
  double r_in =  tpcparams.getDoubleVal("tpcInnerRadius")/10.;
  //double wall_in = tpcparams.getDoubleVal("tpcInnerWallThickness")/10.;

  double Rtpc_i = r_in ; // -wall_in;
  
  const CalorimeterParameters &ecalparamsB= gearMgr->getEcalBarrelParameters() ;
  double Recal_o =  ecalparamsB.getExtent()[0]/10.;

  const CalorimeterParameters &ecalparamsEC= gearMgr->getEcalEndcapParameters() ;
  double Zecal=ecalparamsEC.getExtent()[2]/10;
  
  const ZPlanarParameters& vxdp = gearMgr->getVXDParameters() ;
  double ro_VXD = vxdp.getShellOuterRadius() / 10. ;
  double zo_VXD = vxdp.getShellHalfLength() + vxdp.getShellGap() ;
  
  TGeoGearDistanceProperties &distProp = (TGeoGearDistanceProperties&)gearMgr->getDistanceProperties() ;
    
  //===========================================================
  
  
  

  //  for(int iTheta=0; iTheta<90; iTheta++ ) {
  //    double theta =  iTheta * M_PI/180.  ;
  //    double eta = - log( tan( theta / 2. ) ) ;

  // int N = 1000 ;
  // double etaMin = 0. ;   // 90 deg
  // double etaMax = 3. ;   // ~6 deg.
  // double dEta = ( etaMax - etaMin ) / N  ;
  
  // for(int i= 0 ; i < N ; ++i ) {
  //   double eta = etaMin + i * dEta  ;
  //   double theta =  2. * atan( exp( -eta ) ) ;
  //   // eta = - log( tan( theta / 2. ) ) ;
  //   double iTheta  = theta / M_PI * 180 ;
  
  int N = 10000 ;

  double thetaMin = 0.   ;   
  double thetaMax = M_PI/2. ;   
  // double dTheta = ( thetaMax - thetaMin ) / N  ;
  // for(int i= 0 ; i < N ; ++i ) {
  //   double theta = thetaMin + i * dTheta  ;

  double dTheta = 0.001 ; // mrad
  for(int theta= 0. ; theta < thetaMax ; theta += dTheta ) {

    double eta = - log( tan( theta / 2. ) ) ;
    double iTheta  = theta / M_PI * 180 ;


    //----------
    Vector3D ip( 0.,0.,0. ) ;
    
    double nrSIT  = distProp.getNRadlen( ip , pointOnCylinder(  theta ,  ro_VXD  , zo_VXD ) ) ; 
    
    double nrTPC  = distProp.getNRadlen( ip , pointOnCylinder(  theta ,  Rtpc_i  , Zecal ) ) ; 
    
    double nrEcal = distProp.getNRadlen( ip , pointOnCylinder(  theta ,  Recal_o , Zecal ) ) ; 
    
    std::cout  << iTheta << "  " 
	       << theta << " " 
	       << eta   << " " 
	       << nrSIT << " " 
	       << nrTPC << " " 
	       << nrEcal 
	       << std::endl;
    
  }
  
  
} 


Vector3D pointOnCylinder( double theta, double r, double z, double phi){
  
  double  theta0 = atan2( r, z ) ;

  // return (   theta > theta0      ?    
  // 	     Vector3D(       r           , phi , r / tan( theta ) , Vector3D::cylindrical )  :
  // 	     Vector3D( z * tan( theta )  , phi ,      z           , Vector3D::cylindrical )   ) ;

  Vector3D v = (   theta > theta0      ?    
		   Vector3D(       r           , phi , r / tan( theta ) , Vector3D::cylindrical )  :
		   Vector3D( z * tan( theta )  , phi ,      z           , Vector3D::cylindrical )   ) ;
  

  // std::cout << "  ==== pointOnCylinder( " << theta << ", " << r << ", " << z << ") "  << " tan(theta) : " <<  tan( theta ) << std::endl 
  // 	    << "         " <<   v << std::endl ;  
  
  return v ;
}
