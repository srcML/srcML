# SPDX-License-Identifier: GPL-3.0-only
#
# @file wix.cmake
#
# @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
#
# CPack configuration for WIX installers

# Turn off start menu and possibly desktop links
# unset(CPACK_PACKAGE_EXECUTABLES)
# unset(CPACK_CREATE_DESKTOP_LINKS)

# Exclude other platforms
if(NOT WIN32)
    return()
endif()

# Update the generator list
list(APPEND CPACK_GENERATOR "WIX;ZIP")
list(REMOVE_DUPLICATES CPACK_GENERATOR)

# package naming
set(CPACK_SYSTEM_NAME "windows-x86_64")

# Windows is one package
set(CPACK_ARCHIVE_COMPONENT_INSTALL OFF)

# set icon
install(FILES ${CMAKE_SOURCE_DIR}/package/srcml_icon.ico DESTINATION . COMPONENT SRCML)
set(CPACK_WIX_PRODUCT_ICON ${CMAKE_SOURCE_DIR}/package/srcml_icon.ico)

# Prevent start menu shortcut
unset(CPACK_WIX_PROGRAM_MENU_FOLDER)

set(CPACK_START_MENU_SHORTCUTS "")
set(CPACK_DESKTOP_SHORTCUTS "")

# directory name
set(CPACK_PACKAGE_INSTALL_DIRECTORY "srcML")

# Custom GUID
# set(CPACK_WIX_PRODUCT_GUID "8EB65BD8-DF5B-4DED-A642-80BDBBDEB58C")

# Custom GUID
# set(CPACK_WIX_UPGRADE_GUID "8666C73A-223D-4E5A-BB14-D4740158066F")

# Background on dialogs
set(CPACK_WIX_UI_DIALOG "${CMAKE_CURRENT_SOURCE_DIR}/background.png")
set(CPACK_WIX_UI_BANNER "${CMAKE_CURRENT_SOURCE_DIR}/banner.png")

# ID of custom UI
set(CPACK_WIX_UI_REF "srcMLUI_InstallDir")

# Add to CMake registry so that other CMake files can use find_program()
set(CPACK_WIX_CMAKE_PACKAGE_REGISTRY srcML)

# Extra dialog for adding to path
set(CPACK_WIX_EXTRA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/srcml_extra_dialog.wxs" "${CMAKE_CURRENT_SOURCE_DIR}/install_dir.wxs")

# Path for path environment
set(CPACK_WIX_PATCH_FILE "${CMAKE_CURRENT_SOURCE_DIR}/patch_path_env.xml")

# Template instead of default WiX template
set(CPACK_WIX_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/WIX.template.in")

# Architecture
set(SRCML_SYSTEM_ARCHITECTURE "x86_64")

# Package filename
set(SRCML_WIX_FILE_NAME "${CPACK_PACKAGE_NAME}-${PROJECT_VERSION}-windows-${SRCML_SYSTEM_ARCHITECTURE}.msi")

# Targets for installing generated packages
add_custom_target(install_package_wix
    WORKING_DIRECTORY ${CPACK_OUTPUT_FILE_PREFIX}
    COMMAND msiexec /i ${SRCML_WIX_FILE_NAME} /qn
)
