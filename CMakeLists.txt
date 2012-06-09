#############################################################################
# Main CMakeLists.txt for the Classic Forum
#############################################################################

# Require CMake version 2.6, don't automatically add library paths to linker
cmake_minimum_required(VERSION 2.6)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif()

project(cforum CXX C)

# Set a default build type for single-configuration
# CMake generators if no build type is set.
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

# Tell CMake where to find the additional .cmake module files
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/")

# Set CFlags for different profiles (FIXME: test with different compilers)
# 1) Default flags: We use C99 code
set(CMAKE_C_FLAGS "-std=c99")
# 2) Debug flags: We want to activate debbuging codes and warnings should be errors, also define CF_DEBUG
set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb -Wall -W -pedantic -Werror -DCF_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb -Wall -W -pedantic -Werror -DCF_DEBUG")
# 3) Release flags: We want to optimize the source code
set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
# 4) Release + Debug Info flags: Optimize code but add debug symbols
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
# 5) Release optimized for size
set(CMAKE_C_FLAGS_MINSIZEREL "-Os")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")

# Add default definitions
#   _GNU_SOURCE: allows us to use system implementation of strdup etc. if
#                available (else we provide our own)
#   _REENTRANT:  select reentrant functions by default (thread safety)
add_definitions(-D_REENTRANT -D_GNU_SOURCE "-include config.hh")

# Include Useful Macros
include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckTypeSize)
include(UsePkgConfig)
include(CMakeInstallDirs)

# CMake Prefix Handling
if(NOT CMAKE_INSTALL_PREFIX)
	# default install prefix
	set(CF_PREFIX "/usr/local")
else()
	set(CF_PREFIX "${CMAKE_INSTALL_PREFIX}")
endif()

# Look for dl(), some UNIX systems have it in -ldl, others in C library
check_library_exists(dl dlopen "" HAVE_LIBDL)
if(NOT HAVE_LIBDL)
	check_library_exists("" dlopen "" _HAVE_DIRECT_DLOPEN)
	if(NOT _HAVE_DIRECT_DLOPEN)
		message(FATAL_ERROR "Could not find dlopen() in -ldl or in system C library")
	endif()
endif()

# Look for iconv(), some UNIX systems have it in -liconv, others in C library
check_library_exists(iconv iconv "" HAVE_LIBICONV)
if(NOT HAVE_LIBICONV)
	check_library_exists("" iconv "" _HAVE_DIRECT_ICONV)
	if(NOT _HAVE_DIRECT_ICONV)
		message(FATAL_ERROR "Could not find iconv() in -liconv or in system C library")
	endif()
endif()

# Check for FP constants in float.h
check_symbol_exists(FLT_EPSILON "float.h" HAVE_FLT_EPSILON)
if(NOT HAVE_FLT_EPSILON)
	message(FATAL_ERROR "No C99 standards-compliant float.h with definition of FLT_EPSILON found.")
endif()
check_symbol_exists(DBL_EPSILON "float.h" HAVE_DBL_EPSILON)
if(NOT HAVE_DBL_EPSILON)
	message(FATAL_ERROR "No C99 standards-compliant float.h with definition of DBL_EPSILON found.")
endif()

# Test for these functions since replacements exist
set(CMAKE_REQUIRED_DEFINITIONS -D_GNU_SOURCE)
check_symbol_exists(strdup "string.h" HAVE_STRDUP)
check_symbol_exists(strndup "string.h" HAVE_STRNDUP)
check_symbol_exists(getline "stdio.h" HAVE_GETLINE)
check_symbol_exists(getdelim "stdio.h" HAVE_GETDELIM)
set(CMAKE_REQUIRED_DEFINITIONS)

# Look for threads
find_package(Threads REQUIRED)
if(NOT CMAKE_USE_PTHREADS_INIT)
	message(FATAL_ERROR "Only pthreads are supported currently")
endif()
# usage: target_link_libraries(myexe ${CMAKE_THREAD_LIBS_INIT})



# Find required packages
find_package(V8 REQUIRED)
find_package(IDN REQUIRED)
find_package(ICU REQUIRED)
find_package(CPPUNIT REQUIRED)
find_package(CURL REQUIRED)
find_package(PCREPP REQUIRED)

set(BOOST_LIBS serialization filesystem system)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)

include_directories("${IDN_INCLUDE_DIR}" "${ICU_INCLUDE}" "${Boost_INCLUDE_DIRS}" "${PCREPP_INCLUDE_DIR}")

add_subdirectory(src)
add_subdirectory(etc)

# Copy FindCForum.cmake macro (other programs may use it)
install(
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/FindCForum.cmake
  DESTINATION
    ${CMAKE_INSTALL_DATADIR}/cforum/cmake
)

# eof
