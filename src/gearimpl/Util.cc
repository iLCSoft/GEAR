
#include "gearimpl/Util.h"

#include "gear/PadRowLayout2D.h"
#include "gear/ZPlanarLayerLayout.h"
#include "gear/FTDLayerLayout.h"
#include "gear/SiPlanesLayerLayout.h"
#include "gear/LayerLayout.h"
#include "gear/SimpleMaterial.h"

#include <cstdio>
#include <iostream> 
#include <iomanip>

namespace gear{

  std::ostream& operator<< (  std::ostream& s, const GearMgr& m ) {


    s << " ----------------------------------------------------- " << std::endl 
      << "              GearMgr :                                " << std::endl  
      << " ----------------------------------------------------- " << std::endl ;
    
    
    try{ 

      s << "   ----  DetectorName   ----   " << std::endl << std::endl 
	<< "         " <<  m.getDetectorName() 
	<< std::endl 
	<<  std::endl  ;

    } catch(UnknownParameterException &e){

      s << "     WARNING:  NOT FOUND  !     " << std::endl 
	<< "   please add  it to the <gear> element of your gear file: "
	<< std::endl
	<< std::endl
	<< "   <gear> " << std::endl
	<< "      <global detectorName=\"MyDetectorModle007\" /> " 
	<< std::endl
	<< "      <detectors> ... <detectors/>" << std::endl
	<< "   <gear/> " << std::endl
	<< std::endl ;  
    }

    try{ 
      s <<  m.getBField() <<  std::endl  ;
    } catch(UnknownParameterException &e){

      s << "   ----  BField   ----   " << std::endl 
	<< "     WARNING:  NOT FOUND  !     " << std::endl 
	<< "   please add  it to the <gear> element of your gear file, e.g.: "
	<< std::endl
	<< "   <gear> " << std::endl
	<< "      <BField type=\"ConstantBField\" x=\"0.0\" y=\"0.0\" z=\"4.0\"/> " 
	<< std::endl
	<< "      <detectors> ... <detectors/>" << std::endl
	<< "   <gear/> " << std::endl
	<< std::endl ;  
    }


    try{ 
      // VXD parameters
      s << "   ----  VXD    ---- "  <<  std::endl
	<<  m.getVXDParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){}

    try{ 
      // FTD parameters
      s <<  m.getFTDParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){}
    
    try{ 
      // SIT parameters
      s << "   ----  SIT    ---- "  <<  std::endl
	<<  m.getSITParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){}


    try{ 
      // SET parameters
      s << "   ----  SET    ---- "  <<  std::endl
	<<  m.getSETParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){}


    try{ 
      // SIT parameters
      s <<  m.getSITParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){}


    try{ 
      // SET parameters
      s <<  m.getSETParameters() <<  std::endl  ;
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
      s  << "   ----  Ecal plug   ---- "  << std::endl 
	 <<  m.getEcalPlugParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){}

	try{ 
      s << "   ----  Yoke barrel    ---- "  << std::endl 
	<< m.getYokeBarrelParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){}
    
    try{ 
      s  << "   ----  Yoke endcap   ---- "  << std::endl 
	 <<  m.getYokeEndcapParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){}
    
    try{ 
      s  << "   ----  Yoke plug   ---- "  << std::endl 
	 <<  m.getYokePlugParameters() <<  std::endl  ;
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
      s  << "   ----  Hcal ring    ---- "  << std::endl 
	 <<  m.getHcalRingParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){}
       
    try{ 
      s  << "   ----  Lcal   ---- "  << std::endl 
	 <<  m.getLcalParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){ }

    try{ 
        s  << "   ----  LHcal   ---- "  << std::endl 
            <<  m.getLHcalParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){}

    try{ 
        s  << "   ----  BeamCal   ---- "  << std::endl 
            <<  m.getBeamCalParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){}
       
    try{ 
      // SiPlanes parameters
      s <<  m.getSiPlanesParameters() <<  std::endl  ;
    } catch(UnknownParameterException &e){}  
    
 
    //------------------------------------------------------------------

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

    //------------------------------------------------------------------

    s << " ------------------------------------------------------------------ " << std::endl 
      << "   materials :  " << std::endl ;
      

    const StringVec& mNames = m.getMaterialNames() ;
    
    for(unsigned int i=0 ; i < mNames.size() ; ++i ) {
      
      const SimpleMaterial& sm = m.getSimpleMaterial( mNames[i] ) ;
      
      s <<  sm <<  std::endl ;

    }

    s  << " ------------------------------------------------------------------ " << std::endl ;

    //------------------------------------------------------------------

    return s ;
  }



