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

option(CLANG_TIME_TRACE "Enable clang profiling." OFF)

option(SRCML_BUILD_USEPCH "Use PCH (PreCompiled Headers" OFF)

# Big Sur and later OSX platforms can build for both x86 and arm64, so default to both
string(SUBSTRING ${CMAKE_SYSTEM_VERSION} 0 2 OS_VERSION)
if(APPLE AND CMAKE_OSX_ARCHITECTURES_NOTSET AND OS_VERSION GREATER_EQUAL "20")
    include(CheckCXXCompilerFlag)

    check_cxx_compiler_flag("-arch x86_64" Platform_Supports_x86_64)
    check_cxx_compiler_flag("-arch arm64" Platform_Supports_arm64)

    if(Platform_Supports_x86_64 AND Platform_Supports_arm64)
        set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Build architectures for Mac OS X" FORCE)
        message(STATUS "Detected ability to build for multiple architectures: ${CMAKE_OSX_ARCHITECTURES}")
    endif()
endif()

include_directories(${CMAKE_SOURCE_DIR}/src/libsrcml)

if(CLANG_TIME_TRACE)
    add_compile_options(-ftime-trace)
endif()

if(NOT CMAKE_INCLUDE_SYSTEM_FLAG_CXX)
    set(CMAKE_INCLUDE_SYSTEM_FLAG_CXX /external:I)
    add_compile_options(/external:W0)
endif()

if(NOT DYNAMIC_LOAD_ENABLED)
    add_definitions(-DNO_DLLOAD)
endif()

set(CMAKE_CXX_STANDARD 11)

# Generated source from antlr and the parser
set(CMAKE_GENERATED_SOURCE_DIR ${CMAKE_BINARY_DIR}/parser)

# External code directory
set(CMAKE_EXTERNAL_SOURCE_DIR ${CMAKE_BINARY_DIR}/external)

if(MSVC)

    set(WINDOWS_DEP_PATH ${PROJECT_BINARY_DIR}/deps)

    # Download project dependencies
    set(WINDOWS_DEP_FILENAME VS2021_Dependencies-05-26-21.zip)
    # set(WINDOWS_DEP_FILENAME VS2017_Dependencies-06_20_18.zip)
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
    if(NOT SRCML_BUILD_BREW_LIBARCHIVE AND OS_VERSION GREATER_EQUAL "19")

        # install via Apple opensource
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

endif()

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

