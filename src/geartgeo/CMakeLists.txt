##################################
# cmake file for building GEARTGEO
# @author Astrid Muennich, CERN
##################################

# project name
PROJECT( GEARTGEO )

## project dependencies
SET( ${PROJECT_NAME}_DEPENDS "ROOT" )
#SET( ILC_HOME "/afs/cern.ch/eng/clic/work/sailer/sl5/ilcsoft/v01-06afs" CACHE PATH "Path to ILC Software" FORCE)
#SET( ILC_HOME "/afs/cern.ch/eng/clic/work/sailer/sl5/ilcsoft/v01-06afs" CACHE PATH "Path to ILC Software" FORCE)
#SET( ROOT_HOME "/afs/cern.ch/sw/lcg/app/releases/ROOT/5.22.00b/x86_64-slc5-gcc43-dbg/root" CACHE PATH "Path to ROOT" FORCE)
#SET( CMAKE_MODULE_PATH "${ILC_HOME}/CMakeModules/v01-09" CACHE PATH "Path to CMakeModules" FORCE)

# additional ROOT components
SET( ROOT_USE_COMPONENTS Gdml Geom )

# get list of all source files
AUX_SOURCE_DIRECTORY( . lib_srcs )

# create library with the list of library sources
ADD_LIBRARY( lib_GEARTGEO ${lib_srcs} )

# cmake 2.4.6 cannot find the lib target
IF( "${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" GREATER 2.4.6 )
    # tell lib target to call libGEARTGEO
    ADD_DEPENDENCIES( lib lib_GEARTGEO )
ELSE()
    ADD_CUSTOM_TARGET( lib DEPENDS lib_GEARTGEO )
ENDIF()

# DEPENDENCIES: this code has to be placed before adding any library or
# executable so that these are linked properly against the dependencies
IF( DEFINED ${PROJECT_NAME}_DEPENDS OR DEFINED BUILD_WITH OR DEFINED LINK_WITH )
    IF( NOT DEFINED CMAKE_MODULE_PATH )
        MESSAGE( FATAL_ERROR "CMAKE_MODULE_PATH not set! Set it with: "
            "-DCMAKE_MODULE_PATH=\"/path_to_ilcsoft/CMakeModules\"" )
    ENDIF()
    # load macro
    INCLUDE( "MacroCheckDeps" )
    CHECK_DEPS()
ENDIF()

# link gear library with gear
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



