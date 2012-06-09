#############################################################################
# CMakeLists.txt for the Classic Forum exceptions
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

# libcfexceptions
add_library(cfexceptions SHARED
  cforum_exception.cc
)

install(
  TARGETS
    cfexceptions
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
  FILES
    cforum_exception.hh
  DESTINATION
    "${CMAKE_INSTALL_PREFIX}/include/cforum/exceptions"
)

# eof
