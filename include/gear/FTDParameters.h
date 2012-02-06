// -*- C++ _*_
#ifndef GEAR_FTDPARAMETERS_H
#define GEAR_FTDPARAMETERS_H 1

#include "GEAR.h"
#include "gear/GearParameters.h"

namespace gear 
{

class FTDLayerLayout;

/** Geometry properties of a FTD detector needed for reconstruction code. <br>
 *  <p>The FTD is assumed to consist of a number of disks (layers) located along the Z-axis. 
 *  Each disk consists of a number of trapezoid ladders (petals) that are uniformly distributed
 *  in a circle around the  Z-axis .<br>
 *  The sensitive volumes are rectangular pixels for the disks 1 and 2 covering the petal face
 *  nearest the IP; the remaining disks mount trapezoids covering both faces of the petal (we 
 *  assume 1 sensitive layer per face; in the current implementation there are two).
 *  The sensitive volumes can be placed relative to the (insensitive) ladders.
 *  @see addLayer .</p>
 *  NB: The FTDParameters describe the one half of the detector at the positive z-axis and the other half 
 *  of the detector is built as a reflection in the XY-plane.<br>

 *  @author J. Duarte; IFCA (based from VXDParametersImpl)
 *  @version $Id: 
 */
	
class FTDParameters : virtual public GearParameters
{   
  
public:
    /// Destructor.
    virtual ~FTDParameters() { /* nop */; }

    static const int PIXEL    = 1;
    static const int STRIP    = 2;
   
    static const int FACE    = 1;
    static const int BACK    = 2;

    /** The layer layout in the Vertex */
    virtual const FTDLayerLayout& getFTDLayerLayout() const  = 0;
    
  // here we coiuld add some navigation functions if desirable ....

};

}   // namespace gear
#endif /* ifndef GEAR_FTDPARAMETERS_H */
