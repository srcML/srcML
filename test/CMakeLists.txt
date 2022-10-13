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
cmake_minimum_required(VERSION 3.24)
project(srcML-Test)
enable_testing()

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
