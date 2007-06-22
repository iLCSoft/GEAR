#############################################
# cmake file for building GEAR test binaries
# @author Jan Engels, DESY
#############################################

IF( BUILD_GEAR_TESTS )
    # include directories
    INCLUDE_DIRECTORIES( ../include )

    # list of test binaries
    SET( test_bins
        mergeXML
        testgear
        testtpcproto
        testVTXgear
        testSiPlanesgear
    )

    # create symbolic bin target for calling targets bin_XXX
    ADD_CUSTOM_TARGET( bin VERBATIM )

    # loop through list of test binaries
    FOREACH( bin ${test_bins} )
        # create target for binary
        ADD_EXECUTABLE( bin_${bin} EXCLUDE_FROM_ALL ${bin}.cc )
        # tell bin target to call this bin_XXX target
        ADD_DEPENDENCIES( bin bin_${bin} )
        # avoid conflicts between library and binary target names
        SET_TARGET_PROPERTIES( bin_${bin} PROPERTIES OUTPUT_NAME ${bin} )
        # link binary against library
        TARGET_LINK_LIBRARIES( bin_${bin} lib_${PROJECT_NAME} )
        # install binary commando
        INSTALL( TARGETS bin_${bin} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
    ENDFOREACH()
ENDIF()

