# SPDX-License-Identifier: GPL-3.0-only
#
# @file rpm.cmake
#
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
# 
# CPack configuration for RPM installers

# Exclude other platforms
if(NOT UNIX OR NOT DISTRO OR DISTRO MATCHES "Ubuntu")
    return()
endif()

# Update the generator list
list(APPEND CPACK_GENERATOR "RPM;TGZ;TBZ2")
list(REMOVE_DUPLICATES CPACK_GENERATOR)

# Turn ON/OFF internal CPackRPM debugging
set(CPACK_RPM_PACKAGE_DEBUG OFF)

# icon
set(CPACK_PACKAGE_ICON ${CMAKE_SOURCE_DIR}/package/srcml_icon.ico)

# Generate user and DEVLIBS rpms
set(CPACK_RPM_COMPONENT_INSTALL ON)

# Package release number (NOT srcml or libsrcml release)
# Note: Update when package is updated, but not contents
set(CPACK_RPM_PACKAGE_RELEASE "${SRCML_PACKAGE_RELEASE}")

# Package names
set(CPACK_RPM_SRCML_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
set(CPACK_RPM_DEVLIBS_PACKAGE_NAME "${CPACK_PACKAGE_NAME}-devel")

# Per-distribution package name
cmake_host_system_information(RESULT RPM_VERSION_NUMBER QUERY DISTRIB_VERSION_ID)
if(DISTRO MATCHES "Fedora|CentOS")

    # Distribution version, e.g., fc39 for Fedora 39 ("Fedora Core")
    set(RPM_VERSION "fc${RPM_VERSION_NUMBER}")

elseif(DISTRO MATCHES "openSUSE")

    # Distribution version, e.g., lp152 for leap/15.2
    set(RPM_VERSION "lp${RPM_VERSION_NUMBER}")

endif()

# Package filenames
set(BASE_SRCML_FILE_NAME    "${CPACK_COMPONENT_SRCML_DISPLAY_NAME}-${PROJECT_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}.${RPM_VERSION}.${CMAKE_HOST_SYSTEM_PROCESSOR}")
set(BASE_DEVLIBS_FILE_NAME "${CPACK_COMPONENT_SRCML_DISPLAY_NAME}-devel-${PROJECT_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}.${RPM_VERSION}.${CMAKE_HOST_SYSTEM_PROCESSOR}")
set(CPACK_RPM_SRCML_FILE_NAME "${BASE_SRCML_FILE_NAME}.rpm")
set(CPACK_RPM_DEVLIBS_FILE_NAME "${BASE_DEVLIBS_FILE_NAME}.rpm")
set(CPACK_ARCHIVE_SRCML_FILE_NAME "${BASE_SRCML_FILE_NAME}")
set(CPACK_ARCHIVE_DEVLIBS_FILE_NAME "${BASE_DEVLIBS_FILE_NAME}")

# SRCML package is main, so no extension is added
set(CPACK_RPM_MAIN_COMPONENT SRCML)

# srcml-devel package requires srcml package
set(CPACK_RPM_DEVLIBS_PACKAGE_REQUIRES "${CPACK_PACKAGE_NAME} >= ${PROJECT_VERSION}")

# Homepage URLs
# Note: Default should be CMAKE_PROJECT_HOMEPAGE_URL, but isn't due to components
set(CPACK_RPM_SRCML_PACKAGE_URL ${CMAKE_PROJECT_HOMEPAGE_URL})
set(CPACK_RPM_DEVLIBS_PACKAGE_URL ${CMAKE_PROJECT_HOMEPAGE_URL})

# Package groups
set(CPACK_RPM_SRCML_PACKAGE_GROUP "DEVLIBS/Tools")
set(CPACK_RPM_DEVLIBS_PACKAGE_GROUP "DEVLIBS/Libraries")

# Adds to the release the distribution
set(CPACK_RPM_PACKAGE_RELEASE_DIST ON)

# Required changelog
set(CPACK_RPM_CHANGELOG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ChangeLog.txt")

# License
set(CPACK_RPM_PACKAGE_LICENSE "GPL-3.0-only")

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

# Targets for workflow testing
add_workflow_test_targets(${CMAKE_BINARY_DIR} ${CPACK_OUTPUT_FILE_PREFIX} ${BASE_SRCML_FILE_NAME})

# Targets for installing generated packages
if(DISTRO MATCHES "Fedora|CentOS")

    set(RPM_INSTALLER_COMMAND dnf install -y)

elseif(DISTRO MATCHES "openSUSE")

    set(RPM_INSTALLER_COMMAND zypper install -y --allow-unsigned-rpm)

endif()
add_custom_target(install_package
    WORKING_DIRECTORY ${CPACK_OUTPUT_FILE_PREFIX}
    COMMAND ${RPM_INSTALLER_COMMAND} ./${CPACK_RPM_SRCML_FILE_NAME}
)
add_custom_target(install_targz
    WORKING_DIRECTORY ${CPACK_OUTPUT_FILE_PREFIX}
    COMMAND tar xvf ${CPACK_ARCHIVE_SRCML_FILE_NAME}.tar.gz -C /usr
)
add_custom_target(install_tarbz2
    WORKING_DIRECTORY ${CPACK_OUTPUT_FILE_PREFIX}
    COMMAND tar xvf ${CPACK_ARCHIVE_SRCML_FILE_NAME}.tar.bz2 -C /usr
)
