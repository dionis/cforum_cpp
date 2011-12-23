#############################################################################
# CMakeLists.txt for the Classic Forum routing interface library
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")


# cfrouting
add_library(cfrouting SHARED router.cc route.cc)

target_link_libraries(cfrouting cfexceptions ${ICU_LIBRARY})

# eof
