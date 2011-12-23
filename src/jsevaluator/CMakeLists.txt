#############################################################################
# CMakeLists.txt for the Classic Forum JS evaluator library
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")


# libcfjsevaluator
add_library(cfjsevaluator SHARED js_evaluator.cc js_evaluator_exception.cc)

target_link_libraries(cfjsevaluator cfexceptions ${ICU_LIBRARY} v8)

# eof
