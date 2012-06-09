#############################################################################
# CMakeLists.txt for the Classic Forum framework library
#############################################################################

include_directories(BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(cfmodels SHARED
  message.cc
)

target_link_libraries(cfmodels cfframework ${Boost_LIBRARIES} ${ICU_LIBRARY} ${PCREPP_LIBRARIES})

# eof
