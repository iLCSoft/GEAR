
#include "gearsurf/MeasurementSurfaceStore.h"

#include <sstream>
#include <vector>
#include <algorithm>

#include "gear/GEAR.h"

#include "gearsurf/MeasurementSurface.h"
#include "gearsurf/BoundaryRectangle.h"
#include "gearsurf/BoundaryTrapezoid.h"

#include "gearsurf/CartesianCoordinateSystem.h"



namespace gear{
  
  MeasurementSurfaceStore::MeasurementSurfaceStore(const MeasurementSurfaceStore& right) {
    
    _store_filled = right._store_filled;

    _measurement_surface_map = right._measurement_surface_map;
    
    _fillerName = right._fillerName;
    
  }
  
  
  MeasurementSurfaceStore::~MeasurementSurfaceStore(){
    
    ms_map_it it = _measurement_surface_map.begin();
    std::vector<MeasurementSurface*> deleted_objects;
    
    for( /**/; it!=_measurement_surface_map.end(); ++it) 
      
      if( std::find( deleted_objects.begin(), deleted_objects.end(), (*it).second ) != deleted_objects.end() ) {
        delete (*it).second ;
        deleted_objects.push_back((*it).second) ;
      }
  }
  
  MeasurementSurface const* MeasurementSurfaceStore::GetMeasurementSurface(int ID) const {
    
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
  
  
  std::string MeasurementSurfaceStore::getFillerName() const { 
    
    if ( _store_filled == false ) {
      gear::Exception exp( "MeasurementSurfaceStore::getFillerName(): Store has not yet been filled!" ) ;
      throw  exp ; 
    }

    return _fillerName;
    
  }
  
  void MeasurementSurfaceStore::FillStore(MeasurementSurfaceStoreFiller* filler){

    if( _store_filled ) {
      
      std::stringstream s;
      s << "FillStore: Store already filled with MeasurementSurfaceStoreFiller " << _fillerName ;
      gear::Exception exp( s.str() ) ;
      
      throw exp ; 
      
    }
    else {
      
      _fillerName = filler->getName(); 
      std::vector<MeasurementSurface*> surface_list;
      filler->getMeasurementSurfaces(surface_list);
      
      std::vector<MeasurementSurface*>::iterator it;
      
      for (it = surface_list.begin(); it!=surface_list.end(); ++it) {
        this->addMeasurementSurface(*it);
      }
      
      _store_filled = true;
      
    }    
    
  }
  
  
} // end of gear namespace 












