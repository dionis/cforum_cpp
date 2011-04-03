#############################################################################
# CMakeLists.txt for the Classic Forum unit tests
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(run_json_tests
  run_json_tests.cpp
  JSONTest.cc
)

target_link_libraries(run_json_tests cfjson cppunit)

add_custom_target(tests "run_json_tests" DEPENDS run_json_tests COMMENT "Running CPPUNIT tests...")

# eof
