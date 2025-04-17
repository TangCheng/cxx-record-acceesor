add_executable(ut test.cpp)

find_package(GTest REQUIRED)

target_link_libraries(ut PRIVATE record_accessor GTest::gmock_main)
add_test(NAME ut COMMAND ut)