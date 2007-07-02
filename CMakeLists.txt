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
SET( ${PROJECT_NAME}_MINOR_VERSION 5 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 1 )

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
    SET( CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}" )
ENDIF()

# create symbolic link for include files
IF( NOT EXISTS "${CMAKE_SOURCE_DIR}/src/cpp/include/gearxml" )
    MESSAGE( STATUS "Creating symlink: ${CMAKE_SOURCE_DIR}/src/cpp/include/gearxml..." )
    EXEC_PROGRAM( "${CMAKE_COMMAND}"
        "${CMAKE_SOURCE_DIR}/src/cpp/include"
        ARGS -E create_symlink "../gearxml/include" "gearxml"
        OUTPUT_VARIABLE out_tmp )
ENDIF()
IF( NOT EXISTS "${CMAKE_SOURCE_DIR}/src/cpp/include/gearimpl" )
    MESSAGE( STATUS "Creating symlink: ${CMAKE_SOURCE_DIR}/src/cpp/include/gearimpl..." )
    EXEC_PROGRAM( "${CMAKE_COMMAND}"
        "${CMAKE_SOURCE_DIR}/src/cpp/include"
        ARGS -E create_symlink "../gearimpl/include" "gearimpl"
        OUTPUT_VARIABLE out_tmp )
ENDIF()

# generate header files with ant aid.generate
IF( NOT EXISTS "${CMAKE_SOURCE_DIR}/src/cpp/include/gear/GEAR.h" )
    MESSAGE( STATUS "Generating GEAR header files with ant aid.generate..." )
    IF( NOT DEFINED ENV{JDK_HOME} AND NOT DEFINED ENV{JAVA_HOME} )
        MESSAGE( STATUS "Autodetecting java..." )
        FIND_PACKAGE( Java )
        IF( NOT JAVA_RUNTIME OR NOT JAVA_COMPILE )
            MESSAGE( FATAL_ERROR "Java not found!!" )
        ENDIF()
        SET( java_bin "${JAVA_RUNTIME}" )
        # extract java_bin_path out of full path to java runtime
        STRING( REGEX REPLACE "(.*)\\/java$" "\\1" java_bin_path ${java_bin} )
    ELSE()
        # in case JDK_HOME or JAVA_HOME already set ensure that both variables
        # are set correctly (JDK_HOME as well as JAVA_HOME)
        IF( DEFINED ENV{JDK_HOME} AND DEFINED ENV{JAVA_HOME} )
            IF( "$ENV{JDK_HOME}" STREQUAL "$ENV{JAVA_HOME}" )
                SET( java_path "$ENV{JDK_HOME}" )
            ELSE()
                MESSAGE( STATUS
                    "JDK_HOME and JAVA_HOME are set to different paths!!" )
                MESSAGE( STATUS "JDK_HOME: $ENV{JDK_HOME}" )
                MESSAGE( STATUS "JAVA_HOME: $ENV{JAVA_HOME}" )
                MESSAGE( FATAL_ERROR
                    "Please unset one of them or set both correctly!!" )
            ENDIF()
        ELSE()
            IF( DEFINED ENV{JDK_HOME} )
                SET( java_path "$ENV{JDK_HOME}" )
            ENDIF()
            IF( DEFINED ENV{JAVA_HOME} )
                SET( java_path "$ENV{JAVA_HOME}" )
            ENDIF()
        ENDIF()
        # find java, javac and jar
        FIND_PROGRAM( java_bin
            java
            ${java_path}/bin
            NO_DEFAULT_PATH )
        FIND_PROGRAM( javac_bin
            javac
            ${java_path}/bin
            NO_DEFAULT_PATH )
        # abort if not found
        IF( java_bin-NOTFOUND )
            MESSAGE( FATAL_ERROR "Could not find java!!" )
        ENDIF()
        IF( javac_bin-NOTFOUND )
            MESSAGE( FATAL_ERROR "Could not find javac!!" )
        ENDIF()
        # extract bin path from full path
        STRING( REGEX REPLACE "(.*)\\/java$" "\\1" java_bin_path ${java_bin} )
    ENDIF()

    # parse the output of java -version
    EXEC_PROGRAM( "${java_bin}" ARGS "-version"
            OUTPUT_VARIABLE out_tmp
            RETURN_VALUE out_ret )
    IF( out_ret )
        MESSAGE( STATUS "Error executing java -version!!" )
    ENDIF()
    STRING( REGEX REPLACE ".* version \"([0-9]+\\.[0-9]+\\.[0-9]+).*"
            "\\1" java_version "${out_tmp}" )

    # display info
    MESSAGE( STATUS "Java version ${java_version} configured successfully!" )

    # set environment variables for
    # generating header files with ant aid.generate
    SET( ENV{GEAR} "${CMAKE_SOURCE_DIR}" )
    SET( ENV{PATH} "${CMAKE_SOURCE_DIR}/tools:${java_bin_path}:$ENV{PATH}" )

    # generate header files with ant aid.generate
    EXEC_PROGRAM( ant "${CMAKE_SOURCE_DIR}"
        ARGS aid.generate
        OUTPUT_VARIABLE out_tmp
        RETURN_VALUE out_ret )
    # check if everything went ok
    IF( NOT out_ret )
        MESSAGE( STATUS "Header files generated in ${CMAKE_SOURCE_DIR}/src/cpp/include/gear..." )
    ELSE()
        MESSAGE( STATUS "Error generating header files!!" )
        MESSAGE( FATAL_ERROR "${out_tmp}" )
    ENDIF()
