# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
#
# @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
#
# CMake files for antlrcpp, the replacement for the ANTLR 2.7.7 tool

# header-only runtime for the generated lexers and parser
add_library(antlr_runtime INTERFACE)
target_include_directories(antlr_runtime SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

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
