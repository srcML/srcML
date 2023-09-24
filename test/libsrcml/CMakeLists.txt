# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
# 
# CMake files for libsrcml library

# Separate project driver for testing libsrcml outside of build
cmake_minimum_required(VERSION 3.24)
project(srcML-Test-libsrcml)
enable_testing()

if(SRCML_TEST_INSTALLED OR NOT TARGET srcML::LibsrcML)
    find_package(srcML REQUIRED)
endif()

include_directories(.)

set(CMAKE_CXX_STANDARD 11)

# standard compile options
add_compile_options(
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall>
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wextra>
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-pedantic>

    $<$<CXX_COMPILER_ID:MSVC>:/Wall>
    $<$<CXX_COMPILER_ID:MSVC>:/D_CRT_SECURE_NO_WARNINGS>  # disable strcpy() warnings
    $<$<CXX_COMPILER_ID:MSVC>:/D_CRT_NONSTDC_NO_WARNINGS> # disable read() warnings
)

# handle specific warnings
option(ENABLE_BUILD_WARNINGS "Enable disabled build warnings" OFF)
if(NOT ENABLE_BUILD_WARNINGS)
    add_compile_options(
        $<$<CXX_COMPILER_ID:MSVC>:/wd4514> # removed inline function not called (in one context)
        $<$<CXX_COMPILER_ID:MSVC>:/wd4710> # unmarked inlining notification warning
        $<$<CXX_COMPILER_ID:MSVC>:/wd4711> # selected for automatic inline expansion
        $<$<CXX_COMPILER_ID:MSVC>:/wd4820> # byte padding
    )
endif()

add_subdirectory(testsuite)
