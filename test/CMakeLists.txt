add_executable(ut test.cpp)

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip) # v1.15.x for C++11
FetchContent_MakeAvailable(googletest)

target_link_libraries(ut PRIVATE record_accessor GTest::gmock_main)
add_test(NAME ut COMMAND ut)