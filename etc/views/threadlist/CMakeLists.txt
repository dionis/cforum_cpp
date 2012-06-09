#############################################################################
# CMakeLists.txt for the Classic Forum Main Sourcecode
#############################################################################

# Configuration files
include_directories(BEFORE "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

install(
  FILES
    threadlist.html
    thread.html
  DESTINATION
    "${CMAKE_INSTALL_SYSCONFDIR}/cforum/views/threadlist"
)

