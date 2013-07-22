###
#    CMakeLists.txt
#
#
#    Optional cmake file for srcML.
#    The main way to build srcML is with normal Makefile

cmake_minimum_required(VERSION 2.8)
project(srcML)

# specify g++ compiler
include(CMakeForceCompiler)
CMAKE_FORCE_CXX_COMPILER(/usr/bin/g++ "g++")

# get version and revision
execute_process(COMMAND cat VERSION COMMAND tr -d "\n" OUTPUT_VARIABLE VERSION)
execute_process(COMMAND cat REVISION COMMAND tr -d "\n" OUTPUT_VARIABLE REVISION)
add_definitions(-DVERSION=\"${VERSION}\")
add_definitions(-DREVISION=\"${REVISION}\")
add_definitions(-O3)

# get needed packages and include
find_package(LibArchive)
find_package(LibXml2)
find_package(LibXslt)

if(LibArchive_FOUND)
        include_directories(${LibArchive_INCLUDE_DIRS})
endif()

if(LIBXML2_FOUND)
        include_directories(${LIBXML2_INCLUDE_DIR})
else()

endif()

if(LIBXSLT_FOUND)
        include_directories(${LIBXSLT_INCLUDE_DIR})
endif()

include_directories(C:/antlr/277/include/antlr)

find_library(LIBANTLR_LIB NAMES libantlr.a PATHS /usr/lib /usr/local/lib)

# define needed programs
find_program(ANTLR NAMES antlr runantlr cantlr antlr2 antlr.bat PATHS /usr/bin /opt/local/bin /usr/local/bin C:/antlr/277/bin)
find_program(SED NAMES gsed sed PATHS /opt/local/bin /usr/local /bin)
find_program(GREP grep PATHS /bin /usr/bin)
find_program(ECHO echo PATH /bin)
find_program(CUT cut PATH /usr/bin)
find_program(SVNVERSION svnversion PATHS /usr/local/bin /usr/bin)

# Run antlr
add_custom_command(OUTPUT TextLexer.cpp TextLexer.hpp TextLexerTokenTypes.txt TextLexerTokenTypes.hpp expandedTextLexer.g COMMAND ${ANTLR} -glib \"WSLexer.g\;CommentLexer.g\" TextLexer.g  DEPENDS TextLexer.g WSLexer.cpp CommentLexer.cpp)
add_custom_command(OUTPUT WSLexer.cpp WSLexer.hpp WSLexerTokenTypes.txt WSLexerTokenTypes.hpp COMMAND ${ANTLR} -glib \"PureCommentLexer.g\" WSLexer.g DEPENDS WSLexer.g PureCommentLexer.cpp)
add_custom_command(OUTPUT CommentLexer.cpp CommentLexer.hpp CommentLexerTokenTypes.txt CommentLexerTokenTypes.hpp expandedCommentLexer.g COMMAND ${ANTLR} -glib \"WSLexer.g\" CommentLexer.g DEPENDS CommentLexer.g WSLexer.cpp)
add_custom_command(OUTPUT OperatorLexer.cpp OperatorLexer.hpp OperatorLexerTokenTypes.txt OperatorLexerTokenTypes.hpp expandedOperatorLexer.g COMMAND ${ANTLR} -glib \"WSLexer.g\;CommentLexer.g\;TextLexer.g\" OperatorLexer.g DEPENDS OperatorLexer.g CommentLexer.cpp TextLexer.cpp)
add_custom_command(OUTPUT KeywordLexer.cpp KeywordLexer.hpp KeywordLexerTokenTypes.txt KeywordLexerTokenTypes.hpp expandedKeywordLexer.g COMMAND ${ANTLR} -glib \"WSLexer.g\;CommentLexer.g\;OperatorLexer.g\;TextLexer.g\" KeywordLexer.g DEPENDS KeywordLexer.g OperatorLexer.cpp)
add_custom_command(OUTPUT KeywordCPPLexer.cpp KeywordCPPLexer.hpp KeywordCPPLexerTokenTypes.txt KeywordCPPLexerTokenTypes.hpp expandedKeywordCPPLexer.g COMMAND ${ANTLR} -glib \"WSLexer.g\;CommentLexer.g\;OperatorLexer.g\;TextLexer.g\;KeywordLexer.g\" KeywordCPPLexer.g DEPENDS KeywordCPPLexer.g KeywordLexer.cpp)
add_custom_command(OUTPUT PureCommentLexer.cpp PureCommentLexer.hpp PureCommentLexerTokenTypes.txt PureCommentLexerTokenTypes.hpp COMMAND ${ANTLR} PureCommentLexer.g DEPENDS PureCommentLexer.g)
add_custom_command(OUTPUT srcMLParser.cpp srcMLParser.hpp srcMLParserTokenTypes.txt srcMLParserTokenTypes.hpp COMMAND ${ANTLR} -glib \"WSLexer.g\;CommentLexer.g\;OperatorLexer.g\;TextLexer.g\;KeywordLexer.g\;KeywordCPPLexer.g\" srcMLParser.g DEPENDS srcMLParser.g KeywordCPPLexer.cpp)

# Run special command for macro
add_custom_command(OUTPUT srcMLParserTokenTypesMacro.hpp COMMAND ${GREP} '=' srcMLParserTokenTypes.hpp | ${SED} -e 's/,//g' -e 's/= //g' -e 's/^[ \t]*//' -e 's/^/\#define TOKEN_/g' > srcMLParserTokenTypesMacro.hpp DEPENDS srcMLParserTokenTypes.hpp)

# libsrcml static
add_library(srcml_static STATIC srcMLParser.cpp KeywordCPPLexer.cpp srcMLTranslator.cpp srcMLTranslatorCore.cpp srcMLOutput.cpp srcMLTranslatorOutput.cpp Mode.cpp UTF8CharBuffer.cpp PureCommentLexer.cpp Language.cpp URIStream.cpp srcmlapps.cpp libxml_archive_read.cpp libxml_archive_write.cpp srcMLParserTokenTypesMacro.hpp)
target_link_libraries(srcml_static archive xml2 ${LIBANTLR_LIB})
set_target_properties(srcml_static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ../bin LIBRARY_OUTPUT_DIRECTORY ../bin OUTPUT_NAME srcml)

# libsrcml shared
add_library(srcml_shared SHARED srcMLParser.cpp KeywordCPPLexer.cpp srcMLTranslator.cpp srcMLTranslatorCore.cpp srcMLOutput.cpp srcMLTranslatorOutput.cpp Mode.cpp UTF8CharBuffer.cpp PureCommentLexer.cpp Language.cpp URIStream.cpp srcmlapps.cpp libxml_archive_read.cpp libxml_archive_write.cpp srcMLParserTokenTypesMacro.hpp)
target_link_libraries(srcml_shared archive xml2 ${LIBANTLR_LIB})
set_target_properties(srcml_shared PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ../bin LIBRARY_OUTPUT_DIRECTORY ../bin OUTPUT_NAME srcml)

# srcMLParserTokenTypesMacro
add_custom_target(srcMLParserTokenTypesMacro.hpp DEPENDS srcMLParserTokenTypes.hpp)

# srcml
add_executable(srcml srcml.cpp)

# src2srcml
add_executable(src2srcml src2srcml.cpp srcmlapps.cpp makeargv.cpp)
target_link_libraries(src2srcml archive xml2 ${LIBANTLR_LIB} srcml_static)

# srcml2src
add_executable(srcml2src srcml2src.cpp srcMLUtility.cpp SAX2ExtractUnitsSrc.cpp srcmlapps.cpp SAX2Utilities.cpp SAX2UnitDOM.cpp SAX2UnitDOMRelaxNG.cpp srcexfun.cpp libxml_archive_read.cpp libxml_archive_write.cpp URIStream.cpp makeargv.cpp)
target_link_libraries(srcml2src archive xml2 xslt exslt)

set_target_properties(srcml src2srcml srcml2src PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin)

