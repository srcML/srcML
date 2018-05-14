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
  	addUnitTest(${TEST_NAME} ${TEST_NAME}.cpp)
endforeach()

add_definitions(-DSRCML_XPATH_EXFUN_TEST_DIR="${CMAKE_BINARY_DIR}/bin/xpath_test_data")
file(COPY xpath_test_data DESTINATION "${CMAKE_BINARY_DIR}/bin")

# Copying xpath test data into correct loction.
if(NOT ${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})

    copyDependentFile(copy.xsl)
    copyDependentFile(setlanguage.xsl)
    copyDependentFile(schema.rng)  

endif()
