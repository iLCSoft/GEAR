#ifndef GEAR_GearXML_H
#define GEAR_GearXML_H 1


#include <string>
#include "gear/GearMgr.h"


namespace gear {

  class GearMgrImpl ;

  /** Implementation of GEAR using XML.
   * 
   * @author F. Gaede, DESY
   * @version $Id: GearXML.h,v 1.1.1.1 2005-09-13 14:41:03 gaede Exp $
   */
  class GearXML {
    
  public: 

    /** C'tor specifying the file name */
    GearXML( const std::string& fileName )  ;

    virtual ~GearXML() {}
    
    /** Creates an instance of GearMgr from the data given in the XML file.
     * 
     *  @throws ParseException
     */
    virtual GearMgr* createGearMgr() ;
    
  
  protected:
    std::string _fileName ;
    GearMgrImpl* _gearMgr ;


  }; // class
  
} // namespace gear

#endif /* ifndef GEAR_GearXML_H */
