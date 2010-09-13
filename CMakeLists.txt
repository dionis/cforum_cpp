cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(cforum C)


set(CF_PREFIX "${CMAKE_INSTALL_PREFIX}")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/")
include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckTypeSize)
include(UsePkgConfig)
include(CMakeInstallDirs)
include(FindLua51)

option(ENABLE_DEBUG "enable debugging mode" OFF)
option(ENABLE_LOCK_DEBUG "enable lock debugging" OFF)

# Copy flags for config.h
if(ENABLE_DEBUG STREQUAL "ON")
  set(DEBUG ON)
  add_definitions(-DCF_DEBUG -O0 -g)
else(ENABLE_DEBUG STREQUAL "ON")
  set(DEBUG OFF)
endif(ENABLE_DEBUG STREQUAL "ON")

if(ENABLE_LOCK_DEBUG STREQUAL "ON")
  add_definitions(-DCF_LOCK_DEBUG)
endif(ENABLE_LOCK_DEBUG STREQUAL "ON")

add_definitions(-W -Wall -funsigned-char -I. -D_REENTRANT -D_GNU_SOURCE -I${LUA_INCLUDE_DIR})

CHECK_FUNCTION_EXISTS(kqueue HAVE_KQUEUE)
CHECK_FUNCTION_EXISTS(epoll_create HAVE_EPOLL)
CHECK_FUNCTION_EXISTS(select HAVE_SELECT)

if(NOT HAVE_KQUEUE AND NOT HAVE_EPOLL AND NOT HAVE_SELECT)
  message(FATAL_ERROR "Could not find kqueue(2), nor epoll(4) neither select(2)")
endif(NOT HAVE_KQUEUE AND NOT HAVE_EPOLL AND NOT HAVE_SELECT)

if(NOT LUA51_FOUND)
  message(FATAL_ERROR "Could not find LUA 5.1")
endif(NOT LUA51_FOUND)

# Differences between UNIX systems:
#   Some define dlopen()/... in -ldl
#   Others define it in the C library
#   Same with iconv() in -liconv or in C library

check_library_exists (dl dlopen "" HAVE_LIBDL)
if (NOT HAVE_LIBDL)
  check_library_exists ("" dlopen "" HAVE_DLOPEN)
  if (NOT HAVE_DLOPEN)
    message (FATAL_ERROR "Could not find dlopen() in -ldl or in standard library")
  endif (NOT HAVE_DLOPEN)
endif (NOT HAVE_LIBDL)
check_library_exists (iconv iconv "" HAVE_LIBICONV)
if (NOT HAVE_LIBICONV)
  check_library_exists ("" iconv "" HAVE_ICONV)
  if (NOT HAVE_ICONV)
    message (FATAL_ERROR "Could not find iconv() in -liconv or in standard library")
  endif (NOT HAVE_ICONV)
endif (NOT HAVE_LIBICONV)

# Don't do the same stupid stuff autoconf does in checking for
# lot's of functions that modern OS have anyway. Should anybody
# want to port this software to some obscure architecture, OS or
# compiler, then simply add the necessary checks in order to
# make sure the software runs. Otherwise, don't bother adding
# tests that will pass on any OS somebody tries this software.

# Test for these functions since replacements exist
set (CMAKE_REQUIRED_DEFINITIONS -D_GNU_SOURCE)
check_symbol_exists (strdup "string.h" HAVE_STRDUP)
check_symbol_exists (strndup "string.h" HAVE_STRNDUP)
check_symbol_exists (getline "stdio.h" HAVE_GETLINE)
check_symbol_exists (getdelim "stdio.h" HAVE_GETDELIM)

set (CMAKE_EXTRA_INCLUDE_FILES sys/types.h)
check_type_size (u_char U_CHAR)
check_type_size (bool BOOL)
set (CMAKE_EXTRA_INCLUDE_FILES)

find_package (Threads REQUIRED)
if (NOT CMAKE_USE_PTHREADS_INIT)
  message (FATAL_ERROR "Only pthreads are supported currently")
endif (NOT CMAKE_USE_PTHREADS_INIT)
#use: TARGET_LINK_LIBRARIES(myexe ${CMAKE_THREAD_LIBS_INIT})

find_package(IDN REQUIRED)
find_package(ICU REQUIRED)

include_directories(${IDN_INCLUDE_DIR} ${ICU_INCLUDE})

# Make sure rpath is ok
SET(CMAKE_SKIP_BUILD_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
IF (${CMAKE_INSTALL_LIBDIR} MATCHES "^/")
  SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_LIBDIR}/cforum")
ELSE (${CMAKE_INSTALL_LIBDIR} MATCHES "^/")
  SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/cforum")
ENDIF (${CMAKE_INSTALL_LIBDIR} MATCHES "^/")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_subdirectory(src)

# Copy FindCForum.cmake macro (other programs may use it)
install(
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/FindCForum.cmake
  DESTINATION
    ${CMAKE_INSTALL_DATADIR}/cforum/cmake
)

# eof
