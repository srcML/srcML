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

# Separate project driver for testing the parser outside of build
if(NOT PROJECT_NAME MATCHES "^srcML")
    cmake_minimum_required(VERSION 3.24)
    project(srcML-Test-parser NONE)
    enable_testing()
    find_program(SRCML_EXE NAMES srcml REQUIRED)
elseif(SRCML_TEST_INSTALLED)
    find_program(SRCML_EXE NAMES srcml REQUIRED)
else()
    set(SRCML_EXE $<TARGET_FILE:srcml>)
endif()

add_subdirectory(testsuite)
