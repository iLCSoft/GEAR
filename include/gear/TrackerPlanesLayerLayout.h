// -*- C++ -*-
#ifndef GEAR_TrackerPlanesLAYERLAYOUT_H
#define GEAR_TrackerPlanesLAYERLAYOUT_H 1

#include <vector>
#include <string>

#include <cstdio>
#include <iostream> 
#include <iomanip>


namespace gear {

/**Abstract description of layers in a pixel beam telescope.
 * @author I. Rubinskiy,  DESY
 * @version $Id: 
 */

class TrackerPlanesLayerImpl;

class TrackerPlanesMaterialLayer  {

    
public:
    /// Constructor
    TrackerPlanesMaterialLayer() = default;

    /// Destructor.
    virtual ~TrackerPlanesMaterialLayer() { /* nop */; }

    /// Copy-constructor.
    TrackerPlanesMaterialLayer( const TrackerPlanesMaterialLayer& ) = default;

    virtual int getID() const = 0;

    /** */
    virtual std::string getInfo() const = 0;

    /** The radiation length of nonsensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source.
     */
    virtual double getRadLength() const = 0;

    /** x position of the center of nonsensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source.
     */
    virtual double getPositionX() const = 0;

    /** y position of the center of nonsensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source.
     */
    virtual double getPositionY() const = 0;

    /** z position of the center of nonsensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */
    virtual double getPositionZ() const = 0;

    /** rotation angles according to Euler implementation scheme
     * (in XY plane -> around axis Z: gamma )
     */
    virtual double getRotationXY() const = 0;
    
    /** rotation angles according to Euler implmentation scheme
     * (in ZX plane -> around axis Y: beta  )
     */
    virtual double getRotationZX() const = 0;
  
    /** rotation angles according to Euler implmentation scheme
     *  (in ZY plane -> around axis X: alfa  )
     */
    virtual double getRotationZY() const = 0;


    /** a free parameter string field
     */
    virtual void setID(int value)               = 0;
   
    /** */
    virtual void setInfo(std::string value) = 0;

    /** a free parameter string field
     */
    virtual void setRadLength( double value)    = 0;

    /** a free parameter string field
     */
    virtual void setPositionX( double value)    = 0;

    /** a free parameter string field
     */
    virtual void setPositionY( double value)    = 0;

    /** a free parameter string field
     */
    virtual void setPositionZ( double value)    = 0;

    /** a free parameter string field
     */
    virtual void setRotationXY( double value)   = 0;

    /** a free parameter string field
     */
    virtual void setRotationZX( double value)   = 0;

    /** a free parameter string field
     */
    virtual void setRotationZY( double value)   = 0;
    
    /** a free parameter string field
     */
    virtual void setSizeX( double value)        = 0;
   
    /** a free parameter string field
     */
    virtual void setSizeY( double value)        = 0;
   
    /** a free parameter string field
     */
    virtual void setThickness( double value)    = 0;


    /** Size in x direction of nonsensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */	
    virtual double getSizeX() const = 0;

    /** Size in y direction of nonsensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */	
    virtual double getSizeY() const = 0;

    /** Thickness of nonsensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */	
    virtual double getThickness() const = 0;

};

class TrackerPlanesSensitiveLayer  {

    
public:
    /// Constructor
    TrackerPlanesSensitiveLayer() = default;

    /// Destructor.
    virtual ~TrackerPlanesSensitiveLayer() { /* nop */; }

    /// Copy-constructor.
    TrackerPlanesSensitiveLayer( const TrackerPlanesSensitiveLayer& ) = default;


    /** ID of sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source.*/
    virtual int getID() const = 0;

    /** */
    virtual std::string getInfo() const = 0;

    /** x position of the center of sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source.
     */
    virtual double getPositionX() const = 0;

    /** y position of the center of sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source.
     */
    virtual double getPositionY() const = 0;

    /** z position of the center of sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source.
     */
    virtual double getPositionZ() const = 0;

    /** rotation angles according to Euler implementation scheme
     * (in XY plane -> around axis Z: gamma )
     */
    virtual double getRotationXY() const = 0;
    
    /** rotation angles according to Euler implmentation scheme
     * (in ZX plane -> around axis Y: beta  )
     */
    virtual double getRotationZX() const = 0;
  
    /** rotation angles according to Euler implmentation scheme
     *  (in ZY plane -> around axis X: alfa  )
     */
    virtual double getRotationZY() const = 0;

    /** Size in x direction of sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */	
    virtual double getSizeX() const = 0;

    /** Size in y direction of sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */	
    virtual double getSizeY() const = 0;

    /** Thickness of sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */	
    virtual double getThickness() const = 0;

