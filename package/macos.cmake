# SPDX-License-Identifier: GPL-3.0-only
#
# @file macos.cmake
#
# @copyright Copyright (C) 2019 srcML, LLC. (www.srcML.org)
#
# CPack configuration for macOS installers

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
