# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
#
# @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
#
# CMake files for antlrcpp, the replacement for the ANTLR 2.7.7 tool

# standalone project, for building the tool natively when cross compiling
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    cmake_minimum_required(VERSION 3.28)
    project(antlrcpp CXX)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
else()
    # header-only runtime for the generated lexers and parser
    add_library(antlr_runtime INTERFACE)
    target_include_directories(antlr_runtime SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# the tool runs during the build, so for WASM build it natively for the host
if(EMSCRIPTEN)
    include(ExternalProject)
    set(ANTLRCPP_HOST ${CMAKE_CURRENT_BINARY_DIR}/host/antlrcpp${CMAKE_HOST_EXECUTABLE_SUFFIX})
    ExternalProject_Add(antlrcpp_host
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
        BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/host
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release
        INSTALL_COMMAND ""
        BUILD_ALWAYS ON
        BUILD_BYPRODUCTS ${ANTLRCPP_HOST}
    )
    add_executable(antlrcpp IMPORTED GLOBAL)
    set_target_properties(antlrcpp PROPERTIES IMPORTED_LOCATION ${ANTLRCPP_HOST})
    add_dependencies(antlrcpp antlrcpp_host)
    return()
endif()

add_executable(antlrcpp
    main.cpp
    Tool.cpp
    Preprocessor.cpp
    ANTLRParser.cpp
    Tokdef.cpp
    Grammar.cpp
    MakeGrammar.cpp
    LLkAnalyzer.cpp
    ActionLexer.cpp
    CppCodeGenerator.cpp
)
