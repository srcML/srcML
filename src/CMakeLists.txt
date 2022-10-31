# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2022 srcML, LLC. (www.srcML.org)
# 
# CMake files for building from source

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(SRCML_BUILD_USEPCH "Use PCH (PreCompiled Headers" OFF)

# Big Sur and later macOS can build for both x86 and arm64
string(SUBSTRING ${CMAKE_SYSTEM_VERSION} 0 2 OS_VERSION)
if(APPLE AND OS_VERSION GREATER_EQUAL "20")
    include(CheckCXXCompilerFlag)

    set(ARCHITECTURES "x86_64;arm64")
    check_cxx_compiler_flag("-arch x86_64" PLATFORM_SUPPORTS_x86_64)
    check_cxx_compiler_flag("-arch arm64" PLATFORM_SUPPORTS_arm64)

    if(PLATFORM_SUPPORTS_x86_64 AND PLATFORM_SUPPORTS_arm64)
        list(FIND CMAKE_OSX_ARCHITECTURES "x86_64" AMD_POSITION)
        list(FIND CMAKE_OSX_ARCHITECTURES "arm64" ARM_POSITION)
        if(AMD_POSITION EQUAL -1 OR ARM_POSITION EQUAL -1)
            message(STATUS "Detected ability for a universal build. To do so, use -DCMAKE_OSX_ARCHITECTURE=\"${ARCHITECTURES}\"")
        endif()
    endif()
endif()

# Clang profiling
option(CLANG_TIME_TRACE "Enable clang profiling." OFF)
if(CLANG_TIME_TRACE)
    add_compile_options(-ftime-trace)
endif()

if(MSVC)
    add_compile_options(/Zc:throwingNew)
    string(REPLACE "Ob2" "Ob3" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
endif()

# For macOS Catalina and more recent, the provided libarchive is at least version 3.3
# and all that is needed is the matching header files.
# For earlier versions of macOS, brew must be used with a more recent libarchive.a
if(APPLE AND NOT DEFINED VCPKG_TARGET_TRIPLET)

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

        # Map from macOS version to libarchive number
        if(OS_VERSION EQUAL 19)
            # Catalina
            set(LIBARCHIVE_VERSION "72.140.1")
        elseif(OS_VERSION EQUAL 20)
            # Big Sur
            set(LIBARCHIVE_VERSION "83.100.2")
        elseif(OS_VERSION EQUAL 21)
            # Monterey
            set(LIBARCHIVE_VERSION "83.100.2")
        elseif(OS_VERSION EQUAL 22)
            # Ventura
            set(LIBARCHIVE_VERSION "83.100.2")
        endif()

        # fetch via Apple opensource
        set(LIBARCHIVE_URL "https://opensource.apple.com/source/libarchive/libarchive-${LIBARCHIVE_VERSION}/libarchive/libarchive")
        include(FetchContent)
        FetchContent_Declare(libarchiveInclude1
            URL ${LIBARCHIVE_URL}/archive.h
            DOWNLOAD_NO_EXTRACT TRUE
        )
        FetchContent_MakeAvailable(libarchiveInclude1)
        FetchContent_Declare(libarchiveInclude
            URL ${LIBARCHIVE_URL}/archive_entry.h
            SOURCE_DIR ${libarchiveinclude1_SOURCE_DIR}
            DOWNLOAD_NO_EXTRACT TRUE
        )
        FetchContent_MakeAvailable(libarchiveInclude)

        # Record so that LibArchive::LibArchive finds the headers
        set(LibArchive_INCLUDE_DIR "${libarchiveinclude_SOURCE_DIR}")

        unset(libarchiveInclude1)
        unset(libarchiveInclude)
        unset(LIBARCHIVE_VERSION)
        unset(LIBARCHIVE_URL)

    # macOS versions before Catalina only have libarchive 2, and require homebrew
    # and use a static library
    elseif(BREW_RESULT EQUAL 0 AND EXISTS "${BREW_PREFIX}/opt/libarchive")

        # Make sure LibArchive::LibArchive looks a brew first
        list(APPEND CMAKE_PREFIX_PATH "${BREW_PREFIX}/opt/libarchive")

    else()

        message(FATAL_ERROR "LibArchive >= 3 is required. Install via homebrew\n% brew install libarchive\n")

    endif()

endif()

# For Windows, install redist exe
set(CMAKE_INSTALL_SYSTEM_RUNTIME_COMPONENT SRCML)
include(InstallRequiredSystemLibraries)

# standard compile options
add_compile_options(
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall>
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wextra>
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-pedantic>
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-ansi>

    $<$<CXX_COMPILER_ID:MSVC>:/Wall>
    $<$<CXX_COMPILER_ID:MSVC>:/D_CRT_SECURE_NO_WARNINGS>  # disable strcpy() warnings
    $<$<CXX_COMPILER_ID:MSVC>:/D_CRT_NONSTDC_NO_WARNINGS> # disable read() warnings
)

# specific warnings
option(ENABLE_BUILD_WARNINGS "Enable build warnings" OFF)
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

# CMake 3.24 URL download timestamp
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24")
    cmake_policy(SET CMP0135 NEW)
endif()

add_subdirectory(client)
add_subdirectory(parser)
add_subdirectory(libsrcml)
