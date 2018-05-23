cmake_minimum_required(VERSION 3.10)
project(RedoCourse C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(example_submission)
include_directories(map_mtm)
include_directories(tests)

add_executable(RedoCourse
        map_mtm/map_mtm.h)
