#############################################################################
# CMakeLists.txt for the Classic Forum cgi interface library
#############################################################################

include_directories (BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}")


# libcfcgi
add_library(cfcgi SHARED cgi.cc cgi_exception.cc parameter_exception.cc cgi_parser_exception.cc)

target_link_libraries(cfcgi cfexceptions ${ICU_LIBRARY})

# eof
