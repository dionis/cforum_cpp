#############################################################################
# CMakeLists.txt for the Classic Forum JSON parser library
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

# libcfjson
add_library(cfjson SHARED
  json_array.cc
  json_element.cc
  json_null.cc
  json_string.cc
  json_object.cc
  json_boolean.cc
  json_number.cc
  json_parser.cc
  json_syntax_exception.cc
  json_exception.cc
)

target_link_libraries(cfjson cfexceptions ${ICU_LIBRARY})

install(
  TARGETS
    cfjson
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
  FILES
    json_array.hh
    json_boolean.hh
    json_element.hh
    json_exception.hh
    json_null.hh
    json_number.hh
    json_object.hh
    json_parser.hh
    json_string.hh
    json_syntax_exception.hh
  DESTINATION
    "${CMAKE_INSTALL_PREFIX}/include/cforum/json"
)


# eof
