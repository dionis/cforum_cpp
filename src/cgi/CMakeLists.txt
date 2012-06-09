#############################################################################
# CMakeLists.txt for the Classic Forum cgi interface library
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")


# libcfcgi
add_library(cfcgi SHARED cgi.cc cgi_exception.cc parameter_exception.cc cgi_parser_exception.cc)

target_link_libraries(cfcgi cfexceptions ${ICU_LIBRARY})

install(
  TARGETS
    cfcgi
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
  FILES
    cgi_exception.hh
    cgi.hh
    cgi_parser_exception.hh
    parameter_exception.hh
  DESTINATION
    "${CMAKE_INSTALL_PREFIX}/include/cforum/cgi"
)

# eof
