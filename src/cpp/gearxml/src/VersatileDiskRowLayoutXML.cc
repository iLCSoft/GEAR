#include "gearxml/VersatileDiskRowLayoutXML.h"

#include "gearxml/XMLHandler.h"
// #include "gearxml/GearParametersXML.h"

#include "gearxml/tinyxml.h"
#include "gearimpl/VersatileDiskRowLayout.h"
#include "gear/GearMgr.h"

// #include <vector>
// #include <algorithm>
// #include <sstream>

namespace gear {
  

  TiXmlElement VersatileDiskRowLayoutXML::toXML( const PadRowLayout2D* layout ) const {

    // Check if type is VersatileDiskRowLayout
    
    const VersatileDiskRowLayout* padLayout=dynamic_cast<const VersatileDiskRowLayout*>(layout);
    
    if (padLayout==NULL) {
      
      throw ParseException("VersatileDiskRowLayoutXML::toXML wrong type !");
    }
    
    // append data to PadRowLayout2D
    TiXmlElement padRowLayout2DXML("PadRowLayout2D");    
    
    padRowLayout2DXML.SetAttribute("type","VersatileDiskRowLayout"); 
    
    padRowLayout2DXML.SetDoubleAttribute("rMin",(padLayout->getPlaneExtent())[0]);
    
    for (std::vector<VersatileDiskRowLayout::Row>::const_iterator rowIter = padLayout->rows().begin();
	 rowIter < padLayout->rows().end(); rowIter ++)
    {
	TiXmlElement rowElement("row");
    
	rowElement.SetAttribute("nPad", rowIter->getNPads() ) ;
	rowElement.SetDoubleAttribute("padPitch", rowIter->getPadPitch() ) ; 
	rowElement.SetDoubleAttribute("padPitch", rowIter->getPadPitch() ) ; 
	rowElement.SetDoubleAttribute("rowHeight", rowIter->getRowHeight() ) ; 
	rowElement.SetDoubleAttribute("padWidth", rowIter->getPadWidth() ) ; 
	rowElement.SetDoubleAttribute("padHeight", rowIter->getPadHeight() ) ;
	rowElement.SetDoubleAttribute("offset", rowIter->getOffset() ) ;

	padRowLayout2DXML.InsertEndChild( rowElement ) ;	
    }
    
    return padRowLayout2DXML ;
  }
    
    
  PadRowLayout2D* VersatileDiskRowLayoutXML::fromXML( const TiXmlElement* xmlLayout ) const {

      double rMin = atof(  getXMLAttribute( xmlLayout , "rMin" ) .c_str() ) ;

      VersatileDiskRowLayout * padLayout = new VersatileDiskRowLayout( rMin );

      const TiXmlNode* xmlRow = 0 ;
      while( ( xmlRow = xmlLayout->IterateChildren( "row", xmlRow ) )  != 0  ){

	  int    nPad      =  atoi(  getXMLAttribute( xmlRow , "nPad" ) .c_str() ) ;
	  double padPitch  =  atof(  getXMLAttribute( xmlRow, "padPitch" ) .c_str() ) ;
	  double rowHeight =  atof(  getXMLAttribute( xmlRow , "rowHeight" ) .c_str() ) ;

	  double padWidth  = atof(  getOptionalXMLAttribute( xmlRow , "padWidtht" , "0.0" ).c_str() ) ;
	  double padHeight = atof(  getOptionalXMLAttribute( xmlRow , "padHeight" , "0.0" ).c_str() ) ;
	  double offset    = atof(  getOptionalXMLAttribute( xmlRow , "offset"    , "0.0" ).c_str() ) ;
	  int    repeat    = atoi(  getOptionalXMLAttribute( xmlRow , "repeat"    , "1" ).c_str() ) ;
	  
	  padLayout->addRow( nPad, padPitch, rowHeight, offset, padWidth, padHeight, repeat );	  
      }

      return padLayout;
  }

}

