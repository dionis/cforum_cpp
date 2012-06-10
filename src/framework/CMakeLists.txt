#############################################################################
# CMakeLists.txt for the Classic Forum framework library
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
  configparser.cc
  framework_exception.cc
  framework_error_exception.cc
  redirect_exception.cc
  temporary_redirect_exception.cc
  permanent_redirect_exception.cc
  not_found_exception.cc
  internal_error_exception.cc
  notification_center.cc
  session.cc
  session_file_storage.cc
  session_value.cc
  session_storage.cc
  session_exception.cc
  config_error_exception.cc
  application.cc
  module_exception.cc
  cgi_application.cc
  mongodb.cc
  model.cc
)

target_link_libraries(cfframework cfexceptions cfcgi cfjsevaluator cfjson cftemplate ${MongoDB_LIBRARIES} ${Boost_LIBRARIES} ${ICU_LIBRARY} ${PCREPP_LIBRARIES})

install(
  TARGETS
    cfframework
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
  FILES
    application.hh
    config_error_exception.hh
    framework_exception.hh
    framework_error_exception.hh
    module_exception.hh
    notification_center.hh
    route.hh
    session_exception.hh
    uri_exception.hh
    cgi_application.hh
    configparser.hh
    internal_error_exception.hh
    mongodb.hh
    request.hh
    router.hh
    session_file_storage.hh
    uri.hh
    cgi_request.hh
    controller.hh
    model.hh
    not_found_exception.hh
    route_exception.hh
    route_syntax_exception.hh
    session.hh
    user.hh
  DESTINATION
    "${CMAKE_INSTALL_PREFIX}/include/cforum/framework"
)

# eof
