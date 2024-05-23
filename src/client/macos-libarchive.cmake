# SPDX-License-Identifier: GPL-3.0-only
##
# @file macos-libarchive.cmake
# 
# @copyright Copyright (C) 2023 srcML, LLC. (www.srcML.org)
# 
# libarchive configuration of macOS for the srcML client

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

    # Map from macOS version to libarchive number
    # Catalina
    set(LIBARCHIVE19 "72.140.1")
    # Big Sur
    set(LIBARCHIVE20 "83.100.2")
    # Monterey
    set(LIBARCHIVE21 "83.100.2")
    # Ventura
    set(LIBARCHIVE22 "83.100.2")
    # Sonoma
    set(LIBARCHIVE23 "83.100.2")
    set(LIBARCHIVE_VERSION "${LIBARCHIVE${OS_VERSION}}")

    # fetch via Apple opensource
    set(LIBARCHIVE_URL "https://opensource.apple.com/source/libarchive/libarchive-${LIBARCHIVE_VERSION}/libarchive/libarchive")
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
