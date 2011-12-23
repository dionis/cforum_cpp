#############################################################################
# CMakeLists.txt for the Classic Forum unit tests
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(cfjson_test SHARED json_test.cc)
target_link_libraries(cfjson_test cfjson cppunit)

# eof
