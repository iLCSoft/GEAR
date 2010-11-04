
########################################
# cmake file for building libgeartgeo
# @author Jan Engels, DESY
########################################

INCLUDE_DIRECTORIES( ${ROOT_INCLUDE_DIRS} )

# -------- libgeartgeo -------------------------------
AUX_SOURCE_DIRECTORY( . libgeartgeo_srcs )
ADD_GEAR_LIBRARY( geartgeo ${libgeartgeo_srcs} )
TARGET_LINK_LIBRARIES( geartgeo gear )
TARGET_LINK_LIBRARIES( geartgeo ${ROOT_LIBRARIES} )
# ----------------------------------------------------

