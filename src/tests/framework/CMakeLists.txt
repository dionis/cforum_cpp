#############################################################################
# CMakeLists.txt for the Classic Forum unit tests
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(cfframework_test SHARED URITest.cc UserTest.cc)
target_link_libraries(cfframework_test cfframework cppunit)

# eof
