
#include "gearimpl/FixedPadSizeDiskLayout.h"
#include "gearimpl/Util.h"
#include "gearxml/GearXML.h"
#include "gear/GearMgr.h"
#include "gear/GEAR.h"
#include "gear/VXDParameters.h"

#ifdef CGA
#include "gearcga/CGAGearDistanceProperties.h"
#include "gearcga/CGAGearPointProperties.h"
#endif

#include <iostream>
#include <assert.h>

#include <exception>
#include <cstdlib>

#include <sstream>
#include <fstream>

using namespace gear ;

void testVXD( const VXDParameters& vxdParams ) ;

void testVXDPoint( const Point3D p , const VXDParameters& vxdParams ) ;

void testVXDDist( const Point3D p , const VXDParameters& vxdParams ) ;

void testAllVXDPoint( const VXDParameters& vxdParams ) ;

void testAllVXDDist( const VXDParameters& vxdParams ) ;

void testFixedPadSizeDiskLayout( const FixedPadSizeDiskLayout& pl ) ;

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
  
  
  std::cout << " testgear - instantiated  GearMgr from file " << fileName 
	    << std::endl ;

  std::cout  << *gearMgr  << std::endl ;


  

  try{
    const FixedPadSizeDiskLayout& pl = 
      dynamic_cast<const FixedPadSizeDiskLayout&>( gearMgr->getTPCParameters().getPadLayout() );
    
    testFixedPadSizeDiskLayout( pl ) ;
  }
  catch(std::bad_cast& e){
    std::cout << "  wrong type of layout - expected FixedPadSizeDiskLayout ! " << std::endl ;
  }

  // ---- text VXD ----
  const VXDParameters& vp = 
    dynamic_cast<const VXDParameters&>( gearMgr->getVXDParameters() ) ;

    testVXD( vp ) ;
    
  // --- test writing of XML file ---------

  GearXML::createXMLFile( gearMgr, "testgear_out.xml" ) ;

  // --- testing gearcga ---
#ifdef CGA
  std::ifstream inFile("mokka.steer");
  std::stringstream steer;
  std::string line ;
  while(  ! inFile.eof()  ) {
        getline( inFile, line ) ;
        steer << line << std::endl ;
  }

  CGAGearDistanceProperties * distProp = new CGAGearDistanceProperties(steer.str(), "ProtoDesy0205", "", "", "", "");
                                                                                
        Point3D initial, final;
        std::vector<std::string> matNames, lvNames;
        initial[0] = 0.0;
        initial[1] = 0.0;
        initial[2] = 0.0;
        final[0] = 0.0;
        final[1] = 1000.0;
        final[2] = 1000.0;
                                                                                
        try{
                matNames = distProp->getMaterialNames(initial, final);
                for(unsigned int i=0; i<matNames.size();i++)
                        std::cout << matNames[i].c_str() << std::endl;
                double bDl = distProp->getBdL(initial, final);
                std::cout << "Bdl=" << bDl << std::endl;
                double eDl = distProp->getEdL(initial, final);
                std::cout << "Edl=" << eDl << std::endl;
        }
        catch(NotImplementedException e){}

  CGAGearPointProperties * pointProp = new CGAGearPointProperties(steer.str(), "ProtoDesy0205", "", "", "", "");
                                                                                
        const Point3D position(0.0, 1730.0, 0.0);
        try{
                std::cout << "Material: " <<
                        pointProp->getMaterialName(position) << " Density: " <<
                        pointProp->getDensity(position) << std::endl;
                                                                                
                lvNames = pointProp->getListOfLogicalVolumes(position);
                for(unsigned int i=0; i<lvNames.size();i++)
                        std::cout << lvNames[i].c_str() << std::endl;
                Vector3D B = pointProp->getB(position);
                std::cout << "B=(" << B[0] << "," << B[1] << "," << B[2] <<
                        ")" << std::endl;
        }
        catch(NotImplementedException e){}
