##
# @file debian.cmake
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

# Turn ON/OFF internal CPackDeb debugging
set(CPACK_DEBIAN_PACKAGE_DEBUG OFF)

# icon
set(CPACK_PACKAGE_ICON ${CMAKE_SOURCE_DIR}/package/srcml_icon.ico)

# Generate user and SRCMLDEV rpms
# Note: Yes, this is CPACK_DEB_, not CPACK_DEBIAN
set(CPACK_DEB_COMPONENT_INSTALL ON)

# Debian package names
# Package names should be lowercase
set(CPACK_DEBIAN_SRCML_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
set(CPACK_DEBIAN_SRCMLDEV_PACKAGE_NAME "${CPACK_PACKAGE_NAME}-dev")

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

# Package filenames
set(BASE_SRCML_FILE_NAME "${CPACK_DEBIAN_SRCML_PACKAGE_NAME}_${PROJECT_VERSION}-${CPACK_DEBIAN_PACKAGE_RELEASE}_${DEBIAN_VERSION}")
set(BASE_SRCMLDEV_FILE_NAME "${CPACK_DEBIAN_SRCMLDEV_PACKAGE_NAME}_${PROJECT_VERSION}-${CPACK_DEBIAN_PACKAGE_RELEASE}_${DEBIAN_VERSION}")
set(CPACK_DEBIAN_SRCML_FILE_NAME "${BASE_SRCML_FILE_NAME}.deb")
set(CPACK_DEBIAN_SRCMLDEV_FILE_NAME "${BASE_SRCMLDEV_FILE_NAME}.deb")
set(CPACK_ARCHIVE_SRCML_FILE_NAME "${BASE_SRCML_FILE_NAME}")
set(CPACK_ARCHIVE_SRCMLDEV_FILE_NAME "${BASE_SRCMLDEV_FILE_NAME}")

# Package types
# Current CPack defaults
# Note: unclear whether CPACK_DEBIAN_PACKAGE_SECTION is default for components
set(CPACK_DEBIAN_SRCML_PACKAGE_SECTION "devel")
set(CPACK_DEBIAN_SRCMLDEV_PACKAGE_SECTION "devel")

# Package priorities
# Current CPack defaults
# Note: unclear whether CPACK_DEBIAN_PACKAGE_PRIORITY is default for components
set(CPACK_DEBIAN_SRCML_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_SRCMLDEV_PACKAGE_PRIORITY "optional")

# Copyright file - required by lintian
install(FILES ${CMAKE_SOURCE_DIR}/package/copyright.srcml.Debian DESTINATION share/doc/srcml RENAME copyright COMPONENT SRCML)
install(FILES ${CMAKE_SOURCE_DIR}/package/copyright.srcmldev.Debian DESTINATION share/doc/srcml-dev RENAME copyright COMPONENT SRCMLDEV)

# Changelog file - required by lintian
file(WRITE ${CMAKE_BINARY_DIR}/changelog.srcml.Debian
"srcml (1.0.0-1) UNRELEASED; urgency=medium

  * Initial release

 -- Michael L. Collard <collard@uakron.edu>  Mon, 2 Dec 2019 17:00:00 +0400
")
execute_process(COMMAND gzip -n -9 -f ${CMAKE_BINARY_DIR}/changelog.srcml.Debian)
install(FILES ${CMAKE_BINARY_DIR}/changelog.srcml.Debian.gz DESTINATION share/doc/srcml RENAME changelog.Debian.gz COMPONENT SRCML)
file(WRITE ${CMAKE_BINARY_DIR}/changelog.srcmldev.Debian
"srcml-dev (1.0.0-1) UNRELEASED; urgency=medium

  * Initial release

 -- Michael L. Collard <collard@uakron.edu>  Mon, 2 Dec 2019 17:00:00 +0400
")
execute_process(COMMAND gzip -n -9 -f ${CMAKE_BINARY_DIR}/changelog.srcmldev.Debian)
install(FILES ${CMAKE_BINARY_DIR}/changelog.srcmldev.Debian.gz DESTINATION share/doc/srcml-dev RENAME changelog.Debian.gz COMPONENT SRCMLDEV)

# Autogenerate dependency information
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS ON)
set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS_POLICY ">=")

# Dependency for -dev package on client package
set(CPACK_DEBIAN_SRCMLDEV_PACKAGE_DEPENDS "${CPACK_PACKAGE_NAME} (>= ${PROJECT_VERSION})")

# Recommended packages client
set(CPACK_DEBIAN_SRCML_PACKAGE_RECOMMENDS "libxslt, man")

# Suggested packages client
set(CPACK_DEBIAN_SRCML_PACKAGE_SUGGESTS "zip, unzip, cpio")

# Recommended packages srcml-dev
set(CPACK_DEBIAN_SRCMLDEV_PACKAGE_RECOMMENDS "g++ | clang, libxslt")

# Suggested packages srcml-dev
set(CPACK_DEBIAN_SRCMLDEV_PACKAGE_SUGGESTS "libxml2-dev")

# Remove default postinst and postrm since there doesn't seem to be a way to turn
# off the default ldconfig call in CMake
set(POSTBUILD_SCRIPT "${CMAKE_SOURCE_DIR}/package/debian.post.cmake")
set(CPACK_POST_BUILD_SCRIPTS "${POSTBUILD_SCRIPT}")

# Trigger required for library installed in client to initiate ldconfig
set(TRIGGERS_FILE "${CMAKE_CURRENT_BINARY_DIR}/triggers")
file(WRITE "${TRIGGERS_FILE}" "activate-noawait ldconfig\n")
set(CPACK_DEBIAN_SRCML_PACKAGE_CONTROL_EXTRA "${TRIGGERS_FILE}")
