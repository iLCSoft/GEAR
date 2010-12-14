########################################################
# cmake file for building GEAR
# @author Jan Engels, Desy IT
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)
########################################################


# project name
PROJECT( GEAR )

# project version
SET( ${PROJECT_NAME}_VERSION_MAJOR 0 )
SET( ${PROJECT_NAME}_VERSION_MINOR 16 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )
SET( ${PROJECT_NAME}_VERSION_TWEAK 0 )



### SETTINGS ################################################################

# additionally search for cmake macros/moules defined by this project
LIST( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )
INCLUDE( ilcsoft_default_settings )



### DEPENDENCIES ############################################################

# root is needed to build tgeo
OPTION( GEAR_TGEO "Set to ON to build tgeo (ROOT >= 5.27.06 required)" OFF )

# for backwards compatibility
IF( DEFINED BUILD_TGEO )
    SET( GEAR_TGEO ${BUILD_TGEO} )
ENDIF()

IF( GEAR_TGEO )
    
    # fix for backwards compatibility
    IF( NOT ROOT_DIR AND DEFINED ROOT_HOME )
        SET( ROOT_DIR "${ROOT_HOME}" )
    ENDIF()

    # at least version 5.27.06 of ROOT is required for using GEAR_TGEO
    SET( ROOT_VERSION_CHECK_MINIMUM_REQUIRED TRUE )
    FIND_PACKAGE( ROOT 5.27.06 REQUIRED COMPONENTS Gdml Geom XMLIO )

ENDIF( GEAR_TGEO )



### DOCUMENTATION ###########################################################

OPTION( INSTALL_DOC "Set to OFF to skip build/install Documentation" ON )

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



#############################################################################


# -------- generate GearConfig.h ----------------------------------------------
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake/GearConfig.h.cmake.in"
    "${PROJECT_BINARY_DIR}/gear/GearConfig.h" @ONLY )

INSTALL( FILES "${PROJECT_BINARY_DIR}/gear/GearConfig.h" DESTINATION "include/gear/" )
# -----------------------------------------------------------------------------


ADD_SUBDIRECTORY( ./src )


# install include files
INSTALL( DIRECTORY include DESTINATION . FILES_MATCHING PATTERN "*.h" )



# ------- symlinks in include directory for backwards compatibility ---------
# this symlinks are needed for Mokka 
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

MESSAGE( STATUS "INSTALL_DOC = ${INSTALL_DOC}" )
MESSAGE( STATUS "GEAR_TGEO = ${GEAR_TGEO}" )
MESSAGE( STATUS )



# export library dependencies (keep this as the last line in the file)
EXPORT_LIBRARY_DEPENDENCIES( "${PROJECT_NAME}LibDeps.cmake" )
INSTALL( FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}LibDeps.cmake" DESTINATION lib/cmake )

