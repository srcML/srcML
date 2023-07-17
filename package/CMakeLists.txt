# SPDX-License-Identifier: GPL-3.0-only
#
# @file package.cmake
#
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
#
# CPack configuration for all installers

# Copy preset file to build directory
configure_file(${CMAKE_SOURCE_DIR}/CMakePresets.json ${CMAKE_BINARY_DIR} COPYONLY)

# Copy user preset file to the build directory if it exists
if(EXISTS ${CMAKE_SOURCE_DIR}/CMakeUserPresets.json)
    configure_file(${CMAKE_SOURCE_DIR}/CMakeUserPresets.json ${CMAKE_BINARY_DIR} COPYONLY)
endif()

# especially for archives
set(CPACK_COMPONENTS_GROUPING "ONE_PER_GROUP")

# Package name
set(CPACK_PACKAGE_NAME "srcml")

# Package release number (NOT srcml or libsrcml release)
# Note: Update when package is updated, but not contents
set(SRCML_PACKAGE_RELEASE 1)

# summary
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "srcML Toolkit")

# description
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_SOURCE_DIR}/package/welcome.html)

# vendor
set(CPACK_PACKAGE_VENDOR "srcML, LLC.")

# contact
set(CPACK_PACKAGE_CONTACT "Software Development Laboratories <srcML.org>")

# package version
# set as part of project()

# license
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/COPYING.txt)

# README
set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)

# welcome message
set(CPACK_RESOURCE_FILE_WELCOME ${CMAKE_SOURCE_DIR}/package/welcome.html)

# strip executables
set(CPACK_STRIP_FILES ON)

# output directory
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR}/dist)

# Generate user and development tar.gz's
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)

# Client component, SRCML, is just one component so it appears as on item
# for installers that let you pick what to install
# Development group, SRCMLDEV, is composed of multiple components
include(CPackComponent)
cpack_add_install_type(CLIENT DISPLAY_NAME "srcml cli")
cpack_add_install_type(DEVELOPER DISPLAY_NAME "development")

cpack_add_component(SRCML
                    DISPLAY_NAME "srcml client"
                    DESCRIPTION "srcml cli program with required libraries"
                    REQUIRED
                    INSTALL_TYPES CLIENT DEVELOPER)

cpack_add_component(DEVLIBS
                    DISPLAY_NAME "libsrcml development"
                    DESCRIPTION "Include file, development libraries, i.e.., static library, and CMake configuration"
                    INSTALL_TYPES DEVELOPER)

# Copy and rename the README file for the proper extension
configure_file(${CMAKE_SOURCE_DIR}/README.md ${CMAKE_BINARY_DIR}/README.txt COPYONLY)
set(CPACK_RESOURCE_FILE_README ${CMAKE_BINARY_DIR}/README.txt)

# Generators are defined in the *.cpack files
set(CPACK_GENERATOR)
file(GLOB INSTALLERS "*.cpack")
foreach(INSTALLER IN ITEMS ${INSTALLERS})
    include(${INSTALLER})
endforeach()

message(STATUS "CPack generators: ${CPACK_GENERATOR}")

# Hide the LOCAL components
get_cmake_property(CPACK_COMPONENTS_ALL COMPONENTS)
list(REMOVE_ITEM CPACK_COMPONENTS_ALL "LOCAL")

# needs to be last so not overwritten
include(CPack)
