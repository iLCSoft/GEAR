
#include "gearimpl/FixedPadSizeDiskLayout.h"
#include "gearimpl/Util.h"
#include "gearxml/GearXML.h"
#include "gear/GearMgr.h"

#include <iostream>
#include <assert.h>

#include <exception>
#include <cstdlib>

using namespace gear ;

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


  

  const FixedPadSizeDiskLayout& pl = 
    dynamic_cast<const FixedPadSizeDiskLayout&>( gearMgr->getTPCParameters().getPadLayout() );

  testFixedPadSizeDiskLayout( pl ) ;

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
      
      Point2D p = pl.getPadCenter( pads[j] ) ;

      if( (i==0 && j < 10 ) || ( i == nRow-1 && j > nPad-9 ) ) {
	std::cout << "         pad: "  
		  << " [" << iRow << "," << iPad << "] "
		  << " - ( " << p.first << " , " << p.second  << ") "
		  << std::endl ;
      }

      assert(  pl.getNearestPad(  p.first , p.second ) == pads[j]  ) ;
      assert( pl.isInsidePad(  p.first , p.second , pads[j] ) ) ;

//       if( !(  pl.isInsidePad(  p.first , p.second , pads[j] ) )) {
// 	std::cout << " center is not in pad :( ! " << std::endl ;
//       }
    }
  }
  assert( nPadTotal ==  pl.getNPads() ) ;

}
  
