##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
# 
# The srcML Toolkit is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# The srcML Toolkit is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with the srcML Toolkit; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
# 
# CMake files for libsrcml library

# Separate project driver for testing libsrcml outside of build
if(NOT PROJECT_NAME MATCHES "^srcML")
    cmake_minimum_required(VERSION 3.24)
    project(srcML-Test-libsrcml)
    enable_testing()
endif()

if(SRCML_TEST_INSTALLED OR NOT TARGET libsrcml_link)
    find_library(LIBSRCML_LINK NAMES srcml libsrcml REQUIRED)
else()
    set(LIBSRCML_LINK libsrcml_link)
endif()

include_directories(BEFORE ${CMAKE_SOURCE_DIR}/src/libsrcml)
include_directories(.)

set(CMAKE_CXX_STANDARD 11)

# standard compile options
add_compile_options(
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-fPIC>
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall>
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wextra>
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-pedantic>
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-ansi>

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
