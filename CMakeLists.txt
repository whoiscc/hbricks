cmake_minimum_required(VERSION 3.20)
project(hbricks C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
include_directories(${CMAKE_SOURCE_DIR})

add_library(hbricks_common
        Game/Normal.c
        StrBuf.c)

add_executable(hbricks_gdi+ WIN32
        Platform/GDI+/App.cxx
        Platform/GDI+/View.cxx)
target_link_libraries(hbricks_gdi+ PRIVATE hbricks_common)

add_executable(hbricks_text
        Platform/Text/App.c
        Platform/Text/View.c
        Platform/Text/Emit.c)
target_link_libraries(hbricks_text PRIVATE hbricks_common)

add_subdirectory(Test)
