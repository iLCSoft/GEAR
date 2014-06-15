
#include "gearimpl/GearMgrImpl.h"
#include "gearimpl/GearParametersImpl.h"
#include "gear/CalorimeterParameters.h"
#include "gear/ZPlanarParameters.h"
#include "gear/SiPlanesParameters.h"
#include "gear/TelPlanesParameters.h"

#include "gear/GearPointProperties.h"
#include "gear/GearDistanceProperties.h"
#include "gear/BField.h"
#include "gear/TPCParameters.h"
#include "gearimpl/TPCParametersImpl.h"
#include "gearimpl/CalorimeterParametersImpl.h"
#include "gearimpl/ZPlanarParametersImpl.h"
#include "gearimpl/FTDParametersImpl.h"
#include "gearimpl/SiPlanesParametersImpl.h"
#include "gearimpl/TelPlanesParametersImpl.h"

#include "gearimpl/ConstantBField.h"
#include "gearimpl/SimpleMaterialImpl.h"

#include <sstream>

namespace gear{


  GearMgrImpl::GearMgrImpl() :
    _tpcParameters(0) ,
    _ecalBarrelParameters(0) ,
    _ecalEndcapParameters(0) ,
    _ecalPlugParameters(0) ,
    _yokeBarrelParameters(0) ,
    _yokeEndcapParameters(0) ,
    _yokePlugParameters(0) ,
    _hcalBarrelParameters(0) ,
    _hcalEndcapParameters(0) ,
    _hcalRingParameters(0) ,
    _lcalParameters(0) ,
    _lhcalParameters(0) ,
    _beamcalParameters(0) ,
    _vxdParameters(0) ,
    _sitParameters(0) ,
    _setParameters(0) ,
    _ftdParameters(0) ,
    _telplanesParameters(0) ,
    _siplanesParameters(0) ,
    _pointProperties(0) ,
    _distanceProperties(0) ,
    _bField(0) ,
    _detectorName(""){
      
    _surfaceStore = new MeasurementSurfaceStore();

    }
  
  //~ GearMgrImpl::GearMgrImpl(const  GearMgrImpl & right)
  //~ {
      // the same code will be called in the assigment operator, so it's in 
      // a separate function to avoid code replication
      //~ copy_and_assign(right);
  //~ }

