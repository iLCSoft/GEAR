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

# project version
SET( ${PROJECT_NAME}_MAJOR_VERSION 0 )
SET( ${PROJECT_NAME}_MINOR_VERSION 7 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 0 )

# project options
OPTION( BUILD_SHARED_LIBS "Set to OFF to build static libraries" ON )
OPTION( BUILD_GEAR_TESTS "Set to ON to build GEAR tests" OFF )
OPTION( INSTALL_DOC "Set to OFF to skip build/install Documentation" ON )

# set default cmake build type to RelWithDebInfo
IF( NOT CMAKE_BUILD_TYPE )
    SET( CMAKE_BUILD_TYPE "RelWithDebInfo" )
ENDIF()

# set default install prefix to project root directory
IF( CMAKE_INSTALL_PREFIX STREQUAL "/usr/local" )
    SET( CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}" )
ENDIF()

# library *nix style versioning
SET( ${PROJECT_NAME}_SOVERSION
    "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}" )
SET( ${PROJECT_NAME}_VERSION
    "${${PROJECT_NAME}_SOVERSION}.${${PROJECT_NAME}_PATCH_LEVEL}" )

# create symbolic link for include files
IF( NOT EXISTS "${PROJECT_SOURCE_DIR}/src/cpp/include/gearxml" )
    MESSAGE( STATUS "Creating symlink: ${PROJECT_SOURCE_DIR}/src/cpp/include/gearxml..." )
    EXEC_PROGRAM( "${CMAKE_COMMAND}"
        "${PROJECT_SOURCE_DIR}/src/cpp/include"
        ARGS -E create_symlink "${PROJECT_SOURCE_DIR}/src/cpp/gearxml/include" "gearxml"
        OUTPUT_VARIABLE out_tmp )
ENDIF()
IF( NOT EXISTS "${PROJECT_SOURCE_DIR}/src/cpp/include/gearimpl" )
    MESSAGE( STATUS "Creating symlink: ${PROJECT_SOURCE_DIR}/src/cpp/include/gearimpl..." )
    EXEC_PROGRAM( "${CMAKE_COMMAND}"
        "${PROJECT_SOURCE_DIR}/src/cpp/include"
        ARGS -E create_symlink "${PROJECT_SOURCE_DIR}/src/cpp/gearimpl/include" "gearimpl"
        OUTPUT_VARIABLE out_tmp )
ENDIF()

# generate header files with ant aid.generate
IF( NOT EXISTS "${PROJECT_SOURCE_DIR}/src/cpp/include/gear/GEAR.h" )
    MESSAGE( STATUS "Generating GEAR header files with ant aid.generate..." )
    MESSAGE( STATUS "Autodetecting java..." )
    FIND_PACKAGE( Java )
    IF( NOT JAVA_RUNTIME OR NOT JAVA_COMPILE )
        MESSAGE( FATAL_ERROR "Java not found!! Please make sure that java is in your PATH." )
    ENDIF()
    # extract java bin path out of full path to java runtime
    STRING( REGEX REPLACE "(.*)\\/java$" "\\1" JAVA_RUNTIME_PATH ${JAVA_RUNTIME} )
    # parse the output of java -version
    EXEC_PROGRAM( "${JAVA_RUNTIME}" ARGS "-version"
            OUTPUT_VARIABLE out_tmp
            RETURN_VALUE out_ret )
    IF( out_ret )
        MESSAGE( STATUS "Error executing java -version!!" )
    ENDIF()
    STRING( REGEX REPLACE ".* version \"([0-9]+\\.[0-9]+\\.[0-9]+).*"
            "\\1" JAVA_VERSION "${out_tmp}" )

    # display info
    MESSAGE( STATUS "Detected Java version ${JAVA_VERSION}..." )

    # set environment variables for
    # generating header files with ant aid.generate
    SET( ENV{GEAR} "${PROJECT_SOURCE_DIR}" )
    SET( ENV{PATH} "${PROJECT_SOURCE_DIR}/tools:${JAVA_RUNTIME_PATH}:$ENV{PATH}" )

    # generate header files with ant aid.generate
    EXEC_PROGRAM( ant "${PROJECT_SOURCE_DIR}"
        ARGS aid.generate
        OUTPUT_VARIABLE out_tmp
        RETURN_VALUE out_ret )
    # check if everything went ok
    IF( NOT out_ret )
        MESSAGE( STATUS "Header files generated in ${PROJECT_SOURCE_DIR}/src/cpp/include/gear..." )
    ELSE()
        MESSAGE( STATUS "Error generating header files!!" )
        MESSAGE( FATAL_ERROR "${out_tmp}" )
    ENDIF()
