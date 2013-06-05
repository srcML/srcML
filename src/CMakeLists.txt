###
#    CMakeLists.txt
#
#
#    Optional cmake file for srcML.
#    The main way to build srcML is with normal Makefile

cmake_minimum_required(VERSION 2.8)
project(srcML)

# get version and revision
execute_process(COMMAND cat VERSION COMMAND tr -d "\n" OUTPUT_VARIABLE VERSION)
execute_process(COMMAND cat REVISION COMMAND tr -d "\n" OUTPUT_VARIABLE REVISION)
add_definitions(-DVERSION=\"${VERSION}\")
add_definitions(-DREVISION=\"${REVISION}\")

# get needed packages and include
find_package(LibXml2)
find_package(LibArchive)
find_package(LibXslt)
include_directories(${LIBXML2_INCLUDE_DIR} ${LibArchive_INCLUDE_DIRS} ${LIBXSLT_INCLUDE_DIR})

# define needed programs
find_program(ANTLR NAMES antlr runantlr cantlr antlr2 PATHS /usr/bin /opt/local/bin /usr/local/bin)
find_program(SED NAMES gsed sed PATHS /opt/local/bin /usr/local /bin)
find_program(GREP grep PATHS /bin /usr/bin)
find_program(ECHO echo PATH /bin)
find_program(CUT cut PATH /usr/bin)
find_program(SVNVERSION svnversion PATHS /usr/local/bin /usr/bin)

# Run antlr
add_custom_command(OUTPUT TextLexer.cpp TextLexer.hpp TextLexerTokenTypes.txt TextLexerTokenTypes.hpp COMMAND ${ANTLR} -glib \"WSLexer.g\;CommentLexer.g\" TextLexer.g  DEPENDS TextLexer.g WSLexer.cpp CommentLexer.cpp)
add_custom_command(OUTPUT WSLexer.cpp WSLexer.hpp WSLexerTokenTypes.txt WSLexerTokenTypes.hpp COMMAND ${ANTLR} -glib \"PureCommentLexer.g\" WSLexer.g DEPENDS WSLexer.g PureCommentLexer.cpp)
add_custom_command(OUTPUT CommentLexer.cpp CommentLexer.hpp CommentLexerTokenTypes.txt CommentLexerTokenTypes.hpp COMMAND ${ANTLR} -glib \"WSLexer.g\" CommentLexer.g DEPENDS CommentLexer.g WSLexer.cpp)
add_custom_command(OUTPUT OperatorLexer.cpp OperatorLexer.hpp OperatorLexerTokenTypes.txt OperatorLexerTokenTypes.hpp COMMAND ${ANTLR} -glib \"WSLexer.g\;CommentLexer.g\;TextLexer.g\" OperatorLexer.g DEPENDS OperatorLexer.g CommentLexer.cpp TextLexer.cpp)
add_custom_command(OUTPUT KeywordLexer.cpp KeywordLexer.hpp KeywordLexerTokenTypes.txt KeywordLexerTokenTypes.hpp COMMAND ${ANTLR} -glib \"WSLexer.g\;CommentLexer.g\;OperatorLexer.g\;TextLexer.g\" KeywordLexer.g DEPENDS KeywordLexer.g OperatorLexer.cpp)
add_custom_command(OUTPUT KeywordCPPLexer.cpp KeywordCPPLexer.hpp KeywordCPPLexerTokenTypes.txt KeywordCPPLexerTokenTypes.hpp COMMAND ${ANTLR} -glib \"WSLexer.g\;CommentLexer.g\;OperatorLexer.g\;TextLexer.g\;KeywordLexer.g\" KeywordCPPLexer.g DEPENDS KeywordCPPLexer.g KeywordLexer.cpp)
add_custom_command(OUTPUT PureCommentLexer.cpp PureCommentLexer.hpp PureCommentLexerTokenTypes.txt PureCommentLexerTokenTypes.hpp COMMAND ${ANTLR} PureCommentLexer.g DEPENDS PureCommentLexer.g)
add_custom_command(OUTPUT srcMLParser.cpp srcMLParser.hpp srcMLParserTokenTypes.txt srcMLParserTokenTypes.hpp COMMAND ${ANTLR} -glib \"WSLexer.g\;CommentLexer.g\;OperatorLexer.g\;TextLexer.g\;KeywordLexer.g\;KeywordCPPLexer.g\" srcMLParser.g DEPENDS srcMLParser.g KeywordCPPLexer.cpp)

# Run special command for macro
add_custom_command(OUTPUT srcMLParserTokenTypesMacro.hpp COMMAND ${GREP} '=' srcMLParserTokenTypes.hpp | ${SED} -e 's/,//g' -e 's/= //g' -e 's/^[ \t]*//' -e 's/^/\#define TOKEN_/g' > srcMLParserTokenTypesMacro.hpp DEPENDS srcMLParserTokenTypes.hpp)

# libsrcml
add_library(srcml srcMLParser.cpp KeywordCPPLexer.cpp srcMLTranslator.cpp srcMLTranslatorCore.cpp srcMLOutput.cpp srcMLTranslatorOutput.cpp Mode.cpp UTF8CharBuffer.cpp PureCommentLexer.cpp Language.cpp URIStream.cpp srcmlapps.cpp libxml_archive_read.cpp libxml_archive_write.cpp srcMLParserTokenTypesMacro.hpp)
target_link_libraries(srcml archive xml2 antlr)
set_target_properties(srcml PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ../bin)

# src2srcml
add_executable(src2srcml src2srcml.cpp srcmlapps.cpp makeargv.cpp)
target_link_libraries(src2srcml archive xml2 antlr srcml)
add_custom_target(srcMLParserTokenTypesMacro.hpp DEPENDS srcMLParserTokenTypes.hpp)

# srcml2src
add_executable(srcml2src srcml2src.cpp srcMLUtility.cpp SAX2ExtractUnitsSrc.cpp srcmlapps.cpp SAX2Utilities.cpp SAX2UnitDOM.cpp SAX2UnitDOMRelaxNG.cpp srcexfun.cpp libxml_archive_read.cpp libxml_archive_write.cpp URIStream.cpp makeargv.cpp)
target_link_libraries(srcml2src archive xml2 xslt exslt)

set_target_properties(src2srcml srcml2src PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin)

