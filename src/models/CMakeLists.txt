#############################################################################
# CMakeLists.txt for the Classic Forum framework library
#############################################################################

include_directories(BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(cfmodels SHARED
  message.cc
  thread.cc
)

target_link_libraries(cfmodels cfframework ${Boost_LIBRARIES} ${ICU_LIBRARY} ${PCREPP_LIBRARIES})

install(
  TARGETS
    cfmodels
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
  FILES
    thread.hh
    message.hh
  DESTINATION
    "${CMAKE_INSTALL_PREFIX}/include/cforum/models"
)

# eof
