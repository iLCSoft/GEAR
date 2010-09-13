
##################################
# cmake file for building GEAR
# @author Jan Engels, DESY
##################################


###### LIBGEAR (IMPL) #####################################################

# get list of all source files
AUX_SOURCE_DIRECTORY( ./gearimpl lib_srcs )

# create library with the list of library sources
ADD_LIBRARY( lib_GEAR ${lib_srcs} )

# cmake 2.4.6 cannot find the lib target
IF( "${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" GREATER 2.4.6 )
    # tell lib target to call libGEAR
    ADD_DEPENDENCIES( lib lib_GEAR )
ELSE()
    ADD_CUSTOM_TARGET( lib DEPENDS lib_GEAR )
ENDIF()

# change lib_target properties
SET_TARGET_PROPERTIES( lib_GEAR PROPERTIES
    # create *nix style library versions + symbolic links
    VERSION ${GEAR_VERSION}
    SOVERSION ${GEAR_SOVERSION}
    # allow building static and shared libraries on the same build tree
    CLEAN_DIRECT_OUTPUT 1
    # avoid conflicts between library and binary target names
    OUTPUT_NAME gear )

# install library
INSTALL( TARGETS lib_GEAR DESTINATION lib PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE )


###### LIBGEARXML #########################################################

# get list of all source files
AUX_SOURCE_DIRECTORY( gearxml libxml_srcs )

# create library with the list of library sources
ADD_LIBRARY( lib_GEARXML ${libxml_srcs} )

# cmake 2.4.6 cannot find the lib target
IF( "${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" GREATER 2.4.6 )
    # tell lib target to call libGEARXML
    ADD_DEPENDENCIES( lib lib_GEARXML )
ELSE()
    ADD_CUSTOM_TARGET( lib DEPENDS lib_GEARXML )
ENDIF()

IF( BUILD_TGEO )
    TARGET_LINK_LIBRARIES( lib_GEARXML lib_GEAR lib_GEARTGEO )
ELSE()
    TARGET_LINK_LIBRARIES( lib_GEARXML lib_GEAR )
ENDIF()

# change lib_target properties
SET_TARGET_PROPERTIES( lib_GEARXML PROPERTIES
    # create *nix style library versions + symbolic links
    VERSION ${GEAR_VERSION}
    SOVERSION ${GEAR_SOVERSION}
    # allow building static and shared libraries on the same build tree
    CLEAN_DIRECT_OUTPUT 1
    # avoid conflicts between library and binary target names
    OUTPUT_NAME gearxml )

# install library
INSTALL( TARGETS lib_GEARXML DESTINATION lib PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE )


###### LIBGEARTGEO #########################################################

IF( BUILD_TGEO )
    # get list of all source files
    AUX_SOURCE_DIRECTORY( geartgeo libtgeo_srcs )

    # create library with the list of library sources
    ADD_LIBRARY( lib_GEARTGEO ${libtgeo_srcs} )

    # cmake 2.4.6 cannot find the lib target
    IF( "${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" GREATER 2.4.6 )
        # tell lib target to call libGEARXML
        ADD_DEPENDENCIES( lib lib_GEARTGEO )
    ELSE()
        ADD_CUSTOM_TARGET( lib DEPENDS lib_GEARTGEO )
    ENDIF()

    # link geartgeo library with gear
    TARGET_LINK_LIBRARIES( lib_GEARTGEO lib_GEAR )

    # change lib_target properties
    SET_TARGET_PROPERTIES( lib_GEARTGEO PROPERTIES
        # create *nix style library versions + symbolic links
        VERSION ${GEAR_VERSION}
        SOVERSION ${GEAR_SOVERSION}
        # allow building static and shared libraries on the same build tree
        CLEAN_DIRECT_OUTPUT 1
        # avoid conflicts between library and binary target names
        OUTPUT_NAME geartgeo )

    # install library
    INSTALL( TARGETS lib_GEARTGEO DESTINATION lib PERMISSIONS
            OWNER_READ OWNER_WRITE OWNER_EXECUTE
            GROUP_READ GROUP_EXECUTE
            WORLD_READ WORLD_EXECUTE )
ENDIF()





###### TESTS #########################################################


# mergeXML
ADD_EXECUTABLE( mergeXML ./test/mergeXML.cc )
TARGET_LINK_LIBRARIES( mergeXML lib_GEAR lib_GEARXML )
INSTALL( TARGETS mergeXML DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

# printParameters
ADD_EXECUTABLE( printParameters ./test/printParameters.cc )
TARGET_LINK_LIBRARIES( printParameters lib_GEAR lib_GEARXML )
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

IF( BUILD_TGEO )
    SET( test_bins ${test_bins} testGearTGeo testMaterialBudget testMaterialMapFactory)
ENDIF()

ADD_CUSTOM_TARGET( tests )

FOREACH( bin ${test_bins} )
    ADD_EXECUTABLE( ${bin} EXCLUDE_FROM_ALL ./test/${bin}.cc )
    ADD_DEPENDENCIES( tests ${bin} )
    TARGET_LINK_LIBRARIES( ${bin} lib_GEAR lib_GEARXML )
ENDFOREACH()