#endif //ifdef CGA
}



void testFixedPadSizeDiskLayout( const  FixedPadSizeDiskLayout& pl ) {
  
//   const DoubleVec& ext = pl.getPlaneExtent() ;
  
//   double rMin = ext[0] ;
//   double rMax = ext[1] ;

//   // getPadWidth() returns phi - need to multiply by r 
//   double padWidth = pl.getPadWidth(0) * pl.getPadCenter(0).first ; 

  int nRow = pl.getNRows() ;
  
//   std::cout << "   FixedPadSizeDiskLayout :  " << std::endl
// 	    << "         rMin:      " << rMin  << std::endl
// 	    << "         rMax:      " << rMax  << std::endl
// 	    << "         padHeight: " << pl.getPadHeight(0)  << std::endl 
// 	    << "         padWidth:  " <<  padWidth  << std::endl
// 	    << "         nRows :    " << nRow << std::endl 
//  	    << std::endl 
// 	    << std::endl ;
  
  int nPadTotal = 0 ;

  std::cout << " First (innermost) 10 pads and last (outermost) 10 pads : "  << std::endl ;

  for( int i = 0 ; i < nRow ; i++) {

    if( i==0 || i == nRow-1 ) 
      std::cout << " --------- row : " << i << std::endl ;
    
    const  std::vector<int>& pads = pl.getPadsInRow( i ) ;

    int nPad = pads.size() ;
    nPadTotal += nPad ;
    
    for( int j = 0 ; j < nPad ; j++) {
      

      int iRow = pl.getRowNumber( pads[j] ) ;
      int iPad = pl.getPadNumber( pads[j] ) ;
      
      if( j == 0 ) {
	int ln = pl.getLeftNeighbour(  pl.getPadIndex( iRow , iPad ) ) ; 
	assert(  pl.getPadNumber( ln ) ==  nPad-1 ) ;
      }

      if( j == nPad-1 ) {
	int rn = pl.getRightNeighbour(  pl.getPadIndex( iRow , iPad ) ) ; 
	assert(  pl.getPadNumber( rn ) ==  0 ) ;
      }

      Point2D p = pl.getPadCenter( pads[j] ) ;

      if( (i==0 && j < 10 ) || ( i == nRow-1 && j > nPad-9 ) ) {

	std::cout << "         pad: "  
		  << " [" << iRow << "," << iPad << "] "
		  << " - ( " << p[0] << " , " << p[1]  << ") "
		  << std::endl ;
      }

      assert(  pl.getNearestPad(  p[0] , p[1] ) == pads[j]  ) ;
      assert( pl.isInsidePad(  p[0] , p[1] , pads[j] ) ) ;

//       if( !(  pl.isInsidePad(  p[0] , p[1] , pads[j] ) )) {
// 	std::cout << " center is not in pad :( ! " << std::endl ;
//       }
    }
  }
  assert( nPadTotal ==  pl.getNPads() ) ;



  //---------------------------------
  Point3D r ;
  r[0] = 1. ;
  r[1] = 2. ;
  r[2] = 3. ;

  Point3D r1( r ) ;
  Point3D r2( r1[0] , r1[1] , r1[2] ) ;
  Point3D r3 ;

  std::cout << " test of Point3D  r : " << r[0] << ", " << r[1]  << ", " << r[2] << std::endl ; 
  std::cout << " test of Point3D  r1 : " << r1[0] << ", " << r1[1]  << ", " << r1[2] << std::endl ; 
  std::cout << " test of Point3D  r2: " << r2[0] << ", " << r2[1]  << ", " << r2[2] << std::endl ; 
  std::cout << " test of Point3D  r3: " << r3[0] << ", " << r3[1]  << ", " << r3[2] << std::endl ; 

}
  

