#include <string>
#include <vector>
#include <iostream>

#include "gear/GEAR.h"
#include "geartgeo/TGeoGearPointProperties.h"
#include "geartgeo/TGeoGeometryInitializer.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TGeoNode.h"

namespace gear {
  
  TGeoGearPointProperties::TGeoGearPointProperties(TGeoManager *geoMgr){
    _tgeomanager=geoMgr;
  }
  
//   //vec getPosition returns the current position (if ID is known)
  /** The cellID of the the sensitive detector at pos.
   */
  long64 TGeoGearPointProperties::getCellID(const Vector3D & pos) const throw (NotImplementedException, std::exception ) {
    throw NotImplementedException("getCellID not implemented yet in TGeoGearPointProperties");
    return 0;
    
  }
  
  // 	bool getMaterial  (true if defined material, false if problem)
  /** Name of material at pos.  
   */
  const std::string & TGeoGearPointProperties::getMaterialName(const Vector3D & pos) const throw (NotImplementedException, std::exception ) {
  
    double position[3];
    position[0] = pos[0];
    position[1] = pos[1];
    position[2] = pos[2];
    
    static std::string matName("unknown");
    TGeoNode *node=_tgeomanager->FindNode(position[0],position[1],position[2]);
    if(node)
      matName=node->GetMedium()->GetMaterial()->GetName();
    else
      throw OutsideGeometryException("No geometry node found at given location. Either there is no node placed here or position is outside of top volume.");
    return matName;
    
  }
  /** Density in kg/m^3 at pos.  
   */
  double TGeoGearPointProperties::getDensity(const Vector3D & pos) const throw (NotImplementedException, std::exception ) {
    
    double position[3];
    position[0] = pos[0];
    position[1] = pos[1];
    position[2] = pos[2];
    
    double density=0;
    TGeoNode *node=_tgeomanager->FindNode(position[0],position[1],position[2]);
    if(node)
      density=node->GetMedium()->GetMaterial()->GetDensity();
    else
      throw OutsideGeometryException("No geometry node found at given location. Either there is no node placed here or position is outside of top volume.");
    return density;
    
  }
  double TGeoGearPointProperties::getTemperature(const Vector3D & pos) const throw (NotImplementedException, std::exception ) {
    
    double position[3];
    position[0] = pos[0];
    position[1] = pos[1];
    position[2] = pos[2];

    double temperature=0;
    TGeoNode *node=_tgeomanager->FindNode(position[0],position[1],position[2]);
    if(node)
      temperature=node->GetMedium()->GetMaterial()->GetTemperature();
    else
      throw OutsideGeometryException("No geometry node found at given location. Either there is no node placed here or position is outside of top volume.");
    return temperature;
  }
  
  /** Pressure in P at pos.  
   */
  double TGeoGearPointProperties::getPressure(const Vector3D & pos) const throw (NotImplementedException, std::exception ) {
    
    double position[3];
    position[0] = pos[0];
    position[1] = pos[1];
    position[2] = pos[2];
    
    double pressure=0;
    TGeoNode *node=_tgeomanager->FindNode(position[0],position[1],position[2]);
    if(node)
      pressure=node->GetMedium()->GetMaterial()->GetPressure();
    else
      throw OutsideGeometryException("No geometry node found at given location. Either there is no node placed here or position is outside of top volume.");
    return pressure;
    
  }
  
  /** Radiation length of material in mm at pos.  
   */
  double TGeoGearPointProperties::getRadlen(const Vector3D & pos) const throw (NotImplementedException, std::exception ) {
    
    double position[3];
    position[0] = pos[0];
    position[1] = pos[1];
    position[2] = pos[2];
    
    double radLen=0;
    TGeoNode *node=_tgeomanager->FindNode(position[0],position[1],position[2]);
    if(node)
      radLen=node->GetMedium()->GetMaterial()->GetRadLen();
    else
      throw OutsideGeometryException("No geometry node found at given location. Either there is no node placed here or position is outside of top volume.");
    return radLen;
  }
  
