
##################################
# cmake file for building GEAR
# @author Jan Engels, DESY
##################################

# common definitions to ALL sources
#ADD_DEFINITIONS( "-Wno-long-long" )
#ADD_DEFINITIONS( "-Wall -ansi -pedantic" )

IF(APPLE)
    ADD_DEFINITIONS( "-D__APPLE__" )
ENDIF(APPLE)

# common includes (to ALL sources)
INCLUDE_DIRECTORIES( BEFORE "${PROJECT_BINARY_DIR}" ) # generated GearConfig.h
INCLUDE_DIRECTORIES( BEFORE "${PROJECT_SOURCE_DIR}/include" )
INCLUDE_DIRECTORIES( BEFORE "${PROJECT_SOURCE_DIR}/include/gear" )

INCLUDE_DIRECTORIES( SYSTEM  "${PROJECT_SOURCE_DIR}/include/tinyxml" )


###### LIBRARIES #####################################################

# -------- libgearsurf -------------------------------
INCLUDE_DIRECTORIES( SYSTEM ${CLHEP_INCLUDE_DIRS} ) # needed for gearsurf only

AUX_SOURCE_DIRECTORY( ./gearsurf libgearsurf_srcs )
ADD_SHARED_LIBRARY( gearsurf ${libgearsurf_srcs} )
TARGET_LINK_LIBRARIES( gearsurf ${CLHEP_LIBRARIES} )
INSTALL_SHARED_LIBRARY( gearsurf DESTINATION ${CMAKE_INSTALL_LIBDIR} )
# ----------------------------------------------------


# -------- libgear (impl) ----------------------------
AUX_SOURCE_DIRECTORY( ./gearimpl libgear_srcs )
ADD_SHARED_LIBRARY( gear ${libgear_srcs} )
INSTALL_SHARED_LIBRARY( gear DESTINATION ${CMAKE_INSTALL_LIBDIR} )
TARGET_LINK_LIBRARIES( gear gearsurf )
# ----------------------------------------------------

# -------- libgeartgeo -------------------------------
IF( GEAR_TGEO )
    ADD_SUBDIRECTORY( ./geartgeo )
ENDIF()
# ----------------------------------------------------

# -------- libgearxml --------------------------------
AUX_SOURCE_DIRECTORY( ./gearxml libgearxml_srcs )
ADD_SHARED_LIBRARY( gearxml ${libgearxml_srcs} )
INSTALL_SHARED_LIBRARY( gearxml DESTINATION ${CMAKE_INSTALL_LIBDIR} )
TARGET_LINK_LIBRARIES( gearxml gear ${CMAKE_DL_LIBS} )





###### TESTS #########################################################
   
IF( BUILD_TESTING )

  # mergeXML
  ADD_EXECUTABLE( mergeXML ./test/mergeXML.cc )
  TARGET_LINK_LIBRARIES( mergeXML gear gearxml )

  # printParameters
  ADD_EXECUTABLE( printParameters ./test/printParameters.cc )
  TARGET_LINK_LIBRARIES( printParameters gear gearxml )

  # printMaterials
  IF( GEAR_TGEO )
    ADD_EXECUTABLE( printMaterials ./test/printMaterials.cc )
    TARGET_LINK_LIBRARIES( printMaterials gear gearxml )
    INSTALL( TARGETS printMaterials DESTINATION ${CMAKE_INSTALL_BINDIR} )
  ENDIF()

  INSTALL( TARGETS mergeXML printParameters DESTINATION ${CMAKE_INSTALL_BINDIR} )

  # test binaries
  SET( test_bins
      testgear
      testtpcproto
      testVTXgear
      testSiPlanesgear    
      testAngularLayout
      testDistanceToModule
      testGearMgr
      testIsInsideModule
      testPlaneExtent
      testTPCModule
      testVersatileLayout
      testXMLOut
      testXMLCopy
  )

  IF( GEAR_TGEO )
    SET( test_bins ${test_bins} testGearTGeo testMaterialBudgetNew testInteractionLengths testMaterialBudget testMaterialMap )
  ENDIF()


  FOREACH( _testname ${test_bins} )
      
      ADD_EXECUTABLE( ${_testname} ./test/${_testname}.cc )

      TARGET_LINK_LIBRARIES( ${_testname} gear gearxml )

  ENDFOREACH()


  ### FTD test needs ROOT/TGeo ######

  FIND_PACKAGE( ROOT COMPONENTS Geom )

  IF( ROOT_FOUND )
    INCLUDE_DIRECTORIES( SYSTEM ${ROOT_INCLUDE_DIRS} )

    IF( BUILD_TESTING )
       ADD_EXECUTABLE( testFTD ./test/testFTD.cc )
    ELSE()
       ADD_EXECUTABLE( testFTD EXCLUDE_FROM_ALL ./test/testFTD.cc )
    ENDIF()

    TARGET_LINK_LIBRARIES( testFTD   gear gearxml  ${ROOT_LIBRARIES} ${ROOT_COMPONENT_LIBRARIES} )
  ENDIF()

ENDIF()

