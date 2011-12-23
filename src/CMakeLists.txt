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

add_library(cfconfigparser SHARED configparser.cc)
target_link_libraries(cfconfigparser cfjsevaluator ${ICU_LIBRARY})

add_library(cfframework SHARED uri.cc user.cc request.cc)
target_link_libraries(cfframework cfexceptions cfcgi)


#add_library(cfrouter SHARED router.cc)
#target_link_libraries(cfrouter ${ICU_LIBRARY})


# eof
