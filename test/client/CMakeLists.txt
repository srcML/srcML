# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)

# Separate project driver for testing the client outside of build
cmake_minimum_required(VERSION 3.24)
project(srcML-Test-client NONE)
enable_testing()

add_subdirectory(testsuite)
