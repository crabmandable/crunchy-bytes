cmake_minimum_required(VERSION 3.6)

project(CrunchyBytesTests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_COMPILER_IS_GNUCC)
    set(WFLAGS "-Wall -Wextra -Werror")
endif()

set(CRUNCHY_BYTES_SCHEMA_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/test-schemas/enum_header.toml"
    "${CMAKE_CURRENT_SOURCE_DIR}/test-schemas/name_header.toml"
    "${CMAKE_CURRENT_SOURCE_DIR}/test-schemas/type_one.toml"
    "${CMAKE_CURRENT_SOURCE_DIR}/test-schemas/type_two.toml"
    "${CMAKE_CURRENT_SOURCE_DIR}/test-schemas/type_three.toml"
    )

set(CRUNCHY_BYTES_GLOBALS "${CMAKE_CURRENT_SOURCE_DIR}/../test_globals.toml")
set(CRUNCHY_BYTES_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/test-schemas")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../" crunchy_bytes_interface)

# GTEST
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(test_sources "")
file(GLOB_RECURSE test_sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

add_executable(runTests ${test_sources})
target_link_libraries(runTests pthread crunchy_bytes_interface gtest_main)
