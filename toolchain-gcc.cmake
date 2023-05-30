# SPDX-License-Identifier: GPL-3.0-only
##
# @file toolchain-gcc.cmake
#
# @copyright Copyright (C) 2023 srcML, LLC. (www.srcML.org)
#
# MSVC-specific tool configuration

# Link the stdc++fs library if on gcc 7.5.0
if(CMAKE_CXX_COMPILER_VERSION STREQUAL "7.5.0")
    link_libraries(stdc++fs)
endif()

set(CMAKE_OPTIMIZE_DEPENDENCIES ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
