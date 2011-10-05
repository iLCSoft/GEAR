########################################################
# cmake file for building GEAR
# @author Jan Engels, Desy IT
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)
########################################################


# project name
PROJECT( GEAR )


# project version
SET( ${PROJECT_NAME}_VERSION_MAJOR 1 )
SET( ${PROJECT_NAME}_VERSION_MINOR 1 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )



### DEPENDENCIES ############################################################

FIND_PACKAGE( ILCUTIL COMPONENTS ILCSOFT_CMAKE_MODULES REQUIRED )

# load default settings from ILCSOFT_CMAKE_MODULES
INCLUDE( ilcsoft_default_settings )


# root is needed to build tgeo
OPTION( GEAR_TGEO "Set to ON to build tgeo (ROOT >= 5.27.06 required)" OFF )

# for backwards compatibility
IF( DEFINED BUILD_TGEO )
    SET( GEAR_TGEO ${BUILD_TGEO} )
ENDIF()

IF( GEAR_TGEO )

    # at least version 5.27.06 of ROOT is required for using GEAR_TGEO
    FIND_PACKAGE( ROOT 5.27.06 REQUIRED COMPONENTS Gdml Geom XMLIO )

ENDIF( GEAR_TGEO )



### DOCUMENTATION ###########################################################

OPTION( INSTALL_DOC "Set to OFF to skip build/install Documentation" OFF )

IF( INSTALL_DOC )

    FIND_PACKAGE( Doxygen )

    IF( DOXYGEN_FOUND )
        ADD_SUBDIRECTORY( ./doc )
    ELSE()
        MESSAGE( SEND_ERROR "Could not find doxygen required to build documentation" )
        MESSAGE( "Please install doxygen or set INSTALL_DOC to OFF" )
        MESSAGE( "" )
    ENDIF()

ENDIF()



#############################################################################


# -------- generate GearConfig.h ----------------------------------------------
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake/GearConfig.h.cmake.in"
    "${PROJECT_BINARY_DIR}/gear/GearConfig.h" @ONLY )

INSTALL( FILES "${PROJECT_BINARY_DIR}/gear/GearConfig.h" DESTINATION "include/gear/" )
# -----------------------------------------------------------------------------


ADD_SUBDIRECTORY( ./src )


# install include files
INSTALL_DIRECTORY( ./include DESTINATION . FILES_MATCHING PATTERN "*.h" )



# ------- symlinks in include directory for backwards compatibility ---------
# FIXME: this symlinks are needed to build Mokka. Should fix this code in Mokka
INSTALL( CODE "EXEC_PROGRAM( \"${CMAKE_COMMAND}\" \"${CMAKE_INSTALL_PREFIX}/include\" ARGS -E create_symlink gear/gearimpl gearimpl )" )
INSTALL( CODE "EXEC_PROGRAM( \"${CMAKE_COMMAND}\" \"${CMAKE_INSTALL_PREFIX}/include\" ARGS -E create_symlink gear/gearxml gearxml )" )

IF( GEAR_TGEO )
    INSTALL( CODE "EXEC_PROGRAM( \"${CMAKE_COMMAND}\" \"${CMAKE_INSTALL_PREFIX}/include\" ARGS -E create_symlink gear/geartgeo geartgeo )" )
ENDIF()

ADD_CUSTOM_TARGET( uninstall_links
    COMMAND "${CMAKE_COMMAND}" -E remove "${CMAKE_INSTALL_PREFIX}/include/gearimpl"
    COMMAND "${CMAKE_COMMAND}" -E remove "${CMAKE_INSTALL_PREFIX}/include/gearxml"
    COMMAND "${CMAKE_COMMAND}" -E remove "${CMAKE_INSTALL_PREFIX}/include/geartgeo"
)

ADD_DEPENDENCIES( uninstall uninstall_links )
# ---------------------------------------------------------------------------



# display some variables and write them to cache
DISPLAY_STD_VARIABLES()

MESSAGE( STATUS "GEAR_TGEO = ${GEAR_TGEO}" )
MESSAGE( STATUS )


# generate and install following configuration files
GENERATE_PACKAGE_CONFIGURATION_FILES( GEARConfig.cmake GEARConfigVersion.cmake GEARLibDeps.cmake )

