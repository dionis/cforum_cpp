#include(FindPkgMacros)
#findpkg_begin(V8)

find_path(V8_INCLUDE_DIR NAMES v8.h)
find_library(V8_LIBRARY NAMES v8)

if(NOT V8_LIBRARY)
  find_library(V8_LIBRARY NAMES v8_g)
endif()

#findpkg_finish(V8)
