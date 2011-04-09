#############################################################################
# CMakeLists.txt for the Classic Forum couchdb interface library
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")


# libcfcgi
add_library(cfcgi SHARED CGI.cc Parameter.cc)

target_link_libraries(cfcgi cfexceptions ${ICU_LIBRARY})

# eof
