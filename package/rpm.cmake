##
# @file rpm.cmake
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

# Turn ON/OFF internal CPackRPM debugging
set(CPACK_RPM_PACKAGE_DEBUG ON)

# Generate user and development rpms
set(CPACK_RPM_COMPONENT_INSTALL ON)

# Package release number (NOT srcml or libsrcml release)
# Note: Update when package is updated, but not contents
set(CPACK_RPM_PACKAGE_RELEASE "${SRCML_PACKAGE_RELEASE}")

# Package names
set(CPACK_RPM_CLIENT_PACKAGE_NAME "srcml")
set(CPACK_RPM_DEVELOPMENT_PACKAGE_NAME "srcml-devel")
set(CPACK_ARCHIVE_CLIENT_FILE_NAME "${CPACK_RPM_CLIENT_PACKAGE_NAME}_${PROJECT_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}")
set(CPACK_ARCHIVE_DEVELOPMENT_FILE_NAME "${CPACK_RPM_DEVELOPMENT_PACKAGE_NAME}_${PROJECT_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}")

# CLIENT package is main, so no extension is added
set(CPACK_RPM_MAIN_COMPONENT CLIENT)

# srcml-devel package requires srcml package
set(CPACK_RPM_DEVELOPMENT_PACKAGE_REQUIRES "srcml >= ${PROJECT_VERSION}")

# Homepage URLs
# Note: Default should be CMAKE_PROJECT_HOMEPAGE_URL, but isn't due to components
set(CPACK_RPM_CLIENT_PACKAGE_URL ${CMAKE_PROJECT_HOMEPAGE_URL})
set(CPACK_RPM_DEVELOPMENT_PACKAGE_URL ${CMAKE_PROJECT_HOMEPAGE_URL})

# Package groups
# @todo Perhaps both packages should be in "Development/Tools"
set(CPACK_RPM_CLIENT_PACKAGE_GROUP "Development/Tools")
set(CPACK_RPM_DEVELOPMENT_PACKAGE_GROUP "Development/Libraries")

# Package filenames
set(CPACK_RPM_CLIENT_FILE_NAME RPM-DEFAULT)
set(CPACK_RPM_DEVELOPMENT_FILE_NAME RPM-DEFAULT)

# Adds to the release the distribution
set(CPACK_RPM_PACKAGE_RELEASE_DIST ON)

# Required changelog
set(CPACK_RPM_CHANGELOG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ChangeLog.txt")

# License
set(CPACK_RPM_PACKAGE_LICENSE "GPLv2+")

# post install script for ldconfig
# Note: Believe that newline is required
file(WRITE ${CPACK_PACKAGE_DIRECTORY}/post.sh "/sbin/ldconfig\n")
set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE   ${CPACK_PACKAGE_DIRECTORY}/post.sh)
set(CPACK_RPM_POST_UNINSTALL_SCRIPT_FILE ${CPACK_PACKAGE_DIRECTORY}/post.sh)

# CPack puts directories of installed files into the RPM
# Since they already exist, this is a conflict with other packages
# This excludes the following, mostly for documentation
# Note: There is no issue with this list including directories that are not needed
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION ${CMAKE_INSTALL_MANDIR}/man1 ${CMAKE_INSTALL_MANDIR}
		/usr/local /usr/local/include /usr/local/lib64 /usr/local/bin /usr/local/man /usr/local/man/man1 
		/usr/local/share /usr/local/share/man /usr/local/share/man/man1 /usr/share/man /usr/share/man/man1
		/usr/share/man1)