  std::ostream& operator<< (  std::ostream& s, const  SimpleMaterial& m ) {

    s << "    " << m.getName() 
      <<  std::showpos << std::scientific << std::setprecision(8) 
      << ", A= "    << m.getA() 
      << ", Z= "    << m.getZ() 
      << ", density [kg/m3]= "         << m.getDensity() 
      << ", radiation length [mm]= "   << m.getRadLength() 
      << ", interaction length [mm]= " << m.getIntLength() 
      << std::dec
      << std::endl ;

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
    s << std::endl  << "  coordinateType :      ";
    switch( p.getCoordinateType() )
    {
	case PadRowLayout2D::CARTESIAN : s << "cartesian"; break;
	case PadRowLayout2D::POLAR     : s << "polar";     break;
	default                        : s << "unknown";
    }

    s << std::endl  << "  Number of modules :   "  <<  p.getNModules() << std::endl ;

    std::vector<TPCModule *> moduleVec= p.getModules();
    for (std::vector<TPCModule *>::iterator moduleIter = moduleVec.begin();
	 moduleIter <  moduleVec.end(); moduleIter++)
    {
	s << **moduleIter;
    }
    
    s <<  std::endl ;
    s << std::endl ;
    
    return s ;
  }

  std::ostream& operator<< (  std::ostream& s,  const  TPCModule& m ) {
    
    s << std::endl 
      << "     -----------   TPCModule  ------- "  << std::endl         ;
    
    s << dynamic_cast<const GearParameters&>( m )  ;
    
    s << std::endl  << "  Readout frequency :   "  <<  m.getReadoutFrequency() ;
    s << std::endl  << "  Type :                 ";

    switch( m.getLocalPadLayout().getCoordinateType() )
    {
	case PadRowLayout2D::CARTESIAN : s << "cartesian"; break;
	case PadRowLayout2D::POLAR     : s << "polar";     break;
	default                        : s << "unknown";
    }
    
    s << std::endl  << "  Number of pads :      "  <<  m.getNPads() ;
    s << std::endl  << "  Number of Rows :      "  <<  m.getNRows() ;
    s << std::endl  << "  Offset :              "  <<  m.getOffset()[0] << "  ;  "
                                                   <<  m.getOffset()[1] ;
    try
    {
      s << std::endl  << "  zPosition :           "  <<  m.getZPosition();
    }
    catch ( TPCModule::NoZPositionException & e )
    {
      s << " no z Position set ";
    }

    s << std::endl  << "  Angle :               "  <<  m.getAngle();
    s << std::endl;

    const DoubleVec& ext = m.getPlaneExtent() ;
    s << "         extent (global):    [" << ext[0] << ","<< ext[1] << ","<< ext[2] << ","<< ext[3] << "]"  << std::endl ; 
   
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


  std::ostream& operator<< (  std::ostream& s,  const ZPlanarParameters& p ) {
    
    s << "     --- ZPlanarParameters: "  << std::endl         ;
    
    s << dynamic_cast<const GearParameters&>( p )  ;
    
    const ZPlanarLayerLayout & l = p.getZPlanarLayerLayout() ;

    int type = p.getVXDType() ;
    
    s << std::endl  << "  type : " ;
    
    switch( type ) {

    case ZPlanarParameters::CCD : 

      s << " CCD " << std::endl ;       break ;
    case ZPlanarParameters::CMOS : 

      s << " CMOS " << std::endl ;      break ;
    case ZPlanarParameters::HYBRID :

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
  
  std::ostream& operator<< (  std::ostream& s,  const FTDParameters& p ) 
  {
	  s << std::endl 
		  << "   -----------   FTDParameters  ------- "  << std::endl         ;
	  
    
	  s << dynamic_cast<const GearParameters&>( p )  ;
	  
	  const FTDLayerLayout & l = p.getFTDLayerLayout() ;
	  
	  
	  // layers
	  
	  s << " Layers : " << l.getNLayers()  
		  << std::endl << std::endl ;
	  
	  
	  s <<  " layer parameters (note: ladder length (l) are half lengths ! )" 
		  << std::endl ;
	  
	  std::string buffer;
	  buffer = "  |------------------------------------------------------------------------------------|\n" ;
	  s << buffer ;
	  
	  buffer = "  |      |         support:                      |       sensitive part:                |\n" ;
	  s << buffer ;
	  
	  buffer = "  |  #   |    x    |    X    |    y    |   t    |    x    |    X    |    y    |    t   | \n" ;
	  s << buffer ;
	  
	  buffer = "  |------------------------------------------------------------------------------------|\n" ;
	  s << buffer ;
	  
	  for( int i = 0 ; i < l.getNLayers() ; i++ ) 
	  {
		  char buffer1[1024] ;
		  sprintf(buffer1,"  | %3d  |  %4.2f  |  %4.2f  |  %4.2f  |  %4.2f  |  %4.2f  |  %4.2f  |  %4.2f  |  %4.2f  |\n"
				  , l.getNPetals(i) 
				  , l.getSupportLengthMin(i) 
				  , l.getSupportLengthMax(i)
				  , l.getSupportWidth(i)
				  , l.getSupportThickness(i)
				  , l.getSensitiveLengthMin(i)
				  , l.getSensitiveLengthMax(i) 
				  , l.getSensitiveWidth(i) 
				  , l.getSensitiveThickness(i) ) ;
		  
		  s << buffer1 ;
		  
	  }
	  
	  buffer = "  |------------------------------------------------------------------------------------|\n" ;
	  s << buffer ;    
    return s ;
    
  }

  std::ostream& operator<< (  std::ostream& s,  const SiPlanesParameters& p ) {
    
    s << std::endl 
      << "   -----------   SiPlanesParameters  ------- "  << std::endl         ;
    
    s << dynamic_cast<const GearParameters&>( p )  ;
    
    const SiPlanesLayerLayout & l = p.getSiPlanesLayerLayout() ;


    s <<  std::endl << " Setup ID : " << p.getSiPlanesID() << std::endl;

    int type = p.getSiPlanesType() ;
    
    s << " Telescope type : " ;
    std::string strType ;

    switch( type ) {
      
    case SiPlanesParameters::TelescopeWithDUT : 
      strType = "TelescopeWithDUT" ;
      s << " TelescopeWithDUT" << std::endl ;       
      break ;
    case SiPlanesParameters::TelescopeWithoutDUT : 
      strType = "TelescopeWithoutDUT" ;
      s << " TelescopeWithoutDUT " << std::endl ;      
      break ;
      
    default :  
      
      s << " unknown " << std::endl ; 
    }

    s << " Number of telescope planes : " << p.getSiPlanesNumber() << std::endl;
    
    
    // layers
    
									      
    s << " Number of layers : " << l.getNLayers() << std::endl << std::endl ;

    s <<  " layer parameters : "  << std::endl ;

    char buffer[1024] ;
    
    sprintf(buffer,"  |-------------------------------------------------------------------------------------------------------------------------------------------------|\n") ;
    s << buffer ;

    sprintf(buffer,"  |              ladder:                        |                    sensitive part:                                                                |\n") ;
    s << buffer ;

    sprintf(buffer,"  | ID | pozX| pozY|  pozZ |rotXY|rotZX|rotZY| sizeX| sizeY| Thick| ID| pozX| pozY|  pozZ |sizeX|sizeY| Thick|NpixX|NpixY|PitchX|PitchY| Resol| Rot1| Rot2| Rot3| Rot4| \n") ;

    s << buffer ;

    sprintf(buffer,"  |-------------------------------------------------------------------------------------------------------------------------------------------------|\n") ;
    s << buffer ;

    for( int i = 0 ; i < l.getNLayers() ; i++ ) {

      char buffer1[1024] ;
      sprintf(buffer1,"  |%4d|%5.2f|%5.2f|%7.2f|%5.2f|%5.2f|%5.2f|%6.2f|%6.2f|%6.3f|%3d|%5.2f|%5.2f|%7.2f|%5.2f|%5.2f|%6.3f| %4d| %4d|%6.2f|%6.2f|%6.4f| %4.2f| %4.2f| %4.2f| %4.2f|\n"
	      , l.getID(i) 
	      , l.getLayerPositionX(i) 
	      , l.getLayerPositionY(i) 
	      , l.getLayerPositionZ(i)
	      , l.getLayerRotationXY(i) 
	      , l.getLayerRotationZX(i) 
	      , l.getLayerRotationZY(i)
	      , l.getLayerSizeX(i) 
	      , l.getLayerSizeY(i) 
	      , l.getLayerThickness(i)
  	      , l.getSensitiveID(i) 
 	      , l.getSensitivePositionX(i) 
	      , l.getSensitivePositionY(i) 
	      , l.getSensitivePositionZ(i)
	      , l.getSensitiveSizeX(i) 
	      , l.getSensitiveSizeY(i) 
	      , l.getSensitiveThickness(i) 
	      , l.getSensitiveNpixelX(i) 
	      , l.getSensitiveNpixelY(i) 
	      , l.getSensitivePitchX(i) 
	      , l.getSensitivePitchY(i) 
	      , l.getSensitiveResolution(i)
	      , l.getSensitiveRotation1(i) 
	      , l.getSensitiveRotation2(i) 
	      , l.getSensitiveRotation3(i) 
	      , l.getSensitiveRotation4(i) ) ;
      
      s << buffer1 ;

    }

    sprintf(buffer,"  |-------------------------------------------------------------------------------------------------------------------------------------------------|\n") ;
    s << buffer ;

    // DUT

    if (strType == "TelescopeWithDUT"){

      s <<  " DUT parameters : "  << std::endl ;
      
      s << buffer ;
      
      sprintf(buffer,"  |              ladder:                        |                    sensitive part:                                                         |\n") ;
      s << buffer ;
      
      sprintf(buffer,"  |------------------------------------------------------------------------------------------------------------------------------------------|\n") ;
      s << buffer ;
      
      sprintf(buffer,"  | ID |  pozX| pozY|  pozZ | sizeX| sizeY| Thick| ID| pozX| pozY|  pozZ |sizeX|sizeY| Thick|NpixX|NpixY|PitchX|PitchY| Rot1| Rot2| Rot3| Rot4| \n") ;
      s << buffer ;
      
      sprintf(buffer,"  |------------------------------------------------------------------------------------------------------------------------------------------|\n") ;
      s << buffer ;
      
      char buffer1[1024] ;
      sprintf(buffer1,"  |%4d|%5.2f|%5.2f|%7.2f|%6.2f|%6.2f|%6.3f|%3d|%5.2f|%5.2f|%7.2f|%5.2f|%5.2f|%6.3f| %4d| %4d|%6.2f|%6.2f| %4.2f| %4.2f| %4.2f| %4.2f|\n"
	      , l.getDUTID() 
	      , l.getDUTPositionX() 
	      , l.getDUTPositionY() 
	      , l.getDUTPositionZ()
	      , l.getDUTSizeX() 
	      , l.getDUTSizeY() 
	      , l.getDUTThickness() 
	      , l.getDUTSensitiveID() 
	      , l.getDUTSensitivePositionX() 
	      , l.getDUTSensitivePositionY() 
	      , l.getDUTSensitivePositionZ()
	      , l.getDUTSensitiveSizeX()
	      , l.getDUTSensitiveSizeY() 
	      , l.getDUTSensitiveThickness() 
	      , l.getDUTSensitiveNpixelX() 
	      , l.getDUTSensitiveNpixelY() 
	      , l.getDUTSensitivePitchX() 
	      , l.getDUTSensitivePitchY() 
	      , l.getDUTSensitiveRotation1() 
	      , l.getDUTSensitiveRotation2() 
	      , l.getDUTSensitiveRotation3() 
	      , l.getDUTSensitiveRotation4() ) ;
      
      s << buffer1 ;
      
      sprintf(buffer,"  |------------------------------------------------------------------------------------------------------------------------------------------|\n") ;
      s << buffer ;
      
    }
    return s ;
    
    
  }

}
