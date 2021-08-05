cmake_minimum_required(VERSION 3.20)
project(hbricks C CXX)

set(CMAKE_C_STANDARD 99)

add_library(hbricks_common
        Game.c
        Views/Log.c)
target_include_directories(hbricks_common PRIVATE ${CMAKE_SOURCE_DIR})

add_executable(hbricks_gdi+ WIN32 main.cxx)
target_link_libraries(hbricks_gdi+ hbricks_common)

add_subdirectory(Tests)