ENDIF()

# generate shell script for setting environment
# and calling ant aid.generate
MESSAGE( STATUS "Generating shell script for calling ant..." )
FILE( WRITE "${CMAKE_BINARY_DIR}/script.sh"
            "export GEAR=${CMAKE_SOURCE_DIR}\n"
            "export PATH=${java_bin_path}/bin:${CMAKE_SOURCE_DIR}/tools:$PATH\n"
            "ant $1" )

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

IF( SH-NOTFOUND )
    MESSAGE( STATUS "Unix Shell not found - cannot enable headers target!!" )
ELSE()
    # create "always out-of-date" headers target
    ADD_CUSTOM_TARGET( headers
        COMMAND ${SH} "${CMAKE_BINARY_DIR}/script.sh" aid.generate
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Generating header files with ant aid.generate..."
        VERBATIM )
ENDIF()

# DOCUMENTATION
FIND_PACKAGE( Doxygen )
FIND_PACKAGE( LATEX )
IF( DOXYGEN_FOUND AND LATEX_COMPILER AND MAKEINDEX_COMPILER )

    # this code is not ok because cmake cannot check if the documentation
    # has already been built. we better define a custom command + target
    #ADD_CUSTOM_TARGET( doc
    #    "${CMAKE_COMMAND}" -E chdir
    #    "${CMAKE_SOURCE_DIR}/doc"
    #    "${DOXYGEN_EXECUTABLE}" VERBATIM )

    ADD_CUSTOM_COMMAND(
        OUTPUT  "${CMAKE_SOURCE_DIR}/doc/html"
        COMMAND "${DOXYGEN_EXECUTABLE}"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/doc"
        COMMENT "Building API Documentation..."
        VERBATIM )
    
    #ADD_CUSTOM_COMMAND(
    #    OUTPUT  "${CMAKE_SOURCE_DIR}/doc/latex"
    #    COMMAND "${DOXYGEN_EXECUTABLE}"
    #    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/doc"
    #    COMMENT "Building API Documentation..."
    #    VERBATIM )

    ADD_CUSTOM_TARGET( doc DEPENDS
        "${CMAKE_SOURCE_DIR}/doc/html" )
        #"${CMAKE_SOURCE_DIR}/doc/latex" )

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
    # make sure doxygen is executed before make install
    #INSTALL( CODE "EXEC_PROGRAM(${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/doc)" )
    # by just calling "make doc" doxygen won't run if the documentation has already been built
    INSTALL( CODE "EXEC_PROGRAM(${CMAKE_BUILD_TOOL} ${CMAKE_BINARY_DIR} ARGS doc)" )
    
    # install documentation
    INSTALL( DIRECTORY "${CMAKE_SOURCE_DIR}/doc"
            DESTINATION .
            PATTERN "*CVS*" EXCLUDE )
ENDIF()

##########################################################################################

# library *nix style versioning
SET( ${PROJECT_NAME}_SOVERSION
    "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}" )
SET( ${PROJECT_NAME}_VERSION
    "${${PROJECT_NAME}_SOVERSION}.${${PROJECT_NAME}_PATCH_LEVEL}" )

# add library install path to the rpath list
SET( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )
MARK_AS_ADVANCED( CMAKE_INSTALL_RPATH )

# append link pathes to rpath list
SET( CMAKE_INSTALL_RPATH_USE_LINK_PATH 1 )
MARK_AS_ADVANCED( CMAKE_INSTALL_RPATH_USE_LINK_PATH )

# output directories
SET( EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" CACHE PATH
    "EXECUTABLE_OUTPUT_PATH" FORCE )
SET( LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib" CACHE PATH
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

# create uninstall configuration file 
CONFIGURE_FILE( "${CMAKE_SOURCE_DIR}/cmake_uninstall.cmake.in"
                "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
                IMMEDIATE @ONLY )

# create uninstall target
ADD_CUSTOM_TARGET( uninstall "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake" )

# create configuration file from .in file
CONFIGURE_FILE( "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in"
                "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY )

# install configuration file
INSTALL( FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake" DESTINATION . )

# install include files
INSTALL( DIRECTORY "${CMAKE_SOURCE_DIR}/src/cpp/include"
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
