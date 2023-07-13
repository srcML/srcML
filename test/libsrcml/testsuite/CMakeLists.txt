# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2023 srcML, LLC. (www.srcML.org)
# 
# CMake files for libsrcml tests

# Build and add tests
add_custom_target(build_libsrcml_tests)
set(TEST_FILES "copy.xsl;setlanguage.xsl;schema.rng")
file(GLOB LIB_TESTS test_*.cpp)
foreach(LIB_TEST IN ITEMS ${LIB_TESTS})

    # Create executable test
    get_filename_component(TEST_NAME ${LIB_TEST} NAME_WE)
    add_executable(${TEST_NAME} ${LIB_TEST})
    target_link_libraries(${TEST_NAME} PRIVATE srcML::LibsrcML)
    set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

    # Add the test
    set(TEST_DIR ${CMAKE_BINARY_DIR}/bin/tmp/${TEST_NAME})
    add_test(NAME libsrcml.${TEST_NAME} COMMAND $<TARGET_FILE:${TEST_NAME}>
             WORKING_DIRECTORY ${TEST_DIR})
    file(MAKE_DIRECTORY ${TEST_DIR})
    # set_tests_properties(libsrcml.${TEST_NAME} PROPERTIES TIMEOUT 15)

    # Copy test files if they are used
    foreach(TESTFILE IN ITEMS ${TEST_FILES})
        # Copy file if used
        file(STRINGS "${LIB_TEST}" COPY_XSL REGEX "${TESTFILE}")
        if(COPY_XSL)
            configure_file(${TESTFILE} ${TEST_DIR}/${TESTFILE} COPYONLY)
        endif()
    endforeach()

    # Add to one large target
    add_dependencies(build_libsrcml_tests ${TEST_NAME})
endforeach()
