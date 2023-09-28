# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
# 
# CMake files for building the parser

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
find_package(Java COMPONENTS Runtime REQUIRED)

# Setup ANTLR 2.7 library
include(FetchContent)
FetchContent_Declare(antlrsrc
    URL https://www.antlr2.org/download/antlr-2.7.7.tar.gz
    URL_HASH MD5=01cc9a2a454dd33dcd8c856ec89af090
)
FetchContent_GetProperties(antlrsrc)
if(NOT antlrsrc_POPULATED)
    # Fetch the content using previously declared details
    FetchContent_Populate(antlrsrc)

    if (NOT antlrsrc_PATCHED)
        # Add needed include file for CharScanner.hpp
        # Comment out use of std::binary_function, deprecated in C++11 and removed in C++17
        set(CHARSCANNER_HPP "${antlrsrc_SOURCE_DIR}/lib/cpp/antlr/CharScanner.hpp")
        file(READ ${CHARSCANNER_HPP} FILE_CONTENTS)
        string(REPLACE "#include <antlr/config.hpp>\n\n" "#include <antlr/config.hpp>\n#include <string.h>\n\n" FILE_CONTENTS "${FILE_CONTENTS}")
        string(REPLACE "class ANTLR_API CharScannerLiteralsLess : public ANTLR_USE_NAMESPACE(std)binary_function<ANTLR_USE_NAMESPACE(std)string,ANTLR_USE_NAMESPACE(std)string,bool>"
                    "class ANTLR_API CharScannerLiteralsLess /* : public ANTLR_USE_NAMESPACE(std)binary_function<ANTLR_USE_NAMESPACE(std)string,ANTLR_USE_NAMESPACE(std)string,bool> */" FILE_CONTENTS "${FILE_CONTENTS}")
        file(WRITE ${CHARSCANNER_HPP} "${FILE_CONTENTS}")

        # Remove pragma warning disable that is no longer valid
        set(CONFIG_HPP "${antlrsrc_SOURCE_DIR}/lib/cpp/antlr/config.hpp")
        file(READ ${CONFIG_HPP} FILE_CONTENTS)
        string(REPLACE "# pragma warning( disable : 4786 4231 )" "# pragma warning( disable : 4786 )" FILE_CONTENTS "${FILE_CONTENTS}")
        file(WRITE ${CONFIG_HPP} "${FILE_CONTENTS}")

        # Mark so that further patches are not performed
        set(antlrsrc_PATCHED TRUE CACHE BOOL "Indicates that Antlr source was patched" FORCE)
    endif()
endif()

# antlr library
add_library(antlr OBJECT)
file(GLOB ANTLR_SOURCE ${antlrsrc_SOURCE_DIR}/lib/cpp/src/*.cpp)
list(REMOVE_ITEM ANTLR_SOURCE "${antlrsrc_SOURCE_DIR}/lib/cpp/src/dll.cpp")
target_sources(antlr PRIVATE "${ANTLR_SOURCE}")
target_include_directories(antlr SYSTEM PUBLIC "${antlrsrc_SOURCE_DIR}/lib/cpp/" SYSTEM PRIVATE "${antlrsrc_SOURCE_DIR}/lib/cpp/")

# Generated source from antlr and the parser
set(CMAKE_GENERATED_SOURCE_DIR ${CMAKE_BINARY_DIR}/parser)

# Find antlr runtime
set(ANTLR_EXE ${Java_JAVA_EXECUTABLE} -classpath "${antlrsrc_SOURCE_DIR}/antlr.jar" antlr.Tool)
message("-- ANTLR: ${ANTLR_EXE}")

# parser library
file(GLOB PARSER_SOURCE *.hpp *.cpp)
add_library(parser OBJECT
    ${CMAKE_GENERATED_SOURCE_DIR}/srcMLParser.cpp
    ${CMAKE_GENERATED_SOURCE_DIR}/KeywordLexer.cpp
    ${CMAKE_GENERATED_SOURCE_DIR}/CommentTextLexer.cpp
    ${PARSER_SOURCE}
)
target_include_directories(parser SYSTEM PUBLIC ${antlrsrc_SOURCE_DIR}/lib/cpp)
target_include_directories(parser PRIVATE . ${CMAKE_GENERATED_SOURCE_DIR} PUBLIC . SYSTEM PUBLIC ${CMAKE_GENERATED_SOURCE_DIR})
target_link_libraries(parser PRIVATE LibXml2::LibXml2 antlr)
target_include_directories(parser PRIVATE $<TARGET_PROPERTY:TinySHA1,INTERFACE_INCLUDE_DIRECTORIES>)

# PreCompiled Headers configuration of the parser
if (SRCML_PARSER_PCH)
    target_precompile_headers(parser PRIVATE "${SRCML_PARSER_PCH}")
endif()

target_compile_options(antlr PRIVATE ${SRCML_ANTLR_DISABLED_WARNINGS})
target_compile_options(parser PRIVATE ${SRCML_PARSER_DISABLED_WARNINGS})

# 
# RunAntlr
# Executes ANTLR executable on the provided files.
# 
# Takes 3 parameters OUTPUT_FILES INPUT_FILES DEPENDENCIES and 
#    1 optional parameter INCLUDE_GRAMMAR
# Use with named arguments.
#
macro(RunAntlr OUTPUT_FILES INPUT_FILES DEPENDENCIES INCLUDE_GRAMMAR)

    add_custom_command(OUTPUT  ${OUTPUT_FILES}
        DEPENDS ${INPUT_FILES} ${DEPENDENCIES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${ANTLR_EXE} -o \"${CMAKE_GENERATED_SOURCE_DIR}\" -glib \"${INCLUDE_GRAMMAR}\" ${INPUT_FILES}
        COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT_FILES}
    )

endmacro(RunAntlr)

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
    "OperatorLexer.g\;TextLexer.g"
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
    "OperatorLexer.g\;KeywordLexer.g\;TextLexer.g"
)
