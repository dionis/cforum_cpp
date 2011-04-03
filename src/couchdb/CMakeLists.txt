#############################################################################
# CMakeLists.txt for the Classic Forum couchdb interface library
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")


# libcfcouchdb
add_library(cfcouchdb SHARED Server.cc Document.cc)

target_link_libraries(cfcouchdb cfexceptions cfjson ${CURL_LIBRARY})

# eof
