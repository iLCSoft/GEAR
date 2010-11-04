
##################################
# cmake file for building GEAR
# @author Jan Engels, DESY
##################################

# =========== macro for adding gear library ===============================
ADD_CUSTOM_TARGET( lib )
MACRO( ADD_GEAR_LIBRARY LIBNAME )

    # ARGN contains all arguments after LIBNAME
    ADD_LIBRARY( ${LIBNAME} ${ARGN} )

    # 'make lib' also builds this library
    ADD_DEPENDENCIES( lib ${LIBNAME} )

    # create *nix style library versions + symbolic links
    SET_TARGET_PROPERTIES( ${LIBNAME} PROPERTIES
        VERSION ${GEAR_VERSION}
        SOVERSION ${GEAR_SOVERSION}
        # allow building static and shared libraries on the same build tree
        CLEAN_DIRECT_OUTPUT 1
    )

    # install library
    INSTALL( TARGETS ${LIBNAME} DESTINATION lib PERMISSIONS
            OWNER_READ OWNER_WRITE OWNER_EXECUTE
            GROUP_READ GROUP_EXECUTE
            WORLD_READ WORLD_EXECUTE )

ENDMACRO()
# =========================================================================


# common definitions to ALL sources
ADD_DEFINITIONS( "-Wno-long-long" )
ADD_DEFINITIONS( "-Wall -ansi -pedantic" )

# common includes (to ALL sources)
INCLUDE_DIRECTORIES( "${PROJECT_BINARY_DIR}" ) # generated GearVersion.h
INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}/include" )
INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}/include/gear" )



###### LIBRARIES #####################################################

# -------- libgear (impl) ----------------------------
AUX_SOURCE_DIRECTORY( ./gearimpl libgear_srcs )
ADD_GEAR_LIBRARY( gear ${libgear_srcs} )
# ----------------------------------------------------

IF( BUILD_TGEO )

    #IF( NOT DEFINED CMAKE_MODULE_PATH )
    #    MESSAGE( STATUS "CMAKE_MODULE_PATH not set! Set it with: "
    #        "-DCMAKE_MODULE_PATH=\"/path_to_ilcsoft/CMakeModules\"" )
    #ENDIF()

    #SET( ROOT_USE_COMPONENTS Gdml Geom XMLIO)
    FIND_PACKAGE( ROOT REQUIRED COMPONENTS Gdml Geom XMLIO )

    IF( NOT ROOT_GDML_FOUND OR NOT ROOT_GEOM_FOUND OR NOT ROOT_XMLIO_FOUND )
        MESSAGE( FATAL_ERROR "failed to find required ROOT components (Gdml Geom XMLIO)" )
    ENDIF()

    # FIXME should only include ROOT_INCLUDE_DIRS for tgeo sources
    INCLUDE_DIRECTORIES( ${ROOT_INCLUDE_DIRS} )

    # -------- libgeartgeo -------------------------------
    AUX_SOURCE_DIRECTORY( ./geartgeo libgeartgeo_srcs )
    ADD_GEAR_LIBRARY( geartgeo ${libgeartgeo_srcs} )
    TARGET_LINK_LIBRARIES( geartgeo gear )
    TARGET_LINK_LIBRARIES( geartgeo ${ROOT_LIBRARIES} )
    # ----------------------------------------------------

ENDIF()


# -------- libgearxml --------------------------------
AUX_SOURCE_DIRECTORY( ./gearxml libgearxml_srcs )
ADD_GEAR_LIBRARY( gearxml ${libgearxml_srcs} )
TARGET_LINK_LIBRARIES( gearxml gear )
IF( BUILD_TGEO )

    # enable tgeo source code in gearxml
    SET_SOURCE_FILES_PROPERTIES( ${libgearxml_srcs}
        PROPERTIES COMPILE_FLAGS "-DGEAR_TGEO" )

    # link gearxml to geartgeo
    TARGET_LINK_LIBRARIES( gearxml geartgeo )
ENDIF()
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

IF( BUILD_TGEO )
    SET( test_bins ${test_bins} testGearTGeo testMaterialBudget testMaterialMap testMaterialMapFactory )
ENDIF()

ADD_CUSTOM_TARGET( tests )

FOREACH( bin ${test_bins} )
    ADD_EXECUTABLE( ${bin} EXCLUDE_FROM_ALL ./test/${bin}.cc )
    ADD_DEPENDENCIES( tests ${bin} )
    TARGET_LINK_LIBRARIES( ${bin} gear gearxml )
ENDFOREACH()

