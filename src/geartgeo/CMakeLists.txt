
########################################
# cmake file for building libgeartgeo
# @author Jan Engels, DESY
########################################

INCLUDE_DIRECTORIES( ${ROOT_INCLUDE_DIRS} )

# -------- libgeartgeo -------------------------------
AUX_SOURCE_DIRECTORY( . libgeartgeo_srcs )
ADD_SHARED_LIBRARY( geartgeo ${libgeartgeo_srcs} )
INSTALL_SHARED_LIBRARY( geartgeo DESTINATION lib )
TARGET_LINK_LIBRARIES( geartgeo gear )
TARGET_LINK_LIBRARIES( geartgeo ${ROOT_LIBRARIES} ${ROOT_COMPONENT_LIBRARIES} )
# ----------------------------------------------------

