#include <string>
#include <vector>

#include "gear/GEAR.h"
#include "geartgeo/TGeoGeometryInitializer.h"
#include "TGDMLParse.h"
#include "TGeoVolume.h"


namespace gear {

/** TGeo Geometry Initializer class
 * @author @author A. Muennich, CERN
 * @version $Id: TGeoGeometryInitializer.cc,v 1.1 2006-05-29 13:03:18 musat Exp $
 */
  TGeoGeometryInitializer * TGeoGeometryInitializer::theInitializer = NULL;
  
  TGeoGeometryInitializer * TGeoGeometryInitializer::getTGeoGeometryInitializer(std::string gdmlFile)
  {
    if(theInitializer == NULL)
      theInitializer = new TGeoGeometryInitializer(gdmlFile);
    return theInitializer;
  }
  
  TGeoGeometryInitializer::TGeoGeometryInitializer(std::string gdmlFile)
  {
    TGDMLParse gdmlparser;
     _geoMgr = new TGeoManager("","");
     TGeoVolume *top=gdmlparser.GDMLReadFile(gdmlFile.c_str());
    _geoMgr=top->GetGeoManager();
    _geoMgr->SetTopVolume(top);
    _geoMgr->CloseGeometry();
  }
  
} // namespace gear
