#############################################################################
# CMakeLists.txt for the Classic Forum JS evaluator library
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")


# libcfjsevaluator
add_library(cfjsevaluator SHARED js_evaluator.cc js_evaluator_exception.cc)

target_link_libraries(cfjsevaluator cfexceptions ${ICU_LIBRARY} ${V8_LIBRARY})

install(
  TARGETS
    cfjsevaluator
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
  FILES
    js_evaluator.hh
    js_evaluator_exception.hh
  DESTINATION
    "${CMAKE_INSTALL_PREFIX}/include/cforum/jsevaluator"
)

# eof
