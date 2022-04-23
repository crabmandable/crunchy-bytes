cmake_minimum_required(VERSION 3.6)

project(CerealPack CXX)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(Python3 COMPONENTS Interpreter REQUIRED)
if(NOT ${Python3_FOUND})
    message(FATAL_ERROR "Python3 is required to generate the cereal_pack classes")
endif()

if (NOT DEFINED CEREAL_PACK_SCHEMA_DIR AND NOT DEFINED CEREAL_PACK_SCHEMA_FILES)
    message(FATAL_ERROR
        "A schema directory or schema files should be defined using the CMake variables CEREAL_PACK_SCHEMA_DIR or CEREAL_PACK_SCHEMA_FILES")
elseif(DEFINED CEREAL_PACK_SCHEMA_DIR)
    set(CEREAL_PACK_SCHEMA_FILES "")
    file(GLOB_RECURSE CEREAL_PACK_SCHEMA_FILES "${CEREAL_PACK_SCHEMA_DIR}/*.toml")
endif()

if (NOT DEFINED CEREAL_PACK_OUTPUT_DIR)
    message(FATAL_ERROR
        "An output directory for generated cereal_pack classes must be set using the CMake variable CEREAL_PACK_OUTPUT_DIR")
endif()

if (DEFINED CEREAL_PACK_GLOBALS)
    set(CEREAL_PACK_GLOBALS_OPT "-g" "${CEREAL_PACK_GLOBALS}")
endif()

set(CEREAL_PACK_GENERATE_COMMAND
    "${Python3_EXECUTABLE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/cereal_pack.py"
    "-o" "${CEREAL_PACK_OUTPUT_DIR}"
    "-s" "${CEREAL_PACK_SCHEMA_FILES}"
    "${CEREAL_PACK_GLOBALS_OPT}"
    )

execute_process(
    COMMAND ${CEREAL_PACK_GENERATE_COMMAND} --cmake
    OUTPUT_VARIABLE GENERATED_FILES
    RESULT_VARIABLE RETURN_VALUE
)

if (NOT RETURN_VALUE EQUAL 0)
    message(FATAL_ERROR "Failed to generate source list from cereal_pack schemas. See above")
endif()

set(CEREAL_PACK_GENERATOR_CODE "")
file(GLOB_RECURSE CEREAL_PACK_GENERATOR_CODE
    "${CMAKE_CURRENT_SOURCE_DIR}/cereal_pack.py"
    "${CMAKE_CURRENT_SOURCE_DIR}/generator/*.py")

add_custom_command(
    COMMAND ${CEREAL_PACK_GENERATE_COMMAND}
    DEPENDS ${CEREAL_PACK_GENERATOR_CODE} ${CEREAL_PACK_SCHEMA_FILES} ${CEREAL_PACK_GLOBALS}
    OUTPUT ${GENERATED_FILES}
    COMMENT "Generating cereal_pack classes"
)

set(CEREAL_PACK_SOURCES "")
file(GLOB_RECURSE CEREAL_PACK_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/cereal_pack.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/serialization/*.hpp")


add_library(cereal_pack_interface INTERFACE ${CEREAL_PACK_SOURCES} ${GENERATED_FILES})
target_include_directories(cereal_pack_interface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${CEREAL_PACK_OUTPUT_DIR})
