# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2014-2024 srcML, LLC. (www.srcML.org)
# 
# CMake files for client tests

option(RUN_REMOTE_CLIENT_TESTS "Build remote client tests" ON)

# add client tests
file(GLOB REMOTE_CLIENT_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/remote*.sh")
file(GLOB LOCAL_CLIENT_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/*.sh")
list(FILTER LOCAL_CLIENT_TESTS EXCLUDE REGEX "/remote_|framework_test.sh")

# use Git Bash to run tests on Windows
set(BASH bash)
if (WIN32)
    find_program(GIT_BASH "git-bash" PATHS
        "C:/Program Files/Git"
        "C:/Program Files/Git/usr"
        "C:/Program Files (x86)/Git/usr"
        "C:/Program Files (x86)/Git"
    NO_DEFAULT_PATH)
    if (GIT_BASH)
        set(BASH ${GIT_BASH} --hide)
    endif()
endif()

foreach(LOCAL_CLIENT_TEST IN ITEMS ${LOCAL_CLIENT_TESTS})
    get_filename_component(TEST_NAME ${LOCAL_CLIENT_TEST} NAME)

    add_test(NAME srcml.${TEST_NAME} COMMAND ${BASH} ${LOCAL_CLIENT_TEST})
    # Command line --timeout does NOT override this value
    #set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT 10)

    set_tests_properties(srcml.${TEST_NAME} PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

if(RUN_REMOTE_CLIENT_TESTS)
    foreach(REMOTE_CLIENT_TEST IN ITEMS ${REMOTE_CLIENT_TESTS})
        get_filename_component(TEST_NAME ${REMOTE_CLIENT_TEST} NAME)

        add_test(NAME srcml.${TEST_NAME} COMMAND ${BASH} ${REMOTE_CLIENT_TEST})
        # Command line --timeout does NOT override this value
        #set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT 10)

        set_tests_properties(srcml.${TEST_NAME} PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    endforeach()
endif()
