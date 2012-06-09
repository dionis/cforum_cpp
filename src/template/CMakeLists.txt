#############################################################################
# CMakeLists.txt for the Classic Forum Template parser library
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

# libcfjson
add_library(cftemplate SHARED
  template.cc
  template_parser.cc
  extender.cc
  template_exception.cc
  template_parser_exception.cc
)

target_link_libraries(cftemplate cfexceptions ${V8_LIBRARY} ${ICU_LIBRARY})

install(
  TARGETS
    cftemplate
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
  FILES
    template_exception.hh
    template.hh
    template_parser_exception.hh
  DESTINATION
    "${CMAKE_INSTALL_PREFIX}/include/cforum/template"
)

# eof
