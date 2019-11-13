##
# @file macos.cmake
#
# @copyright Copyright (C) 2019 srcML, LLC. (www.srcML.org)
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

# icon
set(CPACK_PACKAGE_ICON ${CMAKE_SOURCE_DIR}/package/srcml_icon.ico)

# System name based on macOS and version
execute_process(COMMAND /usr/bin/sw_vers -productVersion OUTPUT_VARIABLE OS_NUMERIC OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CPACK_SYSTEM_NAME "macOS-${OS_NUMERIC}")

# Custom template CPack.distribution.dist.in
# Primarily for background image
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/package")

set(CPACK_COMPONENTS_GROUPING IGNORE)

set(CPACK_COMPONENT_SRCML_REQUIRED ON)

set(CPACK_COMPONENTS_GROUPING "ONE_PER_GROUP")

set(CPACK_PACKAGING_INSTALL_PREFIX /usr/local)
set(CPACK_RESOURCE_FILE_WELCOME ${CMAKE_SOURCE_DIR}/package/welcome.html)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/COPYING.txt)
configure_file(${CMAKE_SOURCE_DIR}/README.md ${CMAKE_BINARY_DIR}/README.txt COPYONLY)
set(CPACK_RESOURCE_FILE_README ${CMAKE_BINARY_DIR}/README.txt)
configure_file(${CMAKE_SOURCE_DIR}/package/background.png ${CMAKE_BINARY_DIR}/pkg_resources/background.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/package/Installed.html ${CMAKE_BINARY_DIR}/pkg_resources/installed.html COPYONLY)
set(CPACK_PRODUCTBUILD_RESOURCES_DIR ${CMAKE_BINARY_DIR}/pkg_resources)

# Package filenames
set(CPACK_ARCHIVE_SRCML_FILE_NAME "${CPACK_COMPONENT_SRCML_DISPLAY_NAME}-${PROJECT_VERSION}-${CPACK_SYSTEM_NAME}")
set(CPACK_ARCHIVE_SRCMLDEV_FILE_NAME "${CPACK_COMPONENT_SRCML_DISPLAY_NAME}-dev-${PROJECT_VERSION}-${CPACK_SYSTEM_NAME}")
