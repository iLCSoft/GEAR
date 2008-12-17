#include <gearimpl/TPCModuleImpl.h>
#include <gearimpl/RectangularPadRowLayout.h>
#include <gearimpl/FixedPadSizeDiskLayout.h>
#include <gearimpl/GearMgrImpl.h>
#include <gearimpl/TPCParametersImpl.h>
#include <gear/PadRowLayout2D.h>
#include <gearxml/XMLHandlerMgr.h>
#include <gearxml/TPCParametersXML.h>
#include <gearxml/tinyxml.h>
#include <gearxml/GearXML.h>
#include <iostream>
#include <cmath>
#include <complex>

using namespace gear;
using namespace std;

void dumpCoordinates( PadRowLayout2D *layout)
{
    cout << "#--------------------------------" << endl;
    for (int row = 0; row < layout->getNRows() ; row++)
    {
	const std::vector< int > & padsInRow = layout->getPadsInRow (row);
//	for (unsigned int i=0; i < (padsInRow.size() < 10 ? padsInRow.size() : 10 ); i++)
	for (unsigned int i=0; i < padsInRow.size() ; i++)
	{
	    Vector2D padCenter = layout->getPadCenter(padsInRow[i]);
					  
	    cout << padCenter[0] << "\t" << padCenter[1] << endl;
	}
    }
}

void dumpPlaneExtent(TPCModule const * module)
{

    // now the local plane extent
    PadRowLayout2D const & localPadPlane = module->getLocalPadLayout();
    // define step in y/phi
    // one degree for phi
    // one mm for y
    double local_y_phi_step;

    switch (localPadPlane.getCoordinateType())
    {
	case PadRowLayout2D::POLAR :
	{
	    local_y_phi_step = M_PI/180.;
	}
	break;
	case  PadRowLayout2D::CARTESIAN :
	{
	    local_y_phi_step = 1.;
	}
	break;
	default:
	    throw gear::Exception("dumpPlaneExtent: unkown coorinate type");	
    }

    for (double y_phi = localPadPlane.getPlaneExtent()[2]; y_phi <  localPadPlane.getPlaneExtent()[3] ; y_phi += local_y_phi_step)
    {
	gear::Vector2D globalCoords = module->localToGlobal(localPadPlane.getPlaneExtent()[0] , y_phi);
	cout << "0\t 0\t 0\t 0\t"<<globalCoords[0] << "\t"<< globalCoords[1] <<endl;
    }

    for (double r =  localPadPlane.getPlaneExtent()[0];  r <  localPadPlane.getPlaneExtent()[1]; r +=1)
    {
	gear::Vector2D globalCoords = module->localToGlobal(r, localPadPlane.getPlaneExtent()[3]);
	cout << "0\t 0\t 0\t 0\t"<<globalCoords[0] << "\t"<< globalCoords[1] <<endl;
    }

   // scan y_phi in 5 degree steps
    for (double y_phi = localPadPlane.getPlaneExtent()[3]; y_phi >  localPadPlane.getPlaneExtent()[2] ; y_phi -= local_y_phi_step)
    {
	gear::Vector2D globalCoords = module->localToGlobal(localPadPlane.getPlaneExtent()[1] , y_phi);
	cout << "0\t 0\t 0\t 0\t"<<globalCoords[0] << "\t"<< globalCoords[1] <<endl;
    }

    for (double r =  localPadPlane.getPlaneExtent()[1];  r >  localPadPlane.getPlaneExtent()[0]; r -=1)
    {
	gear::Vector2D globalCoords = module->localToGlobal(r, localPadPlane.getPlaneExtent()[2]);
	cout << "0\t 0\t 0\t 0\t"<<globalCoords[0] << "\t"<< globalCoords[1] <<endl;
    }
}

