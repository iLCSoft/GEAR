// -*- C++ -*-
#ifndef GEAR_TelPlanesLayerLayoutImpl_H
#define GEAR_TelPlanesLayerLayoutImpl_H 1

#include "gear/TelPlanesLayerLayout.h"
#include <vector>
#include <iostream>

namespace gear {

/** Abstract description of layers in pixel beam telescope. <br>
 *  This assumes a number of silicon layers arranged perpendicular to the beam.
 * 
 * @author I. Rubinskiy,  DESY
 * @version $Id: 
 */


// basic scattering only layer
class TelPlanesMaterialLayerImpl: public TelPlanesMaterialLayer  {

    private:
	
    /**      */
    int ID ;	
    /**      */
    std::string info;
    /**      */
    double PositionX ;
    /**      */
    double PositionY ;
    /**      */
    double PositionZ ;
    /**      */
    double RotationXY;
    /**      */
    double RotationZX;
    /**      */
    double RotationZY;
    /**      */
    double SizeX ;
    /**      */
    double SizeY ;
    /**      */
    double Thickness ;
    /**      */
    double RadLength ;
    
    public:
     /** safe default contructor **/
    TelPlanesMaterialLayerImpl() : ID(0), info(""), PositionX(0.), PositionY(0.), PositionZ(0.),  RotationXY(0.), RotationZX(0.), RotationZY(0.), SizeX(0.), SizeY(0.), Thickness(0.), RadLength(0.) { ; } ;

    /// Destructor.
    /**      */
    virtual ~TelPlanesMaterialLayerImpl() { /* nop */; }
    /**      */
    virtual int getID() const { return ID  ; }
    /**      */
    virtual std::string getInfo() const { return info ; }
    /**      */
    virtual double getPositionX() const { return PositionX  ; }
    /**      */
    virtual double getPositionY() const { return PositionY  ; }
    /**      */
    virtual double getPositionZ() const { return PositionZ  ; }
    /**      */
    virtual double getRotationXY() const { return RotationXY  ; }
    /**      */
    virtual double getRotationZX() const { return RotationZX  ; }
    /**      */
    virtual double getRotationZY() const { return RotationZY  ; }
    /**      */
    virtual double getSizeX() const { return SizeX  ; }
    /**      */
    virtual double getSizeY() const { return SizeY  ; }
    /**      */
    virtual double getThickness() const { return Thickness  ; }
    /**      */
    virtual double getRadLength() const { return RadLength  ; }	 
 
    // add "set" methods:
    /**      */
    virtual void setID( int value )           {  ID = value         ; }  
    /**      */
    virtual void setInfo( std::string value) { info = value ; }
    /**      */
    virtual void setRadLength( double value)  {  RadLength = value  ; }
    /**      */
    virtual void setPositionX( double value)  {  PositionX = value  ; }
    /**      */
    virtual void setPositionY( double value)  {  PositionY = value  ; }
    /**      */
    virtual void setPositionZ( double value)  {  PositionZ = value  ; }
    /**      */
    virtual void setRotationXY( double value) {  RotationXY = value ; }
    /**      */
    virtual void setRotationZX( double value) {  RotationZX = value ; }
    /**      */
    virtual void setRotationZY( double value) {  RotationZY = value ; }
    /**      */
    virtual void setSizeX( double value)      {  SizeX = value      ; }
    /**      */
    virtual void setSizeY( double value)      {  SizeY = value      ; }
    /**      */
    virtual void setThickness( double value)  {  Thickness = value  ; }


};

//basic scattering and sensitve layer
class TelPlanesSensitiveLayerImpl: public TelPlanesSensitiveLayer   {

    private:   
   
    /**      */
    int ID ;
    /**      */
    std::string info;
    /**      */
    double PositionX ;
    /**      */
    double PositionY ;
    /**      */
    double PositionZ ;
    /**      */
    double RotationXY;
    /**      */
    double RotationZX;
    /**      */
    double RotationZY;
    /**      */
    double SizeX ;
    /**      */
    double SizeY ;
    /**      */
    double Thickness ;
    /**      */
    double RadLength ;

    /**      */
    int NpixelX;
    /**      */
    int NpixelY;
    /**      */
    double PitchX;
    /**      */
    double PitchY;
    /**      */
    double ResolutionX;
    /**      */
    double ResolutionY;

