cmake_minimum_required (VERSION 3.8)

add_library(my_grep_lib STATIC
    my_grep.hpp
    my_grep.cpp
    logger.hpp
    logger.cpp
    ilogger.hpp
)

add_executable (my_grep 
    main.cpp)

target_link_libraries(my_grep PRIVATE my_grep_lib Threads::Threads)