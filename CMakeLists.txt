cmake_minimum_required(VERSION 3.20)
project(hbricks
        VERSION 0.0.1
        DESCRIPTION "A remake of the Breakout game."
        LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
include_directories(${CMAKE_SOURCE_DIR})

# polyfill Lua on Windows
message(STATUS "Downloading Lua")
file(DOWNLOAD
        https://sourceforge.net/projects/luabinaries/files/5.4.2/Windows%20Libraries/Static/lua-5.4.2_Win64_vc16_lib.zip
        lua-5.4.2_Win64_vc16_lib.zip)
file(ARCHIVE_EXTRACT INPUT lua-5.4.2_Win64_vc16_lib.zip)
find_package(Lua 5.4.2 REQUIRED)

add_library(hbricks_common
        Game/Normal.c
        Lib/StrBuf.c)
target_include_directories(hbricks_common PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(hbricks_common PRIVATE ${LUA_LIBRARIES})

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