    public:
    /** safe default contructor **/
    TelPlanesSensitiveLayerImpl() : ID(0), info(""), PositionX(0.), PositionY(0.), PositionZ(0.), RotationXY(0.), RotationZX(0.), RotationZY(0.),  SizeX(0.), SizeY(0.), Thickness(0.), RadLength(0.) 
                                  , NpixelX(0), NpixelY(0), PitchX(0.), PitchY(0.), ResolutionX(0.), ResolutionY(0.)
     { ; } ;

    /// Destructor.
    virtual ~TelPlanesSensitiveLayerImpl() { /* nop */; }

    /**      */
    virtual int getID() const { return ID  ; }
    /**      */
    virtual std::string getInfo() const { return info ; }
    /**      */
    virtual double getPositionX() const { return PositionX  ; }
    /**      */
    virtual double getPositionY() const { return PositionY  ; }
    /**      */
    virtual double getPositionZ() const { return PositionZ  ; }
    /**      */
    virtual double getRotationXY() const { return RotationXY  ; }
    /**      */
    virtual double getRotationZX() const { return RotationZX  ; }
    /**      */
    virtual double getRotationZY() const { return RotationZY  ; }
    /**      */
    virtual double getSizeX() const { return SizeX  ; }
    /**      */
    virtual double getSizeY() const { return SizeY  ; }
    /**      */
    virtual double getThickness() const { return Thickness  ; }
    /**      */
    virtual double getRadLength() const { return RadLength  ; }

    /**      */
    virtual int getNpixelX() const { return NpixelX  ; }
    /**      */
    virtual int getNpixelY() const { return NpixelY  ; }
    /**      */
    virtual double getPitchX() const { return PitchX  ; }
    /**      */
    virtual double getPitchY() const { return PitchY  ; }
    /**      */
    virtual double getResolutionX() const { return ResolutionX  ; }
    /**      */
    virtual double getResolutionY() const { return ResolutionY  ; }

    /**      */
    virtual void setID( int value )           {  ID = value         ; }  
    /**      */
    virtual void setInfo( std::string value) { info = value ; }
    /**      */
    virtual void setRadLength( double value)  {  RadLength = value  ; }
    /**      */
    virtual void setPositionX( double value)  {  PositionX = value  ; }
    /**      */
    virtual void setPositionY( double value)  {  PositionY = value  ; }
    /**      */
    virtual void setPositionZ( double value)  {  PositionZ = value  ; }
    /**      */
    virtual void setRotationXY( double value) {  RotationXY = value ; }
    /**      */
    virtual void setRotationZX( double value) {  RotationZX = value ; }
    /**      */
    virtual void setRotationZY( double value) {  RotationZY = value ; }
    /**      */
    virtual void setSizeX( double value)      {  SizeX = value      ; }
    /**      */
    virtual void setSizeY( double value)      {  SizeY = value      ; }
    /**      */
    virtual void setThickness( double value)  {  Thickness = value  ; }
    /**      */

    /**      */
    virtual void setNpixelX( int value )        {  NpixelX = value ; }
    /**      */
    virtual void setNpixelY( double value )     {  NpixelY = value ; }
    /**      */
    virtual void setPitchX( double value )      {  PitchX  = value ; }
    /**      */
    virtual void setPitchY( double value )      {  PitchY  = value ; }
    /**      */
    virtual void setResolutionX( double value ) {  ResolutionX = value ; }
    /**      */
    virtual void setResolutionY( double value ) {  ResolutionY = value ; }

};

  typedef std::vector<TelPlanesMaterialLayerImpl> TelPlanesMaterialLayerImplVec ;
  typedef std::vector<TelPlanesSensitiveLayerImpl> TelPlanesSensitiveLayerImplVec ;


// a detector element consisting of a vector of scatterrers and sensors
class TelPlanesLayerImpl: public TelPlanesLayer  {

  private:

    /**      */
    int ID;

    /**      */
    std::string info;

    /** material Layer vector */
    TelPlanesMaterialLayerImplVec _materialVec ;

    /** sensitive Layer vector */
    TelPlanesSensitiveLayerImplVec _sensitiveVec ;

  public:

    /** default constructor */
    TelPlanesLayerImpl() : ID(0), info("")
    { ; };

