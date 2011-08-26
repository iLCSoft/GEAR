#ifndef GEAR_TGeoGeometryInitializer_H
#define GEAR_TGeoGeometryInitializer_H 1

#include <string>
#include <vector>

#include "gear/GEAR.h"

//#include "TGeoManager.h"
class TGeoManager;

namespace gear {

/** TGeo Geometry Initializer class
 * @author A. Muennich, CERN
 * @version $Id$
 */
class TGeoGeometryInitializer {

public: 
    /// Destructor.
    virtual ~TGeoGeometryInitializer() { 
      ;}

  //    static TGeoGeometryInitializer * getTGeoGeometryInitializer(std::string gdmlFile);
    
    TGeoManager* getGeoManager(){return _geoMgr;}
    
    TGeoGeometryInitializer(std::string gdmlFile);

private:


  //static TGeoGeometryInitializer *theInitializer;
    TGeoManager *_geoMgr;
}; // class
} // namespace gear
#endif /* ifndef GEAR_TGeoGEOMETRYINITIALIZER_H */
