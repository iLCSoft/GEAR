#include "gearimpl/TPCParametersImpl.h"

#include "gearimpl/TPCModuleImpl.h"
#include <vector>
#include <sstream>

namespace gear {

 /** @author M, Killenberg, (Bonn)  S, Turnbull, (Saclay/Carleton) 
  * @version $Id: TPCParametersImpl.cc,v 1.3 2009-02-27 09:00:50 gaede Exp $
  */

     
    TPCParametersImpl::TPCParametersImpl( double maxDriftLength, int coordinateType ) : 
	_maxDriftLength(maxDriftLength),
	_coordinateType(coordinateType),
	_driftVelocity(0)
    {
	if ( (maxDriftLength  == -1.) || (coordinateType == -1) )
	{
//	    std::cerr << "TPCParametersImpl::TPCParametersImpl: Warning: "
//		      << "deprecated use of constructor without parameters." << std::endl
//		      << "   Please define maxDriftLength and coordinateType in constructor!"
//		      << std::endl;
	    _maxDriftLength = 0.;
	    _coordinateType = PadRowLayout2D::POLAR; // make polare (disk) the default
	}

	_planeExtent.push_back(0.0);
	_planeExtent.push_back(0.0);
	_planeExtent.push_back(0.0);
	_planeExtent.push_back(0.0);
    }

    // The copy constructor. All modules are owned by the TPCParameters,
    // and deleted in the destructor. So they have to be copied.
    TPCParametersImpl::TPCParametersImpl(TPCParametersImpl const & right)
    {
	copy_and_assign( right);
    }

    void TPCParametersImpl::copy_and_assign(const  TPCParametersImpl & right)
    {
	// create copies of all modules in the _TPCModules vector
	std::vector<TPCModule *>::const_iterator itr;
	
	for(itr = right._TPCModules.begin();itr!=right._TPCModules.end();itr++)
	{
	    _TPCModules.push_back( dynamic_cast<TPCModule *>((*itr)->clone()) );
	}

	// copy all the other variables
	_maxDriftLength = right._maxDriftLength;
	_coordinateType = right._coordinateType;
	_moduleIDMap = right._moduleIDMap;
	_planeExtent    = right._planeExtent;
	_driftVelocity  = right._driftVelocity;

	// call the assignment operator of the GearParametersImpl mother class:
	// *dynamic_cast<GearParametersImpl*>(this) = *dynamic_cast<GearParametersImpl const *>(&right);
	GearParametersImpl::operator = (right);
    }

    /// Destructor.
    TPCParametersImpl::~TPCParametersImpl()
    {
	cleanup();
    }

    ///Assignment operator
    TPCParametersImpl & TPCParametersImpl::operator = (  const TPCParametersImpl & right)
    {
	cleanup();
	copy_and_assign( right );

	return *this;
    }
    
    void TPCParametersImpl::cleanup()
    {
	std::vector<TPCModule *>::iterator itr;
	
	for(itr = _TPCModules.begin();itr!=_TPCModules.end();itr++){
	    delete (*itr);
	}
    }
    
    /** Returns module with the given module ID.
     *  \todo Throw exception if not found
     */
    const TPCModule & TPCParametersImpl::getModule(int moduleID) const{
      std::map<int,int>::const_iterator moduleIter = _moduleIDMap.find(moduleID);
      if ( moduleIter == _moduleIDMap.end() )
      {
	std::stringstream message;
	message << "TPCParameters::getModule: No module with ID " << moduleID 
		<< "  defined in the gear file! "<< std::endl;
	throw gear::Exception(message.str());

      }
      int temp = _moduleIDMap.find(moduleID)->second;
	TPCModule * tempMod = _TPCModules.at(temp);
	return *tempMod;
    }

    /** Returns number of modules in TPC  
     */
    int TPCParametersImpl::getNModules() const{
	return _TPCModules.size();
    }

    /** Returns nearest module to given co-ordinates
     */
    const TPCModule & TPCParametersImpl::getNearestModule(double c0, double c1) const{
	const TPCModule * toReturn = NULL;
	    if(_TPCModules.size()){
	    toReturn = _TPCModules.at(0);
	    double distance = toReturn->getDistanceToModule(c0,c1);
	    std::vector<TPCModule *>::const_iterator itr;
	    double tempDistance; 
	    for(itr = _TPCModules.begin();itr!=_TPCModules.end();itr++){
 		tempDistance =(*itr)->getDistanceToModule(c0,c1);
		if(tempDistance < distance){
		    distance = tempDistance;
		    toReturn = (*itr);
		}
	    }	    
	}else{
	    throw gear::Exception("TPCParameters::GetNearsestModule: No Modules are defined, Cannot find Nearest");
	}
	return *toReturn;
    }

    /** The maximum drift length in the TPC in mm.
     */
    double TPCParametersImpl::getMaxDriftLength() const{
	return _maxDriftLength;
    }

