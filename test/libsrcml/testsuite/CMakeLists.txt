    # SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
# 
# CMake files for libsrcml tests

# Build and add tests
add_custom_target(build_libsrcml_tests)
file(GLOB LIB_TESTS test_*.cpp)
foreach(LIB_TEST IN ITEMS ${LIB_TESTS})
    get_filename_component(TEST_NAME ${LIB_TEST} NAME_WE)
    add_executable(${TEST_NAME} ${LIB_TEST})
    target_link_libraries(${TEST_NAME} PRIVATE srcML::LibsrcML)
    set(TEST_DIR ${CMAKE_BINARY_DIR}/bin/tmp/${TEST_NAME})
    add_test(NAME libsrcml.${TEST_NAME} COMMAND $<TARGET_FILE:${TEST_NAME}>
             WORKING_DIRECTORY ${TEST_DIR})
    file(MAKE_DIRECTORY ${TEST_DIR})
    set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set_tests_properties(libsrcml.${TEST_NAME} PROPERTIES TIMEOUT 15)

    configure_file(copy.xsl ${TEST_DIR}/copy.xsl COPYONLY)
    configure_file(setlanguage.xsl ${TEST_DIR}/setlanguage.xsl COPYONLY)
    configure_file(schema.rng ${TEST_DIR}/schema.rng COPYONLY)

    add_dependencies(build_libsrcml_tests ${TEST_NAME})
endforeach()

# Copy xpath test data
configure_file(copy.xsl ${CMAKE_BINARY_DIR}/bin/copy.xsl COPYONLY)
configure_file(setlanguage.xsl ${CMAKE_BINARY_DIR}/bin/setlanguage.xsl COPYONLY)
configure_file(schema.rng ${CMAKE_BINARY_DIR}/bin/schema.rng COPYONLY)
