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

# Build and add tests
file(GLOB LIB_TESTS test_*.cpp)
foreach(LIB_TEST ${LIB_TESTS})
	get_filename_component(TEST_NAME ${LIB_TEST} NAME_WE)
    add_executable(${TEST_NAME} ${LIB_TEST})
    target_link_libraries(${TEST_NAME} srcml_shared)
    add_test(NAME ${TEST_NAME} COMMAND $<TARGET_FILE:${TEST_NAME}>)
    set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT 15)
endforeach()

# Copying xpath test data
configure_file(copy.xsl ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(setlanguage.xsl ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(schema.rng ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