  //~ void GearMgrImpl::copy_and_assign(const  GearMgrImpl & right) {
    //~ ///FIXME!!! this also contains pointers
    //~ //ParameterMap::iterator it_end = _map.end() ;
    //~ 
    //~ for( ParameterMap::iterator paramIter = _map.begin() ; paramIter != _map.end() ; paramIter++ ) {
      //~ // the map should only have GearParametersImpl, not anything derrived from it
      //~ paramIter->second = new GearParametersImpl( *dynamic_cast<GearParametersImpl *>(paramIter->second) );
    //~ }
    //~ 
    //~ //*** these two need qualification for construction -- how?? CR ***/
    //~ _distanceProperties   = new (right._distanceProperties ); GearDistanceProperties
    //~ _pointProperties      = new (right._pointProperties );GearPointProperties
//~ 
    //~ _beamcalParameters    = new CalorimeterParametersImpl( *dynamic_cast<CalorimeterParametersImpl *>(right._beamcalParameters ));
    //~ _lhcalParameters      = new CalorimeterParametersImpl( *dynamic_cast<CalorimeterParametersImpl *>(right._lhcalParameters ));
    //~ _yokeBarrelParameters = new CalorimeterParametersImpl( *dynamic_cast<CalorimeterParametersImpl *>(right._yokeBarrelParameters ));
    //~ _yokeEndcapParameters = new CalorimeterParametersImpl( *dynamic_cast<CalorimeterParametersImpl *>(right._yokeEndcapParameters ));
    //~ _yokePlugParameters   = new CalorimeterParametersImpl( *dynamic_cast<CalorimeterParametersImpl *>(right._yokePlugParameters ));
    //~ 
    //~ _tpcParameters = new TPCParametersImpl( *dynamic_cast<TPCParametersImpl *> (right._tpcParameters ));
    //~ _ecalBarrelParameters = new CalorimeterParametersImpl( *dynamic_cast<CalorimeterParametersImpl *>(right._ecalBarrelParameters));
    //~ _ecalEndcapParameters = new CalorimeterParametersImpl( *dynamic_cast<CalorimeterParametersImpl *>(right._ecalEndcapParameters));
    //~ _ecalPlugParameters = new CalorimeterParametersImpl( *dynamic_cast<CalorimeterParametersImpl *>(right._ecalPlugParameters));
    //~ _hcalBarrelParameters = new CalorimeterParametersImpl( *dynamic_cast<CalorimeterParametersImpl *>(right._hcalBarrelParameters));
    //~ _hcalEndcapParameters = new CalorimeterParametersImpl( *dynamic_cast<CalorimeterParametersImpl *>(right._hcalEndcapParameters));
    //~ _hcalRingParameters = new CalorimeterParametersImpl( *dynamic_cast<CalorimeterParametersImpl *>(right._hcalRingParameters));
    //~ _lcalParameters = new CalorimeterParametersImpl( *dynamic_cast<CalorimeterParametersImpl *>(right._lcalParameters));
    //~ _vxdParameters = new ZPlanarParametersImpl( *dynamic_cast<ZPlanarParametersImpl *>(right._vxdParameters));
    //~ _sitParameters = new ZPlanarParametersImpl( *dynamic_cast<ZPlanarParametersImpl *>(right._sitParameters));
    //~ _setParameters = new ZPlanarParametersImpl( *dynamic_cast<ZPlanarParametersImpl *>(right._setParameters));
    //~ _ftdParameters = new FTDParametersImpl( *dynamic_cast<FTDParametersImpl *>(right._ftdParameters));
    //~ _siplanesParameters = new SiPlanesParametersImpl( *dynamic_cast<SiPlanesParametersImpl *>(right._siplanesParameters));
//~ 
    //~ // there might be different implementations for bField , so test whether casting works
    //~ if (dynamic_cast<ConstantBField *>(right._bField) != 0)
	//~ _bField = new ConstantBField(*dynamic_cast<ConstantBField *>(right._bField));
	//~ else
	    //~ throw gear::Exception(" GearMgrImpl::GearMgrImpl(const  GearMgrImpl & r) Unknown BField type");
//~ 
    //~ _detectorName = right._detectorName;
    //~ _keys = right._keys;      
//~ 
//~ 
    //~ for( MatMap::const_iterator it = right._matMap.begin(), end = right._matMap.end() ; it != end ; ++it ){
//~ 
      //~ this->registerSimpleMaterial( new SimpleMaterialImpl( *dynamic_cast<const SimpleMaterialImpl*>( it->second ) ) ) ; 
    //~ }
//~ 
    //~ _surfaceStore = new MeasurementSurfaceStore( (*right._surfaceStore) ) ;
//~ 
  //~ }

  GearMgrImpl::~GearMgrImpl() 
  {
      cleanup();
  }

