cmake_minimum_required(VERSION 3.6)

project(CerealPackTests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_COMPILER_IS_GNUCC)
    set(WFLAGS "-Wall -Wextra -Werror")
endif()

set(CEREAL_PACK_SCHEMA_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/test-schemas/nesting.toml"
    "${CMAKE_CURRENT_SOURCE_DIR}/test-schemas/one_bool.toml"
    "${CMAKE_CURRENT_SOURCE_DIR}/test-schemas/simple.toml"
    "${CMAKE_CURRENT_SOURCE_DIR}/test-schemas/using_globals.toml"
    )

set(CEREAL_PACK_GLOBALS "${CMAKE_CURRENT_SOURCE_DIR}/test_globals.toml")
set(CEREAL_PACK_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/test-schemas")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../" cereal_pack_interface)

# GTEST
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(test_sources "")
file(GLOB_RECURSE test_sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

add_executable(runTests ${test_sources})
target_link_libraries(runTests pthread cereal_pack_interface gtest_main)
