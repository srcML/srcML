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

# Generate user and development rpms
# Note: Yes, this is CPACK_DEB_, not CPACK_DEBIAN
set(CPACK_DEB_COMPONENT_INSTALL ON)

# Debian package names
# Package names should be lowercase
set(CPACK_DEBIAN_CLIENT_PACKAGE_NAME "srcml")
set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_NAME "srcml-dev")

# Package release number (NOT srcml or libsrcml release)
# Note: Update when package is updated, but not contents
set(CPACK_DEBIAN_PACKAGE_RELEASE "${SRCML_PACKAGE_RELEASE}")

# Package filenames
set(CPACK_DEBIAN_CLIENT_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_DEVELOPMENT_FILE_NAME DEB-DEFAULT)
set(CPACK_ARCHIVE_CLIENT_FILE_NAME "${CPACK_DEBIAN_CLIENT_PACKAGE_NAME}_${PROJECT_VERSION}-${CPACK_DEBIAN_PACKAGE_RELEASE}")
set(CPACK_ARCHIVE_DEVELOPMENT_FILE_NAME "${CPACK_DEBIAN_DEVELOPMENT_PACKAGE_NAME}_${PROJECT_VERSION}-${CPACK_DEBIAN_PACKAGE_RELEASE}")

# Package types
# Current CPack defaults
# Note: unclear whether CPACK_DEBIAN_PACKAGE_SECTION is default for components
set(CPACK_DEBIAN_CLIENT_PACKAGE_SECTION "devel")
set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_SECTION "devel")

# Package priorities
# Current CPack defaults
# Note: unclear whether CPACK_DEBIAN_PACKAGE_PRIORITY is default for components
set(CPACK_DEBIAN_CLIENT_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_PRIORITY "optional")

# Copyright file - required by lintian
# @todo create real copyright file
# install(FILES ${CMAKE_SOURCE_DIR}/COPYING.txt DESTINATION share/doc/srcml RENAME copyright COMPONENT CLIENT)
# install(FILES ${CMAKE_SOURCE_DIR}/COPYING.txt DESTINATION share/doc/srcml RENAME copyright COMPONENT DEVELOPMENT)

# Changelog file - required by lintian
# file(WRITE ${CMAKE_BINARY_DIR}/changelog.Debian
# "srcml (1.0.0-1) UNRELEASED; urgency=medium

#  * Initial release

# -- Michael L. Collard <collard@uakron.edu>  Mon, 10 Jun 2109 15:48:00 +0400
# ")
# execute_process(COMMAND gzip -n -9 -f ${CMAKE_BINARY_DIR}/changelog.Debian)
# install(FILES ${CMAKE_BINARY_DIR}/changelog.Debian.gz DESTINATION share/doc/srcml COMPONENT CLIENT)
# install(FILES ${CMAKE_BINARY_DIR}/changelog.Debian.gz DESTINATION share/doc/srcml COMPONENT DEVELOPMENT)

# Autogenerate dependency information
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS ON)
set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS_POLICY ">=")

# Dependency for srcml-dev package on srcml package
set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_DEPENDS "srcml (>= ${PROJECT_VERSION})")

# Recommended packages
# Shared between packages srcml and srcml-dev
set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "libxslt, zip, unzip, cpio, tar, man")

# Trigger required for library installed in client to initiate ldconfig
set(TRIGGERS_FILE "${CMAKE_CURRENT_BINARY_DIR}/triggers")
file(WRITE "${TRIGGERS_FILE}" "activate-noawait ldconfig\n")
set(CPACK_DEBIAN_CLIENT_PACKAGE_CONTROL_EXTRA "${TRIGGERS_FILE}")