  void GearMgrImpl::cleanup() {
    
    // clean up all parameters
    if( _tpcParameters ) delete _tpcParameters ;
    if( _ecalBarrelParameters ) delete _ecalBarrelParameters ;
    if( _ecalEndcapParameters ) delete _ecalEndcapParameters ;
    if( _ecalPlugParameters ) delete _ecalPlugParameters ;
    // no need to check for 0, it's save to delete  a 0-pointer
    delete _yokeBarrelParameters ;
    delete _yokeEndcapParameters ;
    delete _yokePlugParameters ;
    if( _hcalBarrelParameters ) delete _hcalBarrelParameters ;
    if( _hcalEndcapParameters ) delete _hcalEndcapParameters ;
    if( _hcalRingParameters ) delete _hcalRingParameters ;
    if( _lcalParameters ) delete  _lcalParameters;
    if( _lhcalParameters ) delete  _lhcalParameters;
    if( _beamcalParameters ) delete  _beamcalParameters;
    if( _vxdParameters ) delete _vxdParameters ;
    if( _sitParameters ) delete _sitParameters ;
    if( _setParameters ) delete _setParameters ;
    if( _ftdParameters ) delete _ftdParameters ;
    if( _telplanesParameters ) delete _telplanesParameters ;
    if( _siplanesParameters ) delete _siplanesParameters ;
    if( _pointProperties ) delete _pointProperties ;
    if( _distanceProperties ) delete _distanceProperties ;
    if( _bField  ) delete  _bField ;
    
    for( ParameterMap::iterator it = _map.begin() ; it != _map.end() ; ++ it ) {
      delete it->second ;
    }
 
    for(MatMap::iterator it = _matMap.begin(), end = _matMap.end() ; it != end ; ++it ){
      delete it->second ;
    }
    
    delete _surfaceStore ; _surfaceStore = 0 ;
    
  }

  //~ GearMgrImpl& GearMgrImpl::operator = (const GearMgrImpl &right)
  //~ {
      // call the cleanup and the copy and assignment afterwards
      //~ cleanup();
      //~ copy_and_assign(right);

      //~ return *this;
  //~ }

  const std::string& GearMgrImpl::getDetectorName() const    

    throw (UnknownParameterException, std::exception ) { 

    if( _detectorName.size() == 0 )
      throw UnknownParameterException( "No DetectorName set ") ;


    return _detectorName ; 
  }


  const GearParameters & GearMgrImpl::getGearParameters(const std::string & key) const 
    
    throw (UnknownParameterException, std::exception ) {
    
    ParameterMap::const_iterator it = _map.find( key ) ;
    if( it == _map.end() || it->second == 0 )
      throw UnknownParameterException( "No parameters set for : " + key ) ;
    return * it->second ;
    
  }   
  
  const TPCParameters & GearMgrImpl::getTPCParameters() const
    throw (UnknownParameterException, std::exception ) {
    
    if( _tpcParameters == 0 )
      throw UnknownParameterException( "No TPCParameters set ") ;

    return  *_tpcParameters ;

  }

  const BField & GearMgrImpl::getBField() const
    throw (UnknownParameterException, std::exception ) {
    
    if( _bField == 0 )
      throw UnknownParameterException( "No BField set ") ;

    return  *_bField ;

  }

  const CalorimeterParameters & GearMgrImpl::getEcalBarrelParameters() const
    throw (UnknownParameterException, std::exception ) {
    
    if( _ecalBarrelParameters == 0 )
      throw UnknownParameterException( "No EcalBarrelParameters set ") ;

    return  *_ecalBarrelParameters ;

  }
  const CalorimeterParameters & GearMgrImpl::getEcalEndcapParameters() const
    throw (UnknownParameterException, std::exception ) {
    
    if( _ecalEndcapParameters == 0 )
      throw UnknownParameterException( "No EcalEndcapParameters set ") ;

    return  *_ecalEndcapParameters ;

  }
  
  const CalorimeterParameters & GearMgrImpl::getEcalPlugParameters() const
    throw (UnknownParameterException, std::exception ) {
    
    if( _ecalPlugParameters == 0 )
      throw UnknownParameterException( "No EcalPlugParameters set ") ;

    return  *_ecalPlugParameters ;

  }

  const CalorimeterParameters & GearMgrImpl::getYokeBarrelParameters() const
    throw (UnknownParameterException, std::exception ) {
    
    if( _yokeBarrelParameters == 0 )
      throw UnknownParameterException( "No YokeBarrelParameters set ") ;

    return  *_yokeBarrelParameters ;

  }
  const CalorimeterParameters & GearMgrImpl::getYokeEndcapParameters() const
    throw (UnknownParameterException, std::exception ) {
    
    if( _yokeEndcapParameters == 0 )
      throw UnknownParameterException( "No YokeEndcapParameters set ") ;

    return  *_yokeEndcapParameters ;

  }
  
