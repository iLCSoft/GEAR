#ifndef GEAR_TGeoGEOMETRYINITIALIZER_H
#define GEAR_TGeoGEOMETRYINITIALIZER_H 1

#include <string>
#include <vector>

#include "gear/GEAR.h"
#include "TGeoManager.h"

namespace gear {

/** TGeo Geometry Initializer class
 * @author A. Muennich, CERN
 * @version $Id: TGeoGeometryInitializer.h,v 1.1 2006-05-29 13:03:18 musat Exp $
 */
class TGeoGeometryInitializer {

public: 
    /// Destructor.
    virtual ~TGeoGeometryInitializer() { 
      ;}

    static TGeoGeometryInitializer * getTGeoGeometryInitializer(std::string gdmlFile);
    
    TGeoManager* getGeoManager(){return _geoMgr;}
    
private:

    TGeoGeometryInitializer(std::string gdmlFile);

    static TGeoGeometryInitializer *theInitializer;
    TGeoManager *_geoMgr;
}; // class
} // namespace gear
#endif /* ifndef GEAR_TGeoGEOMETRYINITIALIZER_H */
