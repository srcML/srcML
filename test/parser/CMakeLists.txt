# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)

# Separate project driver for testing the parser outside of build

cmake_minimum_required(VERSION 3.24)
project(srcML-Test-parser NONE)
enable_testing()

if(PROJECT_IS_TOP_LEVEL OR SRCML_TEST_INSTALLED)
    find_program(SRCML_EXE NAMES srcml REQUIRED)
else()
    set(SRCML_EXE $<TARGET_FILE:srcml>)
endif()

add_subdirectory(testsuite)
