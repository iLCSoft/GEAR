
########################################
# cmake file for building libgeartgeo
# @author Jan Engels, DESY
########################################

INCLUDE_DIRECTORIES( ${CLHEP_INCLUDE_DIRS} ) 

# -------- libgearsurf -------------------------------
AUX_SOURCE_DIRECTORY( . libgearsurf_srcs )
ADD_SHARED_LIBRARY( gearsurf ${libgearsurf_srcs} )
TARGET_LINK_LIBRARIES( gearsurf gear ${CLHEP_LIBRARIES} )
INSTALL_SHARED_LIBRARY( gearsurf DESTINATION lib )
# ----------------------------------------------------

