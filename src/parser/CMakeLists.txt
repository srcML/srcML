# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)
# 
# CMake files for building the parser

# parser debugging
option(SRCML_DEBUG_PARSER "Enable parser debugging output" OFF)

# TinySHA1 external include
include(FetchContent)
FetchContent_Declare(TinySHA1
  GIT_REPOSITORY https://github.com/mohaps/tinysha1
  GIT_TAG        2795aa8de91b1797defdfbff61ed93b22b5ced81
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(TinySHA1)
add_library(TinySHA1 INTERFACE IMPORTED)
target_include_directories(TinySHA1 INTERFACE ${tinysha1_SOURCE_DIR})

find_package(LibXml2 REQUIRED)
find_package(Iconv REQUIRED)

# Enable proper exception catching when building for wasm
if(${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    add_compile_options(-fwasm-exceptions)
endif()

# Generated source from antlr and the parser
set(CMAKE_GENERATED_SOURCE_DIR ${CMAKE_BINARY_DIR}/parser)

# Embed the srcML CMake options list into the parser as one string literal per line
# Reconfigure when the list changes
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/cmake_options.csv)
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/cmake_options.csv CMAKE_OPTIONS_LINES)
string(REPLACE "\r" "" CMAKE_OPTIONS_LINES "${CMAKE_OPTIONS_LINES}")
string(STRIP "${CMAKE_OPTIONS_LINES}" CMAKE_OPTIONS_LINES)
string(REPLACE "\\" "\\\\" CMAKE_OPTIONS_LINES "${CMAKE_OPTIONS_LINES}")
string(REPLACE "\"" "\\\"" CMAKE_OPTIONS_LINES "${CMAKE_OPTIONS_LINES}")
string(REPLACE "\n" "\",\n    \"" CMAKE_OPTIONS_LINES "${CMAKE_OPTIONS_LINES}")
set(CMAKE_OPTIONS_LINES "    \"${CMAKE_OPTIONS_LINES}\",")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/OptionsConfig.h.in ${CMAKE_GENERATED_SOURCE_DIR}/OptionsConfig.h @ONLY)

# ANTLR tool replacement
set(ANTLR_EXE antlrcpp)

# parser library
file(GLOB PARSER_SOURCE *.hpp *.cpp)
add_library(parser OBJECT
    ${CMAKE_GENERATED_SOURCE_DIR}/srcMLParser.cpp
    ${CMAKE_GENERATED_SOURCE_DIR}/KeywordLexer.cpp
    ${CMAKE_GENERATED_SOURCE_DIR}/CommentTextLexer.cpp
    ${PARSER_SOURCE}
)
set_target_properties(parser PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(parser PRIVATE . ${CMAKE_GENERATED_SOURCE_DIR} PUBLIC . SYSTEM PUBLIC ${CMAKE_GENERATED_SOURCE_DIR})
target_link_libraries(parser PRIVATE LibXml2::LibXml2 PUBLIC antlr_runtime)
target_include_directories(parser PRIVATE $<TARGET_PROPERTY:TinySHA1,INTERFACE_INCLUDE_DIRECTORIES>)
if(SRCML_DEBUG_PARSER)
    set_source_files_properties(${CMAKE_GENERATED_SOURCE_DIR}/srcMLParser.cpp PROPERTIES COMPILE_DEFINITIONS SRCML_DEBUG_PARSER)
endif()

# PreCompiled Headers configuration of the parser
if (SRCML_PARSER_PCH)
    target_precompile_headers(parser PRIVATE "${SRCML_PARSER_PCH}")
endif()

target_compile_options(parser PRIVATE ${SRCML_PARSER_DISABLED_WARNINGS})

# 
# RunAntlr
# Executes ANTLR executable on the provided files.
# 
# Takes 3 parameters OUTPUT_FILES INPUT_FILES DEPENDENCIES and 
#    1 optional parameter INCLUDE_GRAMMAR
# Use with named arguments.
#
function(RunAntlr OUTPUT_FILES INPUT_FILES DEPENDENCIES INCLUDE_GRAMMAR)

    add_custom_command(OUTPUT  ${OUTPUT_FILES}
        DEPENDS ${INPUT_FILES} ${DEPENDENCIES} ${ANTLR_EXE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${ANTLR_EXE} -o \"${CMAKE_GENERATED_SOURCE_DIR}\" -glib "\"${INCLUDE_GRAMMAR}\"" ${INPUT_FILES}
        COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT_FILES}
    )

endfunction(RunAntlr)

macro(lexerFiles LEXER)
    set(${LEXER}LexerOutputFiles
        ${CMAKE_GENERATED_SOURCE_DIR}/${LEXER}Lexer.cpp
        ${CMAKE_GENERATED_SOURCE_DIR}/${LEXER}Lexer.hpp
        ${CMAKE_GENERATED_SOURCE_DIR}/${LEXER}LexerTokenTypes.hpp
        ${CMAKE_GENERATED_SOURCE_DIR}/${LEXER}LexerTokenTypes.txt
        ${CMAKE_GENERATED_SOURCE_DIR}/expanded${LEXER}Lexer.g
    )
endmacro()

# Running ANTLR on CommentTextLexer.g
lexerFiles(CommentText)
RunAntlr("${CommentTextLexerOutputFiles}"
    CommentTextLexer.g
    ""
    ""
)

# Running ANTLR on TextLexer.g
lexerFiles(Text)
RunAntlr("${TextLexerOutputFiles}"
    TextLexer.g
    ${CMAKE_GENERATED_SOURCE_DIR}/CommentTextLexer.cpp
    ""
)

# Running ANTLR on OperatorLexer.g
lexerFiles(Operator)
RunAntlr("${OperatorLexerOutputFiles}"
    OperatorLexer.g
    ${CMAKE_GENERATED_SOURCE_DIR}/TextLexer.cpp
    TextLexer.g
)

# Running ANTLR on KeywordLexer.g
lexerFiles(Keyword)
RunAntlr("${KeywordLexerOutputFiles}"
    KeywordLexer.g
    ${CMAKE_GENERATED_SOURCE_DIR}/OperatorLexer.cpp
    "OperatorLexer.g;TextLexer.g"
)

# Running ANTLR on srcMLParser.g
set(srcMLParserOutputFiles
    ${CMAKE_GENERATED_SOURCE_DIR}/srcMLParser.cpp
    ${CMAKE_GENERATED_SOURCE_DIR}/srcMLParser.hpp
    ${CMAKE_GENERATED_SOURCE_DIR}/srcMLParserTokenTypes.hpp
    ${CMAKE_GENERATED_SOURCE_DIR}/srcMLParserTokenTypes.txt 
)
RunAntlr("${srcMLParserOutputFiles}"
    srcMLParser.g
    "srcMLParser.g;${CMAKE_GENERATED_SOURCE_DIR}/KeywordLexer.cpp"
    "OperatorLexer.g;KeywordLexer.g;TextLexer.g"
)