# Setup ANTLR 2.7 library
set(ANTLR_EXTERNAL ${CMAKE_EXTERNAL_SOURCE_DIR}/antlr-2.7.7)
if(NOT EXISTS ${ANTLR_EXTERNAL})
    set(ANTLR_URL https://www.antlr2.org/download/antlr-2.7.7.tar.gz)
    message(STATUS "Download ${ANTLR_URL}")
    file(DOWNLOAD ${ANTLR_URL} ${CMAKE_EXTERNAL_SOURCE_DIR}/antlr-2.7.7.tar.gz)
    file(ARCHIVE_EXTRACT INPUT ${CMAKE_EXTERNAL_SOURCE_DIR}/antlr-2.7.7.tar.gz
        DESTINATION ${CMAKE_EXTERNAL_SOURCE_DIR})

    # Add needed include file for CharScanner.hpp
    set(CHARSCANNER_HPP "${ANTLR_EXTERNAL}/lib/cpp/antlr/CharScanner.hpp")
    file(READ ${CHARSCANNER_HPP} FILE_CONTENTS)
    string(REPLACE "#include <antlr/config.hpp>" "#include <antlr/config.hpp>\n#include <string.h>" FILE_CONTENTS "${FILE_CONTENTS}")
    file(WRITE ${CHARSCANNER_HPP} "${FILE_CONTENTS}")

    # Remove pragma warning disable that is no longer valie
    set(CONFIG_HPP "${ANTLR_EXTERNAL}/lib/cpp/antlr/config.hpp")
    file(READ ${CONFIG_HPP} FILE_CONTENTS)
    string(REPLACE "# pragma warning( disable : 4786 4231 )" "# pragma warning( disable : 4786 )" FILE_CONTENTS "${FILE_CONTENTS}")
    file(WRITE ${CONFIG_HPP} "${FILE_CONTENTS}")

    file(REMOVE ${CMAKE_EXTERNAL_SOURCE_DIR}/antlr-2.7.7.tar.gz)
endif()

set(ANTLR_EXTERNAL ${CMAKE_EXTERNAL_SOURCE_DIR}/antlr-2.7.7)
set(ANTLR_INCLUDE_DIR "${ANTLR_EXTERNAL}/lib/cpp/")

set(LIBXML2_LIBRARIES ${LIBXML2_LIBRARIES} ${Iconv_LIBRARIES})

if(NOT WIN32)
    include_directories(SYSTEM ${LibArchive_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS} ${Iconv_INCLUDE_DIRS} ${LibArchive_INCLUDE_DIR}
                    ${LIBXML2_INCLUDE_DIR} ${CURL_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${ANTLR_INCLUDE_DIR})
endif()

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "dir='${dir}'")
endforeach()

# standard compile options
add_compile_options(
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-fPIC>
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall>
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wextra>
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-pedantic>
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-ansi>

    $<$<CXX_COMPILER_ID:MSVC>:/Wall>
    $<$<CXX_COMPILER_ID:MSVC>:/D_CRT_SECURE_NO_WARNINGS>  # disable strcpy() warnings
    $<$<CXX_COMPILER_ID:MSVC>:/D_CRT_NONSTDC_NO_WARNINGS> # disable read() warnings
)

# handle specific warnings
option(ENABLE_BUILD_WARNINGS "Enable disabled build warnings" OFF)
if(NOT ENABLE_BUILD_WARNINGS)
    add_compile_options(
        $<$<CXX_COMPILER_ID:MSVC>:/wd4068> # unknown pragma
        # $<$<CXX_COMPILER_ID:MSVC>:/wd4101> # unreferenced local variable
        # $<$<CXX_COMPILER_ID:MSVC>:/wd4519> # unmarked inlining notification warning
        $<$<CXX_COMPILER_ID:MSVC>:/wd4514> # removed inline function not called (in one context)
            # $<$<CXX_COMPILER_ID:MSVC>:/wd4668> # preprocessor symbol not defined but used in an #if
        $<$<CXX_COMPILER_ID:MSVC>:/wd4710> # unmarked inlining notification warning
        $<$<CXX_COMPILER_ID:MSVC>:/wd4820> # byte padding
        $<$<CXX_COMPILER_ID:MSVC>:/wd5039> # pointer or reference to potentially throwing function passed to 'extern "C"' function under -EHc
        $<$<CXX_COMPILER_ID:MSVC>:/wd4711> # selected for automatic inline expansion
        $<$<CXX_COMPILER_ID:MSVC>:/wd4706> # assigment within conditional expression

        $<$<CXX_COMPILER_ID:MSVC>:/wd4625> # copy constructor was implicitly defined as deleted
        $<$<CXX_COMPILER_ID:MSVC>:/wd4626> # assignment operator was implicitly defined as deleted
        $<$<CXX_COMPILER_ID:MSVC>:/wd5026> # move constructor was implicitly defined as deleted
        $<$<CXX_COMPILER_ID:MSVC>:/wd5027> # move assignment operator was implicitly defined as deleted
        $<$<CXX_COMPILER_ID:MSVC>:/wd4668> # is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
        $<$<CXX_COMPILER_ID:MSVC>:/wd4868> # compiler may not enforce left-to-right evaluation order in braced initializer list
        $<$<CXX_COMPILER_ID:MSVC>:/wd4371> # layout of class may have changed from a previous version of the compiler due to better packing of member
        $<$<CXX_COMPILER_ID:MSVC>:/wd4623> # default constructor was implicitly defined as deleted
        $<$<CXX_COMPILER_ID:MSVC>:/wd5045> # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
    )
endif()

if(MSVC)

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options("/Od")
        add_compile_options("/Zi")
        add_compile_options("/MDd")
    else()
        add_compile_options("/O2")
    endif()

endif()

add_subdirectory(client)
add_subdirectory(parser)
add_subdirectory(libsrcml)