ENDIF()


# code for *nix only!
IF( UNIX )
    # find shell
    FIND_PROGRAM( SH
      sh
      ${CYGWIN_INSTALL_PATH}/bin
      /bin
      /usr/bin
      /usr/local/bin
      /sbin
      # FIXME add path to MacOS here
    )
    MARK_AS_ADVANCED( SH )

    IF( SH )
        # generate shell script for setting environment before running a command
        MESSAGE( STATUS "Generating shell environment script..." )
        FILE( WRITE "${PROJECT_BINARY_DIR}/script.sh"
                    "export GEAR=${PROJECT_SOURCE_DIR}\n"
                    "export GEARVERSION=${${PROJECT_NAME}_VERSION}\n"
                    "export PATH=${JAVA_RUNTIME_PATH}/bin:${PROJECT_SOURCE_DIR}/tools:$PATH\n"
                    "$1 $2" )

        # create "always out-of-date" headers target
        ADD_CUSTOM_TARGET( headers
            COMMAND ${SH} "${PROJECT_BINARY_DIR}/script.sh" ant aid.generate
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
            COMMENT "Generating header files with ant aid.generate..."
            VERBATIM )

        # DOCUMENTATION
        # FIXME Documentation needs GEARVERSION env var, so we can only
        # build it with script.sh
        FIND_PACKAGE( Doxygen )
        FIND_PACKAGE( LATEX )
        IF( DOXYGEN_FOUND AND LATEX_COMPILER AND MAKEINDEX_COMPILER )

            # this code is not ok because cmake cannot check if the documentation
            # has already been built. we better define a custom command + target
            #ADD_CUSTOM_TARGET( doc
            #    "${CMAKE_COMMAND}" -E chdir
            #    "${PROJECT_SOURCE_DIR}/doc"
            #    "${DOXYGEN_EXECUTABLE}" VERBATIM )

            ADD_CUSTOM_COMMAND(
                OUTPUT "${PROJECT_SOURCE_DIR}/doc/html"
                COMMAND ${SH} "${PROJECT_BINARY_DIR}/script.sh" "${DOXYGEN_EXECUTABLE}"
                WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/doc"
                COMMENT "Building API Documentation..."
                VERBATIM )
            
            #ADD_CUSTOM_COMMAND(
            #    OUTPUT "${PROJECT_SOURCE_DIR}/doc/latex"
            #    COMMAND ${SH} "${PROJECT_BINARY_DIR}/script.sh" "${DOXYGEN_EXECUTABLE}"
            #    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/doc"
            #    COMMENT "Building API Documentation..."
            #    VERBATIM )

            ADD_CUSTOM_TARGET( doc DEPENDS
                "${PROJECT_SOURCE_DIR}/doc/html" )
                #"${PROJECT_SOURCE_DIR}/doc/latex" )

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
    ELSE()
        MESSAGE( STATUS "Unix Shell not found - cannot enable headers target!!" )
        IF( INSTALL_DOC )
            MESSAGE( STATUS "Unix Shell not found - INSTALL_DOC forced to OFF" )
            SET( INSTALL_DOC OFF )
        ENDIF()
    ENDIF()
ENDIF()

IF( INSTALL_DOC )
    # make sure doxygen is executed before make install
    #INSTALL( CODE "EXEC_PROGRAM(${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/doc)" )
    # by just calling "make doc" doxygen won't run if the documentation has already been built
    INSTALL( CODE "EXEC_PROGRAM(${CMAKE_BUILD_TOOL} ${PROJECT_BINARY_DIR} ARGS doc)" )
    
    # install documentation
    INSTALL( DIRECTORY "${PROJECT_SOURCE_DIR}/doc"
            DESTINATION .
            PATTERN "*CVS*" EXCLUDE )
