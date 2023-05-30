# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2023 srcML, LLC. (www.srcML.org)
# 
# CMake files for building from source

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/Zc:throwingNew)
    string(REPLACE "Ob2" "Ob3" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
endif()

# For Windows, install redist exe
set(CMAKE_INSTALL_SYSTEM_RUNTIME_COMPONENT SRCML)
include(InstallRequiredSystemLibraries)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)


# CMake 3.24 URL download timestamp
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24")
    cmake_policy(SET CMP0135 NEW)
endif()

add_subdirectory(client)
add_subdirectory(parser)
add_subdirectory(libsrcml)
