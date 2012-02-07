
#include "MeasurementSurfaceStore.h"

#include <stdexcept>
#include <vector>
#include <algorithm>
#include <sstream>

#include "MeasurementSurface.h"
#include "BoundaryRectangle.h"
#include "BoundaryTrapezoid.h"

//#include <gear/ZPlanarParameters.h>
//#include <gear/ZPlanarLayerLayout.h>
//#include <gear/FTDLayerLayout.h>
//#include <gear/FTDParameters.h>
//#include <gear/GEAR.h>
//#include <gear/GearMgr.h>


#include "CartesianCoordinateSystem.h"



namespace GearSurfaces{
  
  
  void printRotation( const CLHEP::HepRotation& R ){
    
    std::cout.precision(2);
    std::cout.setf( std::ios::fixed , std::ios::floatfield );
    std::cout << "R:\n"
    << "| " << R.xx() << "  " << R.xy() << "  " << R.xz() << " |\n"
    << "| " << R.yx() << "  " << R.yy() << "  " << R.yz() << " |\n"
    << "| " << R.zx() << "  " << R.zy() << "  " << R.zz() << " |\n";
  }
  
  void printVector( const CLHEP::Hep3Vector& vec ){
    
    std::cout.precision(2);
    std::cout.setf( std::ios::fixed , std::ios::floatfield );
    std::cout << "( " << vec.x() << "  " << vec.y() << "  " << vec.z() << " )\n";
    
  }
  
  bool MeasurementSurfaceStore::_isInitialised = false ;
  
  MeasurementSurfaceStore::~MeasurementSurfaceStore(){
    
    ms_map_it it = _measurement_surface_map.begin();
    std::vector<MeasurementSurface*> deleted_objects;
    
    for( /**/; it!=_measurement_surface_map.end(); ++it) 
      
      if( std::find( deleted_objects.begin(), deleted_objects.end(), (*it).second ) != deleted_objects.end() ) {
        delete (*it).second ;
        deleted_objects.push_back((*it).second) ;
      }
  }
  
  MeasurementSurface* MeasurementSurfaceStore::GetMeasurementSurface(int ID) const {
    
    ms_map_it it = _measurement_surface_map.find(ID) ;        
    
    if ( it == _measurement_surface_map.end() ) { 
      
      std::stringstream s;
      s << "GetMeasurementSurface: The surface with ID " << ID << " is not in the map!";
      MeasurementSurfaceStoreException exp( s.str() ) ;
      
      throw exp ; 
    } 
    else { 
      return (*it).second ; 
    }
    
  }
  
  void MeasurementSurfaceStore::initialise(gear::GearMgr* gear_mgr){
    
    if ( ! _isInitialised) {
      //      this->createStore(gear_mgr); 
      _isInitialised = true ;
    }
    
    
  }
  
  void MeasurementSurfaceStore::addMeasurementSurface(MeasurementSurface* ms) {
    
    int ID = ms->getID();
    
    ms_map_it it = _measurement_surface_map.find(ID) ; 
    
    
    
    if ( it != _measurement_surface_map.end() ) { 
      
      std::stringstream s;
      s << "addMeasurementSurface: The surface with ID " << ID << " can't be added as it already is in the map!";
      MeasurementSurfaceStoreException exp( s.str() ) ;
      throw exp; 
      
    } 
    else { 
      _measurement_surface_map[ID] = ms  ; 
    }
  }
  
  
  
} // end of GearSurfaces namespace 












