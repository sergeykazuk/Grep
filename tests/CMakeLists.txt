include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

project(MyGrepTests)

file(GLOB_RECURSE TEST_SOURCES "*.cpp")
add_executable(my_grep_tests ${TEST_SOURCES})

target_include_directories(my_grep_tests PRIVATE ${CMAKE_SOURCE_DIR}/includes)

target_link_libraries(my_grep_tests PRIVATE my_grep_lib gtest gtest_main gmock gmock_main)

include(GoogleTest)
gtest_discover_tests(my_grep_tests)