void dumpModuleExtentDistances(TPCModule const * module)
{
    // define step in y/phi
    // one degree for phi
    // one mm for y
    double y_phi_step;

    switch (module->getTPCCoordinateType())
    {
	case PadRowLayout2D::POLAR :
	{
	    y_phi_step = M_PI/180.;
	}
	break;
	case  PadRowLayout2D::CARTESIAN :
	{
	    y_phi_step = 1.;
	}
	break;
	default:
	    throw gear::Exception("dumpPlaneExtent: unkown coorinate type");	
    }
    

     // scan y_phi in 5 degree steps
    for (double y_phi = module->getModuleExtent()[2]; y_phi <  module->getModuleExtent()[3] ; y_phi += y_phi_step)
	cout << module->getModuleExtent()[0] << "\t"<<y_phi 
	     << "\t 0\t 0\t 0\t 0\t"<< module->getDistanceToModule( module->getModuleExtent()[0] , y_phi)
	    <<endl;


    for (double r =  module->getModuleExtent()[0];  r <  module->getModuleExtent()[1]; r +=1)
	cout << r << "\t"<< module->getModuleExtent()[3] 
	     << "\t 0\t 0\t 0\t 0\t"<< module->getDistanceToModule( r , module->getModuleExtent()[3])
	     <<endl;

   // scan y_phi in 5 degree steps
    for (double y_phi = module->getModuleExtent()[3]; y_phi >  module->getModuleExtent()[2] ; y_phi -= y_phi_step)
	cout <<  module->getModuleExtent()[1] << "\t"<<y_phi 
	     << "\t 0\t 0\t 0\t 0\t"<< module->getDistanceToModule( module->getModuleExtent()[1] , y_phi)
	     <<endl;

    for (double r =  module->getModuleExtent()[1];  r >  module->getModuleExtent()[0]; r -=1)
	cout << r << "\t"<< module->getModuleExtent()[2] 
	     << "\t 0\t 0\t 0\t 0\t"<< module->getDistanceToModule( r , module->getModuleExtent()[2])
	     <<endl;

    // now the local plane extent
    PadRowLayout2D const & localPadPlane = module->getLocalPadLayout();
    // define step in y/phi
    // one degree for phi
    // one mm for y
    double local_y_phi_step;

    switch (localPadPlane.getCoordinateType())
    {
	case PadRowLayout2D::POLAR :
	{
	    local_y_phi_step = M_PI/180.;
	}
	break;
	case  PadRowLayout2D::CARTESIAN :
	{
	    local_y_phi_step = 1.;
	}
	break;
	default:
	    throw gear::Exception("dumpModuleExtent: unkown coorinate type");	
    }

    for (double y_phi = module->getLocalModuleExtent()[2]; y_phi <  module->getLocalModuleExtent()[3] ; y_phi += local_y_phi_step)
    {
	gear::Vector2D globalCoords = module->localToGlobal(module->getLocalModuleExtent()[0] , y_phi);
	cout << "0\t 0\t"<<globalCoords[0] << "\t"<< globalCoords[1] << "\t 0\t 0\t 0"<<endl;
    }

    for (double r =  module->getLocalModuleExtent()[0];  r <  module->getLocalModuleExtent()[1]; r +=1)
    {
	gear::Vector2D globalCoords = module->localToGlobal(r, module->getLocalModuleExtent()[3]);
	cout << "0\t 0\t"<<globalCoords[0] << "\t"<< globalCoords[1] << "\t 0\t 0\t 0"<<endl;
    }

   // scan y_phi in 5 degree steps
    for (double y_phi = module->getLocalModuleExtent()[3]; y_phi >  module->getLocalModuleExtent()[2] ; y_phi -= local_y_phi_step)
    {
	gear::Vector2D globalCoords = module->localToGlobal(module->getLocalModuleExtent()[1] , y_phi);
	cout << "0\t 0\t"<<globalCoords[0] << "\t"<< globalCoords[1] << "\t 0\t 0\t 0"<<endl;
    }

    for (double r =  module->getLocalModuleExtent()[1];  r >  module->getLocalModuleExtent()[0]; r -=1)
    {
	gear::Vector2D globalCoords = module->localToGlobal(r, module->getLocalModuleExtent()[2]);
	cout << "0\t 0\t"<<globalCoords[0] << "\t"<< globalCoords[1] 
	     << "\t 0\t 0\t 0"<<endl;
    }
}

void dumpBox(TPCModule const * module)
{
    for (double x =  -100; x < 100; x+=2)
    {
	cout << x<<"\t"<<100<<"\t 0\t 0\t 0 \t 0\t"
	     << module->getDistanceToModule( x,100 )
	     <<endl;
	cout << x<<"\t"<<-100<<"\t 0\t 0\t 0 \t 0\t"
	     << module->getDistanceToModule( x,-100 )
	     <<endl;
     }    

    for (double y =  -100; y < 100; y+=2)
    {
	cout << 100<<"\t"<<y<<"\t 0\t 0\t 0 \t 0\t"
	     << module->getDistanceToModule( 100,y )
	     <<endl;
	cout << -100<<"\t"<<y<<"\t 0\t 0\t 0 \t 0\t"
	     << module->getDistanceToModule( -100,y )
	     <<endl;
    }
}

int main(int argc,char* argv[] )
{
    if (argc != 2)
    {
	cout << "usage: testDistanceToModue gearfile.xml" << endl;
	return 1;
    }

    GearXML *xmlmgr = new GearXML(argv[1]);
    GearMgr *mgr = xmlmgr->createGearMgr();

    TPCParameters const & mTPC = mgr->getTPCParameters();
    
    const std::vector<TPCModule *> & moduleVec = mTPC.getModules();

    for (std::vector<TPCModule *>::const_iterator moduleIter = moduleVec.begin();
	 moduleIter < moduleVec.end(); moduleIter++)
    {
	dumpModuleExtentDistances(*moduleIter);
	dumpPlaneExtent(*moduleIter);
	dumpCoordinates(*moduleIter);
	dumpBox(*moduleIter);
    }
 
    delete xmlmgr; delete mgr;
}
