SET(V8_FOUND FALSE)

INCLUDE(CheckIncludeFiles)
CHECK_INCLUDE_FILES(v8.h V8_FOUND)

IF(V8_FIND_REQUIRED)
  IF(NOT V8_FOUND)
    MESSAGE(FATAL_ERROR "Could not find V8 library")
  ENDIF()
ENDIF()

