#################################
# cmake file for building GEAR
# @author Jan Engels, DESY
#################################


########################################################
# CMake compatibility issues: don't modify this, please!
CMAKE_MINIMUM_REQUIRED( VERSION 2.6.2 )
#SET( CMAKE_BACKWARDS_COMPATIBILITY 2.4.6 )
MARK_AS_ADVANCED(CMAKE_BACKWARDS_COMPATIBILITY)
# allow more human readable "if then else" constructs
SET( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE )
########################################################


# project name
PROJECT( GEAR )

# needed for using ctest
ENABLE_TESTING()
INCLUDE(CTest)

# project version
SET( ${PROJECT_NAME}_MAJOR_VERSION 0 )
SET( ${PROJECT_NAME}_MINOR_VERSION 15 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 0 )

# project options
OPTION( BUILD_SHARED_LIBS "Set to OFF to build static libraries" ON )
OPTION( BUILD_32BIT_COMPATIBLE "Set to ON to build in 32 bit compatibility mode" OFF )
OPTION( BUILD_TGEO "Set to ON to build tgeo" OFF )
OPTION( INSTALL_DOC "Set to OFF to skip build/install Documentation" ON )

# set default cmake build type to RelWithDebInfo
IF( NOT CMAKE_BUILD_TYPE )
    SET( CMAKE_BUILD_TYPE "RelWithDebInfo" )
ENDIF()

# set default install prefix to project root directory
IF( CMAKE_INSTALL_PREFIX STREQUAL "/usr/local" )
    SET( CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}" )
ENDIF()


# -----------------------------------------------------------------------

# library *nix style versioning
SET( GEAR_SOVERSION "${GEAR_MAJOR_VERSION}.${GEAR_MINOR_VERSION}" )
SET( GEAR_VERSION "${GEAR_SOVERSION}.${GEAR_PATCH_LEVEL}" )



#---------------- 32/64 bit issues ---------------------------------------
IF( CMAKE_SIZEOF_VOID_P EQUAL 4 )
    MESSAGE( STATUS "32 bit architecture detected" )
ENDIF()

IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    MESSAGE( STATUS "64 bit architecture detected" )

    IF( BUILD_32BIT_COMPATIBLE )
        IF( COMMAND SET_PROPERTY )
            SET_PROPERTY(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS 0)
            SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32" )
        ELSE()
            MESSAGE( FATAL_ERROR "to build in 32 bit compatibility mode you need cmake >= 2.6" )
        ENDIF()
        MESSAGE( STATUS "Build in 32 bit compatibility mode" )
    ELSE()
        MESSAGE( STATUS "Build in native 64 bit mode" )
    ENDIF()
ENDIF()
#-------------------------------------------------------------------------



#---------- DOCUMENTATION -----------------------------------------------------
IF( INSTALL_DOC )

    FIND_PACKAGE( Doxygen )
    FIND_PACKAGE( LATEX )

    IF( DOXYGEN_FOUND AND LATEX_COMPILER AND MAKEINDEX_COMPILER )
        ADD_SUBDIRECTORY( ./doc )
    ELSE()
        IF( NOT DOXYGEN_FOUND )
            MESSAGE( STATUS "Doxygen not found in your system!!" )
        ENDIF()
        IF( NOT LATEX_COMPILER OR NOT MAKEINDEX_COMPILER )
            MESSAGE( STATUS "Latex not found in your system!!" )
        ENDIF()
        IF( INSTALL_DOC )
            MESSAGE( STATUS "INSTALL_DOC forced to OFF" )
            SET( INSTALL_DOC OFF )
        ENDIF()
    ENDIF()
ENDIF()
#------------------------------------------------------------------------------



#---------- rpath -----------------------------------------------------
# add install path to the rpath list
SET( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )
MARK_AS_ADVANCED( CMAKE_INSTALL_RPATH )

