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
    project(srcML-Test-libsrcml)
    cmake_minimum_required(VERSION 3.14)
    enable_testing()
endif()

if(NOT TARGET libsrcml_link)
    find_library(LIBSRCML_LINK NAMES libsrcml libsrcml.1 libsrcml.1.dylib libsrcml.so.1 libsrcml.so)
else()
    set(LIBSRCML_LINK libsrcml_link)
endif()

include_directories(.)

include_directories(/usr/local/include)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-DWITH_LIBXSLT)

add_subdirectory(testsuite)
