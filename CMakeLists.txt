cmake_minimum_required(VERSION 3.20)
project(hbricks C CXX)

set(CMAKE_C_STANDARD 99)

add_library(hbricks_common
        Game.c)

add_executable(hbricks WIN32 main.cxx)
target_link_libraries(hbricks hbricks_common)

add_subdirectory(Tests)
