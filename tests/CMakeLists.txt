cmake_minimum_required(VERSION 3.8)
project(MyGrepTests)

file(GLOB_RECURSE TEST_SOURCES "*.cpp")
add_executable(my_grep_tests ${TEST_SOURCES})

target_include_directories(my_grep_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(my_grep_tests gtest gtest_main)

target_link_libraries(my_grep_tests my_grep_lib gmock gmock_main)

include(GoogleTest)

gtest_discover_tests(my_grep_tests)