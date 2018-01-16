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

using namespace gear;
using namespace std;

void dumpCoordinates( PadRowLayout2D *layout)
{
    cout << "#--------------------------------" << endl;
    for (int row = 0; row < layout->getNRows() ; row++)
    {
	const std::vector< int > & padsInRow = layout->getPadsInRow (row);
	for ( unsigned int i=0; i < (padsInRow.size() < 10 ? padsInRow.size() : 10 ); i++)
	{
	    Vector2D padCenter = layout->getPadCenter(padsInRow[i]);

	    // reretrieve the padIndex from the layout
	    int padIndex = layout->getNearestPad( padCenter[0] , padCenter[1] );
					  
	    cout << padCenter[0] << "\t" << padCenter[1] << "\t"
		 << padsInRow[i] << "\t" << padIndex << endl;
	}
    }
}

int main(int argc,char* argv[])
{

    if (argc != 2)
    {
        cout << "usage: " << argv[0] << " gearfile.xml" << endl;
        return 1;
    }
//    GearXML *xmlmgr = new GearXML("testFixedAngle.xml");
//    GearXML *xmlmgr = new GearXML("gear_tpc_proto.xml");
    GearXML *xmlmgr = new GearXML(argv[1]);
    GearMgr *mgr = xmlmgr->createGearMgr();

    TPCParameters const & mTPC = mgr->getTPCParameters();
    
    const std::vector<TPCModule *> & moduleVec = mTPC.getModules();

    TPCParametersXML *modularTPCXML =  new TPCParametersXML;
    
    cout << "vector size is "<< moduleVec.size() << endl;

//    for (std::vector<TPCModule *>::const_iterator moduleIter = moduleVec.begin() ;
//	 moduleIter <  moduleVec.end() ; moduleIter++ )
//    {
//	cout << modularTPCXML->toXML(**moduleIter )<<endl;
//    }
    cout << modularTPCXML->toXML(mTPC )<<endl;

    cout << "drift distance is " << mTPC.getMaxDriftLength() << std::endl;
    
    //  TiXmlElement modularTPCElement = modularTPCXML->toXML(*modularTPCRectangular );
    // cout << modularTPCElement<<endl;
    // cout << modularTPCXML->toXML(*modularTPCCircular )<<endl;

    delete xmlmgr; delete mgr; delete modularTPCXML;
}
