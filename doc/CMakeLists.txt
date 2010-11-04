###############################################
# cmake file for building GEAR documentation
# @author Jan Engels, DESY
###############################################


#---------- DOCUMENTATION ---------------------------------------------------

# build documentation out-of-source
SET( GEN_DOC_DIR "${PROJECT_BINARY_DIR}/docbuild" )

ADD_CUSTOM_COMMAND(
    OUTPUT "${GEN_DOC_DIR}/html/index.html" "${GEN_DOC_DIR}/latex/index.tex"
    # copy over doc from source tree to keep source tree clean
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${PROJECT_SOURCE_DIR}/doc" "${GEN_DOC_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${PROJECT_SOURCE_DIR}/example" "${GEN_DOC_DIR}/example"
    COMMAND "${DOXYGEN_EXECUTABLE}"
    WORKING_DIRECTORY "${GEN_DOC_DIR}"
    COMMENT "Building API Documentation..."
    DEPENDS "${PROJECT_SOURCE_DIR}/doc/overview.html.in"
            "${PROJECT_SOURCE_DIR}/doc/Doxyfile.in"
)

ADD_CUSTOM_TARGET( doc DEPENDS
    "${GEN_DOC_DIR}/html/index.html"
    "${GEN_DOC_DIR}/latex/index.tex"
)

# replace INPUT = @GEAR_SOURCE_DIR@/{src,include}
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/doc/Doxyfile.in" "${GEN_DOC_DIR}/Doxyfile" @ONLY )

# replace @GEAR_VERSION@
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/doc/overview.html.in" "${GEN_DOC_DIR}/overview.html" @ONLY )

# build documentation before 'make install'
INSTALL( CODE "EXECUTE_PROCESS( COMMAND ${CMAKE_BUILD_TOOL} doc)" )


# in-source vs out-of-source installations
IF( CMAKE_INSTALL_PREFIX STREQUAL "${PROJECT_SOURCE_DIR}" )

    # --- in-source installations ---
    INSTALL( DIRECTORY "${GEN_DOC_DIR}/html" DESTINATION "doc" )
    INSTALL( DIRECTORY "${GEN_DOC_DIR}/example" DESTINATION "doc" PATTERN "*/.svn" EXCLUDE )

ELSE()

    # --- out-of-source installations ---
    INSTALL( DIRECTORY "${GEN_DOC_DIR}/" DESTINATION "doc/gear"
        PATTERN "*Make*" EXCLUDE
        PATTERN "*Doxyfile*" EXCLUDE
        PATTERN "*overview.html*" EXCLUDE
        PATTERN "*/.svn" EXCLUDE
        PATTERN "*latex" EXCLUDE
    )

ENDIF( CMAKE_INSTALL_PREFIX STREQUAL "${PROJECT_SOURCE_DIR}" )

#----------------------------------------------------------------------------

