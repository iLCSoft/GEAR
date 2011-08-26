#ifndef GEAR_MATERIALMAPFACTORY_H
#define GEAR_MATERIALMAPFACTORY_H 1

#include <string>
#include <vector>

#include "gear/GEAR.h"
#include "gear/geartgeo/MaterialMap.h"
#include "gear/GearMgr.h"


namespace gear {
  
  /** Factory to create material maps using GearDistanceProperties
   * @author A. Muennich, CERN
   * The intention of this utility is to save computing time, if an application has to call
   * GearDistanceProperties 1000s of times it is slow. If the application can live with some 
   * approximation of material budget thi utility provides a map of the material in memory that
   * can then be querried by the application. The creation of the map happens once and will take some time,
   * but its usage by the application is then fast.
   * The idea is to use the factory to create a map in the interseting region which the precision required.
   * If possible one should make use of detector symmetries to limit the size of the map and gaining in 
   * computing time or invest in more granularity.
   * Run time examples: If GearDistanceProperties are implemented with TGeo
   * myMapFactory->createMaterialMap(-200,200, 10,-200,200,10 ,-200, 200, 10, 0) takes 7.2 s for 1331 
   * entries (10x10x10 steps), 5 s of that is needed to load in the geometry from gdml into ROOT.
   * Creating a map with 100x10x10 takes 22s and 100x100x100 already 28 min.
   * @version $Id$
   */
  class MaterialMapFactory {
    
  public: 
    MaterialMapFactory(GearMgr *gearMgr);
    /// Destructor.
    virtual ~MaterialMapFactory() { 
      ;}
    /** Creating a material map in memory. Three coordinate systems are supported:
     *	coord=0 : CARTESIAN (x,y,z) 
     *  coord=1 : CYLINDRICAL (r,phi,z) with r>0, 0<phi<2Pi
     *  coord=2 : SPERICAL (r,theta,phi) with r>0, 0<theta<Pi 0<phi<2Pi
     *  With min and max the beginning and the end are defined and step is the number of steps
     *  that defines the granularity of the map.
     */
    MaterialMap* createMaterialMap(double xmin, double xmax, int nxsteps,
				   double ymin, double ymax, int nysteps,
				   double zmin, double zmax, int nzsteps, int coord);
    
    
  protected:
    GearMgr *_gearMgr;
    std::map< std::vector<double> , MaterialMap* > _managerMap;
  }; // class
} // namespace gear
#endif /* ifndef  GEAR_MATERIALMAPFACTORY_H */
