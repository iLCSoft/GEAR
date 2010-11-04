#include <string>
#include <vector>

#include "gear/GEAR.h"
#include "geartgeo/TGeoGearDistanceProperties.h"
#include "TVirtualGeoTrack.h"
#include "TGeoNode.h"
#include "TGeoManager.h"

#define MINSTEP 1.e-5

namespace gear {

  TGeoGearDistanceProperties::TGeoGearDistanceProperties(TGeoManager *geoMgr){
    _tgeomanager=geoMgr;
  }
  
  
  void TGeoGearDistanceProperties::beamOn(const Vector3D &p0, const Vector3D &p1) const throw (OutsideGeometryException, std::exception )
  {
    //check if this point was just computetd and the values are still in memory
    if(p0==_p0 && p1==_p1)
      return;
    else{
      _p0=p0;
      _p1=p1;
      _volNames.clear();
      _matNames.clear();
      _distance.clear();
      _intLen.clear();
      _radLen.clear();
    }
    
    double startpoint[3], endpoint[3], direction[3];
    double L=0;
    for(unsigned int i=0; i<3; i++) {
      startpoint[i] = p0[i];
      endpoint[i]   = p1[i];
      direction[i] = endpoint[i] - startpoint[i];
      L+=direction[i]*direction[i];
    }
    
    //normalize direction
    for(unsigned int i=0; i<3; i++)
      direction[i]=direction[i]/sqrt(L);

    _tgeomanager->AddTrack(0,11);
    TGeoNode *node1 = _tgeomanager->InitTrack(startpoint, direction);
    //check if there is a node at startpoint
    if(!node1)
      throw OutsideGeometryException("No geometry node found at given location. Either there is no node placed here or position is outside of top volume.");
    
    while (!_tgeomanager->IsOutside()) 
      {
	TGeoNode *node2;
	TVirtualGeoTrack *track; 
	node2 = _tgeomanager->FindNextBoundaryAndStep(500, 1) ;
	
	if(!node2 || _tgeomanager->IsOutside())
	  break;
	
	double *position =(double*) _tgeomanager->GetCurrentPoint();
	double *previouspos =(double*) _tgeomanager->GetLastPoint();
	double length=_tgeomanager->GetStep();
	track = _tgeomanager->GetLastTrack();
	//protection against infinitive loop in root which should not happen, but well it does...
	//work around until solution within root can be found when the step gets very small e.g. 1e-10
	//and the next boundary is never reached
	
	if(length<MINSTEP)
	  {
	    _tgeomanager->SetCurrentPoint(position[0]+MINSTEP*direction[0], position[1]+MINSTEP*direction[1], position[2]+MINSTEP*direction[2]);
	    length=_tgeomanager->GetStep();
	    node2 = _tgeomanager->FindNextBoundaryAndStep(500, 1) ;
	  }

	//if the next boundary is further than end point
	 if(fabs(position[0])>fabs(endpoint[0]) || fabs(position[1])>fabs(endpoint[1]) 
	 || fabs(position[2])>fabs(endpoint[2]))
	  {
	    length=sqrt(pow(endpoint[0]-previouspos[0],2)
			+pow(endpoint[1]-previouspos[1],2)
			+pow(endpoint[2]-previouspos[2],2));
	
	    track->AddPoint(endpoint[0], endpoint[1], endpoint[2], 0.);
	    _distance.push_back(length);
	    std::string svoln(node1->GetName());
	    std::string svolm(node1->GetMedium()->GetMaterial()->GetName());
	    _volNames.push_back(svoln);
	    _matNames.push_back(svolm);
	    _intLen.push_back(node1->GetMedium()->GetMaterial()->GetIntLen());
	    _radLen.push_back(node1->GetMedium()->GetMaterial()->GetRadLen());
	    break;
	  }
	
	track->AddPoint(position[0], position[1], position[2], 0.);
	_distance.push_back(length);
	std::string svoln(node1->GetName());
	std::string svolm(node1->GetMedium()->GetMaterial()->GetName());
	_volNames.push_back(svoln);
	_matNames.push_back(svolm);
	_intLen.push_back(node1->GetMedium()->GetMaterial()->GetIntLen());
	_radLen.push_back(node1->GetMedium()->GetMaterial()->GetRadLen());
	
	node1=node2;
      }
    _tgeomanager->ClearTracks();
    _tgeomanager->CleanGarbage();
  }