    /** normal constructor */
    TelPlanesLayerImpl( TelPlanesMaterialLayerImplVec& materials, TelPlanesSensitiveLayerImplVec& sensors) : ID(0), info("")
    {
       //          
       _materialVec.clear(); 
       for(unsigned int i=0; i< materials.size(); i++){
          _materialVec.push_back( materials.at(i) );
       }
  
       //
       _sensitiveVec.clear();
       for(unsigned int i=0; i< sensors.size(); i++){
          _sensitiveVec.push_back( sensors.at(i) );
       }
    }

    /** Destructor.
     */
    virtual ~TelPlanesLayerImpl() { /* nop */; }

    /** get methods
     */
    virtual int getID() const { return ID; }
 
    /** get methods
     */
    virtual std::string getInfo() const { return info ; }

    /** set methods
     */
    virtual void setID(int value)  { ID = value; }

    /** get methods
     */
    virtual void setInfo( std::string value) { info = value ; }

    /** The total number of nonSense layers. */
    virtual int getNMaterialLayers() const { return _materialVec.size(); }

    /** The total number of Sensitive layers. */
    virtual int getNSensitiveLayers() const {return _sensitiveVec.size(); }

    /** get methods
     */
    TelPlanesMaterialLayerImplVec& getMaterialLayerVec(){ return _materialVec;}

    /** get methods
     */
    TelPlanesSensitiveLayerImplVec& getSensitiveLayerVec(){ return _sensitiveVec;}

    /** add methods
     */
    virtual void addMaterialLayer(
			      int ID, std::string info,	 
			      double PositionX, double PositionY, double PositionZ,
			      double RotationXY, double RotationZX, double RotationZY,
			      double SizeX, double SizeY, double Thickness, double RadLength
                              );
  

    /** add methods
     */
    virtual void addSensitiveLayer(
			      int ID, std::string info,
			      double PositionX, double PositionY, double PositionZ,
			      double RotationXY, double RotationZX, double RotationZY,
			      double SizeX, double SizeY, double Thickness, double RadLength,
			      int NpixelX, int NpixelY,
			      double PitchX,double PitchY, double ResolutionX, double ResolutionY
                              ); 

};

/** */
typedef std::vector<TelPlanesLayerImpl> TelPlanesLayerImplVec ;

/**  a collection of layers = telescope description */
class TelPlanesLayerLayoutImpl : public TelPlanesLayerLayout {

	protected:

	  /** Layer */
	  typedef double MyMatrix[2][2];
	  
	private:

          /** */
          int ID;

	  /**      */
          std::string info;

	  /** Layer */
	  TelPlanesLayerImplVec _layerVec ;

	public: 

          /** */
	  TelPlanesLayerLayoutImpl() {
	    _layerVec.clear();
	  };

          /** */
	  virtual void addLayer( TelPlanesLayerImpl* layer ){
	    _layerVec.push_back( *layer ) ;
	  }

	  /**  Destructor */
	  virtual ~TelPlanesLayerLayoutImpl() { /* nop */; }
		
          /** */
          virtual int getID() const { return 0; }

          /** */
          virtual void setID(int value) { ID = value; }
 
          /** */
          virtual std::string getInfo() const { return info; }
 
          /** */
          virtual void setInfo(std::string value) { info = value; }
 
	  /** */
	  virtual int getNLayers() const { 
            return _layerVec.size() ; 
          }	 