  const CalorimeterParameters & GearMgrImpl::getYokePlugParameters() const
    throw (UnknownParameterException, std::exception ) {
    
    if( _yokePlugParameters == 0 )
      throw UnknownParameterException( "No YokePlugParameters set ") ;

    return  *_yokePlugParameters ;

  }

 
  
  const CalorimeterParameters & GearMgrImpl::getHcalBarrelParameters() const
    throw (UnknownParameterException, std::exception ) {
    
    if( _hcalBarrelParameters == 0 )
      throw UnknownParameterException( "No HcalBarrelParameters set ") ;

    return  *_hcalBarrelParameters ;

  }
  const CalorimeterParameters & GearMgrImpl::getHcalEndcapParameters() const
    throw (UnknownParameterException, std::exception ) {
    
    if( _hcalEndcapParameters == 0 )
      throw UnknownParameterException( "No HcalEndcapParameters set ") ;

    return  *_hcalEndcapParameters ;

  }

  const CalorimeterParameters & GearMgrImpl::getHcalRingParameters() const
    throw (UnknownParameterException, std::exception ) {
    
    if( _hcalRingParameters == 0 )
      throw UnknownParameterException( "No HcalRingParameters set ") ;

    return  *_hcalRingParameters ;

  }


  const CalorimeterParameters & GearMgrImpl::getLcalParameters() const
    throw (UnknownParameterException, std::exception ) {
    
    if( _lcalParameters == 0 )
      throw UnknownParameterException( "No LcalParameters set ") ;

    return  *_lcalParameters ;

  }

  const CalorimeterParameters & GearMgrImpl::getLHcalParameters() const
    throw (UnknownParameterException, std::exception ) {
    
    if( _lhcalParameters == 0 )
      throw UnknownParameterException( "No LHcalParameters set ") ;

    return  *_lhcalParameters ;

  }

  const CalorimeterParameters & GearMgrImpl::getBeamCalParameters() const
    throw (UnknownParameterException, std::exception ) {
    
    if( _beamcalParameters == 0 )
      throw UnknownParameterException( "No BeamCalParameters set ") ;

    return  *_beamcalParameters ;

  }

  const ZPlanarParameters & GearMgrImpl::getVXDParameters() const
    throw (UnknownParameterException, std::exception ) {

    if( _vxdParameters == 0 )
      throw UnknownParameterException( "No VXDParameters set ") ;

    return *_vxdParameters ;

  }
  
  const FTDParameters & GearMgrImpl::getFTDParameters() const
    throw (UnknownParameterException, std::exception ) {
    
    if( _ftdParameters == 0 )
      throw UnknownParameterException( "No FTDParameters set ") ;
    
    return *_ftdParameters ;
    
  }

  const ZPlanarParameters & GearMgrImpl::getSITParameters() const
    throw (UnknownParameterException, std::exception ) {

    if( _sitParameters == 0 )
      throw UnknownParameterException( "No SITParameters set ") ;

    return *_sitParameters ;

  }
  const ZPlanarParameters & GearMgrImpl::getSETParameters() const
    throw (UnknownParameterException, std::exception ) {

    if( _setParameters == 0 )
      throw UnknownParameterException( "No SETParameters set ") ;

    return *_setParameters ;

  }

  const TelPlanesParameters & GearMgrImpl::getTelPlanesParameters() const
    throw (UnknownParameterException, std::exception ) {

    if( _telplanesParameters == 0 )
      throw UnknownParameterException( "No TelPlanesParameters set ") ;

    return *_telplanesParameters ;

  }  
 
  const SiPlanesParameters & GearMgrImpl::getSiPlanesParameters() const
    throw (UnknownParameterException, std::exception ) {

    if( _siplanesParameters == 0 )
      throw UnknownParameterException( "No SiPlanesParameters set ") ;

    return *_siplanesParameters ;

  }
  
