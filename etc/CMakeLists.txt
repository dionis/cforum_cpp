#############################################################################
# CMakeLists.txt for the Classic Forum Main Sourcecode
#############################################################################

# Configuration files
include_directories(BEFORE "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(views)

install(
  FILES
    cforum.js.example
  DESTINATION
    "${CMAKE_INSTALL_SYSCONFDIR}/cforum"
)