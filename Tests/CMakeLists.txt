include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test_game TestGame.cxx)
target_include_directories(test_game PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_game hbricks_common gtest_main)
add_test(NAME test_game COMMAND test_game)