          /** */
	  virtual const TelPlanesLayerImpl* getLayer( int ID ) const { 

//            TelPlanesLayerImplVec::const_iterator it = _layerVec.begin(); 

            for( TelPlanesLayerImplVec::const_iterator it = _layerVec.begin(); it != _layerVec.end(); it++ ) {  
              if( (*it).getID() == ID ) return &(*it); // return a pointer (*it) is the element of the vector
            }

            return 0; 
          }
   
/** */
	virtual void PrintHeader( std::ostream& s ) const {

		// layers    									      
		s << " Number of layers : " << getNLayers() << std::endl << std::endl ;
 		s << " layer parameters : "  << std::endl ;

      char buffer[1024] ;    
sprintf(buffer,"|----------||----------------------------------------------------------------------||------------------------------------------------------------------------------------------------------------|\n") ;
      s << buffer ; 
sprintf(buffer,"| layer:   ||                 ladder:                                              ||                           sensitive part:                                                                  |\n") ;
      s << buffer ;
sprintf(buffer,"| ID| info || ID| info | pozX| pozY| pozZ|rotXY|rotZX|rotZY| sizX| sizY|Thick|  X  || ID|   info | pozX| pozY| pozZ|rotXY|rotZX|rotZY| sizX| sizY|Thick|  X  |  NX |  NY |PtchX|PtchY| ResX| RexY| \n") ;
      s << buffer ;
sprintf(buffer,"|----------||----------------------------------------------------------------------||------------------------------------------------------------------------------------------------------------|\n") ;
      s << buffer ;

	}
 
/** */
	virtual void PrintLayers( std::ostream& s) const {

            	char buffer[1024] ;

		for( TelPlanesLayerImplVec::const_iterator it = _layerVec.begin(); it != _layerVec.end(); it++ ) {  
			TelPlanesLayerImpl* layer = const_cast<TelPlanesLayerImpl* > (&(*it)) ;

 		sprintf(buffer,"|%3d|%6s||\n"
			, layer->getID() 
			, layer->getInfo().c_str() 
		);
		s << buffer ;

                TelPlanesMaterialLayerImplVec::const_iterator mb = layer->getMaterialLayerVec().begin();
                TelPlanesMaterialLayerImplVec::const_iterator me = layer->getMaterialLayerVec().end();
                TelPlanesSensitiveLayerImplVec::const_iterator sb = layer->getSensitiveLayerVec().begin();
                TelPlanesSensitiveLayerImplVec::const_iterator se = layer->getSensitiveLayerVec().end();

                const char* a=""; 
                for( TelPlanesMaterialLayerImplVec::const_iterator ml = mb ; ml < me; ml++) {
      		  sprintf(buffer,"|%3s|%6s||%3d|%6s|%5.0f|%5.0f|%5.0f|%5.0f|%5.0f|%5.0f|%5.2f|%5.2f|%5.2f|%5.2f||\n"
                      , a,a
                      , (*ml).getID() 
		      , (*ml).getInfo().c_str() 
		      , (*ml).getPositionX() 
		      , (*ml).getPositionY() 
		      , (*ml).getPositionZ()
		      , (*ml).getRotationXY() 
		      , (*ml).getRotationZX() 
		      , (*ml).getRotationZY()
		      , (*ml).getSizeX() 
		      , (*ml).getSizeY() 
		      , (*ml).getThickness()
 		      , (*ml).getRadLength()
 		  ); 
	   	  s << buffer ;
                }
	
                for( TelPlanesSensitiveLayerImplVec::const_iterator sl = sb ; sl < se; sl++) {
	sprintf(buffer,"|%3s|%6s||%3s|%6s|%5s|%5s|%5s|%5s|%5s|%5s|%5s|%5s|%5s|%5s||%3d|%8s|%5.0f|%5.0f|%5.0f|%5.0f|%5.0f|%5.0f|%5.2f|%5.2f|%5.2f|%5.2f`%5d|%5d|%5.1f|%5.1f|%5.2f|%5.2f|\n"
                      , a,a,   a,a,  a,a,  a,a,  a,a,  a,a,  a,a
 		      , (*sl).getID() 
 		      , (*sl).getInfo().c_str()
		      , (*sl).getPositionX() 
		      , (*sl).getPositionY() 
		      , (*sl).getPositionZ()
		      , (*sl).getRotationXY() 
		      , (*sl).getRotationZX() 
		      , (*sl).getRotationZY()
		      , (*sl).getSizeX() 
		      , (*sl).getSizeY() 
		      , (*sl).getThickness() 
		      , (*sl).getRadLength()

		      , (*sl).getNpixelX() 
		      , (*sl).getNpixelY() 
		      , (*sl).getPitchX() 
		      , (*sl).getPitchY() 
		      , (*sl).getResolutionX()
       		      , (*sl).getResolutionY()
	);
		  s << buffer ;
                  }
                }
sprintf(buffer,"|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n") ;
      		s << buffer ;

	}
        
/** */
	virtual void PrintFooter( std::ostream& s) const {
		char buffer[1024] ;

sprintf(buffer,"|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n") ;
		s << buffer ;

	}
     


}; // class

} // namespace gear
#endif /* ifndef GEAR_SIPLANESLAYERLAYOUT_H */
