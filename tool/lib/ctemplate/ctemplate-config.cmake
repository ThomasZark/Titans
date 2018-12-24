set(CTEMPLATE_DIR /data/titans/tool/lib/ctemplate/)

include_directories(${CTEMPLATE_DIR}/include/)

add_library(ctemplate STATIC IMPORTED)
set_target_properties(ctemplate PROPERTIES 
 IMPORTED_LOCATION ${CTEMPLATE_DIR}/lib/libctemplate.a)