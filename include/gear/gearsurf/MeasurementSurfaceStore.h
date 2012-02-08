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
  class GearMgr;
}

  
  namespace GearSurfaces{
        
    
    class MeasurementSurface;

    class MeasurementSurfaceStoreFiller;

    
    class MeasurementSurfaceStore {
      
    public:
      
      /** Accessor Method */
      static MeasurementSurfaceStore& Instance() {
        
        static MeasurementSurfaceStore singleton;
        
        return singleton;
        
      }
      
      // Other non-static member functions
      
    public:
      
      friend class MeasurementSurfaceStoreFiller;
      
      /** Destructor */
      ~MeasurementSurfaceStore();   
      
      void Print() { std::cout << "Print Method" << std::endl; }
      
      /** Get Measurement Surface via ID */
      MeasurementSurface* GetMeasurementSurface( int ID ) const ;  
      
      /** Fill Store. Only one filler will be accepted, the filler may be sent multiple times but it will only be processed once. Whenever a filler is sent it will be checked that it is the same as the first.*/
      void FillStore(MeasurementSurfaceStoreFiller* filler);
      
    private:
      
      MeasurementSurfaceStore() : first_filler(0) { _measurement_surface_map.clear() ;}                               // Private constructor
      
      
      MeasurementSurfaceStore(const MeasurementSurfaceStore&) ;                 // Prevent copy-construction
      MeasurementSurfaceStore& operator=(const MeasurementSurfaceStore&) ;      // Prevent assignment
      
      void addMeasurementSurface(MeasurementSurface* ms); 
            
      
      // only one filler will be accepted, the filler may be sent multiple times but it will only be processed once. Whenever a filler is sent it will be checked that it is the same as the first.
      MeasurementSurfaceStoreFiller* first_filler;
      
      // private member variables
      std::map<int,MeasurementSurface* > _measurement_surface_map;
      
      typedef std::map<int, MeasurementSurface*>::const_iterator ms_map_it ; 
      
      static bool _isInitialised;

      
    };
    
    class MeasurementSurfaceStoreFiller {
      
      friend class MeasurementSurfaceStore;
      
    protected:
      
      virtual void fill_store(std::vector<MeasurementSurface*>&) const = 0 ;
      
      
    } ;

    
  } // end of GearSurfaces namespace 

#endif
