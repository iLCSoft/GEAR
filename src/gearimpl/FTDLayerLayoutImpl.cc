#include "gearimpl/FTDLayerLayoutImpl.h"
#include <math.h>

namespace gear
{
void FTDLayerLayoutImpl::addLayer(int nLadders, int sensorType, double phi, double alpha, 
		// common for ladder and sensitive
		double zposlayer, double zoffset,
		// ladder
		double ladderRinner, double ladderThickness, 
		double ladderLengthMin, double ladderLengthMax,
		double ladderWidth, 
		double ladderRadLength,
		// sensitive
		double sensitiveRinner,	double sensitiveThickness,
		double sensitiveLengthMin, double sensitiveLengthMax,
		double sensitiveWidth, 
		double sensitiveRadLength )
{
	Layer lL, sL ; 
	
	lL.nLadders  = nLadders ;
	lL.sensorType= sensorType;
	lL.phi       = phi ; 
	lL.alpha     = alpha;
	lL.zposition = zposlayer;
	lL.zoffset   = zoffset;
	lL.rInner    = ladderRinner;
	lL.lengthMin = ladderLengthMin; 
	lL.lengthMax = ladderLengthMax; 
	lL.thickness = ladderThickness;
	lL.width     = ladderWidth ;
	lL.radLength = ladderRadLength ;

	sL.nLadders  = nLadders ;
	sL.sensorType= sensorType;
	sL.phi       = phi ; 
	sL.alpha     = alpha;
	sL.zposition = zposlayer;
	sL.zoffset   = zoffset;
	sL.rInner    = sensitiveRinner;
	sL.thickness = sensitiveThickness;
	sL.lengthMin = sensitiveLengthMin;
	sL.lengthMax = sensitiveLengthMax;
	sL.width     = sensitiveWidth ;
	sL.radLength = sensitiveRadLength ;

	_lVec.push_back( lL ) ;
	_sVec.push_back( sL ) ;

	// Get the references frames defining the trapezoid
	_eL = getframe( lL );
	_eS = getframe( sL );

}



// Returns the phi correspondent to the point where is defined the 
// reference frame
double FTDLayerLayoutImpl::getPhiStructure(const int & layerIndex, const int & ladderIndex, const bool & sensitive) const
{
	Layer l;
	if( ! sensitive)
	{
		l = _lVec.at( layerIndex );
	}
	else
	{
		l = _sVec.at( layerIndex );
	}
	
	// Displacing to the edge and extract angle
	double phiedge = tan( (l.lengthMin/2.0)/l.rInner );

	return phiedge;
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


// returns starting phi for first ladder in layer (on side to IP)  (No le veo utilidad)
double FTDLayerLayoutImpl::getStartPhi(const int &layerIndex,const int &ladderIndex,const bool &sensitive ) const 
{
	Layer l ;
	if ( !sensitive ) 
	{
		l = _lVec.at( layerIndex ) ;
	}
	else {
		l = _sVec.at( layerIndex ) ;
	}

	const double endphi = getPhiStructure(layerIndex,ladderIndex,sensitive);
		
	return endphi-2.0*l.phi;
}

// returns ending phi for first ladder in layer (on side to IP). It corresponds with
// the phi where is defined the frame
double FTDLayerLayoutImpl::getEndPhi( const int & layerIndex , const int & ladderIndex, const bool & sensitive ) const 
{
	return getPhiStructure(layerIndex,ladderIndex,sensitive);
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
	
	// first check if layer is completely out of ladder
/*    if( phi < getStartInnerPhi( layerIndex , sensitive ) || phi > getEndInnerPhi( layerIndex, sensitive ) ) {
      return -1 ;
    }

    // check if angle is withhin outer boundaries - easy calculation then
    if( phi >= getStartOuterPhi( layerIndex , sensitive ) && phi <= getEndOuterPhi( layerIndex , sensitive ) ) {
      return ( l.Thickness / cos( phi ) ) ;
    }*/
}    
      
// Return the reference frame defining a ladder (sensitive)
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
