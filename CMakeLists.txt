##
# @file CMakeLists.txt
#
# @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
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
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#    The CMake project file for srcML.
#
cmake_minimum_required(VERSION 2.8.12)
project(srcML)

enable_testing()

# Setting up Project includes.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
include (srcml)

set(CMAKE_MACOSX_RPATH 1)

# Setting up source include directories.
include_directories(BEFORE src/srcml src/libsrcml src/parser src/srcSAX)

# Setting major and minor version numbers (TODO fix this so that it is set using something
# more correct that being input from within the build system).
add_definitions(-DVERSION=\"1\")
add_definitions(-DREVISION=\"1\")

# Including extra directories.
add_subdirectory(src)

if(RUN_TIMING_TESTS)
    add_subdirectory(timing)
endif()

if(BUILD_UNIT_TESTS)
    add_subdirectory(test)
endif()

if(BUILD_CLI_TESTS OR BUILD_LARGE_SYSTEMS_TESTS)
    add_subdirectory(cli)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(${BUILD_LANGUAGE_BINDINGS})
    add_subdirectory(bindings)
endif()
