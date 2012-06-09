#############################################################################
# CMakeLists.txt for the Classic Forum controllers
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(cf_threadlist_controller MODULE threadlist_controller.cc)
target_link_libraries(cf_threadlist_controller cfexceptions cfcgi cfjsevaluator cfjson cftemplate cfmodels ${Boost_LIBRARIES} ${ICU_LIBRARY} ${PCREPP_LIBRARIES})

# eof
