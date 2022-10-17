# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)

# Separate project driver for testing libsrcml outside of build
cmake_minimum_required(VERSION 3.24)
project(srcML-Test)
enable_testing()
include(CTest)

# Turn ON/OFF tests
option(BUILD_CLIENT_TESTS "Build srcml client tests" ON)
option(BUILD_LIBSRCML_TESTS "Build unit tests for libsrcml" OFF)
option(BUILD_PARSER_TESTS "Include tests for parser" OFF)

# Turn ON building all tests
option(BUILD_ALL_TESTS "Build all tests" OFF)
if(BUILD_ALL OR BUILD_ALL_TESTS)
    set(BUILD_CLIENT_TESTS ON)
    set(BUILD_LIBSRCML_TESTS ON)
    set(BUILD_PARSER_TESTS ON)
endif()

option(SRCML_TEST_INSTALLED "Test using installed version" OFF)

if(PROJECT_IS_TOP_LEVEL OR BUILD_CLIENT_TESTS)
    add_subdirectory(client)
endif()

if(PROJECT_IS_TOP_LEVEL OR BUILD_LIBSRCML_TESTS)
    add_subdirectory(libsrcml)
endif()

if(PROJECT_IS_TOP_LEVEL OR BUILD_PARSER_TESTS)
    add_subdirectory(parser)
endif()
