#ifndef GEAR_SimpleMaterialXML_H
#define GEAR_SimpleMaterialXML_H 1


namespace gear {

  class TiXmlNode ;
  class TiXmlElement ;
  class SimpleMaterial ;
  class GearMgr ;

  /** XML handler for SimpleMaterial.
   *
   * @author F. Gaede, DESY
   * @version $Id:$
   */
  class SimpleMaterialXML {
    
  public:

    virtual ~SimpleMaterialXML() { /*nop*/ ;} ;
    
    /** Creates an XML node for the given material
     */
    virtual TiXmlElement toXML( const SimpleMaterial& material ) const ;
    
    /** Creates a SimpleMaterial from the XML element (node)
     */
    virtual SimpleMaterial* fromXML( const TiXmlElement* xmlElement, GearMgr* gearMgr=0) const ;


  }; //class

} // namespace

#endif
