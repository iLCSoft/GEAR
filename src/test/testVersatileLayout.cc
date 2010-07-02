#include <gearimpl/TPCModuleImpl.h>
#include <gearimpl/RectangularPadRowLayout.h>
#include <gearimpl/FixedPadSizeDiskLayout.h>
#include <gearimpl/FixedPadAngleDiskLayout.h>
#include <gearimpl/VersatileDiskRowLayout.h>
#include <gear/PadRowLayout2D.h>
#include <cmath>
#include <iostream>

using namespace gear;
using namespace std;

void dumpCoordinates( PadRowLayout2D *layout)
{
    cout << "#--------------------------------" << endl;
    for (int row = 0; row < layout->getNRows() ; row++)
    {
	const std::vector< int > & padsInRow = layout->getPadsInRow (row);
//	for ( unsigned int i=0; i < (padsInRow.size() < 10 ? padsInRow.size() : 10 ); i++)
	for ( unsigned int i=0; i < padsInRow.size() ; i++)
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
    VersatileDiskRowLayout *circularPadPlane = new VersatileDiskRowLayout(15.);
    circularPadPlane->addRow( 10 /*nPads*/,
			      2. /*padPitch*/,
			      10. /*rowHeight*/, 
			      0. /*offset*/);
    circularPadPlane->addRow( 10 /*nPads*/,
			      2. /*padPitch*/,
			      10. /*rowHeight*/, 
			      1. /*offset*/,0,0,3);
    circularPadPlane->addRow( 10 /*nPads*/,
			      2. /*padPitch*/,
			      10. /*rowHeight*/, 
			      -1. /*offset*/);
    
    dumpCoordinates( circularPadPlane );

    // get coordinates if pad centre 0


    Vector2D padZeroCentre = circularPadPlane->getPadCenter( 0 );
    cout << endl << "# pad centre: "<< padZeroCentre[0] << "\t" << padZeroCentre[1]<< endl<<endl;

    // now check the distances
    cout << "# r\t phi\t d" << endl;
    for (double phi = -M_PI*0.2; phi < M_PI*0.2; phi +=M_PI*0.005 )
    {
	double r = 10.;
//	cout << r << "\t"<< phi<<"\t"<< circularPadPlane->getDistanceToPad(r, phi, 0)<< endl;
	cout << r << "\t"<< phi<<"\t"<< circularPadPlane->getNearestPad(r, phi)<< endl;
    }

    for (double r = 10.; r < 100.; r+=1.)
    {
	double phi = M_PI*0.2;
//	cout << r << "\t"<< phi<<"\t"<< circularPadPlane->getDistanceToPad(r, phi, 0)<< endl;
	cout << r << "\t"<< phi<<"\t"<< circularPadPlane->getNearestPad(r, phi)<< endl;
    }

    for (double phi = M_PI*0.2; phi > -M_PI*0.2; phi -=M_PI*0.005 )
    {
	double r = 100.;
//	cout << r << "\t"<< phi<<"\t"<< circularPadPlane->getDistanceToPad(r, phi, 0)<< endl;
	cout << r << "\t"<< phi<<"\t"<< circularPadPlane->getNearestPad(r, phi)<< endl;
    }

    for (double r = 100.; r > 10.; r-=1.)
    {
	double phi = -M_PI*0.2;
//	cout << r << "\t"<< phi<<"\t"<< circularPadPlane->getDistanceToPad(r, phi, 0)<< endl;
	cout << r << "\t"<< phi<<"\t"<< circularPadPlane->getNearestPad(r, phi)<< endl;
    }


   delete circularPadPlane;
}
