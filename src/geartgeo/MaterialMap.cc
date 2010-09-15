#include <string>
#include <vector>
#include <math.h>

#include "gear/GEAR.h"
#include "gear/geartgeo/MaterialMap.h"
#include "gear/GearDistanceProperties.h"

namespace gear {

  MaterialMap::MaterialMap(GearMgr *gearMgr,double xmin, double xmax, int nxsteps,
			   double ymin, double ymax, int nysteps,
			   double zmin, double zmax, int nzsteps, int coord)
  {
    //check the conformity of the input with the requested coordinate system
    if(xmax<xmin || ymax<ymin || zmax<zmin)
      throw gear::Exception("MaterialMap constructor: Grid range error (max value is smaller than min)!");   
    if(nxsteps<0 || nysteps<0 || nzsteps<0)
      throw gear::Exception("MaterialMap constructor: Grid step error (number of steps must be positive number)!");   
    switch (coord)
      {
      case MaterialMap::CARTESIAN :
	{}
	break;
      case  MaterialMap::CYLINDRICAL:
	{
	  //r>0,0<phi<2Pi 
	  if(xmin<0 || xmax<0)
	    throw gear::Exception("MaterialMap constructor: Grid range error (r>0 for cylindrical coordinates)!"); 
	  if(ymin<0 || ymin>2*M_PI || ymax<0 || ymax>2*M_PI)
	    throw gear::Exception("MaterialMap constructor: Grid range error (0<phi<2Pi for cylindrical coordinates)!"); 
	}
	break;
      case  MaterialMap::SPHERICAL:
	{
	  //r>0,0<theta<Pi ,0<phi<2Pi 
	  if(xmin<0 || xmax<0)
	    throw gear::Exception("MaterialMap constructor: Grid range error (r>0 for shperical coordinates)!"); 
	  if(ymin<0 || ymin>M_PI || ymax<0 || ymax>M_PI)
	    throw gear::Exception("MaterialMap constructor: Grid range error (0<theta<Pi for spherical coordinates)!"); 
	  if(zmin<0 || zmin>2*M_PI || zmax<0 || zmax>2*M_PI)
	    throw gear::Exception("MaterialMap constructor: Grid range error (0<phi<2Pi for spherical coordinates)!"); 
	}
	break;
      default:
	throw gear::Exception("MaterialMap constructor: unkown coordinate type");   
      }
    
    
    const GearDistanceProperties &gDP=gearMgr->getDistanceProperties();
    _xmin   =xmin    ;   
    _xmax   =xmax    ;
    _nxsteps=nxsteps ;
    _ymin   =ymin    ;
    _ymax   =ymax    ;
    _nysteps=nysteps ;
    _zmin   =zmin    ;
    _zmax   =zmax    ;
    _nzsteps=nzsteps ;  
    
    //setting the sizes of the matrix
    _myMap.resize(nxsteps+1);
    for(int s=0;s<=nxsteps;++s)
      {
 	_myMap[s].resize(nysteps+1);
 	for(int s2=0;s2<=nysteps;++s2)
 	  _myMap[s][s2].resize(nzsteps+1);
      }
   
    if(nxsteps==0)
      _xstep=1;
    else
      _xstep=(xmax-xmin)/nxsteps;
    if(nysteps==0)
      _ystep=1;
    else
      _ystep=(ymax-ymin)/nysteps;
    if(nzsteps==0)
      _zstep=1;
    else
      _zstep=(zmax-zmin)/nzsteps;
    
    Vector3D initial,final;
    //always vertex as reference point
    initial[0]=0;
    initial[1]=0;
    initial[2]=0;
    
    for(double x= xmin;x<=xmax;x+=_xstep)//or r
      {
	for(double y= ymin;y<=ymax;y+=_ystep)// or phi or theta
	  {
	    for(double z= zmin;z<=zmax;z+=_zstep) //or theta
	      {
		//GearDistanceProperties need coordinates in cartesian x,y,z
		switch (coord)
		  {
		  case MaterialMap::CARTESIAN :
		    {
		      final[0]=x;
		      final[1]=y;
		      final[2]=z;
		    }
		    break;
		  case  MaterialMap::CYLINDRICAL:
		    {
		      final[0]=x*cos(y);//r*cos(phi)
		      final[1]=x*sin(y);//r*sin(phi)
		      final[2]=z;
		    }
		    break;
		  case  MaterialMap::SPHERICAL:
		    {
		      final[0]=x*sin(y)*cos(z);//r*sin(theta)*cos(phi)
		      final[1]=x*sin(y)*sin(z);//r*sin(theta)*cos(phi)
		      final[2]=x*cos(y);//r*cos(theta)
		    }
		    break;
		  default:
		    throw gear::Exception("MaterialMap constructor: unkown coordinate type");   
		  }
		if(final==initial)//no need to go to geometry 
		  {
		    _values.first=0;
		    _values.second=0;
		  }
		else
		  {
		    _values.first=gDP.getNRadlen( initial, final);
		    _values.second=gDP.getNIntlen( initial, final);     
		  }
		std::vector<int> pos;
		calculateGridIndex(pos,x,y,z);
		_myMap[pos[0]][pos[1]][pos[2]]=_values;
	      }//z
	  }//y
      }//x
  }

