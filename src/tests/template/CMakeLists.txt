#############################################################################
# CMakeLists.txt for the Classic Forum unit tests
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(cftemplate_test SHARED TemplateTest.cc)
target_link_libraries(cftemplate_test cftemplate cppunit)

# eof
