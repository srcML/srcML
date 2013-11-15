# @copyright
# 
# Copyright (C) 2013  SDML (www.srcML.org)
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
# Testing macros/functions and additional variables. These functions will also allow
# for the specification of additional resource locations.
#

#
# addUnitTest
# Creates a unit test from a given file with a given name.
# - TEST_NAME the name of the test.
# - FILE_NAME the name of the unit test file.
# All arguments after the file name are considered to be linker arguments.
# By default all tests are linked against the srcml_static library.
#
#
macro(addUnitTest TEST_NAME FILE_NAME)
    add_executable(${TEST_NAME} ${FILE_NAME})
    target_link_libraries(${TEST_NAME} srcml_static ${ARGN})
    add_test(${TEST_NAME} ${TEST_NAME})
endmacro()


# 
# copyDependentFile 
# This copies a file given as TARGET_NAME to the current bunary directory
# of the build.
# 
macro(copyDependentFile TARGET_NAME )
    file(COPY ${TARGET_NAME} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endmacro()

