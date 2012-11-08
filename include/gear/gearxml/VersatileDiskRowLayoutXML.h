#ifndef GEAR_VersatileDiskRowLayoutXML_H
#define GEAR_VersatileDiskRowLayoutXML_H 1


#include "gearxml/PadRowLayout2DXML.h"

#include <string>
#include <map>

namespace gear {
  
   
  /** XML handler for VersatileDiskRowLayoutXML.
   *
   *  The layout is created row wise, starting from the inner radius,
   *  which is the lower edge of the first pad row. The pad geometry is always described by
   *  \c rowHeight and \c padPitch, which describe the effective pad dimensions.
   *  \c padWidth and \c padHeight are the dimensions of the metal, without the gaps. They are kept
   *  only for backwards compatibility and are irrelevant for the reconstruction.
   *
   *  All values are given im mm. \c padPitch, \c offset (and \c padWidth) are measured in the
   *  middle of the pad row. All offsets are measured with respect to phi=0, which is the zero of the local coordinate phi.
   *  The pads are arranged in positive phi direction, starting at \f$ \phi = offset / r_i\f$  (where \f$ r_i\f$ is the central radius
   *  of row i) for the "right" edge of the first pad (pitch). 
   *
   *  The parameters are visualised below. For visualisation purposes the phi=0 line (local coordinate system) has been rotated.
   *  In GEAR, this rotation with respect to the horizontal axis is the \angle parameter of the module (not shown in the picture because
   *  it is not a parameter of the VersatileDiskRowLayout).
   *
   *  Please note that \c phiMin coincides with phi=0 in this example because 
   *  the first row does not have an offset, and the second offset is positive. This is not necessarily the case.
   *  phiMin reported by the plain extent is \f$ \min( [ offset_i / r_i ] )\f$, phiMax is  \f$ \max( [ (offset_i + nPads_i\cdot padPitch_i) / r_i ] )\f$.
   *  The central angle \c phiModuleCentre = (\c phiMax+ \c phiMin)/2 is only shown for illustration. It is never used or calculated.
   *
   *  Parameters for the layout itself
   *  @param rMin The radius of the inner edge of the first pad row  in mm.
   *
   *  Parameter for the rows
   *  @param nPad Number of pads in the row
   *  @param rowHeight Height of the row in mm
   *  @param padPitch Pitch of the pad in mm, measured at the radius at the pad centre
   *  @param padWidth (optional) Width of the metal of the pad at the central radius in mm. Has to be < \c padPitch. If not given it is \c padPitch.
   *  @param padHeight (optional) Height of the metal of the pad in mm. Has to be < \c rowHeight. If not given it is \c rowHeight.
   *  @param offset (optional) Offset in mm with respect to the alignment axis phi=0. Measured at the central radius of the pad.
   *  @param repeat (optional) Insert \c"repeat" identical rows instead of only one.
   *
   *  \image html ../VersatileDiskRowLayoutXML.png
   *  \image latex ../VersatileDiskRowLayoutXML.eps
   *
   *  The XML syntax (numbers not consistent with the drawing, sorry):
   *  \verbatim
  <!-- Within the module section -->
  <PadRowLayout2D type="VersatileDiskRowLayout" rMin="1439.96">

    <!-- Add the individual rows -->
    <!-- nPad, rowHeight and padPitch are obligatory -->
    <!-- offset (staggering), padWidth, padHeight and repeat are optional -->

    <!-- row 0 -->
    <row nPad="176" padPitch="1.18511" rowHeight="5.36"
     padWidth="1.085106" padHeight="5.26" />

    <!-- rows 1 to 3 -->
    <row nPad="176" padPitch="1.18951" rowHeight="5.36"
     padWidth="1.08951" padHeight="5.26"  offset="0.594754" repeat="3"/>

  </PadRowLayout2D>
   \endverbatim
   * 
   * @author Martin Killenberg, CERN
   * @version $Id$
   */
  class VersatileDiskRowLayoutXML : public PadRowLayout2DXML {
    
  public: 
    
    /** Creates an XML node for the given  PadRowLayout2D.
     */
    virtual TiXmlElement toXML( const PadRowLayout2D* layout ) const ;
    
    
    /** Creates the appropriate PadRowLayout2D subclass from the given
     *  XML element (node) 
     */
    virtual PadRowLayout2D* fromXML( const TiXmlElement* xmlElement) const ;

  }; // class
} // namespace gear

#endif /* ifndef GEAR_VersatileDiskRowLayoutXML_H */