    /** The radiation length of sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source.
     */
    virtual double getRadLength() const = 0;

    /** Number of pixels in x direction in sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */
    virtual int getNpixelX() const = 0;

    /** Number of pixels in y direction in sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */
    virtual int getNpixelY() const = 0;

    /** Pitch size in x direction in sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */
    virtual double getPitchX() const = 0;

    /** Pitch size in y direction in sensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */
    virtual double getPitchY() const = 0;

    /** Intrinsic resolution of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */
    virtual double getResolutionX() const = 0;

    /** Intrinsic resolution of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source. 
     */
    virtual double getResolutionY() const = 0;


    /** a free parameter string field
     */
    virtual void setID(int value)               = 0;
 
    /** */
    virtual void setInfo(std::string value) = 0;

    /** a free parameter string field
     */
    virtual void setRadLength( double value)    = 0;
 
    /** a free parameter string field
     */
    virtual void setPositionX( double value)    = 0;
 
    /** a free parameter string field
     */
    virtual void setPositionY( double value)    = 0;
 
    /** a free parameter string field
     */
    virtual void setPositionZ( double value)    = 0;
 
    /** a free parameter string field
     */
    virtual void setRotationXY( double value)   = 0;
 
    /** a free parameter string field
     */
    virtual void setRotationZX( double value)   = 0;
 
    /** a free parameter string field
     */
    virtual void setRotationZY( double value)   = 0;
 
    /** a free parameter string field
     */
    virtual void setSizeX( double value)        = 0;
 
    /** a free parameter string field
     */
    virtual void setSizeY( double value)        = 0;
 
    /** a free parameter string field
     */
    virtual void setThickness( double value)    = 0;


};


class TrackerPlanesLayer  {

public:

    /// Constructor
    TrackerPlanesLayer() = default;

    /// Destructor.
    virtual ~TrackerPlanesLayer() { /* nop */; }

    // Copy-constructor.
    TrackerPlanesLayer( const TrackerPlanesLayer& ) = default;

    /** Layer ID of nonsensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source.*/
    virtual int getID() const = 0;

    /** */
    virtual std::string getInfo() const = 0;

    /** add a setter method
     * set ID of a new layer
     */
    virtual void setID(int value)  = 0;

    /** */
    virtual void setInfo(std::string value) = 0;

    /** The total number of nonSense layers. */
    virtual int getNMaterialLayers() const = 0;

    /** The total number of Sensitive layers. */
    virtual int getNSensitiveLayers() const = 0;

    /** a free parameter string field
     */
    virtual void addMaterialLayer(
			      int ID, std::string info,
			      double PositionX, double PositionY, double PositionZ,
			      double RotationXY, double RotationZX, double RotationZY,
			      double SizeX, double SizeY, double Thickness, double RadLength
                              ) = 0;

    /** a free parameter string field
     */
    virtual void addSensitiveLayer(
			      int ID, std::string info,
			      double PositionX, double PositionY, double PositionZ,
			      double RotationXY, double RotationZX, double RotationZY,
			      double SizeX, double SizeY, double Thickness, double RadLength,
			      int NpixelX, int NpixelY,
			      double PitchX,double PitchY, double ResolutionX, double ResolutionY
                              ) = 0;

};


class TrackerPlanesLayerLayout  {

public: 
    /// Destructor.
    virtual ~TrackerPlanesLayerLayout() { /* nop */; }

    /** The total number of layers. */
    virtual int getNLayers() const = 0;
 
    /** a free parameter string field
     */
    virtual const TrackerPlanesLayerImpl* getLayer( unsigned int ID ) const = 0;
 
    /** a free parameter string field
     */
    virtual const TrackerPlanesLayerImpl* getLayerByID( int ID ) const = 0;
  
    /** a free parameter string field
     */
    virtual void addLayer( TrackerPlanesLayerImpl* )  = 0;

     /** Layer ID of nonsensitive volume of layer layerIndex - layer indexing starts at 0
     *  for the layer closest to the beam source.*/
    virtual int getID() const = 0;

    /** */
    virtual void setID(int ID) = 0;

    /** */
    virtual std::string getInfo() const = 0;

    /** */
    virtual void setInfo(std::string value) = 0;
 
    /** a free parameter string field
     */
    virtual void PrintHeader( std::ostream& s) const = 0;
 
    /** a free parameter string field
     */
    virtual void PrintLayers( std::ostream& s) const = 0;
 
    /** a free parameter string field
     */
    virtual void PrintFooter( std::ostream& s) const = 0;

}; // class


} // namespace gear
#endif /* ifndef GEAR_TrackerPlanesLAYERLAYOUT_H */
