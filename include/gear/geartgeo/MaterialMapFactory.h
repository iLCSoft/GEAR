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
   * @version $Id: MaterialMapFactory.h,v 1.1 2006-05-29 13:03:18 musat Exp $
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
