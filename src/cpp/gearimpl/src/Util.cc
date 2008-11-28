
#include "gearimpl/Util.h"

#include "gear/PadRowLayout2D.h"
#include "gear/VXDLayerLayout.h"
#include "gear/LayerLayout.h"


namespace gear{

  std::ostream& operator<< (  std::ostream& s, const GearMgr& m ) {


    s << " ----------------------------------------------------- " << std::endl 
      << "              GearMgr :                                " << std::endl  
      << " ----------------------------------------------------- " << std::endl ;
    
    
    s << std::endl 
      << "   parameter sections (detectors) :  "
      << std::endl ;
    
    const StringVec& keys = m.getGearParameterKeys() ;
    
    for(unsigned int i=0 ; i < keys.size() ; ++i ) {
      
      s << std::endl 
	<< "   ------  name : "  <<  keys[i]  << " ------- "  << std::endl         ;

      const GearParameters& p = m.getGearParameters( keys[i] ) ;

      s <<  p <<  std::endl ;

    }


    try{ 
      s <<  m.getBField() <<  std::endl  ;
    } catch(UnknownParameterException &e){}

    try{ 
      // TPC parameters
      s <<  m.getTPCParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){}
    
    // Calorimeter parameters
    
    try{ 
      s << "   ----  Ecal barrel    ---- "  << std::endl 
	<< m.getEcalBarrelParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){}
    
    try{ 
      s  << "   ----  Ecal endcap   ---- "  << std::endl 
	 <<  m.getEcalEndcapParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){}
    
    try{ 
      s  << "   ----  Hcal barrel    ---- "  << std::endl 
	 <<  m.getHcalBarrelParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){}
    
    try{ 
      s  << "   ----  Hcal endcap    ---- "  << std::endl 
	 <<  m.getHcalEndcapParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){}
       
    try{ 
      s  << "   ----  Lcal   ---- "  << std::endl 
	 <<  m.getLcalParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){

      s << "   ----  Lcal   ----  NOT FOUND :( " << std::endl ;

    }
       

    try{ 
      // VXD parameters
      s <<  m.getVXDParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){}
    

    return s ;
  }



  std::ostream& operator<< (  std::ostream& s, const   GearParameters& p ) {

    const StringVec& iKeys = p.getIntKeys() ;

    for(unsigned int i=0 ; i < iKeys.size() ; ++i ) {

      s << std::endl  << "    [int]       "  <<  iKeys[i]  <<  " \t " <<  p.getIntVal( iKeys[i] )  << std::endl ;
    }

    const StringVec& dKeys = p.getDoubleKeys() ;

    for(unsigned int i=0 ; i < dKeys.size() ; ++i ) {

      s << std::endl  << "    [double]    "  <<  dKeys[i]  <<  " \t " <<  p.getDoubleVal( dKeys[i] )  << std::endl ;
    }
    const StringVec& sKeys = p.getStringKeys() ;

    for(unsigned int i=0 ; i < sKeys.size() ; ++i ) {

      s << std::endl  << "    [string]    "  <<  sKeys[i]  <<  " \t " <<  p.getStringVal( sKeys[i] )  << std::endl ;
    }

    const StringVec& iVKeys = p.getIntVecKeys() ;

    for(unsigned int i=0 ; i < iVKeys.size() ; ++i ) {

      s << std::endl  << "    [IntVec]    "  <<  iVKeys[i]  <<  " \t "  ;

      const IntVec v = p.getIntVals( iVKeys[i] ) ;
      for(unsigned int j=0 ; j < v.size() ; ++j ) {
	s << v[j] << " , "   ;
      }

      s << std::endl ;
    }


    const StringVec& dVKeys = p.getDoubleVecKeys() ;

    for(unsigned int i=0 ; i < dVKeys.size() ; ++i ) {

      s << std::endl  << "    [DoubleVec] "  <<  dVKeys[i]  <<  " \t "  ;

      const DoubleVec v = p.getDoubleVals( dVKeys[i] ) ;
      for(unsigned int j=0 ; j < v.size() ; ++j ) {
	s << v[j] << " , "   ;
      }

      s << std::endl ;
    }

    const StringVec& sVKeys = p.getStringVecKeys() ;

    for(unsigned int i=0 ; i < sVKeys.size() ; ++i ) {

      s << std::endl  << "    [StringVec] "  <<  sVKeys[i]  <<  " \t "  ;

      const StringVec v = p.getStringVals( sVKeys[i] ) ;
      for(unsigned int j=0 ; j < v.size() ; ++j ) {
	s << v[j] << " , "   ;
      }

      s << std::endl ;
    }

    return s ;
  }

