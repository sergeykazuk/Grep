cmake_minimum_required(VERSION 3.20)

project ("my_grep")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ENABLE_TESTS "OFF" CACHE BOOL "Enable building tests")

include(CTest)

# Set includes folder globally
include_directories(${CMAKE_SOURCE_DIR}/includes)

# Create static library at top level
add_library(my_grep_lib STATIC
    src/my_grep.cpp
    src/logger.cpp
)

add_executable(my_grep src/main.cpp)
target_link_libraries(my_grep PRIVATE my_grep_lib)

if (ENABLE_TESTS)
    add_subdirectory(tests)

    add_custom_target(copy_test_data ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/tests/test_data
            ${CMAKE_BINARY_DIR}/tests/test_data
    )
    add_dependencies(my_grep_tests copy_test_data)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
