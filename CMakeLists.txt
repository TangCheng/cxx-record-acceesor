cmake_minimum_required(VERSION 3.25)
project(cxx_record_accessor)

set(CMAKE_CXX_STANDARD 17)

add_library(record_accessor INTERFACE)
target_include_directories(record_accessor INTERFACE .)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    target_compile_definitions(record_accessor INTERFACE DEBUG)
endif ()

find_package(Boost REQUIRED COMPONENTS describe)
target_link_libraries(record_accessor INTERFACE Boost::headers)

enable_testing()
add_subdirectory(test)