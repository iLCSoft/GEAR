#ifndef MeasurementSurfaceStore_h
#define MeasurementSurfaceStore_h

/** MeasurementSurfaceStore: Class to hold and manage a collection of MeasurementSufaces  
 *
 * @author S.Aplin DESY
 */

#include <map>
#include <vector>
#include <iostream>

namespace gear{
  
  class MeasurementSurface;
  
  class MeasurementSurfaceStoreFiller;
  
  
  class MeasurementSurfaceStore {
    
  public:
    
    /** Default Constructor */
    MeasurementSurfaceStore() : _store_filled(0) , _fillerName("") { _measurement_surface_map.clear() ;}
 
    /** Copy Constructor */
    MeasurementSurfaceStore( const MeasurementSurfaceStore& ) ;                 
  
    
    /** Destructor */
    ~MeasurementSurfaceStore(); 
    
    /** Get Measurement Surface via ID */
    MeasurementSurface const* GetMeasurementSurface( int ID ) const ;  
    
    /** Fill Store. Only one filler will be accepted. Exception will be thrown on further attempts */
    void FillStore(MeasurementSurfaceStoreFiller* filler);
  
    /** Check if the store has been filled already */
    bool isFilled() const { return _store_filled ; } 

    /** Get the Name of the Filler used to fill the store. In the case of an empty store the string STORE_NOT_YET_FILLED will be returned. */
    std::string getFillerName() const ;
    
  private:
    
    MeasurementSurfaceStore& operator=(const MeasurementSurfaceStore&) ;      // prevent assignment operator
     
    void addMeasurementSurface(MeasurementSurface* ms); 
    
    // private member variables
    std::map<int,MeasurementSurface* > _measurement_surface_map;
    
    typedef std::map<int, MeasurementSurface*>::const_iterator ms_map_it ; 
    
    bool _store_filled;
  
    std::string _fillerName;
    
  };
  
  class MeasurementSurfaceStoreFiller {
    
  public:
    
    virtual void getMeasurementSurfaces(std::vector<MeasurementSurface*>&) const = 0 ;
    virtual std::string getName() const = 0;      
    
  } ;
  
  
} // end of gear namespace 

#endif
