#ifndef GEAR_TPCParametersImpl_H
#define GEAR_TPCParametersImpl_H 1

#include "gear/TPCParameters.h"
#include <map>
#include "gearimpl/GearParametersImpl.h"
#include "gearimpl/GlobalPadIndex.h"



namespace gear {

class TPCModule;

/** A Container for TPCModules which describe the geometry properties of a given 
 * TPC. <br>
 * This assumes nothing about the layouts of the pads within the modules 
 * Most of the functionality is derived from calling functions of constained TPCModules <br>
 * 
 * 
 * @author M, Killenberg, (Bonn)  S, Turnbull, (Saclay/Carleton) 
 * @version $Id: TPCParametersImpl.h,v 1.2 2008-12-19 13:52:34 gaede Exp $
 */

class TPCParametersImpl :  public TPCParameters,    public GearParametersImpl {
    
  public: 
    
    TPCParametersImpl( double maxDriftLength=-1., int coordinateType=-1 ) ;
    TPCParametersImpl( const TPCParametersImpl & ) ;

    TPCParametersImpl & operator = ( const TPCParametersImpl & ) ;

//     /** Copy constructor for base parameters */
//     TPCParametersImpl( GearParameters& gearParameters ) ;

    /// Destructor.
    virtual ~TPCParametersImpl() ;
    
    /** Returns module with the given module ID.
     *  Attention! This is NOT the position/index in the vector which can be
     *  accessed with getModules()!
     */
    virtual const TPCModule& getModule(int moduleID) const; 

    /** Returns the number of modules in the TPC (endplate)
     */
    virtual int getNModules() const;

    /** Returns nearest module to given coordinates (2D).
     *  In case of a full TPC with two end caps the first matching module is returned,
     *  although a module from the other end plate might be closer in 3D.
     *  Use the 3D version of getNearestModule in this case 
     *  to get the module from the correct half TPC.
     */
    virtual const TPCModule & getNearestModule(double c0, double c1) const;

     /** Returns nearest module to given coordinates (3D).
     *  The z coordinate is only used to determine whether the positive or negative 
     *  half TPC is to be used.
     */
    virtual const TPCModule & getNearestModule(double c0, double c1, double z) const;

   /** The maximum drift length in the TPC in mm.
     */
    virtual double getMaxDriftLength() const;

    /** True if coordinate (c0,c1) is within any module.
     *  This may or may not be
     *  on a pad, since with resitive films being on the film is enough
     *  to generate signal on pads (see TPCModule::getBorderWidth() ).
     *  This is the 2D version. Use isInsideModule(double c0, double c1, double z)
     *  to limit to correct end plate in case there are two.
     */
    virtual bool isInsideModule(double c0, double c1) const;

    /** First determines the correct end plate from the z coordinate and then
     *  calls the same function as the 2D version on the modules from the correct 
     *  end cap.
     */
    virtual bool isInsideModule(double c0, double c1, double z) const;

    /** True if coordinate (c0,c1) is within any pad, on any module.
     *  This is the 2D version. Use isInsideModule(double c0, double c1, double z)
     *  to limit to correct end plate in case there are two.
     */
    virtual bool isInsidePad(double c0, double c1) const;

    /** First determines the correct end plate from the z coordinate and then
     *  calls the same function as the 2D version on the modules from the correct 
     *  end cap.
     */
    virtual bool isInsidePad(double c0, double c1, double z) const;

   /** Returns globalPadindex Object for nearest pad to given coordinates (2D).
     *  In case of a full TPC with two end caps the first matching module ID is returned,
     *  although a module from the other end plate might be closer in 3D.
     *  Use the 3D version of getNearestPad in this case 
     *  to get the pad from a module from the correct half TPC.
     */
    virtual GlobalPadIndex getNearestPad(double c0, double c1) const;

    /** Returns globalPadindex Object for nearest pad to given coordinates (3D).
     *  The z coordinate is only used to determine whether the positive or negative 
     *  half TPC is to be used.
     */	
    virtual GlobalPadIndex getNearestPad(double c0, double c1, double z) const;

    /** Extent of the sensitive plane - [xmin,xmax,ymin,ymax] CARTESIAN or 
     *	[rmin,rmax,phimin,phimax] POLAR. These are the outermost coordinates
     *  in the respective coordinate system. It does not mean that the complete
     *  plane is sensitive as there might be dead space between the modules.
     */
    virtual const std::vector<double>  & getPlaneExtent() const;

