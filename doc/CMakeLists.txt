# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
# 
# CMake files for documentation

# Turn ON/OFF building documentation
option(BUILD_CLIENT_DOC "Build client documentation" OFF)
option(INSTALL_CLIENT_DOC "Install (but do not build) client documentation" OFF)
option(BUILD_LIBSRCML_DOC "Build libsrcml documentation" OFF)
if(NOT BUILD_CLIENT_DOC)
    set(INSTALL_CLIENT_DOC ON)
endif()

# Turn ON building all documentation
option(BUILD_ALL_DOCS "Build all documentation" OFF)
if(BUILD_ALL OR BUILD_ALL_DOCS)
    set(BUILD_CLIENT_DOC ON)
    set(BUILD_LIBSRCML_DOC ON)
endif()

if(BUILD_CLIENT_DOC OR INSTALL_CLIENT_DOC)
    add_subdirectory(manpage)
endif()

if(BUILD_LIBSRCML_DOC)
    add_subdirectory(doxygen)
endif()