  std::ostream& operator<< (  std::ostream& s,  const BField& b ) {

    s << std::endl 
      << "   -----------   BField   ------- "  << std::endl  << std::endl        ;

    Vector3D bv = b.at(  Vector3D ( 0., 0., 0. ) ) ;

    s << "       field vector at origin :   Bx = " << bv.x() <<  " , By = " << bv.y() << " , Bz = " << bv.z()  << " [Tesla] " << std::endl ;

    s << dynamic_cast<const GearParameters&>( b )  ;
    
    return s ;

  }


  std::ostream& operator<< (  std::ostream& s,  const  TPCParameters& p ) {
    
    s << std::endl 
      << "   -----------   TPCParameters  ------- "  << std::endl         ;
    
    s << dynamic_cast<const GearParameters&>( p )  ;
    
    s << std::endl  << "  maxDriftLength :      "  <<  p.getMaxDriftLength() ;
    s << std::endl  << "  driftVelocity :       "  <<  p.getDriftVelocity() ;
    s << std::endl  << "  readoutFrequency :    "  <<  p.getReadoutFrequency() ;

    // FIXME: needs to be generalized ....
    const PadRowLayout2D& pl = p.getPadLayout() ;
    
    const DoubleVec& ext = pl.getPlaneExtent() ;
    
    //     double rMin = ext[0] ;
    //     double rMax = ext[1] ;
    //     // getPadWidth() returns phi - need to multiply by r 
    //     double padWidth = pl.getPadWidth(0) * pl.getPadCenter(0)[0]; 
    //     int nRow = pl.getNRows() ;
    //     s <<  std::endl
    //       << "   FixedPadSizeDiskLayout :  " << std::endl
    //       << "         rMin:      " << rMin  << std::endl
    //       << "         rMax:      " << rMax  << std::endl
    //       << "         padHeight: " << pl.getPadHeight(0)  << std::endl 
    //       << "         padWidth:  " <<  padWidth  << std::endl
    //       << "         nRows :    " << nRow << std::endl 
    //       << std::endl 
    //       << std::endl ;
    

    int nRow = pl.getNRows() ;
    int type = pl.getPadLayoutType() ;
    
    s <<  std::endl
      << "   PadRowLayout2D ( type: " ;
    
    if( type == PadRowLayout2D::CARTESIAN )   s << "CARTESIAN )" ;
    if( type == PadRowLayout2D::POLAR )       s << "POLAR )" ;
    
    s << "         nRows :    " << nRow << std::endl ; 
    s << "         extent:    [" << ext[0] << ","<< ext[1] << ","<< ext[2] << ","<< ext[3] << "]"  << std::endl ; 
    
    s <<  std::endl ;
    s << std::endl ;
    
    return s ;
  }

