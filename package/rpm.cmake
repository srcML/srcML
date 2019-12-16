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
set(CPACK_RPM_PACKAGE_DEBUG OFF)

# icon
set(CPACK_PACKAGE_ICON ${CMAKE_SOURCE_DIR}/package/srcml_icon.ico)

# Generate user and SRCMLDEV rpms
set(CPACK_RPM_COMPONENT_INSTALL ON)

# Package release number (NOT srcml or libsrcml release)
# Note: Update when package is updated, but not contents
set(CPACK_RPM_PACKAGE_RELEASE "${SRCML_PACKAGE_RELEASE}")

# Package names
set(CPACK_RPM_SRCML_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
set(CPACK_RPM_SRCMLDEV_PACKAGE_NAME "${CPACK_PACKAGE_NAME}-devel")

# Package filenames
if(DISTRO MATCHES "CentOS|Fedora")

    # CentOS and Fedora generate per-version naming
    set(CPACK_RPM_SRCML_FILE_NAME RPM-DEFAULT)
    set(CPACK_RPM_SRCMLDEV_FILE_NAME RPM-DEFAULT)
    set(CPACK_ARCHIVE_SRCML_FILE_NAME "${CPACK_RPM_SRCML_PACKAGE_NAME}-${PROJECT_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}")
    set(CPACK_ARCHIVE_SRCMLDEV_FILE_NAME "${CPACK_RPM_SRCMLDEV_PACKAGE_NAME}-${PROJECT_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}")

    # Archive packages are not generated with the distro name/version. So fix the archives to agree with the package naming
    # Must be run AFTER RPM packages are built
    add_custom_target(gen_package
       COMMAND ${CMAKE_COMMAND} --build . --target package
       COMMAND bash -c "mv dist/${CPACK_ARCHIVE_SRCML_FILE_NAME}.tar.gz dist/`rpm --specfile dist/_CPack_Packages/Linux/RPM/SPECS/${CPACK_RPM_SRCML_PACKAGE_NAME}.spec`.tar.gz"
       COMMAND bash -c "mv dist/${CPACK_ARCHIVE_SRCML_FILE_NAME}.tar.bz2 dist/`rpm --specfile dist/_CPack_Packages/Linux/RPM/SPECS/${CPACK_RPM_SRCML_PACKAGE_NAME}.spec`.tar.bz2"
       COMMAND bash -c "mv dist/${CPACK_ARCHIVE_SRCMLDEV_FILE_NAME}.tar.gz dist/`rpm --specfile dist/_CPack_Packages/Linux/RPM/SPECS/${CPACK_RPM_SRCMLDEV_PACKAGE_NAME}.spec`.tar.gz"
       COMMAND bash -c "mv dist/${CPACK_ARCHIVE_SRCMLDEV_FILE_NAME}.tar.bz2 dist/`rpm --specfile dist/_CPack_Packages/Linux/RPM/SPECS/${CPACK_RPM_SRCMLDEV_PACKAGE_NAME}.spec`.tar.bz2"
       WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
       VERBATIM)
else()
    # OpenSUSE does not generate per-version naming
    # Distribution version, e.g., lp152 for leap/15.2
    file(STRINGS "/etc/os-release" OS_RELEASE)
    string(REGEX MATCH "VERSION_ID=\"\([^\"]+\)\"" _ "${OS_RELEASE}")
    string(REPLACE "." "" RPM_VERSION_NUMBER ${CMAKE_MATCH_1})
    set(RPM_VERSION "lp${RPM_VERSION_NUMBER}")
    # Package filenames
    set(BASE_SRCML_FILE_NAME "${CPACK_COMPONENT_SRCML_DISPLAY_NAME}-${PROJECT_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}.${RPM_VERSION}")
    set(BASE_SRCMLDEV_FILE_NAME "${CPACK_COMPONENT_SRCML_DISPLAY_NAME}-devel-${PROJECT_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}.${RPM_VERSION}")
    set(CPACK_RPM_SRCML_FILE_NAME "${BASE_SRCML_FILE_NAME}.rpm")
    set(CPACK_RPM_SRCMLDEV_FILE_NAME "${BASE_SRCMLDEV_FILE_NAME}.rpm")
    set(CPACK_ARCHIVE_SRCML_FILE_NAME "${BASE_SRCML_FILE_NAME}")
    set(CPACK_ARCHIVE_SRCMLDEV_FILE_NAME "${BASE_SRCMLDEV_FILE_NAME}")
endif()

# SRCML package is main, so no extension is added
set(CPACK_RPM_MAIN_COMPONENT SRCML)

# srcml-devel package requires srcml package
set(CPACK_RPM_SRCMLDEV_PACKAGE_REQUIRES "${CPACK_PACKAGE_NAME} >= ${PROJECT_VERSION}")

# Homepage URLs
# Note: Default should be CMAKE_PROJECT_HOMEPAGE_URL, but isn't due to components
set(CPACK_RPM_SRCML_PACKAGE_URL ${CMAKE_PROJECT_HOMEPAGE_URL})
set(CPACK_RPM_SRCMLDEV_PACKAGE_URL ${CMAKE_PROJECT_HOMEPAGE_URL})

# Package groups
set(CPACK_RPM_SRCML_PACKAGE_GROUP "SRCMLDEV/Tools")
set(CPACK_RPM_SRCMLDEV_PACKAGE_GROUP "SRCMLDEV/Libraries")

# Adds to the release the distribution
set(CPACK_RPM_PACKAGE_RELEASE_DIST ON)

# Required changelog
set(CPACK_RPM_CHANGELOG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ChangeLog.txt")

# License
set(CPACK_RPM_PACKAGE_LICENSE "GPLv2+")

# post install script for ldconfig
# Note: Believe that newline is required
file(WRITE ${CPACK_BINARY_DIR}/post.sh "/sbin/ldconfig\n")
set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE   ${CPACK_BINARY_DIR}/post.sh)
set(CPACK_RPM_POST_UNINSTALL_SCRIPT_FILE ${CPACK_BINARY_DIR}/post.sh)

# CPack puts directories of installed files into the RPM
# Since they already exist, this is a conflict with other packages
# This excludes the following, mostly for documentation
# Note: There is no issue with this list including directories that are not needed
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION ${CMAKE_INSTALL_MANDIR}/man1 ${CMAKE_INSTALL_MANDIR}
        /usr/local /usr/local/include /usr/local/lib64 /usr/local/bin /usr/local/man /usr/local/man/man1 
        /usr/local/share /usr/local/share/man /usr/local/share/man/man1 /usr/share/man /usr/share/man/man1
        /usr/share/man1)
