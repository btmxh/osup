cmake_minimum_required(VERSION 3.12)

project(osup C)

set(CMAKE_C_STANDARD 90)

add_library(osup
  osup/osup_common.c
  osup/osup_beatmap.c
)

target_include_directories(osup PUBLIC .)

include(CheckLibraryExists)
check_library_exists(m sin "" OSUP_NO_LIBM)

if(${OSUP_NO_LIBM})
  target_link_libraries(osup PUBLIC m)
endif()

option(OSUP_BUILD_TESTS "Build osup tests" ON)
option(OSUP_LOGGING "Enable osup logging, may cause overhead" ON)

if(NOT ${OSUP_LOGGING})
  target_compile_definitions(osup PUBLIC OSUP_NO_LOGGING)
endif()

add_subdirectory(tests)

