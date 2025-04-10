###############################################
# cmake file for building GEAR documentation
# @author Jan Engels, DESY
###############################################


#---------- DOCUMENTATION ---------------------------------------------------

# build documentation out-of-source
SET( DOC_SRC_DIR "${PROJECT_SOURCE_DIR}/doc" )
SET( DOC_BIN_DIR "${PROJECT_BINARY_DIR}/docbuild" )

# directories to search for documentation
SET( DOX_INPUT overview.html ../include/gear ../src )

FILE(GLOB GEAR_DOXYGEN_SOURCES
         ../include/gear/*
            ../include/gear/gearcga/*
            ../include/gear/gearimpl/*
            ../include/gear/geartgeo/*
            ../include/gear/gearsurf/*
            ../include/gear/gearxml/*
            ../src/gearcga/*
            ../src/gearimpl/*
            ../src/geartgeo/*
            ../src/gearsurf/*
            ../src/gearxml/*
    )

# custom command to build documentation
ADD_CUSTOM_COMMAND(
    OUTPUT  "${DOC_BIN_DIR}/html/index.html"
    COMMAND DOX_PROJECT_NAME=${PROJECT_NAME}
            DOX_PROJECT_NUMBER="${${PROJECT_NAME}_VERSION}"
            DOX_OUTPUT_DIRECTORY="${DOC_BIN_DIR}"
            DOX_INPUT="${DOX_INPUT}"
            "${DOXYGEN_EXECUTABLE}"
    WORKING_DIRECTORY "${DOC_SRC_DIR}"
    COMMENT "Building API Documentation..."
    DEPENDS overview.html Doxyfile CMakeLists.txt ${GEAR_DOXYGEN_SOURCES}
   )

ADD_CUSTOM_TARGET( doc DEPENDS "${DOC_BIN_DIR}/html/index.html" )

# build documentation before 'make install'
INSTALL( CODE "EXECUTE_PROCESS( COMMAND ${CMAKE_BUILD_TOOL} doc)" )


# in-source vs out-of-source installations
IF( CMAKE_INSTALL_PREFIX STREQUAL "${PROJECT_SOURCE_DIR}" )

    SET( DOC_INSTALL_PREFIX doc )

    # keep source tree clean
    ADD_CUSTOM_TARGET( uninstall_doc
        COMMAND "${CMAKE_COMMAND}" -E remove_directory "${CMAKE_INSTALL_PREFIX}/doc/html"
        COMMAND "${CMAKE_COMMAND}" -E remove_directory "${CMAKE_INSTALL_PREFIX}/doc/example"
    )
    ADD_DEPENDENCIES( uninstall uninstall_doc )


ELSE()

    SET( DOC_INSTALL_PREFIX doc/gear )
    
    INSTALL( FILES
            ReleaseNotes.md
            eudet_memo_GearModularTPC.pdf
            TPCModule_tangent_rMax.pdf
            gear_banner.png
            TPCModule_tangent_rMax.README
        DESTINATION ${DOC_INSTALL_PREFIX}
    )

ENDIF( CMAKE_INSTALL_PREFIX STREQUAL "${PROJECT_SOURCE_DIR}" )


INSTALL_DIRECTORY( "${DOC_BIN_DIR}/html" DESTINATION ${DOC_INSTALL_PREFIX} )
INSTALL_DIRECTORY( "${PROJECT_SOURCE_DIR}/example" DESTINATION ${DOC_INSTALL_PREFIX} )



#----------------------------------------------------------------------------

