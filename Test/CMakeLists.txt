include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
MESSAGE(STATUS "Populating GoogleTest")
FetchContent_MakeAvailable(googletest)

add_executable(test_hbricks
        LogView.c
        NormalGameTest.cxx
        StrBufTest.cxx)
target_link_libraries(test_hbricks PRIVATE hbricks_common gtest_main)
add_test(NAME hbricks COMMAND test_hbricks)
