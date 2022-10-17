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

set(VCPKG_BUILD_TYPE "release")

cmake_minimum_required(VERSION 3.24)

# Handles problem of commandline tools interfering with Xcode includes
if(APPLE)
    set(PKG_CONFIG_EXECUTABLE "/usr/bin/true")
endif()

# If performed here, before project(), then this prefix becomes part of the CMAKE_SYSTEM_PREFIX_PATH
# and then the installed version of srcML can be found by the tests without building srcml.
# Note that the default for the install prefix is "C:/Program Files (x86)/srcML", which contradicts
# the CMake documentation (https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX.html)
# Also note that without this, the install target places the files in "C:/Program Files (x86)/srcML",
# while the package installer places it in "C:/Program Files/srcML"
# Note that this overwrites any changes to the CMAKE_INSTALL_PREFIX from the command line cmake options.
if(WIN32)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/srcML" CACHE PATH "Set Windows install dir" FORCE)
endif()

# Set libsrcml version from src/libsrcml/srcml.h
# Extract this first because manually setting major, minor, and patch leads to bugs
file(STRINGS "${CMAKE_SOURCE_DIR}/src/libsrcml/srcml.h" SRCML_HEADER REGEX "SRCML_VERSION_STRING")
string(REGEX MATCH "\"([0-9.]+)\"" _ "${SRCML_HEADER}")

project(srcML VERSION "${CMAKE_MATCH_1}" HOMEPAGE_URL "https://www.srcML.org")

message(STATUS "Project version: ${PROJECT_VERSION}")

# Turn ON everything
option(BUILD_ALL "Build all test, documentation, and examples" OFF)

# The default configuration is to compile in Release mode
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

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
