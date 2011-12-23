#############################################################################
# CMakeLists.txt for the Classic Forum exceptions
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

# libcfexceptions
add_library(cfexceptions SHARED
  cforum_exception.cc
  config_error_exception.cc
  js_evaluator_exception.cc
  parameter_exception.cc
  cgi_parser_exception.cc
  uri_exception.cc
)

# eof
