cmake_minimum_required (VERSION 3.8)

project ("my_grep")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(Threads REQUIRED)

set(ENABLE_TESTS "OFF" CACHE BOOL "Enable building tests")

add_subdirectory ("src")

if (ENABLE_TESTS)
    if (MSVC)
        set(gtest_force_shared_crt on)
    endif()

    add_subdirectory(thirdparty/googletest)
    add_subdirectory(tests)

    add_custom_target(copy_test_data ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/tests/test_data
            ${CMAKE_BINARY_DIR}/tests/test_data
    )
    add_dependencies(my_grep_tests copy_test_data)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
