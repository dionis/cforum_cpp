#############################################################################
# CMakeLists.txt for the Classic Forum unit tests
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(cfcouchdb_test SHARED CouchDBTest.cc)
target_link_libraries(cfcouchdb_test cfcouchdb cppunit)

# eof
