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
