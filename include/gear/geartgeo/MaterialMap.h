#ifndef GEAR_MATERIALMAP_H
#define GEAR_MATERIALMAP_H 1

#include <string>
#include <vector>

#include "gear/GEAR.h"
#include "gear/GearMgr.h"

namespace gear {
  
  /** Material map using GearDistanceProperties
   * @author A. Muennich, CERN
   * @version $Id: MaterialMap.h,v 1.1 2006-05-29 13:03:18 musat Exp $
   */
  class MaterialMap {
    
  public: 
     /** Should be called through the MaterialMapFactory!
      *  Creating a material map in memory. Three coordinate systems are supported:
      *	 coord=0 : CARTESIAN (x,y,z)
      *  coord=1 : CYLINDRICAL (r,phi,z) with r>0, 0<phi<2Pi
      *  coord=2 : SPERICAL (r,theta,phi) with r>0, 0<theta<Pi 0<phi<2Pi
      *  With the vector grid containing min and max (the beginning and the end of the map) and the number of steps
      *  that defines the granularity of the map.
      */
    MaterialMap(GearMgr *gearMgr,double xmin, double xmax, int nxsteps,
		double ymin, double ymax, int nysteps,
		double zmin, double zmax, int nzsteps, int coord);
    /// Destructor.
    virtual ~MaterialMap() { 
      ;}
    /** Returns the interaction length from the vertex at (0,0,0) to (x,y,z) using a distance 
     *	wheighted interpolation of the neighbouring grid point values.
     */
    double getInteractionLength(double x, double y, double z) const throw (Exception, std::exception );
    /** Returns the radiation length from the vertex at (0,0,0) to (x,y,z) using a distance 
     *	wheighted interpolation of the neighbouring grid point values.
     */
    double getRadiationLength(double x, double y, double z) const throw (Exception, std::exception );
    /** coord=0 : CARTESIAN (x,y,z)*/
    static const int CARTESIAN = 0 ;
    /**  coord=1 : CYLINDRICAL (r,phi,z) */
    static const int CYLINDRICAL = 1 ;
    /** coord=2 : SPERICAL (r,theta,phi)*/
    static const int SPHERICAL = 2 ;
 
    
  protected:
    double _xmin, _xmax;
    int _nxsteps;
    double _ymin, _ymax; 
    int _nysteps;
    double _zmin, _zmax;
    int _nzsteps;
    double _xstep,_ystep,_zstep;
    std::pair<double,double> _values;
    std::vector< std::vector< std::vector< std::pair<double,double> > > > _myMap;
    void calculateGridIndex(std::vector<int> & gpos,double x, double y, double z) const;
    void interpolateOnGrid(std::pair<double,double> &result, double x, double y, double z) const;
  }; // class
} // namespace gear
#endif /* ifndef  GEAR_MATERIALMAPFACTORY_H */
