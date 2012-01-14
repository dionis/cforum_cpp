#############################################################################
# CMakeLists.txt for the Classic Forum exceptions
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

# libcfexceptions
add_library(cfexceptions SHARED
  cforum_exception.cc
)

# eof