void testVXD( const VXDParameters& vxdParams ) {

  //  Point3D p0 ( 17.8191 , 3.67696 , 10 ) ;
  //  Point3D p1 ( -36.8 , 7 , 10 ) ;
  //   Point3D p2 ( -15 ,-8, .5 ) ;
//   Point3D p3 ( -30 , -2 , .5 ) ;
//   Point3D p4 ( -25 ,-25.0 , 0 ) ;
 
//   testVXDPoint( p0 , vxdParams ) ;
//   testVXDPoint( p1 , vxdParams ) ;
//   testVXDPoint( p2 , vxdParams ) ;
//   testVXDPoint( p3 , vxdParams ) ;
//   testVXDPoint( p4 , vxdParams ) ;
 

//  testVXDDist( p0, vxdParams ) ;
//   testVXDDist( p1, vxdParams ) ;
//   testVXDDist( p2, vxdParams ) ;
//   testVXDDist( p3, vxdParams ) ;
//   testVXDDist( p4, vxdParams ) ;

  testAllVXDDist( vxdParams ) ;
  //  testAllVXDPoint( vxdParams ) ;
}

void testVXDPoint( const Point3D p , const VXDParameters& vxdParams ) {
  
  std::cout <<  p[0] <<" " <<  p[1] <<" "<<  p[2] ;
  bool isPoint = vxdParams.isPointInLadder( p ) ;
  
  if( isPoint ) {
    std::cout <<  p[0] <<" " <<  p[1] <<" "<<  p[2] << " inside";
  }
  
  std::cout << std::endl ;
  
}

void testVXDDist( const Point3D p , const VXDParameters& vxdParams ) {
  
  Vector3D v = vxdParams.distanceToNearestLadder( p ) ;

  std::cout << "testVXDDist " << std::endl 
	    << "\nPoint  (" << p[0] << "," << p[1] << "," << p[2] << ") "
	    << "\nVector (" << v[0] << "," << v[1] << "," << v[2] << ") " << std::endl;

  bool isFound = false ;

  double myStep =1 ;
  double lBound = 1 ;
  double uBound = 1 ;

  for(double r1 = lBound ; r1 <= uBound ; r1+= myStep ) {
    for(double r2 = lBound ; r2 <= uBound ; r2+= myStep ) {
      for(double r3 = lBound ; r3 <= uBound ; r3+= myStep ) {

	Point3D iP ( p[0] + r1* v[0] , p[1] + r2* v[1] , p[2] + r3*v[2] ) ;

	bool isPoint = vxdParams.isPointInLadder( iP ) ;
	if( isPoint ) {
	  std::cout << "Vector point withhin Ladder at r =" << r1 <<" , " << r2 <<" , " << r3 << std::endl ;
	  std::cout << "                                  " << iP[0] << " , " << iP[1] << " , " << iP[2] << std::endl ;
	  isFound = true ;
	  break ;
	}
	else{
	  std::cout << "ERROR Vector NOT pointin in Ladder." << std::endl ;
	}
      }
      if (isFound ) break ;
    }
    if (isFound ) break ;
  }
}

