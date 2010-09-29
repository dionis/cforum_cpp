configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(utils)
add_subdirectory(modules)

# libcfreadline
add_library(cfreadline SHARED readline.c)

# libcfvalidate
add_library(cfvalidate SHARED validate.c)
target_link_libraries(cfvalidate cfutils ${IDN_LIBRARIES})

# libcfhash
add_library(cfhash SHARED hashlib.c)

# libcfcgi
add_library(cfcgi SHARED cgi.c)
target_link_libraries(cfcgi cfhash cfutils)

# libcfconfigparser
add_library(cfconfigparser SHARED configparser.c configparser_lua.c)
target_link_libraries(cfconfigparser cfutils cfhash ${LUA_LIBRARIES})
if(HAVE_LIBDL)
  target_link_libraries(cfconfigparser dl)
endif(HAVE_LIBDL)

# libcfserver
# use kqueue or epoll if possible, else select
if(HAVE_KQUEUE)
  SET(SRC cf_server_kqueue.c)
elseif(HAVE_EPOLL)
  SET(SRC cf_server_epoll.c)
else()
  SET(SRC cf_server_select.c)
endif()

add_library(cfserver SHARED serverlib.c cf_pthread.c operation_queue.c http_server.c threading_manager_worker.c ${SRC})
target_link_libraries(cfserver cfutils cfconfigparser cfreadline ${CMAKE_THREAD_LIBS_INIT})

# cf_server
add_executable(cf_server cf_server.c)
target_link_libraries(cf_server cfutils cfconfigparser cfhash cfutils cfserver ${CMAKE_THREAD_LIBS_INIT})

install(
  TARGETS
    cfreadline
    cfhash
    cfcgi
    cfvalidate
    cfconfigparser
    cfserver
    cf_server
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/cforum
)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
    cf_pthread.h
    cf_server.h
    cgi.h
    configparser.h
    hashlib.h
    readline.h
    serverlib.h
    validate.h
  DESTINATION
    ${CMAKE_INSTALL_PREFIX}/include/cforum
)

# eof
