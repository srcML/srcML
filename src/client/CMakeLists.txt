# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
# 
# CMake files for the srcML client

# if(NOT srcML::LibsrcML)
#     find_package(srcml REQUIRED)
# endif()

# Configure libarchive on macOS
if(APPLE AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(APPLE_NON_VCPKG ON)
endif()
if(APPLE_NON_VCPKG)
    include(macos-libarchive.cmake)
endif()

find_package(LibArchive 3 REQUIRED)
find_package(CURL REQUIRED)

# rpath needs help on macos
if(APPLE)
    # Use rpath for the install directory
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

    # Specify the rpath directory (in this case, /usr/local/lib)
    list(APPEND CMAKE_INSTALL_RPATH "/usr/local/lib")
endif()

include(FetchContent)
FetchContent_Declare(cli11
  URL https://github.com/CLIUtils/CLI11/releases/download/v2.2.0/CLI11.hpp
  DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_MakeAvailable(cli11)
add_library(cli11 INTERFACE IMPORTED)
target_include_directories(cli11 SYSTEM INTERFACE ${cli11_SOURCE_DIR})

# ctpl_stl external include
include(FetchContent)
FetchContent_Declare(ctpl_stl
  GIT_REPOSITORY https://github.com/vit-vit/CTPL.git
  GIT_TAG        v.0.0.2
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(ctpl_stl)
add_library(ctpl_stl INTERFACE IMPORTED)
target_include_directories(ctpl_stl SYSTEM INTERFACE ${ctpl_stl_SOURCE_DIR})

# srcml executable
file(GLOB CLIENT_SOURCE *.hpp *.cpp)
add_executable(srcml ${CLIENT_SOURCE})
target_include_directories(srcml BEFORE PRIVATE .)
target_link_libraries(srcml PRIVATE srcML::LibsrcML LibArchive::LibArchive CURL::libcurl cli11 ctpl_stl)

# Put executable in bin directory
set_target_properties(srcml PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# PreCompiled Headers configuration of the client
if (SRCML_CLIENT_PCH)
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
