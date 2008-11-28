#ifndef GEAR_GearXML_H
#define GEAR_GearXML_H 1


#include <string>
#include "gear/GearMgr.h"


namespace gear {

  class GearMgrImpl ;

  /** Implementation of GEAR using XML.
   * 
   * @author F. Gaede, DESY
   * @version $Id: GearXML.h,v 1.2 2005-11-25 16:08:15 gaede Exp $
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
    
  
    /** Write an XML file to disk from the given GearMgr object.
     */
     static void createXMLFile( GearMgr* mgr, const std::string& fileName ) ;


  protected:

    std::string _fileName ;
    GearMgrImpl* _gearMgr ;

  }; // class
  
} // namespace gear

#endif /* ifndef GEAR_GearXML_H */
