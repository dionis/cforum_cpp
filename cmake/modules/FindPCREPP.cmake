# - Try to find the PCRE C++ regular expression library
# Once done this will define
#
#  PCREPP_FOUND - system has the PCRE library
#  PCREPP_INCLUDE_DIR - the PCRE++ include directory
#  PCREPP_LIBRARIES - The libraries needed to use PCRE
#
# Copyright (c) 2011, Christian Kruse, <cjk@wwwtech.de>
#
# Redistribution and use is allowed according to the terms of the MIT license.



if (PCREPP_INCLUDE_DIR AND PCREPP_LIBRARIES AND PCREPP_PCREPP_LIBRARY)
  # Already in cache, be silent
  set(PCREPP_FIND_QUIETLY TRUE)
endif (PCREPP_INCLUDE_DIR AND PCREPP_LIBRARIES AND PCREPP_PCREPP_LIBRARY)

FIND_PATH(PCREPP_INCLUDE_DIR pcre++.h)
FIND_LIBRARY(PCREPP_PCREPP_LIBRARY NAMES pcre++)

set(PCREPP_LIBRARIES ${PCREPP_PCREPP_LIBRARY} CACHE STRING "The libraries needed to use PCRE++")

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PCREPP DEFAULT_MSG PCREPP_INCLUDE_DIR PCREPP_LIBRARIES)

MARK_AS_ADVANCED(PCREPP_INCLUDE_DIR PCREPP_LIBRARIES PCREPP_PCREPP_LIBRARY)
