##
# @file CMakeLists.txt
#
# @copyright Copyright (C) 2022 srcML, LLC. (www.srcML.org)
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
# CMake files for libsrcml examples

# Any more recent cmake can be specified
cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD          11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

project(srcML-Examples)

# Produces the target srcML::LibsrcML
# Can also specify components. The default component is shared
#   find_package(srcml COMPONENT shared REQUIRED)
#   find_package(srcml COMPONENT static REQUIRED)
find_package(srcML REQUIRED)

# Add each executable as a separate program
# For each target, add the srcML::LibsrcML library

add_executable(srcml_xslt srcml_xslt.cpp)
target_link_libraries(srcml_xslt PRIVATE srcML::LibsrcML)

add_executable(srcml_xpath srcml_xpath.cpp)
target_link_libraries(srcml_xpath PRIVATE srcML::LibsrcML)

add_executable(srcml_transform srcml_transform.cpp)
target_link_libraries(srcml_transform PRIVATE srcML::LibsrcML)

add_executable(srcml_split_archive srcml_split_archive.cpp)
target_link_libraries(srcml_split_archive PRIVATE srcML::LibsrcML)

add_executable(srcml_sort_archive srcml_sort_archive.cpp)
target_link_libraries(srcml_sort_archive PRIVATE srcML::LibsrcML)

add_executable(srcml_relaxng srcml_relaxng.cpp)
target_link_libraries(srcml_relaxng PRIVATE srcML::LibsrcML)

add_executable(srcml_read_archive_memory srcml_read_archive_memory.cpp)
target_link_libraries(srcml_read_archive_memory PRIVATE srcML::LibsrcML)

add_executable(srcml_read_archive_full srcml_read_archive_full.cpp)
target_link_libraries(srcml_read_archive_full PRIVATE srcML::LibsrcML)

add_executable(srcml_read_archive_filename srcml_read_archive_filename.cpp)
target_link_libraries(srcml_read_archive_filename PRIVATE srcML::LibsrcML)

add_executable(srcml_read_archive_file srcml_read_archive_file.cpp)
target_link_libraries(srcml_read_archive_file PRIVATE srcML::LibsrcML)

add_executable(srcml_read_archive_fd srcml_read_archive_fd.cpp)
target_link_libraries(srcml_read_archive_fd PRIVATE srcML::LibsrcML)

add_executable(srcml_list srcml_list.cpp)
target_link_libraries(srcml_list PRIVATE srcML::LibsrcML)

add_executable(srcml_direct_srcml2src srcml_direct_srcml2src.cpp)
target_link_libraries(srcml_direct_srcml2src PRIVATE srcML::LibsrcML)

add_executable(srcml_direct_src2srcml srcml_direct_src2srcml.cpp)
target_link_libraries(srcml_direct_src2srcml PRIVATE srcML::LibsrcML)

add_executable(srcml_direct_language_xml srcml_direct_language_xml.cpp)
target_link_libraries(srcml_direct_language_xml PRIVATE srcML::LibsrcML)

add_executable(srcml_direct_language_list srcml_direct_language_list.cpp)
target_link_libraries(srcml_direct_language_list PRIVATE srcML::LibsrcML)

add_executable(srcml_direct_language srcml_direct_language.cpp)
target_link_libraries(srcml_direct_language PRIVATE srcML::LibsrcML)

add_executable(srcml_create_archive_memory srcml_create_archive_memory.cpp)
target_link_libraries(srcml_create_archive_memory PRIVATE srcML::LibsrcML)

add_executable(srcml_create_archive_full srcml_create_archive_full.cpp)
target_link_libraries(srcml_create_archive_full PRIVATE srcML::LibsrcML)

add_executable(srcml_create_archive_filename srcml_create_archive_filename.cpp)
target_link_libraries(srcml_create_archive_filename PRIVATE srcML::LibsrcML)

add_executable(srcml_create_archive_file srcml_create_archive_file.cpp)
target_link_libraries(srcml_create_archive_file PRIVATE srcML::LibsrcML)

add_executable(srcml_create_archive_fd srcml_create_archive_fd.cpp)
target_link_libraries(srcml_create_archive_fd PRIVATE srcML::LibsrcML)

add_executable(srcml_copy_archive srcml_copy_archive.cpp)
target_link_libraries(srcml_copy_archive PRIVATE srcML::LibsrcML)

add_executable(srcml_convenient srcml_convenient.cpp)
target_link_libraries(srcml_convenient PRIVATE srcML::LibsrcML)

# Copy data files for examples
file(COPY decl_stmt.cpp expr.cpp define.cpp project.xml schema.rng copy.xsl DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
