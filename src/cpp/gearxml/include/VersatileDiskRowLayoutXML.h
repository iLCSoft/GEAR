#ifndef GEAR_VersatileDiskRowLayoutXML_H
#define GEAR_VersatileDiskRowLayoutXML_H 1


#include "gearxml/PadRowLayout2DXML.h"

#include <string>
#include <map>

namespace gear {
  
   
  /** Abstract XML handler for VersatileDiskRowLayoutXML.
   * 
   * @author F. Gaede, DESY
   * @version $Id: VersatileDiskRowLayoutXML.h,v 1.1 2009-02-27 09:00:50 gaede Exp $
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
