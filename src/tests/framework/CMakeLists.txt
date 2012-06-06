#############################################################################
# CMakeLists.txt for the Classic Forum unit tests
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

#add_library(cfframework_test SHARED uri_test.cc user_test.cc route_test.cc router_test.cc my_controller.cc notification_center_test.cc session_test.cc configparser_test.cc)
add_library(cfframework_test SHARED configparser_test.cc)
target_link_libraries(cfframework_test cfframework cppunit)

# eof
