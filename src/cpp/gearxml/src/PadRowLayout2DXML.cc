#include "gearxml/PadRowLayout2DXML.h"

#include "gearxml/FixedPadSizeDiskLayoutXML.h"
#include "gearxml/FixedPadAngleDiskLayoutXML.h"
#include "gearxml/RectangularPadRowLayoutXML.h"
#include "gear/PadRowLayout2D.h"

#include <typeinfo>
#include <iostream>

namespace gear {

  
  PadRowLayout2DXML* PadRowLayout2DXML::getHandler( const std::string& typeName ) {

    return PadRowLayout2DXML::map()[ typeName ] ;
  }

  PadRowLayout2DXML* PadRowLayout2DXML::getHandler(const PadRowLayout2D* l ) {
    
    std::string typeName("UNKNOWN") ;
    
    switch(l->getPadLayoutImplType() )
    {
	case  PadRowLayout2D::FIXEDPADSIZEDISKLAYOUT:
	    typeName = "FixedPadSizeDiskLayout" ; break;
	case  PadRowLayout2D::FIXEDPADANGLEDISKLAYOUT:
	    typeName = "FixedPadAngleDiskLayout" ; break;
	case  PadRowLayout2D::RECTANGULARPADROWLAYOUT:
	    typeName = "RectangularPadRowLayout" ; break;
	// The TPCMODULE is not treated by PadRowLayout2DXML, as 
	// PadRowLayout2DXML only treats PadLayouts contained by a module,
	// which cannot be a module itself.
    }

    return getHandler( typeName ) ;
  }
  

  PadRowLayout2DXML::Map& PadRowLayout2DXML::map() {
    
    static  PadRowLayout2DXML::Map theMap ;
    static FixedPadSizeDiskLayoutXML  aFixedPadSizeDiskLayoutXML ;
    static FixedPadAngleDiskLayoutXML  aFixedPadAngleDiskLayoutXML ;
    static RectangularPadRowLayoutXML aRectangularPadRowLayoutXML ;

    if( theMap.size() == 0 ) {
      
      theMap["FixedPadSizeDiskLayout"] = & aFixedPadSizeDiskLayoutXML ;
      theMap["FixedPadAngleDiskLayout"] = & aFixedPadAngleDiskLayoutXML ;
      theMap["RectangularPadRowLayout"] = & aRectangularPadRowLayoutXML ;
    }
    return theMap ;
  }

  
//   std::string PadRowLayout2DXML::typeName( const PadRowLayout2D* l ) {
    
//     if( dynamic_cast<const FixedPadSizeDiskLayout*>(l) != 0 ) return  "FixedPadSizeDiskLayout" ;
//     else  
//       return "UNKNOWN" ;
//   }
}

