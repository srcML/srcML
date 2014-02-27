##
# @file CMakeLists.txt
#
# @copyright Copyright (C) 2013-2014  SDML (www.srcML.org)
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
addUnitTest(test_UTF8OutputSource test_UTF8OutputSource.cpp)
addUnitTest(test_srcMLSAX2Reader test_srcMLSAX2Reader.cpp ${LIBXML2_LIBRARIES})
addUnitTest(test_srcml_append_transform test_srcml_append_transform.cpp)
addUnitTest(test_srcml_apply_transforms test_srcml_apply_transforms.cpp)
addUnitTest(test_srcml_archive_check_extension test_srcml_archive_check_extension.cpp)
addUnitTest(test_srcml_archive_get test_srcml_archive_get.cpp)
addUnitTest(test_srcml_archive_set test_srcml_archive_set.cpp)
addUnitTest(test_srcml_clear_transforms test_srcml_clear_transforms.cpp)
addUnitTest(test_srcml_clone_archive test_srcml_clone_archive.cpp)
addUnitTest(test_srcml_create_archive test_srcml_create_archive.cpp)
addUnitTest(test_srcml_create_unit test_srcml_create_unit.cpp)
addUnitTest(test_srcml_extract_text test_srcml_extract_text.cpp ${LIBXML2_LIBRARIES})
addUnitTest(test_srcml_global test_srcml_global.cpp)
addUnitTest(test_srcml_global_access test_srcml_global_access.cpp)
addUnitTest(test_srcml_parse_unit test_srcml_parse_unit.cpp)
addUnitTest(test_srcml_read_open test_srcml_read_open.cpp)
addUnitTest(test_srcml_read_unit test_srcml_read_unit.cpp)
addUnitTest(test_srcml_relaxng test_srcml_relaxng.cpp ${LIBXML2_LIBRARIES})
addUnitTest(test_srcml_sax2_utilities test_srcml_sax2_utilities.cpp ${LIBXML2_LIBRARIES})
addUnitTest(test_srcml_unit_get test_srcml_unit_get.cpp)
addUnitTest(test_srcml_unit_set test_srcml_unit_set.cpp)
addUnitTest(test_srcml_unparse_unit test_srcml_unparse_unit.cpp)
addUnitTest(test_srcml_write_open test_srcml_write_open.cpp)
addUnitTest(test_srcml_write_unit test_srcml_write_unit.cpp)
addUnitTest(test_srcml_xpath test_srcml_xpath.cpp ${LIBXML2_LIBRARIES})
addUnitTest(test_srcml_xslt test_srcml_xslt.cpp ${LIBXML2_LIBRARIES})

if(NOT ${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})
    copyDependentFile(copy.xsl)
    copyDependentFile(schema.rng)
    
    add_dependencies(test_srcml_apply_transforms copy.xsl schema.rng)
    add_dependencies(test_srcml_append_transform copy.xsl schema.rng)
    add_dependencies(test_srcml_clear_transforms copy.xsl schema.rng)
    add_dependencies(test_srcml_relaxng schema.rng)
    add_dependencies(test_srcml_xslt copy.xsl)
    
    copyDependentFile(test_srcml_xpath.cpp)
    add_dependencies(test_srcml_sax2_utilities test_srcml_xpath.cpp)
    
endif()
