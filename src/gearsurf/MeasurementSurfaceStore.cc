
#include "gearsurf/MeasurementSurfaceStore.h"

#include <sstream>
#include <vector>

#include "GEAR.h"

#include "gearsurf/MeasurementSurface.h"
#include "gearsurf/BoundaryRectangle.h"
#include "gearsurf/BoundaryTrapezoid.h"

#include "gearsurf/CartesianCoordinateSystem.h"



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
      gear::Exception exp( s.str() ) ;
      
      throw exp ; 
    } 
    else { 
      return (*it).second ; 
    }
    
  }
  
//  void MeasurementSurfaceStore::initialise(gear::GearMgr* gear_mgr){
//    
//    if ( ! _isInitialised) {
//      //      this->createStore(gear_mgr); 
//      _isInitialised = true ;
//    }
//    
//    
//  }
//  
  void MeasurementSurfaceStore::addMeasurementSurface(MeasurementSurface* ms) {
    
    int ID = ms->getID();
    
    ms_map_it it = _measurement_surface_map.find(ID) ; 
    
  
    if ( it != _measurement_surface_map.end() ) { 
      
      std::stringstream s;
      s << "addMeasurementSurface: The surface with ID " << ID << " can't be added as it already is in the map!";
      gear::Exception  exp( s.str() ) ;
      throw exp; 
      
    } 
    else { 
      _measurement_surface_map[ID] = ms  ; 
    }
  }
  
  
  void MeasurementSurfaceStore::FillStore(MeasurementSurfaceStoreFiller* filler){
    
    if ( first_filler == 0 ) {
      std::vector<MeasurementSurface*> surface_list;
      filler->fill_store(surface_list);
      
      std::vector<MeasurementSurface*>::iterator it;
      
      for (it = surface_list.begin(); it!=surface_list.end(); ++it) {
        this->addMeasurementSurface(*it);
      }
      
    }
    else if( filler != first_filler ) {
      
      std::stringstream s;
      s << "FillStore: Store already filled with MeasurementSurfaceStoreFiller " << first_filler ;
      gear::Exception exp( s.str() ) ;
      
      throw exp ; 

      
    }
    
    
  }
  
  
} // end of GearSurfaces namespace 












