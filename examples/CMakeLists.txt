# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
# 
# CMake files for examples

# Turn ON/OFF building examples
option(BUILD_EXAMPLES "Build examples usage files for libsrcml" OFF)
if(BUILD_ALL)
    set(BUILD_EXAMPLES ON)
endif()

add_subdirectory(libsrcml)
