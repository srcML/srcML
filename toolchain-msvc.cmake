# SPDX-License-Identifier: GPL-3.0-only
##
# @file msvc.toolchain.cmake
#
# @copyright Copyright (C) 2023 srcML, LLC. (www.srcML.org)
#
# MSVC-specific tool configuration

# Change to /Ob3 from /Ob2 optimization for MSVC release
string(REPLACE "/Ob2" "/Ob3" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

set(COMPILE_OPTIONS "/Zc:throwingNew")

set(CONFIG_FILE_DESTINATION "cmake")