  double MaterialMap::getInteractionLength(double x, double y, double z)const throw (Exception, std::exception )
  {
    //check whether point is in grid using calculateGridIndex
    std::vector<int> pos;
    calculateGridIndex(pos,x,y,z);
    std::pair<double,double> result;
    interpolateOnGrid(result,x,y,z);
    return result.second;
  }

  double MaterialMap::getRadiationLength(double x, double y, double z)const throw (Exception, std::exception )
  {
    //check whether point is in grid using calculateGridIndex
    std::vector<int> pos;
    calculateGridIndex(pos,x,y,z);
    std::pair<double,double> result;
    interpolateOnGrid(result,x,y,z);
    return result.first;
  }

  void MaterialMap::calculateGridIndex(std::vector<int>  & gpos,double x, double y, double z) const throw (Exception, std::exception )
  {
    //check if in map
    if(x+1e-5<_xmin || x-1e-5>_xmax || y+1e-5<_ymin || y-1e-5>_ymax || z+1e-5<_zmin || z-1e-5>_zmax)
      throw Exception("Coordinate not found in MaterialMap!");
    // convert input to grid spacing
    gpos.resize(3);
    gpos[0]=int((x-_xmin)/_xstep+0.5);
    gpos[1]=int((y-_ymin)/_ystep+0.5);
    gpos[2]=int((z-_zmin)/_zstep+0.5);
  }

  void MaterialMap::interpolateOnGrid(std::pair<double,double> &result, double x, double y, double z) const
  {
    //find eight neighbouring grid points
    double px[2],py[2],pz[2];
    px[0]=x-fmod(x-_xmin,_xstep);
    px[1]=px[0]+_xstep;
    py[0]=y-fmod(y-_ymin,_ystep);
    py[1]=py[0]+_ystep;
    pz[0]=z-fmod(z-_zmin,_zstep);
    pz[1]=pz[0]+_zstep;
    //check wether they are within grid
    int xcount=0,ycount=0,zcount=0;
    if(_xmin<=px[0] && px[0]<=_xmax) 
      xcount++;      
    if(_xmin<=px[1] && px[1]<=_xmax)
      xcount++;
    if(_ymin<=py[0] && py[0]<=_ymax)
      ycount++;
    if(_ymin<=py[1] && py[1]<=_ymax)
      ycount++;
    if(_zmin<=pz[0] && pz[0]<=_zmax)
      zcount++;
    if(_zmin<=pz[1] && pz[1]<=_zmax)
      zcount++;
    //calculate distances from x,y,z to each neighbour
    int counter=0;
    double sumIntL=0,sumRadL=0,sumWeight=0;
    bool isOnGrid=false;
    for(int ix=0;ix<xcount;ix++)
      {
	for(int iy=0;iy<ycount;iy++)
	  {
	    for(int iz=0;iz<zcount;iz++)
	      {
		double d=sqrt((x-px[ix])*(x-px[ix])+(y-py[iy])*(y-py[iy])+(z-pz[iz])*(z-pz[iz]));
		std::vector<int>  gpos;
		calculateGridIndex(gpos,px[ix],py[iy],pz[iz]);
		std::pair<double,double> vals=_myMap[gpos[0]][gpos[1]][gpos[2]];
		counter++;
		if(fabs(d)<1e-5)//point in querry is a grid point
		  {
		    sumRadL=vals.first;
		    sumIntL=vals.second;
		    sumWeight=1;
		    isOnGrid=true;
		    break;
		  }
		else if(!isOnGrid)
		  {
		    sumRadL+=vals.first/d;
		    sumIntL+=vals.second/d;
		    sumWeight+=1./d;
		  }
	      }
	    if(isOnGrid)
	      break;
	  }
	if(isOnGrid)
	  break;
      }
    result.first=sumRadL/sumWeight;
    result.second=sumIntL/sumWeight;
  }

} // namespace gear