ENDIF()


##########################################################################################

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

# output directories
SET( EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin" CACHE PATH
    "EXECUTABLE_OUTPUT_PATH" FORCE )
SET( LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib" CACHE PATH
    "LIBRARY_OUTPUT_PATH" FORCE )
MARK_AS_ADVANCED( EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH )

# DEPENDENCIES: this code has to be placed before adding any library or
# executable so that these are linked properly against the dependencies
IF( DEFINED ${PROJECT_NAME}_DEPENDS OR DEFINED BUILD_WITH OR DEFINED LINK_WITH )
    # load macro
    IF( NOT EXISTS "${CMAKE_MODULE_PATH}/MacroCheckDeps.cmake" )
        MESSAGE( FATAL_ERROR
            "\nSorry, could not find MacroCheckDeps.cmake...\n"
            "Please set CMAKE_MODULE_PATH correctly with: "
            "cmake -DCMAKE_MODULE_PATH=<path_to_cmake_modules>" )
    ENDIF()
    INCLUDE( "${CMAKE_MODULE_PATH}/MacroCheckDeps.cmake" )
    CHECK_DEPS()
ENDIF()

# input directories
ADD_SUBDIRECTORY( src/cpp/gearxml gearxml )
ADD_SUBDIRECTORY( src/cpp/gearimpl gear )
IF( BUILD_GEAR_TESTS )
    ADD_SUBDIRECTORY( src/cpp/test test )
ENDIF()

#--- add mergeXML to be build by default
INCLUDE_DIRECTORIES( ./include )
ADD_EXECUTABLE( mergeXML src/cpp/test/mergeXML.cc )
TARGET_LINK_LIBRARIES( mergeXML lib_GEAR lib_GEARXML )
# install binary commando
INSTALL( TARGETS mergeXML DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

# create uninstall configuration file 
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake_uninstall.cmake.in"
                "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake"
                IMMEDIATE @ONLY )

# create uninstall target
ADD_CUSTOM_TARGET( uninstall "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake" )

# create configuration file from .in file
LIST( APPEND GEAR_DEFINITIONS "-Wno-long-long;-DUSE_GEAR" )
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in"
                "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY )

# install configuration file
INSTALL( FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" DESTINATION . )

# install include files
INSTALL( DIRECTORY "${PROJECT_SOURCE_DIR}/src/cpp/include"
        DESTINATION .
        PATTERN "*~" EXCLUDE
        PATTERN "*CVS*" EXCLUDE )

# display status message for important variables
MESSAGE( STATUS )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS "BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}" )
MESSAGE( STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}" )
MESSAGE( STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )
MESSAGE( STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}" )
MESSAGE( STATUS "BUILD_GEAR_TESTS = ${BUILD_GEAR_TESTS}" )
MESSAGE( STATUS "INSTALL_DOC = ${INSTALL_DOC}" )
MESSAGE( STATUS "To change a value type: cmake -D<Variable>=<Value>" )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS )

# force some variables that could be defined in the command line
# to be written to cache
SET( BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS}" CACHE BOOL
    "Set to OFF to build static libraries" FORCE )
SET( CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}" CACHE PATH
    "Where to install ${PROJECT_NAME}" FORCE )
SET( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE )
SET( CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" CACHE PATH
    "Path to custom CMake Modules" FORCE )
SET( BUILD_GEAR_TESTS "${BUILD_GEAR_TESTS}" CACHE BOOL
    "Set to ON to build GEAR tests" FORCE )
SET( INSTALL_DOC "${INSTALL_DOC}" CACHE BOOL
    "Set to OFF to skip build/install Documentation" FORCE )

# export build settings
INCLUDE( CMakeExportBuildSettings )
CMAKE_EXPORT_BUILD_SETTINGS( "${PROJECT_NAME}BuildSettings.cmake" )

# export library dependencies (keep this as the last line in the file)
EXPORT_LIBRARY_DEPENDENCIES( "${PROJECT_NAME}LibDeps.cmake" )
