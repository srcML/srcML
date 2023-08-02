# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2014-2023 srcML, LLC. (www.srcML.org)
# 
# CMake files for libsrcml examples

cmake_minimum_required(VERSION 3.24)

project(srcML-Examples-libsrcml)

if(SRCML_TEST_INSTALLED OR NOT TARGET srcML::LibsrcML)
    find_package(srcml REQUIRED)
endif()

# Install directory for libsrcml examples
set(LIBSRCML_EXAMPLES_DIRECTORY share/srcml/examples/libsrcml)

# Build examples directory
set(EXAMPLE_ROOT "${CMAKE_BINARY_DIR}/examples/libsrcml/")

# Example files
set(TEST_FILES "decl_stmt.cpp;expr.cpp;define.cpp;project.xml;schema.rng;copy.xsl")

# Setup the examples
# * Create directory with example
# * Generate CMakeLists.txt customized for that example
# * Copy any test files used by the example
# * Append to the CMakeLists.txt commands to copy test files to the binary
file(GLOB EXAMPLES "srcml_*.cpp")
foreach(EXAMPLE IN ITEMS ${EXAMPLES})
    get_filename_component(EXAMPLE_NAME "${EXAMPLE}" NAME_WE)

    set(EXAMPLE_DIR "${EXAMPLE_ROOT}/${EXAMPLE_NAME}")

    # Create a directory for each example
    file(MAKE_DIRECTORY "${EXAMPLE_DIR}")

    # Copy the example source file into its directory
    file(COPY ${EXAMPLE} DESTINATION "${EXAMPLE_DIR}")

    # Customize a CMakeLists.txt for each example
    configure_file(CMakeLists.txt.in "${EXAMPLE_DIR}/CMakeLists.txt")

    get_filename_component(EXAMPLE_NAME "${EXAMPLE_DIR}" NAME)

    # Copy test files if they are used
    set(ADD_COPY_TESTFILES "")
    foreach(TESTFILE IN ITEMS ${TEST_FILES})
        # Copy file if used
        file(STRINGS "${EXAMPLE}" TESTFILE_FOUND REGEX "${TESTFILE}")
        if(TESTFILE_FOUND)
            file(COPY ${TESTFILE} DESTINATION "${EXAMPLE_DIR}")
            string(APPEND ADD_COPY_TESTFILES "file(COPY ${TESTFILE} DESTINATION \${CMAKE_BINARY_DIR})\n")
        endif()
    endforeach()
    if(NOT ADD_COPY_TESTFILES STREQUAL "")
        file(APPEND "${EXAMPLE_DIR}/CMakeLists.txt"
            "\n# Copy data files\n"
            "${ADD_COPY_TESTFILES}\n" )
    endif()

    # Install this example subdirectory
    install(DIRECTORY "${EXAMPLE_DIR}" DESTINATION "${LIBSRCML_EXAMPLES_DIRECTORY}" COMPONENT DEVLIBS)

endforeach()

# Create an overall CMake project in the example root directory
file(COPY_FILE "CMakeLists.root.txt.in" "${EXAMPLE_ROOT}/CMakeLists.txt")

# Copy/install the example files
file(COPY ${TEST_FILES} DESTINATION "${EXAMPLE_ROOT}")
install(FILES ${TEST_FILES} DESTINATION "${LIBSRCML_EXAMPLES_DIRECTORY}" COMPONENT DEVLIBS)

# Append to the build file for each example
foreach(EXAMPLE IN ITEMS ${EXAMPLES})

    # Base filename
    get_filename_component(EXAMPLE_NAME "${EXAMPLE}" NAME_WE)

    # Add test to overall build
    file(APPEND "${EXAMPLE_ROOT}/CMakeLists.txt"
        "# Example ${EXAMPLE_NAME}\n"
        "add_executable(${EXAMPLE_NAME} ${EXAMPLE_NAME}/${EXAMPLE_NAME}.cpp)\n"
        "target_link_libraries(${EXAMPLE_NAME} PRIVATE srcML::LibsrcML)\n"
        "add_custom_target(run_${EXAMPLE_NAME} COMMAND $<TARGET_FILE:${EXAMPLE_NAME}> DEPENDS ${EXAMPLE_NAME} USES_TERMINAL)\n"
        "add_dependencies(run run_${EXAMPLE_NAME})\n\n")

endforeach()

# Install the CMake build file for all of the examples
install(FILES "${EXAMPLE_ROOT}/CMakeLists.txt" DESTINATION "${LIBSRCML_EXAMPLES_DIRECTORY}" COMPONENT DEVLIBS)
