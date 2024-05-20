# SPDX-License-Identifier: GPL-3.0-only
#
# @file nuget.cpack
#
# @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
#
# CPack configuration for NuGet installers

# Exclude other platforms
if(NOT WIN32)
    return()
endif()

# Update the generator list
list(APPEND CPACK_GENERATOR "NuGet")
list(REMOVE_DUPLICATES CPACK_GENERATOR)

# License
set(CPACK_NUGET_SRCML_PACKAGE_LICENSE_EXPRESSION "GPL-3.0-only")
set(CPACK_NUGET_DEVLIBS_PACKAGE_LICENSE_EXPRESSION "GPL-3.0-only")

# Architecture
set(SRCML_SYSTEM_ARCHITECTURE "x86_64")

# Package filename
set(SRCML_NUGET_FILE_NAME "${CPACK_PACKAGE_NAME}.${PROJECT_VERSION}.nupkg")

# Targets for installing generated packages
add_custom_target(install_package_nuget
    #WORKING_DIRECTORY ${CPACK_OUTPUT_FILE_PREFIX}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/dist
    COMMAND mkdir LocalNuGetFeed
    COMMAND nuget add ${SRCML_NUGET_FILE_NAME} -Source LocalNuGetFeed
    COMMAND nuget install srcML -Source ${CMAKE_BINARY_DIR}/LocalNuGetFeed -OutputDirectory "C:/Program Files"
    COMMAND powershell -Command "[System.Environment]::SetEnvironmentVariable('Path', [System.Environment]::GetEnvironmentVariable('Path', [System.EnvironmentVariableTarget]::Machine) + ';C:/Program Files/srcML/bin', [System.EnvironmentVariableTarget]::Machine)"
)
