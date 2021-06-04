##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
# 
# The srcML Toolkit is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# The srcML Toolkit is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with the srcML Toolkit; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
# 
# CMake files for building from source

include_directories(${CMAKE_SOURCE_DIR}/src/libsrcml)

if(NOT DYNAMIC_LOAD_ENABLED)
    add_definitions(-DNO_DLLOAD)
endif()

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_GENERATED_SOURCE_DIR ${CMAKE_BINARY_DIR}/parser)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

    set(WINDOWS_DEP_PATH ${PROJECT_BINARY_DIR}/deps)

    # Download project dependencies
    set(WINDOWS_DEP_FILENAME VS2017_Dependencies-06_20_18.zip)
    set(WINDOWS_DEP_URL http://www.sdml.cs.kent.edu/build/${WINDOWS_DEP_FILENAME})
    if (NOT EXISTS ${WINDOWS_DEP_PATH}/tools) 
        message("-- Download ${WINDOWS_DEP_URL}")
        file(DOWNLOAD ${WINDOWS_DEP_URL} ${WINDOWS_DEP_FILENAME})
        message("-- Unzip ${WINDOWS_DEP_URL} into ${WINDOWS_DEP_PATH}")
        file(ARCHIVE_EXTRACT INPUT ${WINDOWS_DEP_FILENAME} DESTINATION ${PROJECT_BINARY_DIR})
    endif()

    # Use installed dependencies for find_*()
    list(APPEND CMAKE_PREFIX_PATH ${WINDOWS_DEP_PATH}/release
                                  ${WINDOWS_DEP_PATH}/release/lib
                                  ${WINDOWS_DEP_PATH}/release/bin
                                  ${WINDOWS_DEP_PATH}/include)

elseif(APPLE)

    # Detect brew directories on macOS
    execute_process(COMMAND brew --prefix
        OUTPUT_VARIABLE BREW_PREFIX
        RESULT_VARIABLE BREW_RESULT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # Use the brew libarchive even if the platform has the correct version
    option(SRCML_BUILD_BREW_LIBARCHIVE "Use the brew libarchive regardless of macOS version" OFF)

    # macOS Big Sur (20.*.*) and Catalina (19.*.*) include libarchive.a 3.3.*
    # Only need include files
    string(SUBSTRING ${CMAKE_SYSTEM_VERSION} 0 2 OS_VERSION)
    if(NOT SRCML_BREW_LIBARCHIVE AND OS_VERSION GREATER_EQUAL "19")

        # install via Apple opensource
        set(CMAKE_EXTERNAL_SOURCE_DIR ${CMAKE_BINARY_DIR}/external)
        set(LIBARCHIVE_VERSION "libarchive-83.40.4")
        set(LIBARCHIVE_URL "https://opensource.apple.com/source/libarchive/${LIBARCHIVE_VERSION}")
        if(NOT EXISTS ${CMAKE_EXTERNAL_SOURCE_DIR}/archive.h)
            message("-- Download libarchive include files")
            file(DOWNLOAD ${LIBARCHIVE_URL}/libarchive/libarchive/archive.h ${CMAKE_EXTERNAL_SOURCE_DIR}/archive.h)
            file(DOWNLOAD ${LIBARCHIVE_URL}/libarchive/libarchive/archive_entry.h ${CMAKE_EXTERNAL_SOURCE_DIR}/archive_entry.h)
        endif()

        list(APPEND CMAKE_PREFIX_PATH "${CMAKE_EXTERNAL_SOURCE_DIR}")

    # macOS versions before Catalina only have libarchive 2, and require homebrew
    # and use a static library
    elseif(BREW_RESULT EQUAL 0 AND EXISTS "${BREW_PREFIX}/opt/libarchive")

        list(APPEND CMAKE_PREFIX_PATH "${BREW_PREFIX}/opt/libarchive")

    else()
        message(FATAL_ERROR "LibArchive >= 3 is required. Install via homebrew\n% brew install libarchive\n")
    endif()

    # Detect brew antlr2
    if(BREW_RESULT EQUAL 0 AND EXISTS ${BREW_PREFIX}/opt/antlr@2)

        list(APPEND CMAKE_PREFIX_PATH ${BREW_PREFIX}/opt/antlr@2
                                      ${BREW_PREFIX}/opt/antlr@2/include
                                      ${BREW_PREFIX}/opt/antlr@2/lib
                                      ${BREW_PREFIX}/opt/antlr@2/bin)
    else()
        message("   Note: antlr2 not found. Please use brew to install antlr2: ")
        message("      brew install antlr2")
    endif()

endif()

# Find the lib and include directly
find_library(ANTLR_LIBRARY NAMES libantlr-pic.a libantlr.a libantlr2-0.dll antlr.lib)
find_path(ANTLR_INCLUDE_DIR NAMES antlr/LLkParser.hpp)

# Locate packages
find_package(LibArchive 3 REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(CURL REQUIRED)
find_package(Iconv REQUIRED)
find_package(Boost REQUIRED)

if(DYNAMIC_LOAD_ENABLED)
    find_package(LibXslt)
else()
    find_package(LibXslt REQUIRED)
endif()

if(LIBXSLT_FOUND)
    include_directories(SYSTEM ${LIBXSLT_INCLUDE_DIR})
    add_definitions(-DWITH_LIBXSLT)
endif()

set(LIBXML2_LIBRARIES ${LIBXML2_LIBRARIES} ${Iconv_LIBRARIES})

if(NOT WIN32)
    include_directories(SYSTEM ${LibArchive_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS} ${Iconv_INCLUDE_DIRS} ${LibArchive_INCLUDE_DIR}
                    ${LIBXML2_INCLUDE_DIR} ${CURL_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${ANTLR_INCLUDE_DIR})
endif()

# standard compile options
add_compile_options(
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-fPIC>
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall>
    # $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wextra>
    # $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-pedantic>
    # $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-ansi>

    # $<$<CXX_COMPILER_ID:MSVC>:/Wall>
    $<$<CXX_COMPILER_ID:MSVC>:/D_CRT_SECURE_NO_WARNINGS>  # disable strcpy() warnings
    $<$<CXX_COMPILER_ID:MSVC>:/D_CRT_NONSTDC_NO_WARNINGS> # disable read() warnings
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

    string(APPEND CMAKE_CXX_FLAGS_DEBUG " /Od /ZI /MDd")
    string(APPEND CMAKE_CXX_FLAGS_RELEASE " /Ox")

endif()

add_subdirectory(parser)
add_subdirectory(libsrcml)
add_subdirectory(client)
