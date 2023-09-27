# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
#
# @copyright Copyright (C) 2013-2023 srcML, LLC. (www.srcML.org)
#
# CMake files for building from source

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# For Windows, install redist exe
set(CMAKE_INSTALL_SYSTEM_RUNTIME_COMPONENT SRCML)
include(InstallRequiredSystemLibraries)

# Build options
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(BUILD_LIBSRCML "Build (and potentially install) libsrcml" ON)

# Exeternally defined compile and link options
add_compile_options(${SRCML_GLOBAL_COMPILE_OPTIONS})
add_link_options("${SRCML_GLOBAL_LINK_OPTIONS}")

# if(BUILD_LIBSRCML)
    add_subdirectory(parser)
    add_subdirectory(libsrcml)
# endif()
add_subdirectory(client)
