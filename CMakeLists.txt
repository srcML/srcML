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
# The master CMake project file for srcML

cmake_minimum_required(VERSION 3.14)

# Set libsrcml version from src/libsrcml/srcml.h
# Extract this first because manually setting major, minor, and patch leads to bugs
file(STRINGS "${CMAKE_SOURCE_DIR}/src/libsrcml/srcml.h" SRCML_HEADER REGEX "SRCML_VERSION_STRING")
string(REGEX MATCH "\"([0-9.]+)\"" _ "${SRCML_HEADER}")

project(srcML VERSION "${CMAKE_MATCH_1}" HOMEPAGE_URL "https://www.srcML.org")

message(STATUS "Project version: ${PROJECT_VERSION}")

# Turn ON/OFF tests
option(BUILD_CLIENT_TESTS "Build srcml client tests" ON)
option(BUILD_LIBSRCML_TESTS "Build unit tests for libsrcml" OFF)
option(BUILD_PARSER_TESTS "Include tests for parser" OFF)

# Turn ON building all tests
option(BUILD_ALL_TESTS "Build all tests" OFF)
if(BUILD_ALL_TESTS)
    set(BUILD_CLIENT_TESTS ON)
    set(BUILD_LIBSRCML_TESTS ON)
    set(BUILD_PARSER_TESTS ON)
endif()

# Turn ON/OFF building examples
option(BUILD_EXAMPLES "Build examples usage files for libsrcml" OFF)

# Turn ON/OFF building documentation
option(BUILD_CLIENT_DOC "Build client documentation" OFF)
option(INSTALL_CLIENT_DOC "Install (but do not build) client documentation" OFF)
option(BUILD_LIBSRCML_DOC "Build libsrcml documentation" OFF)
if(NOT BUILD_CLIENT_DOC)
    set(INSTALL_CLIENT_DOC ON)
endif()

# Turn ON building all documentation
option(BUILD_ALL_DOCS "Build all documentation" OFF)
if(BUILD_ALL_DOCS)
    set(BUILD_CLIENT_DOC ON)
    set(BUILD_LIBSRCML_DOC ON)
endif()

# Turn ON everything
option(BUILD_ALL "Build all test, documentation, and examples" OFF)
if(BUILD_ALL)
    set(BUILD_ALL_TESTS ON)
    set(BUILD_EXAMPLES ON)
    set(BUILD_ALL_DOCS ON)
endif()

# Build options
option(BUILD_LIBSRCML_STATIC "Build a static version of libsrcml" ON)
option(LINK_LIBSRCML_STATIC "Link srcml client, tests, and examples with static version of libsrcml" OFF)

# The default configuration is to compile in Release mode
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

# Dynamic Load libraries (Unix only)
if(NOT WIN32)
    option(DYNAMIC_LOAD_ENABLED "Dynamically load some libraries such as libxslt and libexslt" ON)
endif()

enable_testing()
include(CTest)

# Determine distribution
# Does not work for all distributions, but does work for supported Linux distributions
if(EXISTS /etc/os-release)
    file(STRINGS /etc/os-release DISTRO REGEX "^NAME=")
endif()

# Fedora and CentOS do not put /usr/local/{bin,lib64} on the default search paths
# So change installation to /usr
if(DISTRO AND DISTRO MATCHES "CentOS|Fedora")
    set(CMAKE_INSTALL_PREFIX /usr)
endif()

# used as defaults for installation and packaging
include(GNUInstallDirs)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(doc)
add_subdirectory(examples)

# packaging
add_subdirectory(package)
