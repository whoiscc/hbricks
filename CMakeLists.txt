cmake_minimum_required(VERSION 3.20)
project(hbricks C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
include_directories(${CMAKE_SOURCE_DIR})

add_library(hbricks_common
        Game.c
        LogView.c)

add_executable(hbricks_gdi+ WIN32
        Platforms/GDI+/App.cxx
        Platforms/GDI+/View.cxx)
target_link_libraries(hbricks_gdi+ hbricks_common)

add_executable(hbricks_text
        Platforms/Text/App.c
        Platforms/Text/View.c)
target_link_libraries(hbricks_text hbricks_common)

add_subdirectory(Tests)
