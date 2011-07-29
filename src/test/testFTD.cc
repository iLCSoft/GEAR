// _*_ C++ _*_
//
// testFTD.cc : testing the FTD geometry extracted
//              from GEAR
//
// Compilation:  
//              g++ `root-config --cflags` -L`root-config --libdir` \
//                       `root-config --libs` -lGeom 
//                       -I/usr/local/ilcsoft/v01-08/gear/v00-13/src/cpp/include \
//	                 -L/usr/local/ilcsoft/v01-08/gear/v00-13/lib \
//			 -lgear -lgearxml -o testFTD testFTD.cc
//
// @author: J. Duarte Campderros, IFCA
// 27, July 2011

#include "TGeoManager.h"
#include "TGeoMatrix.h"

#include "gearxml/GearXML.h"
#include "gearimpl/GearMgrImpl.h"
#include "gearimpl/FTDParametersImpl.h"

#include<iostream>
#include<cmath>

using namespace gear;

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		std::cout << "usage: " << argv[0] << " gearfile.xml" << std::endl;
		return 1;
	}
	
	// Extracting FTD geometry
	GearXML *xmlmgr = new GearXML(argv[1]);
	GearMgr *mgr = xmlmgr->createGearMgr();
	
	FTDParameters const & ftdParams = mgr->getFTDParameters();



	// Geometry construction ROOT
	TGeoManager * ftdgeom = new TGeoManager("FTD","The FTD sub-detector geometry from GEAR");

	// As we only need visualization, use only air (vacuum)
	// and Si
	TGeoMaterial * vacuum = new TGeoMaterial("vacuum",0.0,0.0,0.0);
	TGeoMaterial * vacuumTrans = new TGeoMaterial("vacuum_trans",0.0,0.0,0.0);
	vacuumTrans->SetTransparency(50);
	TGeoMaterial *si = new TGeoMaterial("si",28.09,14,2.33);

	TGeoMedium * Air = new TGeoMedium("Vacuum",0,vacuum);
	TGeoMedium * AirTrans = new TGeoMedium("Vacuum_Trans",0,vacuumTrans);
	TGeoMedium * Si  = new TGeoMedium("Silicon",1,si);

	// Create volumes
	TGeoVolume *top = ftdgeom->MakeBox("top",Air,12000,12000,12000);
	ftdgeom->SetTopVolume(top);
	ftdgeom->SetTopVisible(0);
	
	// Specular Matrix
	TGeoRotation speRot; 
	speRot.SetAngles(90,180,0);

	// Disks
	const FTDLayerLayout & ftdLayer = ftdParams.getFTDLayerLayout();
	
	const int nLayers = ftdLayer.getNLayers();

	
	double ZstartOuterShell = -1;
	double ZendOuterShell = -1;
	TGeoRotation petalRot;
	for(int i = 0; i < ftdLayer.getNLayers(); i++)
	{
		const double zposition = ftdLayer.getZposition(i);
		const double diskRinn = ftdLayer.getLadderRinner(i);
		const double diskRout = diskRinn+ftdLayer.getLadderWidth(i);
		const double halfthickness = (2.0*ftdLayer.getZoffset(i)+ftdLayer.getLadderThickness(i)+
			ftdLayer.getSensitiveThickness(i))/2.0;
		TGeoVolume * disk = ftdgeom->MakeTube("Air disk",AirTrans,
				diskRinn,
				diskRout,
				halfthickness);
		disk->SetLineColor(kBlue-5);
		disk->SetFillColor(kBlue-5);
		disk->SetAttVisibility(0);
		
		top->AddNode(disk,i+1,new TGeoTranslation(0,0,zposition-halfthickness) );
		TGeoHMatrix hm = speRot*TGeoTranslation(0,0,zposition-halfthickness);
		top->AddNode(disk,-(i+1),new TGeoHMatrix(hm) );
		if( i == 3 )
		{
			ZstartOuterShell = zposition-halfthickness;
		}
		else if( i == 6)
		{
			ZendOuterShell = zposition+halfthickness;
		}
		//Petals Support as placeholders of the sensitives
		const double total_halfthickness = (ftdLayer.getLadderThickness(i)+
			ftdLayer.getSensitiveThickness(i))/2.0;
		const double dy_half = ftdLayer.getLadderWidth(i)/2.0;
		const double dxMin_half = ftdLayer.getLadderLengthMin(i)/2.0;
		const double dxMax_half = ftdLayer.getLadderLengthMax(i)/2.0;
		TGeoVolume * petal = ftdgeom->MakeTrap("Petal Support",AirTrans,
				total_halfthickness,
				0.0,
				0.0,
				dy_half,
				dxMin_half,
				dxMax_half,
				0.0,
				dy_half,
				dxMin_half,
				dxMax_half,
				0.0);
		petal->SetLineColor(kGray-3);
		petal->SetFillColor(kGray-3);
		petal->SetVisibility(0);
		// Silicon
		const double halfthicknessSi = ftdLayer.getSensitiveThickness(i)/2.0;
		const double dy_halfSi = ftdLayer.getSensitiveWidth(i)/2.0;
		const double dxMin_halfSi = ftdLayer.getSensitiveLengthMin(i)/2.0;
		const double dxMax_halfSi = ftdLayer.getSensitiveLengthMax(i)/2.0;
		TGeoVolume * sensor = ftdgeom->MakeTrap("Petal Support",AirTrans,
				halfthicknessSi,
				0.0,
				0.0,
				dy_halfSi,
				dxMin_halfSi,
				dxMax_halfSi,
				0.0,
				dy_halfSi,
				dxMin_halfSi,
				dxMax_halfSi,
				0.0);
		sensor->SetLineColor(kOrange-5);
		sensor->SetFillColor(kOrange-5);

		petal->AddNode(sensor,1,new TGeoTranslation(0,0,ftdLayer.getZoffset(i)-ftdLayer.getLadderThickness(i)
					-halfthicknessSi));
		if( i > 2 )
		{
			petal->AddNode(sensor,2,new TGeoTranslation(0,0,ftdLayer.getZoffset(i)+ftdLayer.getLadderThickness(i)
					+halfthicknessSi));
		}

		
		double phi = ftdLayer.getPhi(i)*180.0/M_PI;
		for(int j=0; j < ftdLayer.getNLadders(i); j++)
		{
			// FIXME: Missing the turbine-blade rotation
			int zsign = pow(-1,j);
			petalRot.SetAngles(2.0*phi*j,0.0,0.0);
			
			TGeoHMatrix hmpetal = petalRot*TGeoTranslation(0.0,diskRinn+dy_half,
					zsign*ftdLayer.getZoffset(i));
			disk->AddNode(petal,j+1,new TGeoHMatrix(hmpetal));
		}
	}

	// Outer Shield: Basado en los discos, superficie de ayuda para
	//               metodos de GEAR
	//const double outerR = ftdParams.getShellOuterRadius();
	//const double half_z = ftdParams.getShellHalfLength();
	const double outerR = ftdLayer.getLadderRinner(ftdLayer.getNLayers()-1)
		+ftdLayer.getLadderWidth(ftdLayer.getNLayers()-1);
	const double half_z= (ZendOuterShell-ZstartOuterShell)/2.0;

	TGeoVolume *outerShield = ftdgeom->MakeTube("outer_shell",AirTrans,
			outerR,
			outerR+1.0,
			half_z
			);
	outerShield->SetLineColor(kBlue-3);
	outerShield->SetFillColor(kBlue-3);
	outerShield->SetAttVisibility(0);
	
	top->AddNodeOverlap(outerShield,1, new TGeoTranslation(0,0,ZstartOuterShell+half_z) );
	TGeoHMatrix hmO = speRot*TGeoTranslation(0,0,ZstartOuterShell+half_z);
	top->AddNodeOverlap(outerShield,2, new TGeoHMatrix(hmO) );
	
	// Inner SHield
	const double innerRMin = ftdLayer.getLadderRinner(0);
	const double innerRMax = ftdLayer.getLadderRinner(ftdLayer.getNLayers()-1);
	const double ZstartInnerShell = ftdLayer.getZposition(0)-(ftdLayer.getLadderThickness(0)+
			ftdLayer.getSensitiveThickness(0))/2.0;
	const double half_zInner= (ZendOuterShell-ZstartInnerShell)/2.0;

	TGeoVolume *innerShield = ftdgeom->MakeCone("inner_shell",AirTrans,
			half_zInner,
			innerRMin-1.0,
			innerRMin,
			innerRMax-1.0,
			innerRMax
			);
	innerShield->SetLineColor(kBlue-3);
	innerShield->SetFillColor(kBlue-3);
	innerShield->SetAttVisibility(0);
	
	top->AddNodeOverlap(innerShield,1, new TGeoTranslation(0,0,ZstartInnerShell+half_zInner) );
	TGeoHMatrix hmI = speRot*TGeoTranslation(0,0,ZstartInnerShell+half_zInner);
	top->AddNodeOverlap(innerShield,2, new TGeoHMatrix(hmI) );

	//close geometry
	ftdgeom->CloseGeometry();
	ftdgeom->Export("testFTD.root");
	
	//delete f;
	delete ftdgeom;

	delete xmlmgr;
}

