# SPDX-License-Identifier: GPL-3.0-only
#
# @file debian.cmake
#
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
#
# CPack configuration for Debian installers

# Exclude other platforms
if(NOT UNIX OR NOT DISTRO OR NOT DISTRO MATCHES "Ubuntu")
    return()
endif()

# Update the generator list
list(APPEND CPACK_GENERATOR "DEB;TGZ;TBZ2")
list(REMOVE_DUPLICATES CPACK_GENERATOR)

# Turn ON/OFF internal CPackDeb debugging
set(CPACK_DEBIAN_PACKAGE_DEBUG OFF)

# Generate user and DEVLIBS rpms
# Note: Yes, this is CPACK_DEB_, not CPACK_DEBIAN
set(CPACK_DEB_COMPONENT_INSTALL ON)

# Debian package names
# Package names should be lowercase
set(CPACK_DEBIAN_SRCML_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
set(CPACK_DEBIAN_DEVLIBS_PACKAGE_NAME "${CPACK_PACKAGE_NAME}-dev")

# Package release number (NOT srcml or libsrcml release)
# Note: Update when package is updated, but not contents
set(CPACK_DEBIAN_PACKAGE_RELEASE "${SRCML_PACKAGE_RELEASE}")

# Distribution version, e.g., 18.04
file(STRINGS "/etc/os-release" OS_RELEASE)
string(REGEX MATCH "VERSION_ID=\"\([0-9.]+\)\"" _ "${OS_RELEASE}")
set(OS_RELEASE_VERSION_ID "${CMAKE_MATCH_1}")
string(REGEX MATCH "ID=\([a-z]+\)" _ "${OS_RELEASE}")
set(OS_RELEASE_ID "${CMAKE_MATCH_1}")
set(DEBIAN_VERSION "${OS_RELEASE_ID}${OS_RELEASE_VERSION_ID}")

# Architecture
if(CMAKE_SYSTEM_PROCESSOR EQUAL "aarch64")
    set(SRCML_SYSTEM_ARCHITECTURE "arm64")
else()
    set(SRCML_SYSTEM_ARCHITECTURE "amd64")
endif()

# Package filenames
set(BASE_SRCML_FILE_NAME "${CPACK_DEBIAN_SRCML_PACKAGE_NAME}_${PROJECT_VERSION}-${CPACK_DEBIAN_PACKAGE_RELEASE}_${DEBIAN_VERSION}_${CMAKE_SYSTEM_PROCESSOR}")
set(BASE_DEVLIBS_FILE_NAME "${CPACK_DEBIAN_DEVLIBS_PACKAGE_NAME}_${PROJECT_VERSION}-${CPACK_DEBIAN_PACKAGE_RELEASE}_${DEBIAN_VERSION}_${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_DEBIAN_SRCML_FILE_NAME "${BASE_SRCML_FILE_NAME}.deb")
set(CPACK_DEBIAN_DEVLIBS_FILE_NAME "${BASE_DEVLIBS_FILE_NAME}.deb")
set(CPACK_ARCHIVE_SRCML_FILE_NAME "${BASE_SRCML_FILE_NAME}")
set(CPACK_ARCHIVE_DEVLIBS_FILE_NAME "${BASE_DEVLIBS_FILE_NAME}")

# Package types
# Current CPack defaults
# Note: unclear whether CPACK_DEBIAN_PACKAGE_SECTION is default for components
set(CPACK_DEBIAN_SRCML_PACKAGE_SECTION "devel")
set(CPACK_DEBIAN_DEVLIBS_PACKAGE_SECTION "devel")

# Package priorities
# Current CPack defaults
# Note: unclear whether CPACK_DEBIAN_PACKAGE_PRIORITY is default for components
set(CPACK_DEBIAN_SRCML_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_DEVLIBS_PACKAGE_PRIORITY "optional")

# Copyright file - required by lintian
# install(FILES ${CMAKE_SOURCE_DIR}/COPYING.txt DESTINATION share/doc/srcml RENAME copyright COMPONENT SRCML)
# install(FILES ${CMAKE_SOURCE_DIR}/COPYING.txt DESTINATION share/doc/srcml RENAME copyright COMPONENT DEVLIBS)

# Changelog file - required by lintian
# file(WRITE ${CMAKE_BINARY_DIR}/changelog.Debian
# "srcml (1.0.0-1) UNRELEASED; urgency=medium

#  * Initial release

# -- Michael L. Collard <collard@uakron.edu>  Mon, 2 Dec 2019 17:00:00 +0400
# ")
# execute_process(COMMAND gzip -n -9 -f ${CMAKE_BINARY_DIR}/changelog.Debian)
# install(FILES ${CMAKE_BINARY_DIR}/changelog.Debian.gz DESTINATION share/doc/srcml COMPONENT SRCML)
# install(FILES ${CMAKE_BINARY_DIR}/changelog.Debian.gz DESTINATION share/doc/srcml COMPONENT DEVLIBS)

# Autogenerate dependency information
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS ON)
set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS_POLICY ">=")

# Dependency for -dev package on client package
set(CPACK_DEBIAN_DEVLIBS_PACKAGE_DEPENDS "${CPACK_PACKAGE_NAME} (>= ${PROJECT_VERSION})")

# Recommended packages
# Shared between client and dev packages
set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "libxslt, zip, unzip, cpio, tar, man")

# Trigger required for library installed in client to initiate ldconfig
set(TRIGGERS_FILE "${CMAKE_CURRENT_BINARY_DIR}/triggers")
file(WRITE "${TRIGGERS_FILE}" "activate-noawait ldconfig\n")
set(CPACK_DEBIAN_SRCML_PACKAGE_CONTROL_EXTRA "${TRIGGERS_FILE}")
