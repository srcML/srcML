# SPDX-License-Identifier: GPL-3.0-only
##
# @file macos-libarchive.cmake
# 
# @copyright Copyright (C) 2023-2024 srcML, LLC. (www.srcML.org)
# 
# libarchive configuration of macOS for the srcML client

if(NOT APPLE OR DEFINED VCPKG_TARGET_TRIPLET)
    return()
endif()

# For macOS Catalina and more recent, the provided libarchive is at least version 3.3
# and all that is needed is the matching header files.
# For earlier versions of macOS, brew must be used with a more recent libarchive.a
# Detect brew directories on macOS
execute_process(COMMAND brew --prefix
    OUTPUT_VARIABLE BREW_PREFIX
    RESULT_VARIABLE BREW_RESULT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Use the brew libarchive even if the platform has the correct version
option(SRCML_BUILD_BREW_LIBARCHIVE "Use the brew libarchive regardless of macOS version" OFF)

# macOS since Catalina (19.*.*) includes libarchive.a 3.3+
# Only need include files
string(SUBSTRING ${CMAKE_SYSTEM_VERSION} 0 2 OS_VERSION)
if(NOT SRCML_BUILD_BREW_LIBARCHIVE AND OS_VERSION GREATER_EQUAL "19")

    # Use the local tar to see which version of libarchive is needed
    execute_process(
        COMMAND /usr/bin/tar --version
        OUTPUT_VARIABLE TAR_VERSION_OUTPUT
        ERROR_VARIABLE TAR_VERSION_ERROR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(TAR_VERSION_ERROR)
        message(FATAL_ERROR "Error on macOS getting tar version: ${TAR_VERSION_ERROR}")
    endif()

    string(REGEX MATCH "libarchive ([0-9]+\\.[0-9]+\\.[0-9]+)" LIBARCHIVE_VERSION_MATCH "${TAR_VERSION_OUTPUT}")
    if(NOT LIBARCHIVE_VERSION_MATCH)
        message(FATAL_ERROR "Could not determine libarchive version from tar output")
    endif()
    string(REGEX REPLACE "libarchive " "" LIBARCHIVE_VERSION "${LIBARCHIVE_VERSION_MATCH}")

    # Print the detected libarchive version
    message(STATUS "Detected libarchive version: ${LIBARCHIVE_VERSION}")

    # fetch via libarchive repositories
    set(LIBARCHIVE_URL "https://raw.githubusercontent.com/libarchive/libarchive/v${LIBARCHIVE_VERSION}/libarchive")
    message(STATUS "Download ${LIBARCHIVE_URL}/archive.h")
    include(FetchContent)
    FetchContent_Declare(libarchiveInclude1
        URL ${LIBARCHIVE_URL}/archive.h
        DOWNLOAD_NO_EXTRACT TRUE
    )
    FetchContent_MakeAvailable(libarchiveInclude1)
    message(STATUS "Download ${LIBARCHIVE_URL}/archive_entry.h")
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