  const GearPointProperties & GearMgrImpl::getPointProperties() const 
    throw (NotImplementedException, std::exception ) {

    if( _pointProperties == 0 )
      throw UnknownParameterException( "No PointProperties set or implemented ") ;
    
    return  *_pointProperties ;
  }
  
  
  
  const GearDistanceProperties & GearMgrImpl::getDistanceProperties() const 
    throw (NotImplementedException, std::exception ) {

    if( _distanceProperties == 0 )
      throw UnknownParameterException( "No DistanceProperties set or implemented ") ;
    
    return  *_distanceProperties ;

  }

  void GearMgrImpl::setGearParameters( const std::string & key, GearParameters* parameters ) {

    if( parameters == 0 )   // don't allow  null pointers 
      return  ;    
    
    ParameterMap::iterator it = _map.find( key ) ;

    if( it != _map.end() ) {
      
      delete it->second ;
      it->second = parameters ;
      
    } else {
      
      _map[ key ] = parameters ; 
      
    }
    
    
  }
  
  void GearMgrImpl::setTPCParameters( TPCParameters* tpcParameters ) {
    
    if(  _tpcParameters ) 
      delete _tpcParameters ;

    _tpcParameters = tpcParameters ;
  }

  void GearMgrImpl::setBField( BField* b){
    
    _bField = b ;
  }

  void GearMgrImpl::setEcalBarrelParameters( CalorimeterParameters* ecalBarrelParameters ) {
    
    _ecalBarrelParameters = ecalBarrelParameters ;
  }
  void GearMgrImpl::setEcalEndcapParameters( CalorimeterParameters* ecalEndcapParameters ) {
    
    _ecalEndcapParameters = ecalEndcapParameters ;
  }

  void GearMgrImpl::setEcalPlugParameters( CalorimeterParameters* ecalPlugParameters ) {
    
    _ecalPlugParameters = ecalPlugParameters ;
  }

  void GearMgrImpl::setYokeBarrelParameters( CalorimeterParameters* yokeBarrelParameters ) {
    
    _yokeBarrelParameters = yokeBarrelParameters ;
  }
  void GearMgrImpl::setYokeEndcapParameters( CalorimeterParameters* yokeEndcapParameters ) {
    
    _yokeEndcapParameters = yokeEndcapParameters ;
  }

  void GearMgrImpl::setYokePlugParameters( CalorimeterParameters* yokePlugParameters ) {
    
    _yokePlugParameters = yokePlugParameters ;
  }


  void GearMgrImpl::setHcalBarrelParameters( CalorimeterParameters* hcalBarrelParameters ) {
    
    _hcalBarrelParameters = hcalBarrelParameters ;
  }

  void GearMgrImpl::setHcalEndcapParameters( CalorimeterParameters* hcalEndcapParameters ) {

    _hcalEndcapParameters = hcalEndcapParameters ;
  }

  void GearMgrImpl::setHcalRingParameters( CalorimeterParameters* hcalRingParameters ) {

    _hcalRingParameters = hcalRingParameters ;
  }

  void GearMgrImpl::setLcalParameters( CalorimeterParameters* lcalParameters ) {

    try {

      lcalParameters->getDoubleVal("beam_crossing_angle") ;

    }catch( UnknownParameterException ){

      std::cout << "WARNING GearMgrImpl::setLcalParameters: added "
	" missing parameter beam_crossing_angle 0.0 ! " << std::endl ;
      
      GearParametersImpl* gp 
	= dynamic_cast<GearParametersImpl*>(lcalParameters) ;

      if( gp ) 
	gp->setDoubleVal("beam_crossing_angle", 0.0 ) ;

    }
    _lcalParameters = lcalParameters ;
  }

  void GearMgrImpl::setLHcalParameters( CalorimeterParameters* lhcalParameters ) {

    try {

      lhcalParameters->getDoubleVal("beam_crossing_angle") ;

    }catch( UnknownParameterException ){

    std::cout << "WARNING GearMgrImpl::setLHcalParameters: added "
	" missing parameter beam_crossing_angle 0.0 ! " << std::endl ;
      
      GearParametersImpl* gp 
	= dynamic_cast<GearParametersImpl*>(lhcalParameters) ;

      if( gp ) 
	gp->setDoubleVal("beam_crossing_angle", 0.0 ) ;

    }
    _lhcalParameters = lhcalParameters ;
  }

