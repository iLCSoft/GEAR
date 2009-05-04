#include <gearimpl/TPCModuleImpl.h>
#include <gearimpl/RectangularPadRowLayout.h>
#include <gearimpl/FixedPadSizeDiskLayout.h>
#include <gear/PadRowLayout2D.h>
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

int main()
{
    // create a module with rectangular layout
    RectangularPadRowLayout *rectangularPadPlane = new RectangularPadRowLayout(-10, 10);
    rectangularPadPlane->addRow(3, 10, 2., 5.);

    dumpCoordinates( rectangularPadPlane );

    //
    TPCModuleImpl *rectangularModule1 = new TPCModuleImpl(0, rectangularPadPlane, PadRowLayout2D::CARTESIAN, 40);

    rectangularModule1->setOffset(25. ,20.);
    rectangularModule1->setAngle(M_PI/4);
    dumpCoordinates( rectangularModule1 );

//    TPCModuleImpl *rectangularModule2 = new TPCModuleImpl(0, new RectangularPadRowLayout(*rectangularPadPlane), PadRowLayout2D::POLAR, 40 );
//    dumpCoordinates( rectangularModule2 );
//
//    rectangularModule2->setOffset(25. ,20.);
//    rectangularModule2->setAngle(M_PI/4);
//    dumpCoordinates( rectangularModule2 );
    
    FixedPadSizeDiskLayout *circularPadPlane = new FixedPadSizeDiskLayout(15., 30., 5., 2.);
   

    TPCModuleImpl *circularModule1 = new TPCModuleImpl(0, circularPadPlane, PadRowLayout2D::CARTESIAN, 40);

    circularModule1->setOffset(25. ,20.);
    circularModule1->setAngle(M_PI/4);
    dumpCoordinates( circularModule1 );

    TPCModuleImpl *circularModule2 = new TPCModuleImpl(0, new FixedPadSizeDiskLayout( *circularPadPlane), PadRowLayout2D::POLAR, 40);

    circularModule2->setOffset(25. ,20.);
    circularModule2->setAngle(M_PI/4);
    dumpCoordinates( circularModule2 );

    delete circularModule2; delete circularModule1; delete rectangularModule1; // delete rectangularModule2;
}
