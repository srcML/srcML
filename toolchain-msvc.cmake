# SPDX-License-Identifier: GPL-3.0-only
##
# @file msvc.toolchain.cmake
#
# @copyright Copyright (C) 2023 srcML, LLC. (www.srcML.org)
#
# MSVC-specific tool configuration

# Use vcpkg
if(DEFINED ENV{VCPKG_ROOT})
    message(STATUS "Building with vcpkg")
    include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
else()
    message(ERROR "VCPKG_ROOT not defined")
endif()

# Change to /Ob3 from /Ob2 optimization for MSVC release
string(REPLACE "/Ob2" "/Ob3" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

add_compile_options(/Zc:throwingNew /EHsc)

set(CONFIG_FILE_DESTINATION "cmake")

# MSVC stopped automatically defining this
add_definitions(-DWIN32)