  void GearMgrImpl::setBeamCalParameters( CalorimeterParameters* beamcalParameters ) {

    try {

      beamcalParameters->getDoubleVal("beam_crossing_angle") ;

    }catch( UnknownParameterException ){

      std::cout << "WARNING GearMgrImpl::setBeamCalParameters: added "
	" missing parameter beam_crossing_angle 0.0 ! " << std::endl ;
      
      GearParametersImpl* gp 
	= dynamic_cast<GearParametersImpl*>(beamcalParameters) ;

      if( gp ) 
	gp->setDoubleVal("beam_crossing_angle", 0.0 ) ;

    }
    _beamcalParameters = beamcalParameters ;
  }

  void GearMgrImpl::setVXDParameters( ZPlanarParameters* vxdParameters ) {

    _vxdParameters = vxdParameters ;
  }

  void GearMgrImpl::setFTDParameters( FTDParameters* ftdParameters ) {
    
    _ftdParameters = ftdParameters ;
  }
  
  void GearMgrImpl::setSITParameters( ZPlanarParameters* sitParameters ) {
    
    _sitParameters = sitParameters ;
  }

  void GearMgrImpl::setSETParameters( ZPlanarParameters* setParameters ) {

    _setParameters = setParameters ;
  }

  void GearMgrImpl::setTelPlanesParameters( TelPlanesParameters* telplanesParameters ) {

    _telplanesParameters = telplanesParameters ;
  }

  void GearMgrImpl::setSiPlanesParameters( SiPlanesParameters* siplanesParameters ) {

    _siplanesParameters = siplanesParameters ;
  }

  void GearMgrImpl::setPointProperties( GearPointProperties* pointProperties) {
    
    _pointProperties = pointProperties ;
  }

  void GearMgrImpl::setDistanceProperties( GearDistanceProperties* distanceProperties) {
 
   _distanceProperties = distanceProperties ;
  }
  
  const std::vector<std::string>& GearMgrImpl::getGearParameterKeys() const {

    _keys.clear() ;
    _keys.reserve( _map.size() ) ;

    for( ParameterMap::const_iterator it = _map.begin() ; it != _map.end() ; ++it ){
      _keys.push_back( it->first ) ;
    }
    return _keys ;
  }

  const std::vector<std::string>& GearMgrImpl::getMaterialNames() const {
    
    _matNames.clear() ;
    _matNames.reserve( _matMap.size() ) ;
    
    for( MatMap::const_iterator it = _matMap.begin() ; it != _matMap.end() ; ++it ){
      _matNames.push_back( it->first ) ;
    }
    return _matNames ;
  
  }


  const SimpleMaterial& GearMgrImpl::getSimpleMaterial( const std::string name ) const 
    throw (UnknownParameterException, std::exception ) {
    
    MatMap::const_iterator it = _matMap.find( name ) ;
    
    if( it == _matMap.end() ){
      
      std::stringstream mess ;
      mess << " GearMgrImpl::getSimpleMaterial - unknown material with name " << name << "  ! " ;
      
      throw UnknownParameterException( mess.str() ) ;
    }

    return *it->second ;
 }
  

  void GearMgrImpl::registerSimpleMaterial( const SimpleMaterial* material) 
    throw(Exception , std::exception ) {

    MatMap::iterator it = _matMap.find( material->getName() ) ;
    
    if( it!= _matMap.end() ){
      
      std::stringstream mess ;
      mess << " GearMgrImpl::registerSimpleMaterial - material with name " << material->getName() << " already registered ! " ;
      
      throw Exception( mess.str() ) ;
    }
    
    _matMap[ material->getName() ] = material ;

  }



}
