cmake_minimum_required(VERSION 3.5)
project(id3v2lib VERSION 1.0.0)

include(GNUInstallDirs)

option(BUILD_SHARED_LIBS "Build shared library" ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_STANDARD 99)

set(ID3V2_PUBLIC_HEADERS 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/id3v2lib/id3v2lib.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/id3v2lib/types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/id3v2lib/constants.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/id3v2lib/header.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/id3v2lib/frame.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/id3v2lib/utils.h
)

add_library(id3v2
    ${ID3V2_PUBLIC_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frame.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/header.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/id3v2lib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/types.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.c
)

set_target_properties(id3v2 PROPERTIES PUBLIC_HEADER "${ID3V2_PUBLIC_HEADERS}")

target_include_directories(id3v2
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/id3v2lib
)

install(TARGETS id3v2
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/id3v2lib
)
