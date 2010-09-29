## required modules

set(MODULES_LIST flt_dummy)

add_library(flt_dummy MODULE flt_dummy.c)


## optional modules

set(OPTIONAL_MODULES flt_dummy_optional)

foreach (module ${OPTIONAL_MODULES})
  option(ENABLE_${module} "Enable optional module ${module}" OFF)
endforeach (module)

foreach (module ${OPTIONAL_MODULES})
  if (ENABLE_${module} STREQUAL "ON")
    message(STATUS "Enabling optional module ${module}")
    add_library(${module} MODULE ${module}.c)
    list(APPEND MODULES_LIST ${module})    
  endif (ENABLE_${module} STREQUAL "ON")
endforeach (module)


## set build properties and install rules

set_target_properties(
    ${MODULES_LIST}
  PROPERTIES
    LINK_FLAGS "-export-dynamic -avoid-version"
    PREFIX ""
)

install(
  TARGETS
    ${MODULES_LIST}
  LIBRARY
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cforum
)


# eof

