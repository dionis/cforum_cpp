#############################################################################
# CMakeLists.txt for the Classic Forum JSON parser library
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(cgi)
add_subdirectory(json)

add_executable(run_tests run_tests.cc)
target_link_libraries(run_tests
  cfjson_test
  cfcgi_test
)

add_custom_target(tests "run_tests" DEPENDS run_tests COMMENT "Running CPPUNIT tests...")

# eof
