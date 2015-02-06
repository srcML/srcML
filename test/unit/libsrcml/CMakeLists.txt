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
addUnitTest(test_srcml_sax2_reader test_srcml_sax2_reader.cpp ${LIBXML2_LIBRARIES})
addUnitTest(test_srcml_append_transform test_srcml_append_transform.cpp)
addUnitTest(test_srcml_apply_transforms test_srcml_apply_transforms.cpp)
addUnitTest(test_srcml_archive_check_extension test_srcml_archive_check_extension.cpp)
addUnitTest(test_srcml_archive_get test_srcml_archive_get.cpp)
addUnitTest(test_srcml_archive_set test_srcml_archive_set.cpp)
addUnitTest(test_srcml_clear_transforms test_srcml_clear_transforms.cpp)
addUnitTest(test_srcml_archive_clone test_srcml_archive_clone.cpp)
addUnitTest(test_srcml_archive_create test_srcml_archive_create.cpp)
addUnitTest(test_srcml_unit_create test_srcml_unit_create.cpp)
addUnitTest(test_srcml_convenience test_srcml_convenience.cpp)
addUnitTest(test_srcml_extract_text test_srcml_extract_text.cpp ${LIBXML2_LIBRARIES})
addUnitTest(test_srcml_global test_srcml_global.cpp)
addUnitTest(test_srcml_global_access test_srcml_global_access.cpp)
addUnitTest(test_srcml_unit_parse test_srcml_unit_parse.cpp)
addUnitTest(test_srcml_archive_read_open test_srcml_archive_read_open.cpp)
addUnitTest(test_srcml_read_unit test_srcml_read_unit.cpp)
addUnitTest(test_srcml_relaxng test_srcml_relaxng.cpp ${LIBXML2_LIBRARIES})
addUnitTest(test_srcml_unit_get test_srcml_unit_get.cpp)
addUnitTest(test_srcml_unit_set test_srcml_unit_set.cpp)
addUnitTest(test_srcml_unit_unparse test_srcml_unit_unparse.cpp)
addUnitTest(test_srcml_archive_write_open test_srcml_archive_write_open.cpp)
addUnitTest(test_srcml_write_unit test_srcml_write_unit.cpp)
addUnitTest(test_srcml_write_by_element test_srcml_write_by_element.cpp)
addUnitTest(test_srcml_xpath test_srcml_xpath.cpp ${LIBXML2_LIBRARIES})
addUnitTest(test_srcml_xslt test_srcml_xslt.cpp ${LIBXML2_LIBRARIES})
addUnitTest(test_language_extension_registry test_language_extension_registry.cpp)
addUnitTest(test_srcml_translator test_srcml_translator.cpp ${LIBXML2_LIBRARIES})

add_definitions(-DSRCML_XPATH_EXFUN_TEST_DIR="${CMAKE_BINARY_DIR}/bin/xpath_test_data")
addUnitTest(test_srcexfun_impl test_srcexfun_impl.cpp)

add_custom_target(copy_test_data_target ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/xpath_test_data" "${CMAKE_BINARY_DIR}/bin/xpath_test_data")
add_dependencies(test_srcexfun_impl copy_test_data_target)

# Copying xpath test data into correct loction.
if(NOT ${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})

    copyDependentFile(copy.xsl)
    copyDependentFile(setlanguage.xsl)
    copyDependentFile(schema.rng)   
    copyDependentFile(test_srcml_xpath.cpp)

endif()