void testAllVXDPoint( const VXDParameters& vxdParams ) {

  double testStart = -65 ;
  double testEnd = 65 ;
  double testStep = 0.05 ;
  double zStart = 10 ;
  double zEnd = 11 ;
  double zStep = 1 ;

  float myStatusP = 0, myStatusL = 0;
  int nLadder = 0 , nSensitive = 0 ;

  std::ofstream myfile;
  const char *fileName = "/scratch/lippe/myOutput.txt" ;
  myfile.open (fileName, std::ios::out | std::ios::trunc );

  int nPoints = (testEnd-testStart)*(testEnd-testStart)*(zEnd-zStart) / ( testStep * testStep * zStep ) ;
  std::cout << "Testing " << nPoints << " Points to file : " << fileName <<std::endl ;
  
  // x
  for ( double x=testStart ; x < testEnd ; x += testStep ) {
    
    // Status
    float curStatus = (x - testStart) / (testEnd - testStart) * 100 ;
    if ( curStatus > ( myStatusP + 0.25 ) ) {
      myStatusP = curStatus ;
      std::cout << "." << std::flush ;
    }
    if ( curStatus > ( myStatusL + 10 ) ) {
      myStatusL = curStatus ;
      std::cout << std::floor(myStatusL) << "% finished" << std::endl ;
    }
	 
    // y
    for ( double y=testStart ; y < testEnd ; y += testStep ) {
      
      // z
      for ( double z = zStart ; z < zEnd ; z += zStep ) {
	
	Point3D p ( x , y , z ) ;
	bool isPoint = vxdParams.isPointInLadder( p ) ;
	bool isSensitive = vxdParams.isPointInSensitive( p ) ;

	if( isPoint ) nLadder ++;
	if( isSensitive ) nSensitive ++;

	if( isPoint || isSensitive ) {
	  myfile <<  p[0] <<" " <<  p[1] <<" "<<  p[2] << " " << isSensitive << "\n" ;
	}

      } // z
    } // y
  } //x
  
  myfile.close() ;
  
  std::cout << "\ntotal tested points: " << nPoints << "\n" 
	    << "          in ladder: " << nLadder << " (" << nLadder/nPoints*100 << ") \n"
	    << "       in sensitive: " << nSensitive << " (" << nSensitive/nPoints*100 << ")\n"
	    << "Points finished " << std::endl ;

}


void testAllVXDDist( const VXDParameters& vxdParams ) {
  
  double testStart = -65 ;
  double testEnd = 65 ;
  double testStep = 1 ;
  double zStart = -120 ;
  double zEnd = 120 ;
  double zStep = 10 ;
    
  float myStatusP = 0, myStatusL = 0;
  
  std::ofstream myfile, myHisto;
  const char *fileName     = "/scratch/lippe/myOutput.txt" ;
  const char *histoName  = "/scratch/lippe/myHistogram.txt" ;
  myfile.open (fileName, std::ios::out | std::ios::trunc );
  myHisto.open( histoName, std::ios::out | std::ios::trunc ) ;
  
  int nPoints = (testEnd-testStart)*(testEnd-testStart)*(zEnd-zStart) / ( testStep * testStep * zStep ) ;
  std::cout << "Testing " << nPoints << " Points to file : " << fileName <<std::endl ;
  
  // x
  for ( double x=testStart ; x < testEnd ; x += testStep ) {
    
    // Status
    float curStatus = (x - testStart) / (testEnd - testStart) * 100 ;
    if ( curStatus > ( myStatusP + 0.25 ) ) {
      myStatusP = curStatus ;
      std::cout << "." << std::flush ;
    }
    if ( curStatus > ( myStatusL + 10 ) ) {
	myStatusL = curStatus ;
	std::cout << std::floor(myStatusL) << "% finished" << std::endl ;
    }
    
    // y
    for ( double y=testStart ; y < testEnd ; y += testStep ) {
      
      // z
      for ( double z = zStart ; z < zEnd ; z += zStep ) {
	
	Point3D p ( x , y , z ) ;
	Vector3D nv = vxdParams.distanceToNearestLadder( p ) ;
	Point3D np ( p[0] + nv[0] , p[1] + nv[1] , p[2] + nv[2] ) ;
	  
	//bool isPoint = vxdParams.isPointInLadder( np ) ;
	
	//if( !isPoint ) {
	  Vector3D nv2 = vxdParams.distanceToNearestLadder( np ) ;
	  myHisto << sqrt( nv2[0]*nv2[0] + nv2[1]*nv2[1] + nv2[2]*nv2[2] ) << "\n" ;
	  myfile << np[0] <<" " <<  np[1] <<" "<<  np[2] << 0 << "\n" ;
	  //}
	
      } // z
    } // y
  } //x
  
  myfile.close() ;
  myHisto.close() ;
    
  std::cout << "Points finished " << std::endl ;
}