  std::ostream& operator<< (  std::ostream& s,  const CalorimeterParameters& p ) {


    s << dynamic_cast<const GearParameters&>( p )  ;

    if( p.getLayoutType() == CalorimeterParameters::BARREL ) {

      s << std::endl  << "  calorimeter type:  barrel  " <<  std::endl ;

    } else {

      s << std::endl  << "  calorimeter type:  endcap  " <<  std::endl ;
    }

    s  << "   symmetry order:  " << p.getSymmetryOrder() << std::endl ;
    s  << "   phi0:            " << p.getPhi0()          << std::endl ;


    const DoubleVec& ext = p.getExtent() ;

    double rMin = ext[0] ;
    double rMax = ext[1] ;
    double zMin = ext[2] ;
    double zMax = ext[3] ;
    
    s <<  std::endl
      << "   dimensions :  " << std::endl
      << "         rMin:      " << rMin  << std::endl
      << "         rMax:      " << rMax  << std::endl
      << "         zMin:      " << zMin  << std::endl
      << "         zMax:      " << zMax  << std::endl
      << std::endl ;
    
    const LayerLayout& l = p.getLayerLayout() ;

    int nLayer = l.getNLayers() ;

    s <<  std::endl
      << "   layers    :    #total "  <<  nLayer << std::endl ;

    double lastThickness = 0. ;
    double lastAbsorber  = 0. ;

    for( int i=0 ; i < nLayer ; ++i ) {

      double thisThickness = l.getThickness(i)  ;
      double thisAbsorber  =  l.getAbsorberThickness(i) ;

      if( (thisThickness != lastThickness || thisAbsorber != lastAbsorber ) || 
 	  ( i == nLayer-1 ) ) {
	
	s << "       id: " << i << " thickness: " <<  thisThickness
	  << " absorber thickness: " <<  thisAbsorber 
	  << " distance: " <<  l.getDistance(i)
	  << " cellsize-0: " << l.getCellSize0(i) 
	  << " cellsize-1: " << l.getCellSize1(i) 
	  << std::endl ;
      }
      
      lastThickness = thisThickness ;
      lastAbsorber = thisAbsorber ;
    }


    return s ;
  }


  std::ostream& operator<< (  std::ostream& s,  const VXDParameters& p ) {
    
    s << std::endl 
      << "   -----------   VXDParameters  ------- "  << std::endl         ;
    
    s << dynamic_cast<const GearParameters&>( p )  ;
    
    const VXDLayerLayout & l = p.getVXDLayerLayout() ;

    int type = p.getVXDType() ;
    
    s << std::endl  << " vxd type : " ;
    
    switch( type ) {

    case VXDParameters::CCD : 

      s << " CCD " << std::endl ;       break ;
    case VXDParameters::CMOS : 

      s << " CMOS " << std::endl ;      break ;
    case VXDParameters::HYBRID :

      s << " HYBRID " << std::endl ;    break ;
    default :  

      s << " unknown " << std::endl ; 
    }

    s << " Shell innerR : " << p.getShellInnerRadius() << " outerR : " << p.getShellOuterRadius() << std::endl 
      << "       length : " << p.getShellHalfLength()  << " gap "      << p.getShellGap() << std::endl ;
    
    
    // layers
    
    s << " Layers : " << l.getNLayers()  
      << std::endl << std::endl ;


    s <<  " layer parameters (note: ladder length (l) are half lengths ! )" 
      << std::endl ;

    char buffer[1024] ;
    
    sprintf(buffer,"  |------------------------------------------------------------------------------------|\n") ;
    s << buffer ;

    sprintf(buffer,"  |      |       ladder:                        |       sensitive part:                |\n") ;
    s << buffer ;

    sprintf(buffer,"  |  #   |    d    |    w    |    l    |   t    |    d    |    w    |    l    |    t   | \n") ;
    s << buffer ;

    sprintf(buffer,"  |------------------------------------------------------------------------------------|\n") ;
    s << buffer ;

    for( int i = 0 ; i < l.getNLayers() ; i++ ) {

      char buffer1[1024] ;
      sprintf(buffer1,"  | %3d  |  %4.2f  |  %4.2f  |  %4.2f  |  %4.2f  |  %4.2f  |  %4.2f  |  %4.2f  |  %4.2f  |\n"
	      , l.getNLadders(i) 
	      , l.getLadderDistance(i) 
	      , l.getLadderWidth(i)
	      , l.getLadderLength(i)
	      , l.getLadderThickness(i)
	      , l.getSensitiveDistance(i)
	      , l.getSensitiveWidth(i) 
	      , l.getSensitiveLength(i) 
	      , l.getSensitiveThickness(i) ) ;
      
      s << buffer1 ;

    }

    sprintf(buffer,"  |------------------------------------------------------------------------------------|\n") ;
    s << buffer ;
    
    return s ;
    
    
  }
  
}
