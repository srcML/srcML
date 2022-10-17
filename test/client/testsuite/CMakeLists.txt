# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
# 
# CMake files for client tests

# add client tests
file(GLOB CLIENT_TESTS *.sh)
list(REMOVE_ITEM CLIENT_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/framework_test.sh")

foreach(CLIENT_TEST IN ITEMS ${CLIENT_TESTS})
    get_filename_component(TEST_NAME ${CLIENT_TEST} NAME)

    add_test(NAME srcml.${TEST_NAME} COMMAND ${CLIENT_TEST})
    # Command line --timeout does NOT override this value
    #set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT 10)
endforeach()
