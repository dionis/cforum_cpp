#############################################################################
# CMakeLists.txt for the Classic Forum Template parser library
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

# libcfjson
add_library(cftemplate SHARED
  Template.cc
)

target_link_libraries(cftemplate cfexceptions ${ICU_LIBRARY})

# eof
