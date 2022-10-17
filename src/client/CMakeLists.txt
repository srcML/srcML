# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
# 
# CMake files for the srcML client

find_package(LibArchive 3 REQUIRED)
find_package(CURL REQUIRED)
if(NOT MSVC)
    find_package(Threads REQUIRED)
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
if(NOT MSVC)
    target_link_libraries(srcml PRIVATE Threads::Threads)
endif()

set_target_properties(srcml PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# With MSVC, interprocedural optimization leads to a 10x static library, with only a slight reduction in size for the dynamic library
set_target_properties(srcml PROPERTIES INTERPROCEDURAL_OPTIMIZATION $<IF:$<CXX_COMPILER_ID:MSVC>,OFF,ON>)

# Largest impace for PCH builds of the client
if(SRCML_BUILD_USEPCH)
    target_precompile_headers(srcml PRIVATE <srcml_cli.hpp> <srcml_input_src.hpp> <srcml_pipe.hpp> <ParseQueue.hpp> <src_input_libarchive.hpp> <SRCMLStatus.hpp> <WriteQueue.hpp> <ParseRequest.hpp> <TraceLog.hpp> <curl/curl.h>)
endif()

if(APPLE)
    # Making the exported_symbols_list an empty file reduces size of executable, as strip does not work
    set_target_properties(srcml PROPERTIES LINK_FLAGS "-exported_symbols_list /dev/null")
endif()

# Dependencies are installed via libsrcml
install(TARGETS srcml COMPONENT SRCML)

target_compile_options(srcml PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/wd4355> # this': used in base member initializer list
    $<$<CXX_COMPILER_ID:MSVC>:/wd5204> # class has virtual functions, but its trivial destructor is not virtual
)

if(MSVC)
    set_source_files_properties(srcml_cli.cpp PROPERTIES COMPILE_FLAGS /wd4866)
endif()