    /** True if coordinate (c0,c1) is within any module may or may not be
     *  on a pad, since with resitive films being on the film is enough.
     */
    bool TPCParametersImpl::isInsideModule(double c0, double c1) const{
	bool toReturn = false; 
	std::vector<TPCModule *>::const_iterator itr;
	
	for(itr = _TPCModules.begin();itr!=_TPCModules.end();itr++){
	    toReturn = toReturn||(*itr)->isInsideModule(c0,c1);
	}
	return toReturn;
    }

    /** True if coordinate (c0,c1) is within any pad, on any module.
     */
    bool TPCParametersImpl::isInsidePad(double c0, double c1) const{
	bool toReturn = false;
	std::vector<TPCModule *>::const_iterator itr;
	
	for(itr = _TPCModules.begin();itr!=_TPCModules.end();itr++){
	    toReturn = toReturn||(*itr)->isInsidePad(c0,c1);
	}
	return toReturn;
    }

    /** returns globalPadindex Object for nearest Pad to given co-ordinates.
     */
    GlobalPadIndex TPCParametersImpl::getNearestPad(double c0, double c1) const{


	if(!_TPCModules.size()) {
	    throw gear::Exception("TPCParametersImpl::getNearestPad: No Modules are defined, Cannot find Nearest");
	}
	//For each module, get distance to nearest pad, compare, return shortest.
	std::vector<TPCModule *>::const_iterator itr, best_itr;
	itr = _TPCModules.begin();
	best_itr = itr;

	GlobalPadIndex closest_pad( (*itr)->getNearestPad(c0, c1), (*itr)->getModuleID() );

	if( _TPCModules.size() == 1 ) {

		return closest_pad;
	}

	// initialize the distance
	double shortest_distance = (*itr)->getDistanceToPad( c0, c1, closest_pad.getPadIndex() );

	// start at the second module
	for( itr+1; itr!=_TPCModules.end(); itr++) {

		GlobalPadIndex temp_pad( (*itr)->getNearestPad( c0, c1 ), (*itr)->getModuleID() );

		const double distance = (*itr)->getDistanceToPad( c0, c1, temp_pad.getPadIndex() );

		if( distance < shortest_distance ) {

			closest_pad = temp_pad;
			shortest_distance = distance;
		}
	}

	return closest_pad;
    }

    /** Extent of the sensitive plane - [xmin,xmax,ymin,ymax] CARTESIAN or 
     *	[rmin,rmax,phimin,phimax] POLAR.
     */
    const std::vector<double>  & TPCParametersImpl::getPlaneExtent() const{
	if(!_TPCModules.size()) {
	    throw gear::Exception("TPCParametersImpl::getPlaneExtent: No Modules are defined, Bad User");
	}
	
	return _planeExtent;
    }

    /** returns Coordinate type as an int
     */
    int TPCParametersImpl::getCoordinateType() const{
	return _coordinateType;
    }

    void  TPCParametersImpl::addModule( TPCModule * TPCModule ) {
	if(TPCModule==NULL) {
	    throw gear::Exception("TPCParametersImpl::addModule: This Module is not defined, Bad User");
	}
	
	// check whether module has correct mother coordinate type
	if (TPCModule->getTPCCoordinateType() != _coordinateType)
	{
	    throw gear::Exception("TPCParametersImpl::addModule: Module has wrong coordinate type, Bad User");
	}

	int currentVectorSize = _TPCModules.size();
	
	if(_moduleIDMap.find(TPCModule->getModuleID())!=_moduleIDMap.end()) {
	    throw gear::Exception("TPCParametersImpl::addModule: This Module Number has already been used.");	    
	}
	_moduleIDMap[TPCModule->getModuleID()] = currentVectorSize; 
	_TPCModules.push_back(TPCModule);


 	//init active area to NULL
 	    
	if(TPCModule->getModuleExtent().at(0) < _planeExtent.at(0)) {
	    _planeExtent.at(0)=TPCModule->getModuleExtent().at(0);
	}
	if(TPCModule->getModuleExtent().at(1) > _planeExtent.at(1)) {
	    _planeExtent.at(1)=TPCModule->getModuleExtent().at(1);
	}
	if(TPCModule->getModuleExtent().at(2) < _planeExtent.at(2)) {
	    _planeExtent.at(2)=TPCModule->getModuleExtent().at(2);
	}
	if(TPCModule->getModuleExtent().at(3) > _planeExtent.at(3)) {
	    _planeExtent.at(3)=TPCModule->getModuleExtent().at(3);
	}
    }

    /** returns refeance to vector of Modules 
     */
    const std::vector<TPCModule *> & TPCParametersImpl::getModules() const{
	return _TPCModules;    
    }

