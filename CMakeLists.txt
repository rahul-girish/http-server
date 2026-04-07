cmake_minimum_required(VERSION 3.23)

project(http-server)

add_library(server)

target_sources(server
    PRIVATE
    src/server.c

    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            include
        FILES
            include/server.h
)

add_library(linkedlist)

target_sources(linkedlist
    PRIVATE
    src/linkedlist.c

    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            include
        FILES
            include/linkedlist.h
)

add_executable(main)

target_sources(main
    PRIVATE
        src/main.c
)

target_link_libraries(main
    PRIVATE
)

option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()