    /** Returns coordinate type as an int (see PadRowLayout2D::CARTESIAN,
     *  PadRowLayout2D::POLAR)
     */
    virtual int getCoordinateType() const;

    /** Returns vector of all modules in this TPC (endplate).
    */
    virtual const std::vector<TPCModule *> & getModules() const;
   
    /** Adds a Module to the vector of modules, or throws an exception. 
     */
    virtual void  addModule( TPCModule * TPCModule ) ;

    /** Kept for backward compatibility.
     *  In case there is only one module, which has no angle and no
     *  offset wrt. the gobal coordinate systen, it gives back the 
     *  PadRowLayout2D of this module.
     *  Otherwise it throws a gear::Exception.
     *
     *  \deprecated{Please use getModule(moduleID) instead.
     *  The TPCModule is an implementation of PadRowLayout2D}
     */
    virtual const PadRowLayout2D& getPadLayout() const ;
    
    /** The electron drift velocity in the TPC in mm/s.
     *  Kept for backwards compatibility. 
     *
     *  \deprecated{This should come from  conditions data.}
     */
    virtual double getDriftVelocity() const ;
    
    /** Kept for backward compatibility.
     *  In case there is only one module it gives back the 
     *  redaout frequency of this module.
     *  Otherwise it throws a gear::Exception.
     *  
     *  \deprecated{Use TPCModule::getReadoutFrequency() instead.}
     */
    virtual double getReadoutFrequency() const ;

    /** \deprecated{Calls addModule with a module it creates from the padLayout}
     */
    virtual void setPadLayout( PadRowLayout2D * padLayout ) ;
    
    /** \deprecated{MaxDriftLength should be set in constructor}
     */
    virtual void  setMaxDriftLength( double maxDriftLength ) ;
    
    /** \deprecated{Drift velocity should come from conditons data}
     */
    virtual void  setDriftVelocity( double driftVelocity ) ;
    
    /** \deprecated{Readout frequency is individual for modules.}
     */
    virtual void  setReadoutFrequency( double readoutFrequency ) ;

protected:

  // one vector with all the modules, and one for each half tpc
  std::vector<TPCModule *>  _TPCModules, _modulesPositiveHalfTPC, _modulesNegativeHalfTPC;

  /// Version for internal usage, for both all modules or only one half TPC
  const TPCModule & getNearestModule(double c0, double c1, 
				     std::vector<TPCModule *> const & modulesVector) const;
  
  /// Versions for internal usage, for both all modules or only one half TPC
  bool isInsideModule(double c0, double c1, std::vector<TPCModule *> const & modulesVector) const;

  /// Versions for internal usage, for both all modules or only one half TPC
  bool isInsidePad(double c0, double c1, std::vector<TPCModule *> const & modulesVector ) const;

  /// Versions for internal usage, for both all modules or only one half TPC
  GlobalPadIndex getNearestPad(double c0, double c1,
			       std::vector<TPCModule *> const & modulesVector ) const;
 

  double _maxDriftLength ;

  int _coordinateType;

  /** The cathode position is needed internally to distinguish the half TPCs.
   *  If only one half TPC is used the cathode positon is _zAnode - _maxDriftLength for positive,
   *  _zAnode + _maxDriftLength for negative half TPC ( the latter including _zAnode = 0, prototype case).
   *  In case both half TPCs are populated with modules it is ( _zAnode_positive + _zAnode_negative ) / 2;
   */
  double _cathodePosition;
    
  /** A map with the moduleID as key and the index in the _TPCModules vector as value.
   *  For internal use in this class only.
   */
  std::map<int,int> _moduleIDMap ;

  std::vector<double> _planeExtent ;

  /// Drift velocity is deprecated, should come from conditions data
  double _driftVelocity ;

  /** function to copy all internal variables, incl. the objects
   *  pointed to and owned by the TPCParameters.
   *  Used by constructor and assigment operator to avoid code duplication
   */
  void copy_and_assign(const  TPCParametersImpl & );

  /** function to delete all the objects
   *  pointed to and owned by the GearMgr.
   *  Used by desctructor and assigment operator to avoid code duplication
   */
  void cleanup();

  // Internal function to avoid code duplication
  // Recalculation of the z postion is necessary when a module is added and when the 
  // maximal drift distance is changed.
  void setCathodePosition();

}; // class

} // namespace gear


#endif /* ifndef GEAR_TPCParametersImpl_H */
