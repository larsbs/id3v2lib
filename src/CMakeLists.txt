INCLUDE_DIRECTORIES(${id3v2lib_SOURCE_DIR}/include ${id3v2lib_SOURCE_DIR}/include/id3v2lib)

SET(id3v2_src frame.c header.c id3v2lib.c types.c utils.c)
SET(id3v2_h_directory ${id3v2lib_SOURCE_DIR}/include/id3v2lib)
SET(id3v2_headers ${id3v2_h_directory}/constants.h ${id3v2_h_directory}/frame.h ${id3v2_h_directory}/header.h ${id3v2_h_directory}/types.h ${id3v2_h_directory}/utils.h)

ADD_LIBRARY(id3v2 STATIC ${id3v2_src})

INSTALL(TARGETS id3v2 DESTINATION lib)
INSTALL(FILES ${id3v2_headers} DESTINATION include/id3v2lib)
INSTALL(FILES ${id3v2lib_SOURCE_DIR}/include/id3v2lib.h DESTINATION include)