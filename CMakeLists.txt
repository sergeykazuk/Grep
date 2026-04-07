cmake_minimum_required(VERSION 3.20)

project ("my_grep")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ENABLE_TESTS "OFF" CACHE BOOL "Enable building tests")
option(ENABLE_CLANG_TIDY "Enable clang-tidy checks" OFF)

if (ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy)
    if (CLANG_TIDY_EXE)
        set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE})
    else()
        message(WARNING "ENABLE_CLANG_TIDY is ON, but clang-tidy was not found")
    endif()
endif()

include(CTest)

# Set includes folder globally
include_directories(${CMAKE_SOURCE_DIR}/includes)

# Create static library at top level
add_library(my_grep_lib STATIC
    src/my_grep.cpp
    src/logger.cpp
    src/logging/formatters.cpp
    src/logging/consolebackend.cpp
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

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
