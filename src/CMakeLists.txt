#############################################################################
# CMakeLists.txt for the Classic Forum Main Sourcecode
#############################################################################

# Configuration files
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake" "${CMAKE_CURRENT_BINARY_DIR}/config.h")
include_directories(BEFORE "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

# Add utility libraries and modules
add_subdirectory(json)
add_subdirectory(exceptions)
add_subdirectory(jsevaluator)
add_subdirectory(couchdb)
add_subdirectory(cgi)
add_subdirectory(template)

add_subdirectory(tests)

add_library(cfconfigparser SHARED Configparser.cc)
target_link_libraries(cfconfigparser cfjsevaluator ${ICU_LIBRARY})

add_library(cfuri SHARED URI.cc)
target_link_libraries(cfuri cfexceptions)

#add_library(cfrouter SHARED Router.cc)
#target_link_libraries(cfrouter ${ICU_LIBRARY})


# eof