    /** Kept for backward compatibility.
     *  In case there is only one module, which has no angle and no
     *  offset wrt. the gobal coordinate systen, it gives back the 
     *  PadRowLayout2D of this module.
     *  Otherwise it throws a gear::Exception.
     *
     *  \deprecated{Please use getModule(moduleID) instead.
     *  The TPCModule is an implementation of PadRowLayout2D}
     */
    const PadRowLayout2D & TPCParametersImpl::getPadLayout() const
    {
	//std::cout << "TPCParametersImpl::getPadLayout() :  nModules = " << _TPCModules.size() 
	//     << std::endl;
	if (_TPCModules.size() > 1) 
	    throw gear::Exception("You cannot use TPCParametersImpl::getPadLayout() with more than one module. Use getModule(moduleID) instead!");
	if (_TPCModules.size() < 1) 
	    throw gear::Exception("You cannot use TPCParametersImpl::getPadLayout() without a module. Set the module / pad layout first!");
	//n.b. This ensures that there is at least one module,
	// so it's save to use _TPCModules[0]
	
//	std::cerr << "TPCParametersImpl: Warning: "
//		  << "deprecated use of getPadLayout()." << std::endl
//		  << "   Use getTPCModule( moduleID ) instead!"
//		  << std::endl;

	if ( (_TPCModules[0]->getAngle() != 0) || ((_TPCModules[0]->getOffset())[0] != 0) 
	     || ((_TPCModules[0]->getOffset())[1] != 0) ) 
	    throw gear::Exception("Module coordinate system is not identical to coordinate system in pad plane. Use getModule(moduleID) instead!");

	return *(_TPCModules[0]);
    }
    
    /** The electron drift velocity in the TPC in mm/s.
     *  Kept for backwards compatibility. 
     *
     *  \deprecated{This should come from  conditions data.}
     */
    double TPCParametersImpl::getDriftVelocity() const
    {
//	std::cerr << "TPCParametersImpl: Warning: "
//		  << "deprecated use of getDriftVelocity()." << std::endl
//		  << "   Drift velosity should come from conditions data!"
//		  << std::endl;

	return _driftVelocity ;
    }
    
    /** Kept for backward compatibility.
     *  In case there is only one module it gives back the 
     *  redaout frequency of this module.
     *  Otherwise it throws a gear::Exception.
     *  
     *  \deprecated{Use TPCModule::getReadoutFrequency() instead.}
     */
    double TPCParametersImpl::getReadoutFrequency() const
    {
	if (_TPCModules.size() > 1) 
	    throw gear::Exception("You cannot use TPCParametersImpl::getRadoutFreuqency() with more than one module. Use getModule(moduleID) instead!");
	if (_TPCModules.size() < 1) 
	    throw gear::Exception("You cannot use TPCParametersImpl::getRadoutFreuqency() without a module. Set the module / pad plane first!");
	//n.b. This ensures that there is at least one module,
	// so it's save to use _TPCModules[0]
	return _TPCModules[0]->getReadoutFrequency();
    }
	
    void TPCParametersImpl::setMaxDriftLength( double maxDriftLength )
    { 
//	std::cerr << "TPCParametersImpl: Warning: "
//		  << "deprecated use of setMaxDriftLength()." << std::endl
//		  << "   Please define maxDriftLength in constructor!"
//		  << std::endl;

	_maxDriftLength = maxDriftLength ;
    } 
    
    void TPCParametersImpl::setDriftVelocity( double driftVelocity )
    { 
//	std::cerr << "TPCParametersImpl: Warning: "
//		  << "deprecated use of setDriftVelocity()." << std::endl
//		  << "   Drift velosity should come from conditions data!"
//		  << std::endl;

	_driftVelocity = driftVelocity ;
    } 
    
    void TPCParametersImpl::setReadoutFrequency( double readoutFrequency )
    { 
//	std::cerr << "TPCParametersImpl: Warning: "
//		  << "deprecated use of setReadoutFrequency." << std::endl
//		  << "   Readout frequency is property of a TPCModule!"
//		  << std::endl;

	if (_TPCModules.size() > 1) 
	    throw gear::Exception("You cannot use TPCParametersImpl:::setReadoutFrequency() with more than one module. Use getModule(moduleID) instead!");
	if (_TPCModules.size() < 1) 
	    throw gear::Exception("You cannot use TPCParametersImpl:::setReadoutFrequency() without a module. Set the module / pad plane first!");
	//n.b. This ensures that there is at least one module,
	// so it's save to use _TPCModules[0]
	dynamic_cast<TPCModuleImpl*>(_TPCModules[0])->setReadoutFrequency(readoutFrequency);
    }

    void TPCParametersImpl::setPadLayout( PadRowLayout2D * padLayout )
    { 
//	std::cerr << "TPCParametersImpl: Warning: "
//		  << "deprecated use of setPadLayout." << std::endl
//		  << "   Please use addModule instead!"
//		  << std::endl;

	if (_TPCModules.size() != 0) 
	    throw gear::Exception("There is already one module in TPCParametersImpl. Will not replace it!");

	// check whether the padLayout actually is a module
	if ( padLayout->getPadLayoutImplType() == PadRowLayout2D::TPCMODULE )
	    throw gear::Exception("Wrong type: PadLayout is a module, cannot set this using setPadLayout. Use addModule()!");
	    
	addModule( new TPCModuleImpl(0, padLayout, _coordinateType ));
    }

} // namespace gear
