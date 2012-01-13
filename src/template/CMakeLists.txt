#############################################################################
# CMakeLists.txt for the Classic Forum Template parser library
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

# libcfjson
add_library(cftemplate SHARED
  template.cc
  template_parser.cc
  extender.cc
)

target_link_libraries(cftemplate cfexceptions ${V8_LIBRARY} ${ICU_LIBRARY})

# eof
