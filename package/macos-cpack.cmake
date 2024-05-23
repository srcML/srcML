# SPDX-License-Identifier: GPL-3.0-only
#
# @file macos.cmake
#
# @copyright Copyright (C) 2019-2023 srcML, LLC. (www.srcML.org)
#
# CPack configuration for macOS installers

# Exclude other platforms
if(NOT APPLE)
    return()
endif()

# Append to the generator list
list(APPEND CPACK_GENERATOR "productbuild;TGZ;TBZ2")
list(REMOVE_DUPLICATES CPACK_GENERATOR)

# System name based on macOS and version, then used in package name
execute_process(COMMAND /usr/bin/sw_vers -productVersion OUTPUT_VARIABLE OS_NUMERIC OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CPACK_SYSTEM_NAME "macOS-${OS_NUMERIC}")

# Archive package filenames
set(BASE_SRCML_FILE_NAME "${CPACK_COMPONENT_SRCML_DISPLAY_NAME}-${PROJECT_VERSION}-${CPACK_SYSTEM_NAME}")
set(CPACK_ARCHIVE_SRCML_FILE_NAME "${BASE_SRCML_FILE_NAME}")

# Targets for installing generated packages
add_custom_target(install_package
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/dist
    COMMAND installer -pkg ${BASE_SRCML_FILE_NAME}.pkg -target CurrentUserHomeDirectory
)
add_custom_target(install_archive
    WORKING_DIRECTORY ${CPACK_OUTPUT_FILE_PREFIX}
    COMMAND apt-get install -y ./${CPACK_ARCHIVE_SRCML_FILE_NAME}.tgz
)

# Set for discovery of custom template CPack.distribution.dist.in
# * Removes readme
# * Adds conclusion
# * Sets background image
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/package")

# For archives, all components are in one file
set(CPACK_ARCHIVE_COMPONENT_INSTALL OFF)

# The srcml component is required
set(CPACK_COMPONENT_SRCML_REQUIRED ON)

# One package
set(CPACK_COMPONENTS_GROUPING "ONE_PER_GROUP")

# Install in /usr/local
set(CPACK_PACKAGING_INSTALL_PREFIX /usr/local)

# Welcome content
set(CPACK_RESOURCE_FILE_WELCOME ${CMAKE_SOURCE_DIR}/package/welcome.html)

# License
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/COPYING.txt)

# Where to find additional files
set(CPACK_PRODUCTBUILD_RESOURCES_DIR ${CMAKE_BINARY_DIR}/pkg_resources)

# Background image
configure_file(${CMAKE_SOURCE_DIR}/package/background.png ${CPACK_PRODUCTBUILD_RESOURCES_DIR}/background.png COPYONLY)

# Conclusion
configure_file(${CMAKE_SOURCE_DIR}/package/installed.html ${CPACK_PRODUCTBUILD_RESOURCES_DIR}/installed.html COPYONLY)

# Targets for workflow testing
add_workflow_test_targets(${CMAKE_BINARY_DIR} ${CPACK_OUTPUT_FILE_PREFIX} ${BASE_SRCML_FILE_NAME})
