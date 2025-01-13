# v01-09-04

* 2025-01-13 Thomas Madlener ([PR#15](https://github.com/iLCSoft/GEAR/pull/15))
  - Switch to central Key4hep CI build workflows and remove clicdp nightlies based ones
  - Fix warnings that are now enabled

* 2025-01-13 jmcarcell ([PR#14](https://github.com/iLCSoft/GEAR/pull/14))
  - Add GNUInstallDirs to set CMAKE_INSTALL_LIBDIR so that the default rpath is correct and can be used in downstream projects, like in `k4MarlinWrapper`

# v01-09-03

* 2024-07-01 jmcarcell ([PR#10](https://github.com/iLCSoft/GEAR/pull/10))
  - Fix warnings about not passing exceptions by reference
  - Fix warnings about an implicit copy constructor defined
  - Fix other warnings, about fallthroughs, a weird UTF-8 character (spanish accent in comments in spanish) and unused results

# v01-09-02

* 2024-01-15 tmadlener ([PR#12](https://github.com/iLCSoft/GEAR/pull/12))
  - Remove no longer working gcc8 based workflow
  - Update github actions versions
  - Add key4hep based workflow

* 2023-08-15 jmcarcell ([PR#13](https://github.com/iLCSoft/GEAR/pull/13))
  - Respect CMAKE_INSTALL_<dir>

* 2023-07-17 jmcarcell ([PR#11](https://github.com/iLCSoft/GEAR/pull/11))
  - Drop the CMake test target that is not doing anything

* 2023-07-17 jmcarcell ([PR#9](https://github.com/iLCSoft/GEAR/pull/9))
  - Fix build with docs ON

# v01-09-01

* 2022-06-28 Thomas Madlener ([PR#8](https://github.com/iLCSoft/GEAR/pull/8))
  - Fix cmake doxygen configuration to work with CMake >= 3.17

* 2022-06-28 Thomas Madlener ([PR#7](https://github.com/iLCSoft/GEAR/pull/7))
  - Migrate CI to use github actions and remove travis CI

* 2020-09-07 Valentin Volkl ([PR#6](https://github.com/iLCSoft/GEAR/pull/6))
  - put more tests under the scope of BUILD_TESTING. Also fixes a build issue with spack https://github.com/key4hep/k4-spack/pull/33

# v01-09

* 2019-08-07 Frank Gaede ([PR#5](https://github.com/iLCSoft/GEAR/pull/5))
  - make compatible with c++17:
        - remove all `throw(...)` declarations

# v01-08

* 2018-01-12 Frank Gaede ([PR#1](https://github.com/iLCSoft/gear/pull/1))
  - fix all compiler warnings (gcc5.4)
      - uninitialized, unused, shadow, ...
      - don't create GEARLibDeps.cmake 
      - also fix warnings in tinyxml

* 2018-01-15 Frank Gaede ([PR#2](https://github.com/iLCSoft/gear/pull/2))
  - fix export of CLHEP library dependency
        - resolve location of CLHEP::CLHEP import target

* 2018-01-16 Frank Gaede ([PR#4](https://github.com/iLCSoft/gear/pull/4))
  - fix more warnings in tinyxml.h

* 2018-01-16 Frank Gaede ([PR#3](https://github.com/iLCSoft/gear/pull/3))
  - move tinyxml.h back to gearxml (for MarlinTPC) 
  - rm check for this!=0 in tinyxml.h (-Wundefined-bool-conversion warning w/ clang)

# v01-07

 # v01-06-01

    - patch releases:
      - make computation of number of pad rows in 
        FixedPadSizeDiskLayout more robust
      - needed for ILD_s1_v01 
  

 # v01-06
     - made compatible w/ c++11 and ROOT 6
        - removed -ansi -pedantic -Wno-long-long
        - fixed narrowing in initializer lists


 # v01-05-01

   - bug fix in src/gearimpl/FixedPadAngleDiskLayout.cc:
       - unititalized variable (rowNum_left)

 # v01-05

   - Vector3D.h: fixed compiler warnings of type:
     declaration of 'phi' shadows a member of 'this' 

   - added search field to doxygen doc

 # v01-04-02

  - F. Gaede
    - fixed a few warning wrt unused parameters  (F. Gaede)
    - made some methods public in FTDParametersImpl
    - fix in ZPlanarParametersImpl::isPointInLadder for missing shell (shell parameters zero)


 # v01-04-01

  - I.Rubinsky
    - Merge with telPlanes branch: introducing new TrackePlanes geometry classes 
    - Add set methods to SiPlanes and TrackerPlanes classes
    - Adding an TrackerPlanes example geometry
    - Introduce a redundant method for backward compatibility to old code using SiPlanes interface
    - Patching an interface function for backward compatibility of the SiPlanes geoemtry layout

    - Fixed bug that extents were calculated before the test for identical coordinate systems was performed  (M.Killenberg)

 # v01-04

   - added set methods to SiPlanesLayerLayout (I.Rubinsky)

   - bug fix for  FixedPadAngleDiskLayout::getNearestPad (A.Muennich)
     - Wrap around of angle was not done correctly.


 # v01-03-01  
    - added convenience wrapper BField::at(double x, double y, double z)

 # v01-03  

  - F.Gaede: added test/testInteractionLengths
      new example program to print interaction lengths as a function
      of theta for different cylinders between the calorimeters
    - updated testMaterialBudgetNew.cc: incl. 1st layer of Ecal

    - added example macros for DBD material plots:
       ./example/drawintlen_DBD.C
       ./example/drawmat_DBD.C
       ./example/treeFromAscii.C

  - C.Rosemann: fixed many issues found by coverity
    mostly unitialized member variables
    GearMgrImpl.cc:make assignment operator and copy c'tor private
    -> see Changelog for details

  - M.Killenberg: wrote XML documentation for the VersatileDiskRowLayout
     doc/VersatileDiskRowLayoutXML.*
    - improved the documentation of localToGlobal and globalToLocal in TPCModule
    - speedup of TPCParametersImpl::getModule(), removed duplicate search in map
    - getDistanceToPad called distanceToBox with wrong oder of arguments


  - bug fixes:
     - fixed error handling in stepping in ./src/geartgeo/TGeoGearDistanceProperties.cc
     - two bug fixes for the RectangularPadRowLayout (M.Killenberg)
     - fixed endless loop in case of rounding errors TPCParametersImpl/RectangularPadRowLayout
     - two pi wrap around for coordinates in TPCModuleImpl (Bo Li)



 # v01-02-02  

     - new utility printMaterials to print material between two points
     - added some debug printout to testMaterialBudgetNew.cc


 # v01-02-01  

    - extended functionality of GlobalPadIndex of the TPC (M. Killenberg)
    - fixed bug in globalToLocal of TPCModule (check for identical coorinate systems was wrong)
    - restore stream formatting after use
    - added CMP0008 cmake policy statement
    - removed unnecessary const in function declarations (clang++ warning)


 # v01-02  

      - Added new functionality for surfaces. 
        Surfaces have a CoordinateSystem and may optionally be bounded. 
        So far only Cartesian Coordinate System are available with rectangular and trapezoid boundaries. 
        gearsurf remains in a separate directory and builds a separate library which is then linked to the gear library. 
        All new functionality is included within the gear namespace.         

      - TPCParametersXML now allows for multiple default modules ( M. Killenberg )

 # v01-01  
      - added SensorID helper class with side, layer, module, sensor to Gear.h

      - added ZPlanarParameters::isPointInSensitive(Vector3D p, SensorID* sensorID=0 )
        to compute the detector element (sensor) that contains the point
  
      - added test program testMaterialBudgetNew 
        computes material budget in cylinders just inside the SIT, TPC and ECal 

      - fixed FTDLayerLayoutImpl  ( J.Duarte)  
          Added in the layeer instantiation relative methods the new parameters:
	  - zposition (centered position of the disk support)
	  - zsign0 ( sign of the offset of the first petal)
	  Removed the zposition for the support and sensors from the layer


 # v01-00  

     - Added and FTDLayerLayout  by J. Duarte Campderros, IFCA   

     - renamed VXDParameters and VXDLayerLayout to ZPlanarParameters and ZPlanarLayerLayout
       which can also be used for SIT and SET 

        - changes should be backward compatible through typedefs      
  
     - added get/setSITParameters()  and get/setSETParameters() to GearMgr


     - added simple material description to Gear

        - allowd to have materials of the form
             SimpleMaterial( name , A , Z, density, radLength, intLength )
          stored in the GearMgr 

        - materials are referenced via their names (stored in user parameters)


     - added constructors for double[3] and float[3] to Vector3D 
       - could be used with LCIO classes, e.g.:  
            gear::Vector3D pos( hit->getPosition() ) ;

     - added src/test/testXMLCopy.cc   (simple copy of xml files through GearXML)



 # v00-17  

     - made TGeo implementation of the detailed material properties a plugin,
       i.e. no dependence on ROOT, unless it is requested in the Gear file that TGeo should be used (tag <GDML>)

     - added old signature of addLayer for backwards compatibility (without layerRotation parameters introduced in v00-16)

     - renamed GearVersion.h to GearConfig.h (to include other general purpose configuration definitions)

     - src/gearxml/SiPlanesParametersXML.cc: xmlSen to xmlLad change (bug fix)

     - removed deprecated GEAR_DEFINITIONS (-DUSE_GEAR) from GEARConfig.cmake


 # v00-16  

   - updated SiPlanesParameters (I.Rubinsky) :
       Three new Layer related parameters  added.
       Three rotation angles: rotationXY = alfa, rotationZX = beta, rotationZY = gamma -> rotations around the local sensor frame axes
 
 # v00-15  

        * A. Muennich:
          New implementation of GearDistanceProperties and GearPointProperties using TGeo from ROOT:
      	    - reads in the geometry from a gdml file to instantiate the volume hierarchy
	    - requires newest ROOT version 5.27.06 or the svn trunk
            - MaterialMapFactory: A tool to create a material map in memory that can then be used to look up the
	      values during the run of the application. Saves computing time if many many querries are needed, 
	      e.g. Pandora where each calorimeter cell wants to know the material before.
        BUILD:
	To switch on the new functionality use -DGEAR_TGEO=1 and provide the CMAKE_MODULE_PATH and ROOT info
	cmake -DGEAR_TGEO=1 -DCMAKE_MODULE_PATH="xx" -DROOT_DIR=$ROOTSYS ..	
        USAGE:
        - in gear/example there are two gdml files: World_ILD00.gdml ( full detetcor geometry of mokka model ILD00)
          and TPC_ILD00.gdml (just the TPC from mokka model ILD00). The corresponding gear steering files are
          gearTGeo_ILD00.xml which is a full gear file and gear_TGeo.xml which just has the part to use the new 
          features.	
	  
          Test programs are available to run in the example directory:
	  ../build/bin/testMaterialBudget gear_TGeo.xml
	  Several test exist:
	  testGearTGeo: basic test for usage of  DistanceProperties and PointProperties with TGeo	
	  testMaterialMap: uses DistanceProperties to create a material scan in r, theta
	  testMaterialBudget: uses information from gear file to compute the envelopes of subdetectors and the
		              calculates the material budget before the TPC, of the TPC and up to th ECAL in 
                              dependence on theta. (needs full gear file with detectors = gearTGeo_ILD00.xml)
	  testMaterialMapFactory: example of how to use the MaterialMapFactory tool to create a MaterialMap in memory.
                                  This is usefull if an application (e.g. Pandora) has to querry the material budget 
 		                  for certain point many many times.


        * Imrovements and fixes for TPC -   M. Killenberg
           - implemented z-Position of TPCModule
           - improved performance of local<->global coordinate transformations in TPCModule
           - fixed bug in TPCModuleXML
          
           - FixedPadSizeDiskLayout now can be segment of a circle 
             changes 100% backward compatible  (Ching Bon Lam)
           - improved getNearestPad() in VersatileDiskRowLayout


 # v00-14-02

    * fixed bug in calculation of overall TPC plane extent (M.Killenberg)

 # v00-14-01  

    * added release notes (missing in v00-14)  

    * fixed generation of html documentation (was in ./doc/gear/html -> ./doc/html)

 # v00-14  
    * removed aid files and generation of header files
      - now pure C++

    * improved performance of TPCParametersImpl::getNearestPad() (beJason Abernathy)

    * generate html documentation in build directory 
      -> will be removed with 'make uninstall' 

 # v00-13-01  

    * gcc 4.4 compliant

 # v00-13  
     * fixed missing xml in/output for generic parameters in TPCParametersXML
       and TPCModuleXML
        - made testgear work with modular TPC
        - made testXMLOut example run
          (M. Killenberg)
     * bug fix: use class variable for _maxDriftLength and 
       _coordinateType - make polar coordinates default
          (F. Gaede)
     * bug fix: swapped getRightNeighbour / getLeftNeighbour in FixedPadSizeDiskLayout
          (M. Killenberg)

 # v00-12  
    changes added by M.Killenberg:
    - Added VersatileDiscRowLayout: new flexible pad layout for polar coordinates
    - Implemented modular TPC endplate functionality
    ( see doc/eudet_memo_GearModularTPC.pdf for documentation )

 # v00-11-01  
     bug fix release
	- fixed reading of lhcal/hcal parameters (hcal endcap had lhcal params...)
        - added FixedPadAngleDiskLayout: new padlayout for const-phi segments (M.Killenberg)

 # v00-11  
  - added parameters for BeamCal and LHcal:
        just like Lcal, BeamCal and LHcal sections in xml also need mandatory user
        parameter 'beam_crossing_angle' - enforced in GearMgr::setLcalParameters
        ( will also be written with MokkaGear)
  - cmake improvements:
    - added GEAR_LIBRARY_DIRS
    - added 32 bit compatibility build option
    - made cmake 2.6 compatible
	
 # v00-10  
   - added Yoke calorimeter parameters :
	   YokeBarrel, YokeEndcap and YokePlug
				   
 # v00-09-01
    - added tool to print detector parameters: printParameters


 # v00-09
  - added new attribute GearMgr::get(set)DetectorName()
    stored in <global detectorName="LDC01_06Sc"/> element
	-> can be used for automated consistency checks of gear file and lcio file
       throw UnknownParameterExeption if not set 

  - improved  RectangularPadRowLayout::getNearestPad()  - (J.Abernathy)


 # v00-08-03
   - bug fix release:
      - fixed bug in VXDParameters::isEqual() w/ small values and exact zeros

 # v00-08-02
   - bug fix release:

     - clarified definition of phi0 and offset in VCXDParameters:
       phi0: azimuthal angle of normal to first ladder
       offset: shift of ladder in direction of increasing phi

     - use old definition of phi0 internally)

     - changed sign of offset wrt. v00-08

 # v00-08  
   - added HcalRing and EcalPlug calorimeter parameters

 # v00-07  
   - no interface change wrt. v00-06

   - improved CMake files 

   - made compatible with MacOs
 
   - build the mergeXML tool by default
     -> added example file gear_merge_reco.xml
   
   - bug fixes:
     - 	 FixedPadSizeDiskLayout::getPadIndex
	 throws std::out_of_range if needed

     -   src/cpp/gearimpl/src/CalorimeterParametersImpl.cc: bug fix
	 K.Harder: getExtent: nLayer>0

     -  fixed minor memory leaks in globals/singletons

 # v00-06-01|

   - changed xml output format to scientific with precision 9: "%.9e"

 # v00-06  
  - added SiPlanes detector for pixel telescope
    (author T.Klimkovich, DESY)
 
  - bug fix for BField: warning if not present

  - improved CMake  - now default build tool

 
 # v00-05  
   - added BField map (only constant)
     ( double bfield = gearMgr->getBField().at( Vector3D(0,0,0) ).z() ; )

 # v00-04-02|
   patch release - identical to v00-04-01, except
   -  added support for cmake (experimental)

# v00-04-01|
   
  - patch release: reintroduce typedef for Point2D/3D

# v00-04 

   - replaced Point3D and Point2D from all interfaces - only Vector3D/Vector2D
     are used now. Kept Point3D/Point2D definition for backward compatibility.

   - added implementation of simple 3d vector with scalar product, addition,
     and coordinate transform (cartesian, cylindrical and spherical).
     This vetcor will also be used in a future LCIo release.

   - added parameters for 'Lcal' (Calorimeter type endcap symmetry=1) 
     as suggested by B.Pawlik. Lcal sections in xml need mandatory user 
     parameter 'beam_crossing_angle' - enforced in GearMgr::setLcalParameters
     ( will also be written with MokkaGear)	

# v00-03 

  - initial version of VXDParameters implementation
    -> see example file gear_ldc.xml

  - first version of gearcga - implementation of GearPointProperties 
    and GearDistanceProperties ( author: Gabriel Mussat )
    -> needs geant4 and Mokka installed 
    ( note: as Mokka depeneds on Gear gearcga can only be build in 
     a second  build process after Mokka has been built with the first
     build of Gear )
    -> see commented environment variables in env.sh

  - mergeXML: program to merge two XML files - can be used to 
    add missing (user) information to existing xml file, e.g
    a file created with MokkaGear

  - introduced RectangularPadRowLayout for TPC prototypes
    -> see example file gear_tpc_proto.xml

  - implemented Point2D and Point3D coherently
    
  - Point2D no longer is pair<double,double>
    ->  Point2D p ;
        p.first needs to be changed to p[0]  and 
        p.second needs to be changed to p[1]

  - updated (clarified) GearPointProperties and GearDistanceProperties
   

# v00-02  
  
  - first public version of GEAR
    
  - added calorimeter cell sizes


# v00-01-snowmass
  
 - pre-release for snowmass DVD
