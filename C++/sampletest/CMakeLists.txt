cmake_minimum_required(VERSION 3.14)

project(MyProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

FetchContent_MakeAvailable(googletest)

add_library(my_lib 
           PrintAB.cpp
)

target_include_directories(my_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)

add_executable(
    unit_tests
    test/CanMessage_hash_table_test.cpp

    test/PrintABTest.cpp

    test/BoundedQueue_Daynamic_test.cpp
)

target_include_directories(
    unit_tests
    PRIVATE
     ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

target_link_libraries(
    unit_tests
    PRIVATE
    my_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)