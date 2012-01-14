#############################################################################
# CMakeLists.txt for the Classic Forum Main Sourcecode
#############################################################################

# Configuration files
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.hh.cmake" "${CMAKE_CURRENT_BINARY_DIR}/config.hh")
include_directories(BEFORE "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

# Add utility libraries and modules
add_subdirectory(json)
add_subdirectory(exceptions)
add_subdirectory(jsevaluator)
add_subdirectory(couchdb)
add_subdirectory(cgi)
add_subdirectory(template)
add_subdirectory(framework)

add_subdirectory(tests)



# eof
