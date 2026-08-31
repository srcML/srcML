# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)
#
# CMake files for the srcML client

# Configure libarchive on macOS
include(macos-libarchive.cmake)

find_package(LibArchive 3 REQUIRED)
find_package(CURL REQUIRED)
find_package(Threads REQUIRED)

# rpath needs help on macos
# Should be in a preset, but needed for all builds
if(APPLE)
    # Use rpath for the install directory
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

    # Specify the rpath directory (in this case, /usr/local/lib)
    list(APPEND CMAKE_INSTALL_RPATH "/usr/local/lib")
endif()

include(FetchContent)
FetchContent_Declare(cli11
  URL https://github.com/CLIUtils/CLI11/releases/download/v2.7.0/CLI11.hpp
  DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_MakeAvailable(cli11)
add_library(cli11 INTERFACE IMPORTED)
target_include_directories(cli11 SYSTEM INTERFACE ${cli11_SOURCE_DIR})

# ctpl_stl external include
include(FetchContent)
FetchContent_Declare(ctpl_stl_src
    GIT_REPOSITORY https://github.com/vit-vit/CTPL.git
    GIT_TAG        v.0.0.2
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(ctpl_stl_src)
set(MARKER_FILE "${ctpl_stl_src_SOURCE_DIR}/ctpl_stl_patched.marker")
if(NOT EXISTS ${MARKER_FILE})
    message(STATUS "Patching CTPL sources")

    # Patch the header file using your patch
    set(CTPL_STL_H "${ctpl_stl_src_SOURCE_DIR}/ctpl_stl.h")
    file(READ "${CTPL_STL_H}" FILE_CONTENTS)
    string(REPLACE
        "std::function<void(int id)> * _f;"
        "std::function<void(int id)> * _f = nullptr;"
        FILE_CONTENTS "${FILE_CONTENTS}")
    file(WRITE "${CTPL_STL_H}" "${FILE_CONTENTS}")

    # Create marker file to indicate patch applied
    file(WRITE ${MARKER_FILE} "CTPL patched.")
endif()
add_library(ctpl_stl INTERFACE IMPORTED)
target_include_directories(ctpl_stl SYSTEM INTERFACE ${ctpl_stl_src_SOURCE_DIR})

# clip cross-platform clipboard library
include(FetchContent)
FetchContent_Declare(clip
    GIT_REPOSITORY https://github.com/dacap/clip.git
    GIT_TAG        v1.15
    GIT_SHALLOW    TRUE
)
# only text support
set(CLIP_ENABLE_IMAGE OFF CACHE BOOL "" FORCE)
set(CLIP_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CLIP_TESTS OFF CACHE BOOL "" FORCE)
set(CLIP_INSTALL OFF CACHE BOOL "" FORCE)
# Scope BUILD_SHARED_LIBS to clip only so it builds statically
block()
    set(BUILD_SHARED_LIBS OFF)
    FetchContent_MakeAvailable(clip)
endblock()

# On Linux, clip links libxcb, which adds a runtime dependency on libxcb.so.1.
# When SRCML_CLIENT_STATIC_LIBXCB is ON, link libxcb statically so srcml has no
# runtime libxcb dependency. The static archives ship in the Debian/Ubuntu -dev
# packages but not on Fedora/openSUSE, so this is enabled per-platform via the
# presets rather than autodetected.
option(SRCML_CLIENT_STATIC_LIBXCB "Statically link libxcb into the srcml client (Linux)" OFF)

if(UNIX AND NOT APPLE AND SRCML_CLIENT_STATIC_LIBXCB)
    find_library(XCB_STATIC_LIB   NAMES libxcb.a)
    find_library(XAU_STATIC_LIB   NAMES libXau.a)
    find_library(XDMCP_STATIC_LIB NAMES libXdmcp.a)
    if(NOT (XCB_STATIC_LIB AND XAU_STATIC_LIB AND XDMCP_STATIC_LIB))
        message(FATAL_ERROR "SRCML_CLIENT_STATIC_LIBXCB=ON but the static libxcb/libXau/libXdmcp archives were not found")
    endif()
    message(STATUS "clip: linking libxcb statically")
    # clip is a static library, so it propagates its xcb dependency to srcml
    # through INTERFACE_LINK_LIBRARIES. Replace the dynamic xcb there with the
    # static archives (xcb before Xau/Xdmcp, which provide its symbols).
    set_target_properties(clip PROPERTIES
        LINK_LIBRARIES "${XCB_STATIC_LIB};${XAU_STATIC_LIB};${XDMCP_STATIC_LIB};Threads::Threads"
        INTERFACE_LINK_LIBRARIES "${XCB_STATIC_LIB};${XAU_STATIC_LIB};${XDMCP_STATIC_LIB};Threads::Threads")
endif()

# srcml executable
file(GLOB CLIENT_SOURCE *.hpp *.cpp)
add_executable(srcml ${CLIENT_SOURCE})
target_include_directories(srcml BEFORE PRIVATE .)
target_link_libraries(srcml PRIVATE srcML::LibsrcML LibArchive::LibArchive CURL::libcurl Threads::Threads cli11 ctpl_stl clip)

# Put executable in bin directory
set_target_properties(srcml PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# PreCompiled Headers configuration of the client
if(SRCML_CLIENT_PCH)
    target_precompile_headers(srcml PRIVATE "${SRCML_CLIENT_PCH}")
endif()

# Set the flags for the srcml client
target_compile_options(srcml PRIVATE "${SRCML_CLIENT_COMPILE_FLAGS}")
set_target_properties(srcml PROPERTIES LINK_FLAGS "${SRCML_CLIENT_LINK_FLAGS}")

# Install the client
install(TARGETS srcml COMPONENT SRCML)

# On Windows with vcpkg, install all the dlls that the vcpkg used
# Handled in libsrcml

if(MSVC)
    set_source_files_properties(srcml_cli.cpp PROPERTIES COMPILE_FLAGS /wd4866)
endif()
