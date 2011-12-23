#############################################################################
# CMakeLists.txt for the Classic Forum couchdb interface library
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")


# libcfcouchdb
add_library(cfcouchdb SHARED server.cc document.cc chunk.cc response.cc couch_error_exception.cc)

target_link_libraries(cfcouchdb cfexceptions cfjson cfcgi ${CURL_LIBRARY})

# eof
