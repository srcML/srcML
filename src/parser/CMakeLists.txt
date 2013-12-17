#
#    CMakeLists.txt
# @copyright
# 
# Copyright (C) 2013  SDML (www.srcML.org)
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
set(CommentTextLexerOutputFiles ${CMAKE_CURRENT_SOURCE_DIR}/CommentTextLexer.cpp;${CMAKE_CURRENT_SOURCE_DIR}/CommentTextLexer.hpp;${CMAKE_CURRENT_SOURCE_DIR}/CommentTextLexerTokenTypes.hpp;${CMAKE_CURRENT_SOURCE_DIR}/CommentTextLexerTokenTypes.txt;${CMAKE_CURRENT_SOURCE_DIR}/expandedCommentTextLexer.g
)
RunAntlr("${CommentTextLexerOutputFiles}" ${CMAKE_CURRENT_SOURCE_DIR}/CommentTextLexer.g "" "")

# Running ANTLR on TextLexer.g
set(TextLexerOutputFiles
    ${CMAKE_CURRENT_SOURCE_DIR}/TextLexer.cpp;${CMAKE_CURRENT_SOURCE_DIR}/TextLexer.hpp;${CMAKE_CURRENT_SOURCE_DIR}/TextLexerTokenTypes.hpp;${CMAKE_CURRENT_SOURCE_DIR}/TextLexerTokenTypes.txt;${CMAKE_CURRENT_SOURCE_DIR}/expandedTextLexer.g
)

RunAntlr("${TextLexerOutputFiles}" ${CMAKE_CURRENT_SOURCE_DIR}/TextLexer.g ${CMAKE_CURRENT_SOURCE_DIR}/CommentTextLexer.cpp "")

# Running ANTLR on OperatorLexer.g
set(OperatorLexerOutputFiles
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorLexer.cpp;${CMAKE_CURRENT_SOURCE_DIR}/OperatorLexer.hpp;${CMAKE_CURRENT_SOURCE_DIR}/OperatorLexerTokenTypes.hpp;${CMAKE_CURRENT_SOURCE_DIR}/OperatorLexerTokenTypes.txt;${CMAKE_CURRENT_SOURCE_DIR}/expandedOperatorLexer.g
)
RunAntlr("${OperatorLexerOutputFiles}"
  ${CMAKE_CURRENT_SOURCE_DIR}/OperatorLexer.g
  ${CMAKE_CURRENT_SOURCE_DIR}/TextLexer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TextLexer.g
)

# Running ANTLR on KeywordLexer.g
set(KeywordLexerOutputFiles
    ${CMAKE_CURRENT_SOURCE_DIR}/KeywordLexer.cpp;${CMAKE_CURRENT_SOURCE_DIR}/KeywordLexer.hpp;${CMAKE_CURRENT_SOURCE_DIR}/KeywordLexerTokenTypes.hpp;${CMAKE_CURRENT_SOURCE_DIR}/KeywordLexerTokenTypes.txt;${CMAKE_CURRENT_SOURCE_DIR}/expandedKeywordLexer.g
)
RunAntlr("${KeywordLexerOutputFiles}"
  ${CMAKE_CURRENT_SOURCE_DIR}/KeywordLexer.g
  ${CMAKE_CURRENT_SOURCE_DIR}/OperatorLexer.cpp
  "${CMAKE_CURRENT_SOURCE_DIR}/OperatorLexer.g\;${CMAKE_CURRENT_SOURCE_DIR}/TextLexer.g"
)

# Running ANTLR on srcMLParser.g
set(SrcMLParserOutputFiles
    ${CMAKE_CURRENT_SOURCE_DIR}/srcMLParser.cpp;${CMAKE_CURRENT_SOURCE_DIR}/srcMLParser.hpp;${CMAKE_CURRENT_SOURCE_DIR}/srcMLParserTokenTypes.hpp;${CMAKE_CURRENT_SOURCE_DIR}/srcMLParserTokenTypes.txt	
)
RunAntlr("${SrcMLParserOutputFiles}"
  ${CMAKE_CURRENT_SOURCE_DIR}/srcMLParser.g
  "${CMAKE_CURRENT_SOURCE_DIR}/srcMLParser.g;${CMAKE_CURRENT_SOURCE_DIR}/KeywordLexer.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/OperatorLexer.g\;${CMAKE_CURRENT_SOURCE_DIR}/KeywordLexer.g\;${CMAKE_CURRENT_SOURCE_DIR}/TextLexer.g"
)

# Running SED/GREP macro for token extraction.
get_property(SED_EXE GLOBAL PROPERTY SED_EXE)
get_property(GREP_EXE GLOBAL PROPERTY GREP_EXE)
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/srcMLParserTokenTypesMacro.hpp
    COMMAND ${GREP_EXE} '=' ${CMAKE_CURRENT_SOURCE_DIR}/srcMLParserTokenTypes.hpp | ${SED_EXE} -e 's/,//g' -e 's/= //g' -e 's/^[ \t]*//' -e "s/^/\#define TOKEN_/g" > ${CMAKE_CURRENT_SOURCE_DIR}/srcMLParserTokenTypesMacro.hpp
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/srcMLParserTokenTypes.hpp
)

list(REMOVE_ITEM PARSER_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/srcMLParserTokenTypesMacro.hpp
    ${SrcMLParserOutputFiles}
    ${OperatorLexerOutputFiles}
    ${TextLexerOutputFiles}
    ${KeywordLexerOutputFiles}
    ${CommentTextLexerOutputFiles}
)
list(REMOVE_ITEM PARSER_SOURCE
    ${SrcMLParserOutputFiles}
    ${OperatorLexerOutputFiles}
    ${TextLexerOutputFiles}
    ${KeywordLexerOutputFiles}
    ${CommentTextLexerOutputFiles}
)

get_property(SVN_ENABLED GLOBAL PROPERTY SVN_ENABLED)
if(NOT ${SVN_ENABLED})
    list(REMOVE_ITEM PARSER_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/svn_io.cpp)
endif()

add_library(parser OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/srcMLParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/KeywordLexer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CommentTextLexer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/srcMLParserTokenTypesMacro.hpp
    ${PARSER_HEADERS}
    ${PARSER_SOURCE}
)