  /** Interaction length of material in mm at pos.  
   */
  double TGeoGearPointProperties::getIntlen(const Vector3D & pos) const throw (NotImplementedException, std::exception ) {
    
    double position[3];
    position[0] = pos[0];
    position[1] = pos[1];
    position[2] = pos[2];
    
    double intLen=0;
    TGeoNode *node=_tgeomanager->FindNode(position[0],position[1],position[2]);
    if(node)
      intLen=node->GetMedium()->GetMaterial()->GetIntLen();
    else
      throw OutsideGeometryException("No geometry node found at given location. Either there is no node placed here or position is outside of top volume.");
    return intLen;
  }
  
  /** Position in local coordinate 
   */
  Vector3D TGeoGearPointProperties::getLocalPosition(const Vector3D & pos) const throw (NotImplementedException, std::exception ) {
    
    const Double_t global[3]={pos[0],pos[1],pos[2]};
    Double_t local[3];

    _tgeomanager->MasterToLocal(global, local);
    
    return Vector3D(local[0],local[1],local[2]) ;
    
  }
  
  /** The magnetic field vector at pos in [Tesla].  
   */
  Vector3D TGeoGearPointProperties::getB(const Vector3D & pos) const throw (NotImplementedException, std::exception ) {
      throw NotImplementedException("getB not implemented yet in TGeoGearPointProperties");
      return Vector3D() ; 
  }
  
  /** The electric field vector at pos in [V/m].  
   */
  Vector3D TGeoGearPointProperties::getE(const Vector3D & pos) const throw (NotImplementedException, std::exception ) {
     throw NotImplementedException("getE not implemented yet in TGeoGearPointProperties");
     return Vector3D();
     
  }
  
  /** Names of (geant4) logical volumes in heirarchy starting at given pos ending with the world volume. 
   */
  std::vector<std::string>  TGeoGearPointProperties::getListOfLogicalVolumes(const Vector3D & pos) const throw (NotImplementedException, std::exception ) {
    
    double position[3];
    position[0] = pos[0];
    position[1] = pos[1];
    position[2] = pos[2];
    std::vector<std::string> names;
    TGeoNode *node= _tgeomanager->FindNode(position[0],position[1],position[2]);
    if(!node)
      throw OutsideGeometryException("No geometry node found at given location. Either there is no node placed here or position is outside of top volume.");
    
    TGeoVolume *cvol=_tgeomanager->GetCurrentVolume();
    int count=0;
    while(cvol)
      {
	const char *name=cvol->GetName();
	names.push_back(name);
	_tgeomanager->CdUp();
	cvol=_tgeomanager->GetCurrentVolume();
	if(name==_tgeomanager->GetTopVolume()->GetName())
	  break;
	count++;
      }
    return names;
  }
    /** Names of (geant4) physical volumes in heirarchy starting at given pos ending with the world volume. 
     */
  std::vector<std::string>  TGeoGearPointProperties::getListOfPhysicalVolumes(const Vector3D & pos) const throw (NotImplementedException, std::exception ) {
    
    double position[3];
    position[0] = pos[0];
    position[1] = pos[1];
    position[2] = pos[2];
    
    std::vector<std::string> names;
    TGeoNode *node=_tgeomanager->FindNode(position[0],position[1],position[2]);
    if(!node)
      throw OutsideGeometryException("No geometry node found at given location. Either there is no node placed here or position is outside of top volume.");
    
    int count=0;
    while(node)
      {
	const char *name=node->GetName();
	names.push_back(name);
	_tgeomanager->CdUp();
	node=_tgeomanager->GetCurrentNode();
	if(name==_tgeomanager->GetTopNode()->GetName())
	  break;
	count++;
      }
    return names;
  }
 
 /** Names of (geant4) region that contains the given pos.
     */
  std::string TGeoGearPointProperties::getRegion(const Vector3D & pos) const throw (NotImplementedException, std::exception ){
    throw NotImplementedException("getRegion not implemented yet in TGeoGearPointProperties");
    return "";
  }
  /** True if region that contains pos is defined as a tracker.
   */
  bool TGeoGearPointProperties::isTracker(const Vector3D & pos) const throw (NotImplementedException, std::exception ){
    throw NotImplementedException("isTracker not implemented yet in TGeoGearPointProperties");
    return 0;
  }
  /** True if region that contains pos is defined as a calorimeter.
   */
  bool TGeoGearPointProperties::isCalorimeter(const Vector3D & pos) const throw (NotImplementedException, std::exception ){
    throw NotImplementedException("isCalorimeter not implemented yet in TGeoGearPointProperties");
    return 0;
     
  }
  
} // namespace gear
