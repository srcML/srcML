cmake_minimum_required(VERSION 2.8)
project(srcML)

execute_process(COMMAND cat VERSION COMMAND tr -d "\n" OUTPUT_VARIABLE VERSION)
execute_process(COMMAND cat REVISION COMMAND tr -d "\n" OUTPUT_VARIABLE REVISION)
add_definitions(-DVERSION=\"${VERSION}\")
add_definitions(-DREVISION=\"${REVISION}\")

find_package(LibXml2)
find_package(LibArchive)
find_package(LibXml2)
include_directories(${LIBXML2_INCLUDE_DIR})

add_custom_command(OUTPUT TextLexer.cpp COMMAND antlr2 -glib \"WSLexer.g\;CommentLexer.g\" TextLexer.g  DEPENDS TextLexer.g WSLexer.cpp CommentLexer.cpp)
add_custom_command(OUTPUT WSLexer.cpp COMMAND antlr2 -glib \"PureCommentLexer.g\" WSLexer.g DEPENDS WSLexer.g PureCommentLexer.cpp)
add_custom_command(OUTPUT CommentLexer.cpp COMMAND antlr2 -glib \"WSLexer.g\" CommentLexer.g DEPENDS CommentLexer.g WSLexer.cpp)
add_custom_command(OUTPUT OperatorLexer.cpp COMMAND antlr2 -glib \"WSLexer.g\;CommentLexer.g\;TextLexer.g\" OperatorLexer.g DEPENDS OperatorLexer.g CommentLexer.cpp TextLexer.cpp)
add_custom_command(OUTPUT KeywordLexer.cpp COMMAND antlr2 -glib \"WSLexer.g\;CommentLexer.g\;OperatorLexer.g\;TextLexer.g\" KeywordLexer.g DEPENDS KeywordLexer.g OperatorLexer.cpp)
add_custom_command(OUTPUT KeywordCPPLexer.cpp COMMAND antlr2 -glib \"WSLexer.g\;CommentLexer.g\;OperatorLexer.g\;TextLexer.g\;KeywordLexer.g\" KeywordCPPLexer.g DEPENDS KeywordCPPLexer.g KeywordLexer.cpp)
add_custom_command(OUTPUT PureCommentLexer.cpp COMMAND antlr2 PureCommentLexer.g DEPENDS PureCommentLexer.g)
add_custom_command(OUTPUT srcMLParser.cpp COMMAND antlr2 -glib \"WSLexer.g\;CommentLexer.g\;OperatorLexer.g\;TextLexer.g\;KeywordLexer.g\;KeywordCPPLexer.g\" srcMLParser.g DEPENDS srcMLParser.g KeywordCPPLexer.cpp)

add_custom_command(OUTPUT srcMLParserTokenTypesMacro.hpp COMMAND /usr/bin/grep '=' srcMLParserTokenTypes.hpp | /usr/local/bin/gsed -e 's/,//g' -e 's/= //g' -e 's/^[ \t]*//' -e 's/^/\#define TOKEN_/g' > srcMLParserTokenTypesMacro.hpp DEPENDS srcMLParser.cpp)

add_library(srcml srcMLParser.cpp KeywordCPPLexer.cpp srcMLTranslator.cpp srcMLTranslatorCore.cpp srcMLOutput.cpp srcMLTranslatorOutput.cpp Mode.cpp UTF8CharBuffer.cpp PureCommentLexer.cpp Language.cpp URIStream.cpp srcmlapps.cpp libxml_archive_read.cpp libxml_archive_write.cpp srcMLParserTokenTypesMacro.hpp)

add_executable(src2srcml src2srcml.cpp srcmlapps.cpp makeargv.cpp)
target_link_libraries(src2srcml archive xml2 antlr srcml)
add_custom_target(srcMLParserTokenTypesMacro.hpp DEPENDS srcMLParser.cpp)

add_executable(srcml2src srcml2src.cpp srcMLUtility.cpp SAX2ExtractUnitsSrc.cpp srcmlapps.cpp SAX2Utilities.cpp SAX2UnitDOM.cpp SAX2UnitDOMRelaxNG.cpp srcexfun.cpp libxml_archive_read.cpp libxml_archive_write.cpp URIStream.cpp makeargv.cpp)
target_link_libraries(srcml2src archive xml2 xslt exslt)
set_target_properties(src2srcml srcml2src PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin)
