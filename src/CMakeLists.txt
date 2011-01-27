
##################################
# cmake file for building GEAR
# @author Jan Engels, DESY
##################################

# common definitions to ALL sources
ADD_DEFINITIONS( "-Wno-long-long" )
ADD_DEFINITIONS( "-Wall -ansi -pedantic" )

IF(APPLE)
 ADD_DEFINITIONS( "-DAPPLE" )
ENDIF(APPLE)
# common includes (to ALL sources)
INCLUDE_DIRECTORIES( "${PROJECT_BINARY_DIR}" ) # generated GearConfig.h
INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}/include" )
INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}/include/gear" )



###### LIBRARIES #####################################################

# -------- libgear (impl) ----------------------------
AUX_SOURCE_DIRECTORY( ./gearimpl libgear_srcs )
ADD_SHARED_LIBRARY( gear ${libgear_srcs} )
# ----------------------------------------------------

# -------- libgeartgeo -------------------------------
IF( GEAR_TGEO )
    ADD_SUBDIRECTORY( ./geartgeo )
ENDIF()
# ----------------------------------------------------


# -------- libgearxml --------------------------------
AUX_SOURCE_DIRECTORY( ./gearxml libgearxml_srcs )
ADD_SHARED_LIBRARY( gearxml ${libgearxml_srcs} )
TARGET_LINK_LIBRARIES( gearxml gear ${CMAKE_DL_LIBS} )

INSTALL( TARGETS gear gearxml DESTINATION lib )


#IF( GEAR_TGEO )
#    # link gearxml to geartgeo
#    TARGET_LINK_LIBRARIES( gearxml geartgeo )
#ENDIF()
# ----------------------------------------------------



###### TESTS #########################################################

# mergeXML
ADD_EXECUTABLE( mergeXML ./test/mergeXML.cc )
TARGET_LINK_LIBRARIES( mergeXML gear gearxml )
INSTALL( TARGETS mergeXML DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

# printParameters
ADD_EXECUTABLE( printParameters ./test/printParameters.cc )
TARGET_LINK_LIBRARIES( printParameters gear gearxml )
INSTALL( TARGETS printParameters DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

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
)

IF( GEAR_TGEO )
    SET( test_bins ${test_bins} testGearTGeo ) #testMaterialBudget testMaterialMap testMaterialMapFactory )
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

