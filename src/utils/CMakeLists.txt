include_directories (BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR})

add_library(
    cfutils
  SHARED
    arrayutils.c
    datautils.c
    ipcutils.c
    listutils.c
    memoryutils.c
    stringutils.c
    treeutils.c
    utils.c
)
target_link_libraries(cfutils ${ICU_LIBRARY})

install(
  FILES
    arrayutils.h
    datautils.h
    ipcutils.h
    listutils.h
    memoryutils.h
    stringutils.h
    treeutils.h
    utils.h
  DESTINATION
    ${CMAKE_INSTALL_PREFIX}/include/cforum/utils
)

# eof