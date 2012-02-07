#ifndef MeasurementSurfaceStore_h
#define MeasurementSurfaceStore_h

/** MeasurementSurfaceStore: Class to hold and manage a collection of MeasurementSufaces  
 *
 * @author S.Aplin DESY
 */

#include <map>
#include <exception>
#include <vector>
#include <string>

namespace gear{
  class GearMgr;
  class ZPlanarParameters;
  class FTDParameters;
}

  
  namespace GearSurfaces{
    
    class MeasurementSurface;
    
    
    class MeasurementSurfaceStoreException: public std::exception {
      
    public:
      
      MeasurementSurfaceStoreException( std::string info= "unspecified error" ): _info( info ){}
      
      ~MeasurementSurfaceStoreException() throw() { /*no_op*/; } 
      
      virtual const char* what() const throw() {
        
        std::string what = "MeasurementSurfaceStoreException occurred: " + _info;
        return what.c_str();
      }
      
    private:
      
      
      std::string _info;
      
    };
    
    
    class MeasurementSurfaceStore {
      
    public:
      
      /** Accessor Method */
      static MeasurementSurfaceStore& Instance() {
        
        static MeasurementSurfaceStore singleton;
        
        return singleton;
        
      }
      
      // Other non-static member functions
      
    public:
      
      /** Destructor */
      ~MeasurementSurfaceStore();   
      
      void initialise(gear::GearMgr* gear_mgr) ;
      
      /** Get Measurement Surface via ID */
      MeasurementSurface* GetMeasurementSurface( int ID ) const ;  
      
      
    private:
      
      MeasurementSurfaceStore() { _measurement_surface_map.clear() ;}                               // Private constructor
      
      
      
      MeasurementSurfaceStore(const MeasurementSurfaceStore&) ;                 // Prevent copy-construction
      MeasurementSurfaceStore& operator=(const MeasurementSurfaceStore&) ;      // Prevent assignment
      
      void addMeasurementSurface(MeasurementSurface* ms); 
      
      void createStore(gear::GearMgr* gear_mgr);
      
      /** adds MeasurementSufaces to the store
       * @param param: the ZPlanarParameters pointer of the detector, of which the measurement surfaces shall be added
       * 
       * @param det_id: the detector id (as in ILDConf)
       */
      void storeZPlanar( const gear::ZPlanarParameters* param , int det_id );
      
      void storeFTD( const gear::FTDParameters* param );
      
      // private member variables
      std::map<int,MeasurementSurface* > _measurement_surface_map;
      
      typedef std::map<int, MeasurementSurface*>::const_iterator ms_map_it ; 
      
      static bool _isInitialised;
      
#define HARDCODEDGEAR 1
#ifdef HARDCODEDGEAR
      
      /** the strip angles for every layer */
      std::vector< double > _VTXStripAngles;
      std::vector< double > _SITStripAngles;
      std::vector< double > _SETStripAngles;
      
      /** the strip angles for every layer and sensor */
      std::vector< std::vector< double > > _FTDStripAngles;
      
      
#endif
      
    };
    
  } // end of GearSurfaces namespace 

#endif