# add install path to the rpath list (apple)
IF( APPLE )
    SET( CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib" )
    MARK_AS_ADVANCED( CMAKE_INSTALL_NAME_DIR )
ENDIF()

# append link pathes to rpath list
SET( CMAKE_INSTALL_RPATH_USE_LINK_PATH 1 )
MARK_AS_ADVANCED( CMAKE_INSTALL_RPATH_USE_LINK_PATH )
#----------------------------------------------------------------------


# output directories
SET( EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin" CACHE PATH
    "EXECUTABLE_OUTPUT_PATH" FORCE )
SET( LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib" CACHE PATH
    "LIBRARY_OUTPUT_PATH" FORCE )
MARK_AS_ADVANCED( EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH )


# -------- generate GearVersion.h ---------------------------------------------
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake/GearVersion.h.in"
    "${PROJECT_BINARY_DIR}/GearVersion.h" @ONLY )

INSTALL( FILES "${PROJECT_BINARY_DIR}/GearVersion.h" DESTINATION "include/gear/" )
# -----------------------------------------------------------------------------

# install include files
INSTALL( DIRECTORY "${PROJECT_SOURCE_DIR}/include" DESTINATION .
        PATTERN "*~" EXCLUDE
        PATTERN "*/.svn" EXCLUDE
)

# create symlinks in include directory
INSTALL( CODE "EXEC_PROGRAM( \"${CMAKE_COMMAND}\" \"${CMAKE_INSTALL_PREFIX}/include\" ARGS -E create_symlink gear/gearimpl gearimpl )" )
INSTALL( CODE "EXEC_PROGRAM( \"${CMAKE_COMMAND}\" \"${CMAKE_INSTALL_PREFIX}/include\" ARGS -E create_symlink gear/gearxml gearxml )" )
IF( BUILD_TGEO )
    INSTALL( CODE "EXEC_PROGRAM( \"${CMAKE_COMMAND}\" \"${CMAKE_INSTALL_PREFIX}/include\" ARGS -E create_symlink gear/geartgeo geartgeo )" )
ENDIF()


ADD_SUBDIRECTORY( ./src )


# create uninstall configuration file 
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
                "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake"
                IMMEDIATE @ONLY )

# create uninstall target
ADD_CUSTOM_TARGET( uninstall
    "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake"
    COMMAND "${CMAKE_COMMAND}" -E remove_directory "${CMAKE_INSTALL_PREFIX}/doc/html"
    COMMAND "${CMAKE_COMMAND}" -E remove_directory "${CMAKE_INSTALL_PREFIX}/doc/example"
    COMMAND "${CMAKE_COMMAND}" -E remove "${CMAKE_INSTALL_PREFIX}/include/gearimpl"
    COMMAND "${CMAKE_COMMAND}" -E remove "${CMAKE_INSTALL_PREFIX}/include/gearxml"
    COMMAND "${CMAKE_COMMAND}" -E remove "${CMAKE_INSTALL_PREFIX}/include/geartgeo"
)

# create configuration file from .in file
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake/GEARConfig.cmake.in"
                "${PROJECT_BINARY_DIR}/GEARConfig.cmake" @ONLY )

# install configuration file
INSTALL( FILES "${PROJECT_BINARY_DIR}/GEARConfig.cmake" DESTINATION . )


# display status message for important variables
MESSAGE( STATUS )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS "BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}" )
MESSAGE( STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}" )
MESSAGE( STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )
#IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
#    MESSAGE( STATUS "BUILD_32BIT_COMPATIBLE = ${BUILD_32BIT_COMPATIBLE}" )
#ENDIF()
MESSAGE( STATUS "INSTALL_DOC = ${INSTALL_DOC}" )
MESSAGE( STATUS "BUILD_TGEO = ${BUILD_TGEO}" )
MESSAGE( STATUS "To change a value type: cmake -D<Variable>=<Value>" )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS )

# force some variables that could be defined in the command line
# to be written to cache
SET( BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS}" CACHE BOOL
    "Set to OFF to build static libraries" FORCE )
SET( CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}" CACHE PATH
    "Where to install GEAR" FORCE )
SET( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE )
SET( INSTALL_DOC "${INSTALL_DOC}" CACHE BOOL
    "Set to OFF to skip build/install Documentation" FORCE )
SET( BUILD_TGEO "${BUILD_TGEO}" CACHE BOOL
    "Set to ON to build tgeo" FORCE )

# export library dependencies (keep this as the last line in the file)
EXPORT_LIBRARY_DEPENDENCIES( "GEARLibDeps.cmake" )
INSTALL( FILES "${PROJECT_BINARY_DIR}/GEARLibDeps.cmake" DESTINATION lib/cmake )

