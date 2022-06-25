cmake_minimum_required(VERSION 3.6)

project(CruncyBytes CXX)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(Python3 COMPONENTS Interpreter REQUIRED)
if(NOT ${Python3_FOUND})
    message(FATAL_ERROR "Python3 is required to generate the crunchy_bytes classes")
endif()

if (NOT DEFINED CRUNCHY_BYTES_SCHEMA_DIR AND NOT DEFINED CRUNCHY_BYTES_SCHEMA_FILES)
    message(FATAL_ERROR
        "A schema directory or schema files should be defined using the CMake variables CRUNCHY_BYTES_SCHEMA_DIR or CRUNCHY_BYTES_SCHEMA_FILES")
elseif(DEFINED CRUNCHY_BYTES_SCHEMA_DIR)
    set(CRUNCHY_BYTES_SCHEMA_FILES "")
    file(GLOB_RECURSE CRUNCHY_BYTES_SCHEMA_FILES "${CRUNCHY_BYTES_SCHEMA_DIR}/*.toml")
endif()

if (NOT DEFINED CRUNCHY_BYTES_OUTPUT_DIR)
    message(FATAL_ERROR
        "An output directory for generated crunchy_bytes classes must be set using the CMake variable CRUNCHY_BYTES_OUTPUT_DIR")
endif()

if (DEFINED CRUNCHY_BYTES_GLOBALS)
    set(CRUNCHY_BYTES_GLOBALS_OPT "-g" "${CRUNCHY_BYTES_GLOBALS}")
endif()

set(CRUNCHY_BYTES_GENERATE_COMMAND
    "${Python3_EXECUTABLE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/crunchy_bytes.py"
    "-o" "${CRUNCHY_BYTES_OUTPUT_DIR}"
    "-s" "${CRUNCHY_BYTES_SCHEMA_FILES}"
    "${CRUNCHY_BYTES_GLOBALS_OPT}"
    )

execute_process(
    COMMAND ${CRUNCHY_BYTES_GENERATE_COMMAND} --cmake
    OUTPUT_VARIABLE GENERATED_FILES
    RESULT_VARIABLE RETURN_VALUE
)

if (NOT RETURN_VALUE EQUAL 0)
    message(FATAL_ERROR "Failed to generate source list from crunchy_bytes schemas. See above")
endif()

set(CRUNCHY_BYTES_GENERATOR_CODE "")
file(GLOB_RECURSE CRUNCHY_BYTES_GENERATOR_CODE
    "${CMAKE_CURRENT_SOURCE_DIR}/crunchy_bytes.py"
    "${CMAKE_CURRENT_SOURCE_DIR}/generator/*.py")

add_custom_command(
    COMMAND ${CRUNCHY_BYTES_GENERATE_COMMAND}
    DEPENDS ${CRUNCHY_BYTES_GENERATOR_CODE} ${CRUNCHY_BYTES_SCHEMA_FILES} ${CRUNCHY_BYTES_GLOBALS}
    OUTPUT ${GENERATED_FILES}
    COMMENT "Generating crunchy_bytes classes"
)

add_library(crunchy_bytes_interface INTERFACE ${GENERATED_FILES})
target_include_directories(crunchy_bytes_interface INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/includes" "${CRUNCHY_BYTES_OUTPUT_DIR}")
