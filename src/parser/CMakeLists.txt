#
#    CMakeLists.txt
# @copyright
# 
# Copyright (C) 2013-2014  SDML (www.srcML.org)
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
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

file(GLOB PARSER_HEADERS *.hpp)
file(GLOB PARSER_SOURCE *.cpp)

# Running ANTLR on CommentTextLexer.g
set(CommentTextLexerOutputFiles
    CommentTextLexer.cpp
    CommentTextLexer.hpp
    CommentTextLexerTokenTypes.hpp
    CommentTextLexerTokenTypes.txt
    expandedCommentTextLexer.g
)
RunAntlr("${CommentTextLexerOutputFiles}" CommentTextLexer.g "" "")

# Running ANTLR on TextLexer.g
set(TextLexerOutputFiles
    TextLexer.cpp
    TextLexer.hpp
    TextLexerTokenTypes.hpp
    TextLexerTokenTypes.txt
    expandedTextLexer.g
)
RunAntlr("${TextLexerOutputFiles}" TextLexer.g CommentTextLexer.cpp "")

# Running ANTLR on OperatorLexer.g
set(OperatorLexerOutputFiles
    OperatorLexer.cpp
    OperatorLexer.hpp
    OperatorLexerTokenTypes.hpp
    OperatorLexerTokenTypes.txt
    expandedOperatorLexer.g
)
RunAntlr("${OperatorLexerOutputFiles}"
    OperatorLexer.g
    TextLexer.cpp
    TextLexer.g
)

# Running ANTLR on KeywordLexer.g
set(KeywordLexerOutputFiles
    KeywordLexer.cpp
    KeywordLexer.hpp
    KeywordLexerTokenTypes.hpp
    KeywordLexerTokenTypes.txt
    expandedKeywordLexer.g
)

RunAntlr("${KeywordLexerOutputFiles}"
    KeywordLexer.g
    OperatorLexer.cpp
    "OperatorLexer.g\;TextLexer.g"
)

# Running ANTLR on srcMLParser.g
set(srcMLParserOutputFiles
    srcMLParser.cpp
    srcMLParser.hpp
    srcMLParserTokenTypes.hpp
    srcMLParserTokenTypes.txt	
)
RunAntlr("${srcMLParserOutputFiles}"
    srcMLParser.g
    "srcMLParser.g;KeywordLexer.cpp"
    "OperatorLexer.g\;KeywordLexer.g\;TextLexer.g"
)

# Running SED/GREP macro for token extraction.
get_property(SED_EXE GLOBAL PROPERTY SED_EXE)
get_property(GREP_EXE GLOBAL PROPERTY GREP_EXE)

if(WIN32)
    get_property(WINDOWS_DEP_PATH GLOBAL PROPERTY WINDOWS_DEP_PATH)
    add_custom_command(
        OUTPUT srcMLParserTokenTypesMacro.hpp
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS srcMLParserTokenTypes.hpp
        COMMAND ${GREP_EXE} -e "=" srcMLParserTokenTypes.hpp > PhaseOne.txt
        COMMAND ${SED_EXE} -e "s/,//g" -e "s/= //g" -e "s/^[ \\t]*//" -e "s/^/\#define TOKEN_/g" PhaseOne.txt > srcMLParserTokenTypesMacro.hpp 
    )
else()
    add_custom_command(
        OUTPUT srcMLParserTokenTypesMacro.hpp
        DEPENDS srcMLParserTokenTypes.hpp
        COMMAND ${GREP_EXE} '=' ${CMAKE_CURRENT_SOURCE_DIR}/srcMLParserTokenTypes.hpp | ${SED_EXE} -e 's/,//g' -e 's/= //g' -e 's/^[ \t]*//' -e "s/^/\#define TOKEN_/g" > ${CMAKE_CURRENT_SOURCE_DIR}/srcMLParserTokenTypesMacro.hpp
    )
endif()


set(updatedRemoveList "")
foreach(itemToRemove ${KeywordLexerOutputFiles} ${srcMLParserOutputFiles} ${OperatorLexerOutputFiles} ${TextLexerOutputFiles} ${CommentTextLexerOutputFiles} srcMLParserTokenTypesMacro.hpp)
    get_filename_component(absPath ${itemToRemove} ABSOLUTE)
    list(APPEND updatedRemoveList ${absPath})
endforeach()


# Removing generated files from file list.
list(REMOVE_ITEM PARSER_HEADERS ${updatedRemoveList})
list(REMOVE_ITEM PARSER_SOURCE ${updatedRemoveList})

get_property(SVN_ENABLED GLOBAL PROPERTY SVN_ENABLED)
if(NOT ${SVN_ENABLED})
    list(REMOVE_ITEM PARSER_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/svn_io.cpp)
endif()

add_library(parser OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/srcMLParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/KeywordLexer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CommentTextLexer.cpp
    srcMLParserTokenTypesMacro.hpp
    ${PARSER_HEADERS}
    ${PARSER_SOURCE}
)
