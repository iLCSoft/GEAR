
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



Vector3D pointOnCylinder( double theta, double r, double z, double phi=0.02 ) ; //0.0) ;


void printMaterial(  TGeoGearDistanceProperties& distProp, const gear::Vector3D& start ,  const gear::Vector3D& point ) {


  gear::Vector3D dir = point - start ; 
  dir = ( 1. / dir.r() ) *  dir ;
  
  
  std::vector<std::string> names = 
    distProp.getMaterialNames( start , point ) ; 
  
  std::vector<double> radl ; 
  
  std::vector<double> thicks = 
    distProp.getMaterialThicknesses( start   , point ) ; 
  
  double lambda = 0. ;
  
  std::cerr << " ################################# materials between the two points :  \n " 
	    <<   start << "\n" << point 
	    << " dir vector " << dir << "\n"
	    << " ############################################################# " 
	    << std::endl ;
  
  
  
  for(unsigned j=0 ; j < names.size() ; ++j){
    
    lambda +=  thicks[j]  ;
    
    gear::Vector3D local = start + lambda * dir ;
    double rL =  distProp.getNRadlen( start, local )  ;
    double iL =  distProp.getNIntlen( start, local )  ;
    
    std::cerr << " ------ " <<  names[j] << " r:  " << lambda  << " - " << thicks[j] << " cm  - "  <<  rL << "  - iL: " << iL 
	      <<  std::endl ;  
    
  }

  std::cerr << "\n  ================================================================================\n \n "  ;
  
} 



