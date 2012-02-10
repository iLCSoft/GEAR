#include "gear/GEAR.h"
#include "gearimpl/FTDLayerLayoutImpl.h"
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <sstream>

namespace gear
{
  void FTDLayerLayoutImpl::addLayer(int nPetals, int nSensors, bool isDoubleSided,  int sensorType, double petalOpAngle, double phi0, double alpha, 
		// common for support and sensitive
		double zposition, double zoffset, double zsignpetal0,
		// support
		//double supportZposlayer, 
		double supportRinner, double supportThickness, 
		double supportLengthMin, double supportLengthMax,
		double supportWidth, 
		double supportRadLength,
		// sensitive
		//double sensitiveZposlayer, 
		double sensitiveRinner,	double sensitiveThickness,
		double sensitiveLengthMin, double sensitiveLengthMax,
		double sensitiveWidth, 
		double sensitiveRadLength )
{
	Layer lL, sL ; 
	
	lL.nPetals     = nPetals ;
        lL.nSensors    = nSensors;
        lL.isDoubleSided = isDoubleSided;
	lL.sensorType  = sensorType;
	//lL.phi       = phi ; 
	lL.petalOpenningAngle= petalOpAngle;
	lL.phi0        = phi0; 
	lL.alpha       = alpha;
	lL.zposition   = zposition; //supportZposlayer ;
	lL.zoffset     = zoffset;
	lL.zsign0      = zsignpetal0;
	lL.rInner      = supportRinner;
	lL.lengthMin   = supportLengthMin; 
	lL.lengthMax   = supportLengthMax; 
	lL.thickness   = supportThickness;
	lL.width       = supportWidth ;
	lL.radLength   = supportRadLength ;

	sL.nPetals     = nPetals ;
        sL.nSensors    = nSensors;
        sL.isDoubleSided = isDoubleSided;
	sL.sensorType  = sensorType;
	//sL.phi       = phi ; 
	sL.petalOpenningAngle= petalOpAngle;
	sL.phi0        = phi0; 
	sL.alpha       = alpha;
	sL.zposition   = zposition; //sensitiveZposlayer;
	sL.zoffset     = zoffset;
	sL.zsign0      = zsignpetal0;
	sL.rInner      = sensitiveRinner;
	sL.thickness   = sensitiveThickness;
	sL.lengthMin   = sensitiveLengthMin;
	sL.lengthMax   = sensitiveLengthMax;
	sL.width       = sensitiveWidth ;
	sL.radLength   = sensitiveRadLength ;

	_lVec.push_back( lL ) ;
	_sVec.push_back( sL ) ;

	// Get the references frames defining the trapezoid
	//FIXME: TO BE DEPRECATED
	_eL = getframe( lL );
	_eS = getframe( sL );
}


// Correct the index of the layer in order to return a value < 7
// (layers symmetric in z)
int FTDLayerLayoutImpl::getEquivLayer(const int & layerIndex) const
{
//FIXME: TO DEBUG!!
	int correctedId = -1;
	// Using the index of the vectors 0->2N-1
	if( layerIndex > 6)
	{
		correctedId = layerIndex-getNLayers();
	}
	else
	{
		correctedId = layerIndex;
	}
	
	if( correctedId < 0 )
	{
		std::cout << "The disk layer introduced is not in the right format. Use the description:\n " <<
			"Z-Positives disks: 0,1,...,N-1\n " <<
			"Z-negatives disks: 7,8,...,2N-1\n" << std::endl;
	}

	return correctedId;

}

// Return the Z position defined in the centroid of the sensitive
double FTDLayerLayoutImpl::getSupportZposition(const int & layerIndex, const int & petalIndex) const
{
	int layerId = getEquivLayer(layerIndex);

	Layer petal = _lVec.at(layerId);
	// Extract if the zoffset is positive or negative: id=0 -> zsign0, id=1--> -zsign0, and so on
	int offsetsign = petal.zsign0*(int)pow((double)-1,(double)petalIndex);
	
	double zpos = petal.zposition+offsetsign*petal.zoffset;
	// Taking into account the negative disks
	// RECALL: all the z-position are positives (by definition we used for 
	//         the z-negative disks (-1)*zposition
	int finalsign = 1;
	if( layerId != layerIndex )
	{
		finalsign = -1;
	}
	
	return finalsign*zpos;
}

// Return the Z position defined in the centroid of the sensitive
double FTDLayerLayoutImpl::getSensitiveZposition(const int & layerIndex, const int & petalIndex, 
		const int & sensorIndex) const
{
	double zsupport = getSupportZposition(layerIndex,petalIndex);
	int layerId = getEquivLayer(layerIndex);

	// Extract if the sensor is back to IP or facing the IP
	int zdisksign = (int)(zsupport/fabs(zsupport));
	int sign = -zdisksign; // the direction of the sensor: we start with a value directed to the IP -> if disk is at +z , the direction is - and vice versa
	
	if( ( _lVec.at(layerId).isDoubleSided )&&( sensorIndex > _lVec.at(layerId).nSensors/2 ) ){ // If the sensor is on the back
          
                sign = zdisksign; //the it looks into the other direction
                
        }
	
	if( sensorIndex > _lVec.at(layerId).nSensors || sensorIndex < 1 )
	{
	  std::stringstream ss ;
	  ss << "FTDLayerLayoutImpl::getSensitiveZposition Error!!" 
	     << " The Sensor Index \'" << sensorIndex 
	     << "\' is out of range. There are only sensors from 1 to " << _lVec.at(layerId).nSensors  << " !"
	     << std::endl;

	  throw gear::Exception( ss.str() ) ;
	}
	double petalthickness = _lVec.at(layerId).thickness;
	double sensorthickness = _sVec.at(layerId).thickness;
	double zpos = zsupport+sign*(petalthickness+sensorthickness)/2.0;

	return zpos;
}



// Returns the phi of the petal centroid with respect to the x-axis
double FTDLayerLayoutImpl::getPhiPetalCd(const int & layerIndex, const int & petalIndex) const
{
	int layerId = getEquivLayer(layerIndex);

  	return _lVec.at(layerId).phi0+
		(double)petalIndex*2.0*_lVec.at(layerId).petalOpenningAngle;
}

// Returns the maximum radius for a given layer (No le veo utilidad aun)
double FTDLayerLayoutImpl::getMaxRadius(const int & layerIndex, const bool & sensitive ) const 
{
	Layer l ;
	if(!sensitive) 
	{
		l = _lVec.at( layerIndex );
	}
	else 
	{
		l = _sVec.at( layerIndex );
	}
	
	return l.rInner+l.width;
}


// returns starting phi for first petal in layer (on side to IP)  (No le veo utilidad)
double FTDLayerLayoutImpl::getStartPhi(const int &layerIndex,const int &petalIndex,const bool &sensitive ) const 
{
	Layer l ;
	if ( !sensitive ) 
	{
		l = _lVec.at( layerIndex ) ;
	}
	else {
		l = _sVec.at( layerIndex ) ;
	}

	const double endphi = 0.0; // FIXME: PROV getPhiPetalCd(layerIndex,petalIndex,sensitive);
		
	return endphi-2.0*l.phi0 ;
}

// returns ending phi for first petal in layer (on side to IP). It corresponds with
// the phi where is defined the frame
double FTDLayerLayoutImpl::getEndPhi( const int & layerIndex , const int & petalIndex, const bool & sensitive ) const 
{
	//return getPhiPetalCd(layerIndex,petalIndex,sensitive);
	return 0.0; //FIXME
}


// returns thickness under a certain incident angle
double FTDLayerLayoutImpl::getThicknessForAngle(const int & layerIndex,const double & theta,
		const double & phi,const bool & sensitive ) const
{
	Layer l ;
	if (!sensitive) {
		l = _lVec.at( layerIndex ) ;
	}
	else {
		l = _sVec.at( layerIndex ) ;
	}
	
	double angularThickness ;

	// Nota que falta comprobar:
	//                  -si entra por el petalo
	//                  -cual es la distancia que recorre antes de salir
	//                  -para ello hay que controlar si sale antes de que
	//                   acabe el petalo, en Y es fácil pero en x, al ser
	//                   un petalo, la distancia es variable y dependera
	//                   de la zona Y de salida del rayo
	//                  -controlar y prohibir angulos que no esten en
	//                   theta\in[0,PI/2)   phi\in(0,PI/2]
	angularThickness = l.thickness*sqrt(1.0/(sin(theta)*sin(theta))+tan(phi)*tan(phi));

	return  angularThickness;
	
	// first check if layer is completely out of petal
/*    if( phi < getStartInnerPhi( layerIndex , sensitive ) || phi > getEndInnerPhi( layerIndex, sensitive ) ) {
      return -1 ;
    }

    // check if angle is withhin outer boundaries - easy calculation then
    if( phi >= getStartOuterPhi( layerIndex , sensitive ) && phi <= getEndOuterPhi( layerIndex , sensitive ) ) {
      return ( l.Thickness / cos( phi ) ) ;
    }*/
}    
      
// Return the reference frame defining a petal  (sensitive)
std::vector<vframe> FTDLayerLayoutImpl::getframe( const Layer & l )
{
	// The e1 corresponds directly with x-axis
	vframe e1(1.0, 0.0, 0.0);
	// the e2 corresponds directly with the z-axis with changed sign
	// (this is done in order to keep the positive definition)
	vframe e2(0.0,0.0,-1.0);
	// the e3 corresponds with the y-axis with an inclination angle (sigma)
	// defining the trapezoid
	double sigma = atan( l.width/((l.lengthMax-l.lengthMin)/2.0) );
	vframe e3(0.0,cos(sigma),sin(sigma));

	std::vector<vframe> fr;
	fr.reserve(3);
	fr[0] = e1;
	fr[1] = e2;
	fr[2] = e3;

	return fr;
}



} //namespace
