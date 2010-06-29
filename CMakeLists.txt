#################################
# cmake file for building GEAR
# @author Jan Engels, DESY
#################################


########################################################
# CMake compatibility issues: don't modify this, please!
CMAKE_MINIMUM_REQUIRED( VERSION 2.4.6 )
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
SET( ${PROJECT_NAME}_MINOR_VERSION 14 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 0 )

# project options
OPTION( BUILD_SHARED_LIBS "Set to OFF to build static libraries" ON )
OPTION( BUILD_32BIT_COMPATIBLE "Set to ON to build in 32 bit compatibility mode" OFF )
OPTION( INSTALL_DOC "Set to OFF to skip build/install Documentation" ON )

# set default cmake build type to RelWithDebInfo
IF( NOT CMAKE_BUILD_TYPE )
    SET( CMAKE_BUILD_TYPE "RelWithDebInfo" )
ENDIF()

# set default install prefix to project root directory
IF( CMAKE_INSTALL_PREFIX STREQUAL "/usr/local" )
    SET( CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}" )
ENDIF()

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



# library *nix style versioning
SET( GEAR_SOVERSION "${GEAR_MAJOR_VERSION}.${GEAR_MINOR_VERSION}" )
SET( GEAR_VERSION "${GEAR_SOVERSION}.${GEAR_PATCH_LEVEL}" )

#---------- DOCUMENTATION -----------------------------------------------------
FIND_PACKAGE( Doxygen )
FIND_PACKAGE( LATEX )
IF( DOXYGEN_FOUND AND LATEX_COMPILER AND MAKEINDEX_COMPILER )

    ADD_CUSTOM_COMMAND(
        OUTPUT  "${PROJECT_BINARY_DIR}/doc/gear_banner.png"
        # first we copy directory from source tree to binary tree to
        # keep the source tree unchanged
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${PROJECT_SOURCE_DIR}/doc"
            "${PROJECT_BINARY_DIR}/doc"
        VERBATIM
    )

    ADD_CUSTOM_COMMAND(
        OUTPUT "${PROJECT_BINARY_DIR}/doc/html"
        COMMAND "${DOXYGEN_EXECUTABLE}"
        WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/doc"
        COMMENT "Building API Documentation..."
        DEPENDS "${PROJECT_BINARY_DIR}/doc/gear_banner.png" "${PROJECT_BINARY_DIR}/doc/overview.html" "${PROJECT_BINARY_DIR}/doc/Doxyfile"
    )
    
    ADD_CUSTOM_TARGET( doc DEPENDS
        "${PROJECT_BINARY_DIR}/doc/html" )

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

IF( INSTALL_DOC )

    # create documentation Doxyfile
    CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/doc/Doxyfile.in"
                    "${PROJECT_BINARY_DIR}/doc/Doxyfile" IMMEDIATE @ONLY )

    # create documentation overview.html
    CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/doc/overview.html.in"
                    "${PROJECT_BINARY_DIR}/doc/overview.html" IMMEDIATE @ONLY )

    # make sure doxygen is executed before make install
    #INSTALL( CODE "EXEC_PROGRAM(${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/doc)" )
    # by just calling "make doc" doxygen won't run if the documentation has already been built
    INSTALL( CODE "EXEC_PROGRAM(${CMAKE_BUILD_TOOL} ${PROJECT_BINARY_DIR} ARGS doc)" )
    
    # install documentation
    INSTALL( DIRECTORY "${PROJECT_BINARY_DIR}/doc/" DESTINATION "doc/gear"
        PATTERN "Doxyfile" EXCLUDE
        PATTERN "latex" EXCLUDE
        PATTERN "overview.html*" EXCLUDE
        PATTERN "*/.svn" EXCLUDE
        )

    # install examples
    INSTALL( DIRECTORY "${PROJECT_SOURCE_DIR}/example" DESTINATION "doc/gear" PATTERN "*/.svn" EXCLUDE )

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

# create symbolic lib target for calling target lib_XXX
ADD_CUSTOM_TARGET( lib )

# FIXME: check for 32/64 bit
ADD_DEFINITIONS( "-Wno-long-long" )
ADD_DEFINITIONS( "-Wall -ansi -pedantic" )


# -------- generate GearVersion.h ---------------------------------------------
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake/GearVersion.h.in"
    "${PROJECT_BINARY_DIR}/GearVersion.h" @ONLY )

INSTALL( FILES "${PROJECT_BINARY_DIR}/GearVersion.h" DESTINATION "include/gear/" )

INCLUDE_DIRECTORIES( "${PROJECT_BINARY_DIR}" )
# -----------------------------------------------------------------------------

INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}/include" )
INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}/include/gear" )

# install include files
INSTALL( DIRECTORY "${PROJECT_SOURCE_DIR}/include"
        DESTINATION .
        PATTERN "*~" EXCLUDE
        PATTERN "*/.svn" EXCLUDE )

INSTALL( CODE "EXEC_PROGRAM( ${CMAKE_COMMAND} ${PROJECT_SOURCE_DIR}/include ARGS -E create_symlink gear/gearimpl gearimpl)" )
INSTALL( CODE "EXEC_PROGRAM( ${CMAKE_COMMAND} ${PROJECT_SOURCE_DIR}/include ARGS -E create_symlink gear/gearxml gearxml)" )

ADD_SUBDIRECTORY( ./src )

# create uninstall configuration file 
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
                "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake"
                IMMEDIATE @ONLY )

# create uninstall target
ADD_CUSTOM_TARGET( uninstall
    "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake"
    # cmake 2.4 does not know remove_directory
    #COMMAND "${CMAKE_COMMAND}" -E remove_directory "${CMAKE_INSTALL_PREFIX}/doc/gear"
    COMMAND rm -rf "${CMAKE_INSTALL_PREFIX}/doc/gear"
    COMMAND "${CMAKE_COMMAND}" -E remove "${CMAKE_INSTALL_PREFIX}/include/gearimpl"
    COMMAND "${CMAKE_COMMAND}" -E remove "${CMAKE_INSTALL_PREFIX}/include/gearxml"
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
IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    MESSAGE( STATUS "BUILD_32BIT_COMPATIBLE = ${BUILD_32BIT_COMPATIBLE}" )
ENDIF()
MESSAGE( STATUS "INSTALL_DOC = ${INSTALL_DOC}" )
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

# export build settings
INCLUDE( CMakeExportBuildSettings )
CMAKE_EXPORT_BUILD_SETTINGS( "GEARBuildSettings.cmake" )
INSTALL( FILES "${PROJECT_BINARY_DIR}/GEARBuildSettings.cmake" DESTINATION lib/cmake )

# export library dependencies (keep this as the last line in the file)
EXPORT_LIBRARY_DEPENDENCIES( "GEARLibDeps.cmake" )
INSTALL( FILES "${PROJECT_BINARY_DIR}/GEARLibDeps.cmake" DESTINATION lib/cmake )

