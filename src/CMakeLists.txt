
##################################
# cmake file for building GEAR
# @author Jan Engels, DESY
##################################

# common definitions to ALL sources
ADD_DEFINITIONS( "-Wno-long-long" )
ADD_DEFINITIONS( "-Wall -ansi -pedantic" )

IF(APPLE)
    ADD_DEFINITIONS( "-D__APPLE__" )
ENDIF(APPLE)

# common includes (to ALL sources)
INCLUDE_DIRECTORIES( BEFORE "${PROJECT_BINARY_DIR}" ) # generated GearConfig.h
INCLUDE_DIRECTORIES( BEFORE "${PROJECT_SOURCE_DIR}/include" )
INCLUDE_DIRECTORIES( BEFORE "${PROJECT_SOURCE_DIR}/include/gear" )



###### LIBRARIES #####################################################

# -------- libgearsurf -------------------------------
INCLUDE_DIRECTORIES( ${CLHEP_INCLUDE_DIRS} ) # needed for gearsurf only

AUX_SOURCE_DIRECTORY( ./gearsurf libgearsurf_srcs )
ADD_SHARED_LIBRARY( gearsurf ${libgearsurf_srcs} )
TARGET_LINK_LIBRARIES( gearsurf ${CLHEP_LIBRARIES} )
INSTALL_SHARED_LIBRARY( gearsurf DESTINATION lib )
# ----------------------------------------------------


# -------- libgear (impl) ----------------------------
AUX_SOURCE_DIRECTORY( ./gearimpl libgear_srcs )
ADD_SHARED_LIBRARY( gear ${libgear_srcs} )
INSTALL_SHARED_LIBRARY( gear DESTINATION lib )
TARGET_LINK_LIBRARIES( gear gearsurf )
# ----------------------------------------------------

# -------- libgeartgeo -------------------------------
IF( GEAR_TGEO )
    ADD_SUBDIRECTORY( ./geartgeo )
ENDIF()
# ----------------------------------------------------

# -------- libgearxml --------------------------------
AUX_SOURCE_DIRECTORY( ./gearxml libgearxml_srcs )
ADD_SHARED_LIBRARY( gearxml ${libgearxml_srcs} )
INSTALL_SHARED_LIBRARY( gearxml DESTINATION lib )
TARGET_LINK_LIBRARIES( gearxml gear ${CMAKE_DL_LIBS} )


#IF( GEAR_TGEO )
#    # link gearxml to geartgeo
#    TARGET_LINK_LIBRARIES( gearxml geartgeo )
#ENDIF()
# ----------------------------------------------------



###### TESTS #########################################################

# mergeXML
ADD_EXECUTABLE( mergeXML ./test/mergeXML.cc )
TARGET_LINK_LIBRARIES( mergeXML gear gearxml )

# printParameters
ADD_EXECUTABLE( printParameters ./test/printParameters.cc )
TARGET_LINK_LIBRARIES( printParameters gear gearxml )

INSTALL( TARGETS mergeXML printParameters DESTINATION bin )

# test binaries
SET( test_bins
    testgear
    testtpcproto
    testVTXgear
    testSiPlanesgear    
    testAngularLayout
    testDistanceToModule
    testGearMgr
    testIsInsideModule
    testPlaneExtent
    testTPCModule
    testVersatileLayout
    testXMLOut
    testXMLCopy
)

IF( GEAR_TGEO )
  SET( test_bins ${test_bins} testGearTGeo testMaterialBudgetNew testMaterialBudget testMaterialMap ) #testMaterialMapFactory )
ENDIF()

ADD_CUSTOM_TARGET( tests )

FOREACH( _testname ${test_bins} )
    
    IF( BUILD_TESTING )
        ADD_EXECUTABLE( ${_testname} ./test/${_testname}.cc )
    ELSE()
        ADD_EXECUTABLE( ${_testname} EXCLUDE_FROM_ALL ./test/${_testname}.cc )
    ENDIF()

    ADD_DEPENDENCIES( tests ${_testname} )
    TARGET_LINK_LIBRARIES( ${_testname} gear gearxml )

ENDFOREACH()


### FTD test needs ROOT/TGeo ######

FIND_PACKAGE( ROOT COMPONENTS Geom )

IF( ROOT_FOUND )
  INCLUDE_DIRECTORIES( ${ROOT_INCLUDE_DIRS} )

  IF( BUILD_TESTING )
     ADD_EXECUTABLE( testFTD ./test/testFTD.cc )
  ELSE()
     ADD_EXECUTABLE( testFTD EXCLUDE_FROM_ALL ./test/testFTD.cc )
  ENDIF()

  ADD_DEPENDENCIES( tests testFTD )
  TARGET_LINK_LIBRARIES( testFTD   gear gearxml  ${ROOT_LIBRARIES} ${ROOT_COMPONENT_LIBRARIES} )
ENDIF()



