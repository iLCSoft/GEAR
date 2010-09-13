#include <string>
#include <vector>

#include "gear/GEAR.h"
#include "gear/geartgeo/MaterialMapFactory.h"

namespace gear {
  
  MaterialMapFactory::MaterialMapFactory(GearMgr *gearMgr)
  {
    _gearMgr=gearMgr;
  }

  MaterialMap* MaterialMapFactory:: createMaterialMap(double xmin, double xmax, int nxsteps,
					 double ymin, double ymax, int nysteps,
					 double zmin, double zmax, int nzsteps, 
					 int coord)
  {
    double gridA[]={xmin,xmax,nxsteps,ymin,ymax,nysteps,zmin,zmax,nzsteps};
    std::vector<double> gridV;
    gridV.assign(gridA,gridA+9);

    if(_managerMap.find(gridV)==_managerMap.end())
	_managerMap[gridV]=new MaterialMap(_gearMgr,xmin,xmax,nxsteps,ymin,ymax,nysteps,zmin,zmax,nzsteps,coord);

    return _managerMap[gridV];
    
  }
} // namespace gear
