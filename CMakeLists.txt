cmake_minimum_required(VERSION 3.25)
project(cxx_record_accessor)

set(CMAKE_CXX_STANDARD 11)

add_library(record_accessor)
target_include_directories(record_accessor INTERFACE .)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    target_compile_definitions(record_accessor INTERFACE DEBUG)
endif ()

add_subdirectory(record)

enable_testing()
add_subdirectory(test)