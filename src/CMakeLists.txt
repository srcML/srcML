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

message(STATUS "CMake install prefix: ${CMAKE_INSTALL_PREFIX}")

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

    message("MSVC BUILD")

    add_definitions(-DWITH_LIBXSLT)
    set(WINDOWS_DEP_PATH ${PROJECT_BINARY_DIR}/deps)

    include_directories(${WINDOWS_DEP_PATH}/include)
    link_directories(${WINDOWS_DEP_PATH}/$(ConfigurationName)/lib)
    
    set(LIBXSLT_LIBRARIES libxslt.lib)
    set(LIBXSLT_EXSLT_LIBRARY libexslt.lib)
    set(LibArchive_LIBRARIES archive.lib)
    set(LIBXML2_LIBRARIES libxml2.lib libiconv.lib)
    set(CURL_LIBRARIES libcurl.lib)
    set(ANTLR_LIBRARY antlr.lib)

    set(Boost_NO_SYSTEM_PATHS ON)
    set(Boost_LIBRARY_DIR_RELEASE ${WINDOWS_DEP_PATH}/release/lib)
    set(Boost_LIBRARY_DIR_DEBUG ${WINDOWS_DEP_PATH}/debug/lib)
    set(BOOST_INCLUDE_DIR ${WINDOWS_DEP_PATH}/include)
    set(BOOST_INCLUDEDIR ${WINDOWS_DEP_PATH}/include)
    set(Boost_INCLUDE_DIRS ${WINDOWS_DEP_PATH}/include)
#    find_package(Boost COMPONENTS program_options REQUIRED)

else()

    set(WINDOWS_DEP_PATH "")

    if(NOT APPLE)
        find_package(LibArchive 3 REQUIRED)
    else()

        # Detect brew directories on macOS
        execute_process(COMMAND brew --prefix
            OUTPUT_VARIABLE BREW_PREFIX
            RESULT_VARIABLE BREW_RESULT
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        set(BREW "YES")
    
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
                message("-- Downloading libarchive include files")
                file(DOWNLOAD ${LIBARCHIVE_URL}/libarchive/libarchive/archive.h ${CMAKE_EXTERNAL_SOURCE_DIR}/archive.h)
                file(DOWNLOAD ${LIBARCHIVE_URL}/libarchive/libarchive/archive_entry.h ${CMAKE_EXTERNAL_SOURCE_DIR}/archive_entry.h)
            endif()
            set(LibArchive_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/external)
            set(LibArchive_INCLUDE_DIR ${LibArchive_INCLUDE_DIRS})
            set(LibArchive_LIBRARIES libarchive.dylib)

        # macOS versions before Catalina only have libarchive 2, and require homebrew
        # and use a static library
        elseif(BREW AND EXISTS "${BREW_PREFIX}/opt/libarchive")

            set(LibArchive_INCLUDE_DIRS ${BREW_PREFIX}/opt/libarchive/include)
            set(LibArchive_LIBRARIES ${BREW_PREFIX}/opt/libarchive/lib/libarchive.a ${BREW_PREFIX}/lib/libzstd.a ${BREW_PREFIX}/lib/liblz4.a ${BREW_PREFIX}/lib/liblzma.a libbz2.dylib libcompression.dylib libz.dylib libxar.dylib libiconv.dylib libexpat.dylib)

        else()
            message(FATAL_ERROR "LibArchive >= 3 is required. Install via homebrew\n% brew install libarchive\n")
        endif()

        # Detect brew antlr2
        if(BREW AND EXISTS ${BREW_PREFIX}/opt/antlr@2)
            list(APPEND CMAKE_PREFIX_PATH "${BREW_PREFIX}/opt/antlr@2")
            include_directories("${BREW_PREFIX}/opt/antlr@2/include")
        else()
            message("   Note: antlr2 not found. Please use brew to install antlr2: ")
            message("      brew install antlr2")
        endif()

    endif()

    # Locating packages
    find_package(LibXml2 REQUIRED)
    find_package(CURL REQUIRED)
    find_package(Iconv REQUIRED)
    set(Boost_NO_BOOST_CMAKE ON)
    set(Boost_USE_STATIC_LIBS ON)
    find_package(Boost COMPONENTS REQUIRED)

    # add include directories
    include_directories(${LibArchive_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${Iconv_INCLUDE_DIRS} ${LibArchive_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR} ${CURL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

    if(DYNAMIC_LOAD_ENABLED)
        find_package(LibXslt)
    else()
        find_package(LibXslt REQUIRED)
    endif()

    if(LIBXSLT_FOUND)
        include_directories(${LIBXSLT_INCLUDE_DIR})
        add_definitions(-DWITH_LIBXSLT)
    endif()

    # Locating the antlr library.
    find_library(ANTLR_LIBRARY NAMES libantlr-pic.a libantlr.a libantlr2-0.dll antlr.lib PATHS /usr/lib /usr/local/lib)

endif()

if(${CMAKE_COMPILER_IS_GNUCXX})

    add_compile_options(
        -fPIC -Wno-long-long -Wall -Wextra  -Wall -pedantic -Wempty-body -Wignored-qualifiers -Wsign-compare -Wtype-limits -Wuninitialized -Wno-pragmas -Wno-variadic-macros
    )

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")

    add_compile_options(
        -fPIC -Wno-long-long -Wall -Wextra -Wshorten-64-to-32 -Wno-unknown-pragmas -Wno-int-to-void-pointer-cast
    )

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    message(FATAL_ERROR "Configuration Not Implemented: ${CMAKE_CXX_COMPILER_ID}. Build not configured for selected compiler.")
    
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /ZI /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox")

    add_compile_options(/W3 /wd4068 /wd4101 /D_CRT_SECURE_NO_WARNINGS /D_CRT_NONSTDC_NO_WARNINGS)

else()
    message(FATAL_ERROR "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}. Build not configured for selected compiler.")
endif()

add_subdirectory(parser)
add_subdirectory(libsrcml)
add_subdirectory(client)
