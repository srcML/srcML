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

# Separate project driver for testing libsrcml outside of build
if(NOT PROJECT_NAME MATCHES "^srcML")
    project(srcML-Test)
    cmake_minimum_required(VERSION 3.14)
    option(BUILD_CLIENT_TESTS "Build srcml client tests" ON)
    option(BUILD_LIBSRCML_TESTS "Build unit tests for libsrcml" ON)
    option(BUILD_PARSER_TESTS "Include tests for parser" ON)
    enable_testing()
else()
    include_directories(BEFORE ${CMAKE_SOURCE_DIR}/src/libsrcml)
endif()

if(BUILD_CLIENT_TESTS)
    add_subdirectory(client)
endif()

if(BUILD_LIBSRCML_TESTS)
    add_subdirectory(libsrcml)
endif()

if(BUILD_PARSER_TESTS)
    add_subdirectory(parser)
endif()
