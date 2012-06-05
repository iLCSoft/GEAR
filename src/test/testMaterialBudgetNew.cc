
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
  double r_o  =  tpcparams.getDoubleVal("tpcOuterRadius")/10.;
  //  double wall_o = tpcparams.getDoubleVal("tpcOuterWallThickness")/10.;
  
  double Rtpc_i = r_in ; // -wall_in;
  double Rtpc_o = r_o ; // + wall_o ;
  
  const CalorimeterParameters &ecalparamsB= gearMgr->getEcalBarrelParameters() ;
  double Recal_i =  ecalparamsB.getExtent()[0]/10. - 3.0 ; // workaround for Ecal Si layer
  //  double Recal_i =  ecalparamsB.getExtent()[0]/10. ; // workaround for Ecal Si layer

  const CalorimeterParameters &ecalparamsEC= gearMgr->getEcalEndcapParameters() ;
  double Zecal=ecalparamsEC.getExtent()[2]/10;

  //  Zecal = 235.1 ; // FIXME: exclude collong pipes for test !!!!!!
  //  Zecal = 246.0 ; // FIXME: include first ecal layers

  const ZPlanarParameters& vxdp = gearMgr->getVXDParameters() ;
  double ro_VXD = vxdp.getShellOuterRadius() / 10. ;
  double zo_VXD = vxdp.getShellHalfLength()/10. + vxdp.getShellGap()/10. ;
  
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
  
  int N = 1000 ;

  double thetaMin = 0.   ;   
  double thetaMax = M_PI/2. ;   

  // double thetaMin = 0.54   ;   
  // double thetaMax = 0.56 ;   

  double dTheta = ( thetaMax - thetaMin ) / N  ;

 
  //std::cout  << " iTheta, theta, eta, nrVXD, nrSIT, nrTPC, nrSET " << std::endl ; 

  std::cerr << " ************** VXD cylinder  r = " <<  ro_VXD << ", z = " <<   zo_VXD << std::endl ;
  std::cerr << " ************** SIT cylinder  r = " <<  Rtpc_i << ", z = " <<   Zecal << std::endl ;
  std::cerr << " ************** TPC cylinder  r = " <<  Rtpc_o << ", z = " <<   Zecal << std::endl ;
  std::cerr << " ************** SET cylinder  r = " <<  Recal_i << ", z = " <<  Zecal << std::endl ;


  for(int i= 0 ; i < N ; ++i ) {
    double theta = thetaMin + i * dTheta  ;
    
    // double dTheta = 0.001 ; // mrad
    // for(int theta= 0. ; theta < thetaMax ; theta += dTheta ) {
    
    double eta = ( theta == 0.0 ? 0. : - log( tan( theta / 2. ) ) );
    double iTheta  = theta / M_PI * 180 ;
    
    
    //----------
    Vector3D ip( 0.,0.,0. ) ;
    
    double nrVXD  = distProp.getNRadlen( ip , pointOnCylinder(  theta ,  ro_VXD  , zo_VXD ) ) ; 
    
    double nrSIT  = distProp.getNRadlen( ip , pointOnCylinder(  theta ,  Rtpc_i  , Zecal ) ) ; 
    
    double nrTPC  = distProp.getNRadlen( ip , pointOnCylinder(  theta ,  Rtpc_o  , Zecal ) ) ; 
    
    double nrSET  = distProp.getNRadlen( ip , pointOnCylinder(  theta ,  Recal_i , Zecal ) ) ; 
    
    if( i== 9 ) { //N-2 ) {


      gear::Vector3D dir =  pointOnCylinder(  theta ,  Recal_i , Zecal ) - ip   ;
      dir = ( 1. / dir.r() ) *  dir ;


      std::vector<std::string> names = 
	distProp.getMaterialNames( ip , pointOnCylinder(  theta ,  Recal_i , Zecal ) ) ; 
      
      std::vector<double> radl ; 
      
      std::vector<double> thicks = 
	distProp.getMaterialThicknesses( ip , pointOnCylinder(  theta ,  Recal_i , Zecal ) ) ; 
      
      double lambda = 0. ;

      std::cerr << " ################################# materials between the two points :  \n " 
		<< ip << "\n" << pointOnCylinder(  theta ,  Recal_i , Zecal ) 
		<< "  theta " << theta * 180. /3.141592 << "\n" 
		<< " dir vector " << dir << "\n"
		<< " ############################################################# " 
		<< std::endl ;



      for(unsigned j=0 ; j < names.size() ; ++j){
	
	lambda +=  thicks[j]  ;

	gear::Vector3D local = ip + lambda * dir ;
	double rL =  distProp.getNRadlen( ip , local )  ;

	std::cerr << " ------ " <<  names[j] << " -  " << thicks[j] << " cm  - " 
	  	  <<  rL
		  <<  std::endl ;  

      }


    } 

    std::cout  << iTheta << "  " 
	       << theta << " " 
	       << eta   << " " 
	       << nrVXD << " " 
	       << nrSIT << " " 
	       << nrTPC << " " 
	       << nrSET 
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