/** Program uses geartgeo and geometry info in gear file to print integrated interaction lengths for 
 *  points on cylinders that are just outside the SET, ECal, Hcal, Coil, Yoke - as seen from the IP
 *  You can use Mokka to create the needed xml adn gdml files, e.g:
 *  Mokka -M ILD_o1_v05
 *      /Mokka/Visu/Detector/DumpGDML world ILD_o1_v05
 *      exit/quit
 *  mv GearOutput.xml gear_ILD_o1_v05.xml
 *  add to gear_ILD_o1_v05.xml the line :
 *     <GDMLFile name="ILD_o1_v05.gdml"/>
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
  
  // // const TPCParameters &tpcparams= gearMgr->getTPCParameters() ;
  // // double r_in =  tpcparams.getDoubleVal("tpcInnerRadius")/10.;
  // // double r_o  =  tpcparams.getDoubleVal("tpcOuterRadius")/10.;
  // // //  double wall_o = tpcparams.getDoubleVal("tpcOuterWallThickness")/10.;
  
  // // double Rtpc_i = r_in ; // -wall_in;
  // // double Rtpc_o = r_o ; // + wall_o ;
  

  float Cos8  = cos(M_PI/8.0);
  float Cos12  = cos(M_PI/12.0);
  float Cos16 = cos(M_PI/16.);

  const CalorimeterParameters &ecalparamsB= gearMgr->getEcalBarrelParameters() ;
  double Recal_i =  ecalparamsB.getExtent()[0]/10. ; 
  double Recal_o =  ecalparamsB.getExtent()[1]/10. ; 
  //Recal_o /= Cos8 ; // we are at very small angles

  const CalorimeterParameters &ecalparamsEC= gearMgr->getEcalEndcapParameters() ;
  double Zecal_i=ecalparamsEC.getExtent()[2]/10;
  double Zecal_o=ecalparamsEC.getExtent()[3]/10;

  const CalorimeterParameters &hcalparamsB= gearMgr->getHcalBarrelParameters() ;
  double Rhcal_i =  hcalparamsB.getExtent()[0]/10. ; 
  double Rhcal_o =  hcalparamsB.getExtent()[1]/10. ; 
  Rhcal_o /= Cos16 ;
  const CalorimeterParameters &hcalparamsEC= gearMgr->getHcalEndcapParameters() ;
  double Zhcal_i=hcalparamsEC.getExtent()[2]/10;
  double Zhcal_o=hcalparamsEC.getExtent()[3]/10;

  const gear::GearParameters&  pCoil = gearMgr->getGearParameters("CoilParameters");
  double Zcoil_o =  pCoil.getDoubleVal("Coil_cryostat_inner_cyl_half_z" )/10 ;
  double Rcoil_i = pCoil.getDoubleVal("Coil_cryostat_inner_cyl_inner_radius" )/10 ;
  double Rcoil_o = pCoil.getDoubleVal("Coil_cryostat_inner_cyl_outer_radius" )/10 ;

  const CalorimeterParameters &yokeparamsB= gearMgr->getYokeBarrelParameters() ;
  double Ryoke_i =  yokeparamsB.getExtent()[0]/10. ; 
  double Ryoke_o =  yokeparamsB.getExtent()[1]/10. ; 
  Ryoke_o /= Cos12 ;
  const CalorimeterParameters &yokeparamsEC= gearMgr->getYokeEndcapParameters() ;
  double Zyoke_o=yokeparamsEC.getExtent()[3]/10;
  const gear::CalorimeterParameters&  yokeplug = gearMgr->getYokePlugParameters();
  double Zyoke_i=yokeplug.getExtent()[2]/10;


  // average between consecutive detectors (this should be right in the gaps)
  Recal_i -= 20. ;
  Zecal_i -= 20. ;

  Recal_o = ( Recal_o + Rhcal_i ) / 2. ;
  Zecal_o = ( Zecal_o + Zhcal_i ) / 2. ;
 
  Rhcal_o = ( Rhcal_o + Rcoil_i ) / 2. ;
  Zhcal_o = ( Zhcal_o + Zyoke_i ) / 2. ;
  
  Rcoil_o = ( Rcoil_o + Ryoke_i ) / 2. ;
  Zcoil_o = Zhcal_o  ;
  
  Ryoke_o += 50. ;
  Zyoke_o += 50. ;



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


  std::cerr << " ************** SET  cylinder  r = " <<  Recal_i << ", z = " <<  Zecal_i << std::endl ;
  std::cerr << " ************** Ecal cylinder  r = " <<  Recal_o << ", z = " <<  Zecal_o << std::endl ;
  std::cerr << " ************** Hcal cylinder  r = " <<  Rhcal_o << ", z = " <<  Zhcal_o << std::endl ;
  std::cerr << " ************** Coil cylinder  r = " <<  Rcoil_o << ", z = " <<  Zcoil_o << std::endl ;
  std::cerr << " ************** Yoke cylinder  r = " <<  Ryoke_o << ", z = " <<  Zyoke_o << std::endl ;


  // write a String, so that you can directly read with root:
  //    TTree  t ; t.ReadFile("intLen_ILD_o2_v05.txt")

  std::cout  << "iTheta/d:theta/d:eta/d:nilSET/d:nilEcal/d:nilHcal/d:nilCoil/d:nilYoke/d" << std::endl ;
 
  for(int i= 0 ; i < N ; ++i ) {
    double theta = thetaMin + i * dTheta  ;
    
    // double dTheta = 0.001 ; // mrad
    // for(int theta= 0. ; theta < thetaMax ; theta += dTheta ) {
    
    double eta = ( theta == 0.0 ? 0. : - log( tan( theta / 2. ) ) );
    double iTheta  = theta / M_PI * 180 ;
    
    //----------
    Vector3D ip( 0.,0.,0. ) ;

    Vector3D pSET  = pointOnCylinder(  theta ,  Recal_i , Zecal_i ) ;
    Vector3D pEcal = pointOnCylinder(  theta ,  Recal_o , Zecal_o ) ;
    Vector3D pHcal = pointOnCylinder(  theta ,  Rhcal_o , Zhcal_o ) ;
    Vector3D pCoil = pointOnCylinder(  theta ,  Rcoil_o,  Zcoil_o ) ;
    Vector3D pYoke = pointOnCylinder(  theta ,  Ryoke_o , Zyoke_o ) ;

    double nilSET   = distProp.getNIntlen( ip , pSET  ) ; 
    double nilEcal  = distProp.getNIntlen( ip , pEcal ) ; 
    double nilHcal  = distProp.getNIntlen( ip , pHcal ) ; 
    double nilCoil  = distProp.getNIntlen( ip , pCoil ) ; 
    double nilYoke  = distProp.getNIntlen( ip , pYoke ) ; 
    
    if(  i == N-5  ) {

      printMaterial( distProp , pSET  , pEcal ) ;
      printMaterial( distProp , pEcal , pHcal ) ;
      printMaterial( distProp , pHcal , pCoil ) ;
      printMaterial( distProp , pCoil , pYoke ) ;
      
      // double R_o = Ryoke_i ;
      // double Z_o = Zyoke_
      // gear::Vector3D start =  pointOnCylinder(  theta ,  Recal_i , Zecal_i ) ;
      // gear::Vector3D point =  pointOnCylinder(  theta ,  R_o , Z_o )   ;
      // gear::Vector3D dir = point - start ; 
      // dir = ( 1. / dir.r() ) *  dir ;
      // std::vector<std::string> names = 
      // 	distProp.getMaterialNames( start , point ) ; 
      // std::vector<double> radl ; 
      // std::vector<double> thicks = 
      // 	distProp.getMaterialThicknesses( start   , point ) ; 
      // double lambda = 0. ;
      //
      // std::cerr << " ################################# materials between the two points :  \n " 
      // 		<<   start << "\n" << point 
      // 		<< "  theta " << theta * 180. /3.141592 << "\n" 
      // 		<< " dir vector " << dir << "\n"
      // 		<< " ############################################################# " 
      // 		<< std::endl ;
      // for(unsigned j=0 ; j < names.size() ; ++j){
      // 	lambda +=  thicks[j]  ;
      // 	gear::Vector3D local = start + lambda * dir ;
      // 	double rL =  distProp.getNRadlen( start, local )  ;
      // 	double iL =  distProp.getNIntlen( start, local )  ;
      // 	std::cerr << " ------ " <<  names[j] << " r:  " << lambda  << " - " << thicks[j] << " cm  - "  <<  rL << "  - iL: " << iL 
      // 		  <<  std::endl ;  
      // }
      
    } 
    

    // ###################################################

    std::cout  << iTheta  << " " 
	       << theta   << " " 
	       << eta     << " " 
	       << nilSET  << " " 
	       << nilEcal << " " 
	       << nilHcal << " " 
	       << nilCoil << " " 
	       << nilYoke << " " 
	       << std::endl;

    
    // ###################################################
    
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
