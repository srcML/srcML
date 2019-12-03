##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
# 
# The srcML Toolkit is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# The srcML Toolkit is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with the srcML Toolkit; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
# 
# CMake files for building the parser

# Find antlr binary
find_program(ANTLR_EXE NAMES antlr runantlr cantlr antlr2 antlr.bat PATHS /usr/bin /opt/local/bin /usr/local/bin ${WINDOWS_DEP_PATH}/tools/antlr/277/bin)

# Assume all C++ source files are parser
file(GLOB PARSER_SOURCE *.hpp *.cpp)

# 
# RunAntlr
# Executes ANTLR executable on the provided files.
# 
# Takes 3 parameters OUTPUT_FILES INPUT_FILES DEPENDENCIES and 
#    1 optional parameter INCLUDE_GRAMMAR
# Use with named arguments.
#
macro(RunAntlr OUTPUT_FILES INPUT_FILES DEPENDENCIES INCLUDE_GRAMMAR)

    if("${INCLUDE_GRAMMAR}" STREQUAL "")
        if(WIN32)
            # Handling windows relative path problem (Not sure why this is an issue but it's given me a headache).
            add_custom_command(OUTPUT  ${OUTPUT_FILES}
                DEPENDS ${INPUT_FILES} ${DEPENDENCIES}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                COMMAND ${ANTLR_EXE} -o \"${CMAKE_GENERATED_SOURCE_DIR}\" ${INPUT_FILES}
            )
        else()
            add_custom_command(OUTPUT  ${OUTPUT_FILES}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                COMMAND ${ANTLR_EXE} -o \"${CMAKE_GENERATED_SOURCE_DIR}\" ${INPUT_FILES} DEPENDS ${INPUT_FILES} ${DEPENDENCIES}
                COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT_FILES}
            )
        endif()
    else()
        if(WIN32)
            add_custom_command(OUTPUT  ${OUTPUT_FILES}
                DEPENDS ${INPUT_FILES} ${DEPENDENCIES}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                COMMAND ${ANTLR_EXE} -o \"${CMAKE_GENERATED_SOURCE_DIR}\" -glib \"${INCLUDE_GRAMMAR}\" ${INPUT_FILES}
            )
        else()
            add_custom_command(OUTPUT  ${OUTPUT_FILES}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                COMMAND ${ANTLR_EXE} -o \"${CMAKE_GENERATED_SOURCE_DIR}\" -glib \"${INCLUDE_GRAMMAR}\" ${INPUT_FILES} DEPENDS ${INPUT_FILES} ${DEPENDENCIES}
                COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT_FILES}
            )
        endif()
    endif()
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

add_library(parser OBJECT
    ${CMAKE_GENERATED_SOURCE_DIR}/srcMLParser.cpp
    ${CMAKE_GENERATED_SOURCE_DIR}/KeywordLexer.cpp
    ${CMAKE_GENERATED_SOURCE_DIR}/CommentTextLexer.cpp
    ${PARSER_SOURCE}
)

target_include_directories(parser PUBLIC . ${CMAKE_SOURCE_DIR}/src/libsrcml ${CMAKE_GENERATED_SOURCE_DIR})
