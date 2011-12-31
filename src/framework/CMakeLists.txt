#############################################################################
# CMakeLists.txt for the Classic Forum couchdb interface library
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(cfframework SHARED
  uri.cc
  user.cc
  request.cc
  cgi_request.cc
  uri_exception.cc
  router.cc
  route.cc
  route_exception.cc
  route_syntax_exception.cc
  controller.cc
  framework_exception.cc
  not_found_exception.cc
  internal_error_exception.cc
  notification_center.cc
  session.cc
  session_file_storage.cc
  session_value.cc
  session_storage.cc
  session_exception.cc
)

target_link_libraries(cfframework cfexceptions cfcgi boost_serialization boost_filesystem boost_system ${ICU_LIBRARY} ${PCREPP_LIBRARIES})

# eof
