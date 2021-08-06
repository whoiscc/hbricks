cmake_minimum_required(VERSION 3.20)
project(hbricks C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)  # MSVC requires at least C++20
include_directories(${CMAKE_SOURCE_DIR})

add_library(hbricks_common
        Game.c
        Views/Log.c)

add_executable(hbricks_gdi+ WIN32
        Apps/GDI+.cxx
        Views/GDI+.cxx)
target_link_libraries(hbricks_gdi+ hbricks_common)

add_executable(hbricks_text
        Apps/Text.c
        Views/Text.c)
target_link_libraries(hbricks_text hbricks_common)

add_subdirectory(Tests)
