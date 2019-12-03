##
# @file package.cmake
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
# CMake files for packages

# Client component, SRCML, is just one component so it appears as on item
# for installers that let you pick what to install
# Development group, SRCMLDEV, is composed of multiple components
include(CPackComponent)
cpack_add_component(SRCML
                    DISPLAY_NAME "srcml"
                    DESCRIPTION "srcml cli program with required library"
                    REQUIRED)

cpack_add_component_group(SRCMLDEV
                    DISPLAY_NAME "Development"
                    DESCRIPTION "Include file, static library, and examples for developing C++ programs with libsrcml")

cpack_add_component(INCLUDE
                    DISPLAY_NAME "srcml.h"
                    DESCRIPTION "Include file, srcml.h"
                    DISABLED
                    GROUP SRCMLDEV)

cpack_add_component(DEVLIBS
                    DISPLAY_NAME "libsrcml.a"
                    DESCRIPTION "Development libraries, i.e.., static library"
                    DISABLED
                    GROUP SRCMLDEV)

cpack_add_component(EXAMPLES
                    DISPLAY_NAME "libsrcml examples"
                    DESCRIPTION "Include file, srcml.h"
                    DISABLED
                    GROUP SRCMLDEV)

# especially for archives
set(CPACK_COMPONENTS_GROUPING "ONE_PER_GROUP")

# Package name
set(CPACK_PACKAGE_NAME "srcml") 

# Package release number (NOT srcml or libsrcml release)
# Note: Update when package is updated, but not contents
set(SRCML_PACKAGE_RELEASE 1)

# executables and text label
set(CPACK_PACKAGE_EXECUTABLES srcml;srcML)

# desktop links
set(CPACK_CREATE_DESKTOP_LINKS "${CPACK_PACKAGE_NAME}")

# Generator settings
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CPACK_PACKAGE_FILE_NAME "srcML-Win")
elseif(APPLE)
    set(CPACK_GENERATOR "productbuild;TGZ;TBZ2")
else()
    if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
        set(CPACK_GENERATOR "TGZ;TBZ2")
    elseif(DISTRO MATCHES "Ubuntu")
        set(CPACK_GENERATOR "DEB;TGZ;TBZ2")
    else()
        set(CPACK_GENERATOR "RPM;TGZ;TBZ2")
    endif()
endif()
message(STATUS "CPack generators: ${CPACK_GENERATOR}")

# summary
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "srcML Toolkit")

# description
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_SOURCE_DIR}/package/welcome.html)

# vendor
set(CPACK_PACKAGE_VENDOR "srcML, LLC.")

# contact
set(CPACK_PACKAGE_CONTACT "Software Development Laboratories srcML.org>")
 
# package version
# set as part of project()

# license
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/COPYING.txt)

# README
set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)

# welcome message
set(CPACK_RESOURCE_FILE_WELCOME ${CMAKE_SOURCE_DIR}/package/welcome.html)

# strip executables
#set(CPACK_STRIP_FILES ON)
 
# output directory
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR}/dist)

# Generate user and development tar.gz's
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)

# specific configurations
if (CPACK_GENERATOR MATCHES "RPM")
    include(rpm.cmake)
elseif (CPACK_GENERATOR MATCHES "DEB")
    include(debian.cmake)
elseif (WIN32)
    include(nsis.cmake)
elseif (APPLE)
    include(macos.cmake)
endif()

# Perform any platform-specific fixes or updating of the installer
# THis noop version basically just runs the normal packaging, e.g., cpack
if(NOT TARGET gen_package)
    add_custom_target(gen_package
       COMMAND ${CMAKE_COMMAND} --build . --target package
       WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
       VERBATIM)
endif()

# needs to be last so not overwritten
include(CPack)
