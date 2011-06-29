#include <gearimpl/GearMgrImpl.h>
//#include <gearxml/XMLHandlerMgr.h>
//#include <gearxml/tinyxml.h>
#include <gearxml/GearXML.h>
#include <iostream>

using namespace gear;
using namespace std;

/**Simple test program that reads in a gear XML file and writes out a copy of this file going through
 * the XML interface. The two fiels should be equivalent ( but possibly not identicall).
 */

int main(int argc,char* argv[])
{

    if (argc != 3)
    {
      cout << "usage: " << argv[0] << " gearfile_IN.xml gearfile_OUT.xml " << endl;
        return 1;
    }

    GearXML *xmlmgr = new GearXML( argv[1] );

    GearMgr *mgr = xmlmgr->createGearMgr();

    xmlmgr->createXMLFile (  mgr,  argv[2] ) ;


    //-----

    delete xmlmgr ; 
    delete mgr ; 
}
