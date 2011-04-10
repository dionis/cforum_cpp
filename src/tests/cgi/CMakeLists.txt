#############################################################################
# CMakeLists.txt for the Classic Forum unit tests
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(cfcgi_test SHARED CGITest.cc)
target_link_libraries(cfcgi_test cfcgi cppunit)

# eof
