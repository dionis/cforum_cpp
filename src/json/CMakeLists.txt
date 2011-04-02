#############################################################################
# CMakeLists.txt for the Classic Forum JSON parser library
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")

# libcfjson
add_library(cfjson SHARED
  JSONArray.cc
  JSONElement.cc
  JSONNull.cc
  JSONObject.cc
  JSONString.cc
  JSONBoolean.cc
  JSONNumber.cc
  JSONParser.cc
)

target_link_libraries(cfjson cfexceptions ${ICU_LIBRARY})

# eof