  /** List of matrial names along the distance between [p0,p1] .
   */
  const std::vector<std::string>&  TGeoGearDistanceProperties::getMaterialNames(const Vector3D & p0, const Vector3D & p1) const throw (NotImplementedException, std::exception ) {
    
    //check if itp0 and p1 are the same point, if so don't do anything, no material in between
    if(p0==p1)
      _matNames.clear();
    else
      beamOn(p0, p1);
    
    return _matNames;
  }
  
  /** List of volume names along the distance between [p0,p1] .
   */
  const std::vector<std::string>  TGeoGearDistanceProperties::getVolumeNames(const Vector3D & p0, const Vector3D & p1) const throw (NotImplementedException, std::exception ) {
    
    //check if itp0 and p1 are the same point, if so don't do anything, no material in between
    if(p0==p1)
      _volNames.clear();
    else
      beamOn(p0, p1);
    
    return _volNames;
  }
  
  /** List of matrial thicknesses in mm along the distance between [p0,p1] - runs parallel to the array
   *  returned by  getMaterialNames().
   */
  const std::vector<double>&  TGeoGearDistanceProperties::getMaterialThicknesses(const Vector3D & p0, const Vector3D & p1) const throw (NotImplementedException, std::exception ) {
        
    //check if itp0 and p1 are the same point, if so don't do anything, no material in between
    if(p0==p1)
      _distance.clear();
    else
      beamOn(p0, p1);
    
    return _distance;
  }
  
  /** The number of radiation lengths along the distance between [p0,p1] .
   */
  double TGeoGearDistanceProperties::getNRadlen(const Vector3D & p0, const Vector3D & p1) const throw (NotImplementedException, std::exception ) {
    
     //check if itp0 and p1 are the same point, if so don't do anything, no material in between
    if(p0==p1)
      return 0;

    beamOn(p0, p1);
    
    double nRadLen=0;
    for(unsigned int i=0; i < _radLen.size(); i++){
      if(_radLen[i]!=0)
	nRadLen += _distance[i]/_radLen[i];
    }
    return nRadLen;
  }
  
  
  /** The number of interaction lengths along the distance between [p0,p1] .
   */
  double TGeoGearDistanceProperties::getNIntlen(const Vector3D & p0, const Vector3D & p1) const throw (NotImplementedException, std::exception ) {
    
   if(p0==p1)
      return 0;

     beamOn(p0, p1);
    
    double nInterLen = 0;
    for(unsigned int i=0; i<_intLen.size(); i++)
      {
	if(_intLen[i]!=0)
	  nInterLen += _distance[i]/_intLen[i];
      }
    return nInterLen;
  }
  
  
  /** The integrated magnetic field along  the distance between [p0,p1] in Tesla*mm.  
   */
  double TGeoGearDistanceProperties::getBdL(const Vector3D & p0, const Vector3D & p1) const throw (NotImplementedException, std::exception ) {
    
    throw NotImplementedException("getBdl not implemented yet in TGeoGearPointProperties");
    return 0;
  }
    
    
  
  /** The integrated electric field along  the distance between [p0,p1] in  mVolt.  
   */
  double TGeoGearDistanceProperties::getEdL(const Vector3D & p0, const Vector3D & p1) const throw (NotImplementedException, std::exception ) {
    
    throw NotImplementedException("getEdl not implemented yet in TGeoGearPointProperties");
    return 0;
  }
    

} // namespace gear
