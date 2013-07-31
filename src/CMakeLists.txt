###
#    CMakeLists.txt
#
#
#    Optional cmake file for srcML.
#    The main way to build srcML is with normal Makefile

cmake_minimum_required(VERSION 2.8)
project(srcML)


# specify g++ compiler
if(NOT CMAKE_SYSTEM_NAME MATCHES "Windows")
       include(CMakeForceCompiler)
	CMAKE_FORCE_CXX_COMPILER(/usr/bin/g++ "g++")
endif()

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
find_package(Boost COMPONENTS program_options)

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

if(BOOST_FOUND)
        include_directories(${Boost_INCLUDE_DIR})
endif()

if(NOT DEFINED LIBXSLT_EXSLT_LIBRARY)
		set(LIBXSLT_EXSLT_LIBRARY exslt)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
		set(LIBREGEX_LIBRARY regex)
endif()

include_directories(C:/antlr/277/include/antlr)

find_library(LIBANTLR_LIB NAMES libantlr.a libantlr2-0.dll PATHS /usr/lib /usr/local/lib ../dlls)

# define needed programs
find_program(ANTLR NAMES antlr runantlr cantlr antlr2 antlr.bat PATHS /usr/bin /opt/local/bin /usr/local/bin C:/antlr/277/bin)
find_program(SED NAMES gsed sed PATHS /opt/local/bin /usr/local /bin)
find_program(GREP grep PATHS /bin /usr/bin)
find_program(ECHO echo PATH /bin)
find_program(CUT cut PATH /usr/bin)
find_program(SVNVERSION svnversion PATHS /usr/local/bin /usr/bin)

# Run antlr
add_custom_command(OUTPUT TextLexer.cpp TextLexer.hpp TextLexerTokenTypes.txt TextLexerTokenTypes.hpp expandedTextLexer.g COMMAND rm -f TextLexer.cpp TextLexer.hpp TextLexerTokenTypes.txt TextLexerTokenTypes.hpp expandedTextLexer.g COMMAND ${ANTLR} -glib \"WSLexer.g\;CommentLexer.g\" TextLexer.g  DEPENDS TextLexer.g WSLexer.cpp CommentLexer.cpp)
add_custom_command(OUTPUT WSLexer.cpp WSLexer.hpp WSLexerTokenTypes.txt WSLexerTokenTypes.hpp COMMAND rm -f WSLexer.cpp WSLexer.hpp WSLexerTokenTypes.txt WSLexerTokenTypes.hpp COMMAND ${ANTLR} -glib \"PureCommentLexer.g\" WSLexer.g DEPENDS WSLexer.g PureCommentLexer.cpp)
add_custom_command(OUTPUT CommentLexer.cpp CommentLexer.hpp CommentLexerTokenTypes.txt CommentLexerTokenTypes.hpp expandedCommentLexer.g COMMAND rm -f CommentLexer.cpp CommentLexer.hpp CommentLexerTokenTypes.txt CommentLexerTokenTypes.hpp expandedCommentLexer.g COMMAND ${ANTLR} -glib \"WSLexer.g\" CommentLexer.g DEPENDS CommentLexer.g WSLexer.cpp)
add_custom_command(OUTPUT OperatorLexer.cpp OperatorLexer.hpp OperatorLexerTokenTypes.txt OperatorLexerTokenTypes.hpp expandedOperatorLexer.g COMMAND rm -f OperatorLexer.cpp OperatorLexer.hpp OperatorLexerTokenTypes.txt OperatorLexerTokenTypes.hpp expandedOperatorLexer.g COMMAND ${ANTLR} -glib \"WSLexer.g\;CommentLexer.g\;TextLexer.g\" OperatorLexer.g DEPENDS OperatorLexer.g CommentLexer.cpp TextLexer.cpp)
add_custom_command(OUTPUT KeywordLexer.cpp KeywordLexer.hpp KeywordLexerTokenTypes.txt KeywordLexerTokenTypes.hpp expandedKeywordLexer.g COMMAND rm -f KeywordLexer.cpp KeywordLexer.hpp KeywordLexerTokenTypes.txt KeywordLexerTokenTypes.hpp expandedKeywordLexer.g COMMAND ${ANTLR} -glib \"WSLexer.g\;CommentLexer.g\;OperatorLexer.g\;TextLexer.g\" KeywordLexer.g DEPENDS KeywordLexer.g OperatorLexer.cpp)
add_custom_command(OUTPUT KeywordCPPLexer.cpp KeywordCPPLexer.hpp KeywordCPPLexerTokenTypes.txt KeywordCPPLexerTokenTypes.hpp expandedKeywordCPPLexer.g COMMAND rm -f KeywordCPPLexer.cpp KeywordCPPLexer.hpp KeywordCPPLexerTokenTypes.txt KeywordCPPLexerTokenTypes.hpp expandedKeywordCPPLexer.g COMMAND ${ANTLR} -glib \"WSLexer.g\;CommentLexer.g\;OperatorLexer.g\;TextLexer.g\;KeywordLexer.g\" KeywordCPPLexer.g DEPENDS KeywordCPPLexer.g KeywordLexer.cpp)
add_custom_command(OUTPUT PureCommentLexer.cpp PureCommentLexer.hpp PureCommentLexerTokenTypes.txt PureCommentLexerTokenTypes.hpp COMMAND rm -f PureCommentLexer.cpp PureCommentLexer.hpp PureCommentLexerTokenTypes.txt PureCommentLexerTokenTypes.hpp COMMAND ${ANTLR} PureCommentLexer.g DEPENDS PureCommentLexer.g)
add_custom_command(OUTPUT srcMLParser.cpp srcMLParser.hpp srcMLParserTokenTypes.txt srcMLParserTokenTypes.hpp COMMAND rm -f srcMLParser.cpp srcMLParser.hpp srcMLParserTokenTypes.txt srcMLParserTokenTypes.hpp COMMAND ${ANTLR} -glib \"WSLexer.g\;CommentLexer.g\;OperatorLexer.g\;TextLexer.g\;KeywordLexer.g\;KeywordCPPLexer.g\" srcMLParser.g DEPENDS srcMLParser.g KeywordCPPLexer.cpp)

# Run special command for macro
add_custom_command(OUTPUT srcMLParserTokenTypesMacro.hpp COMMAND ${GREP} '=' srcMLParserTokenTypes.hpp | ${SED} -e 's/,//g' -e 's/= //g' -e 's/^[ \t]*//' -e 's/^/\#define TOKEN_/g' > srcMLParserTokenTypesMacro.hpp DEPENDS srcMLParserTokenTypes.hpp)

# libsrcml static
add_library(srcml_static STATIC srcMLParser.cpp KeywordCPPLexer.cpp srcMLTranslator.cpp srcMLTranslatorCore.cpp srcMLOutput.cpp srcMLTranslatorOutput.cpp Mode.cpp UTF8CharBuffer.cpp PureCommentLexer.cpp Language.cpp URIStream.cpp srcmlapps.cpp libxml_archive_read.cpp libxml_archive_write.cpp srcMLParserTokenTypesMacro.hpp srcMLUtility.cpp SAX2ExtractUnitsSrc.cpp srcmlapps.cpp SAX2Utilities.cpp SAX2UnitDOM.cpp SAX2UnitDOMRelaxNG.cpp srcexfun.cpp)
target_link_libraries(srcml_static ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} ${LIBREGEX_LIBRARY})
set_target_properties(srcml_static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ../bin LIBRARY_OUTPUT_DIRECTORY ../bin OUTPUT_NAME srcml)

if(NOT CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
       # libsrcml shared
       add_library(srcml_shared SHARED srcMLParser.cpp KeywordCPPLexer.cpp srcMLTranslator.cpp srcMLTranslatorCore.cpp srcMLOutput.cpp srcMLTranslatorOutput.cpp Mode.cpp UTF8CharBuffer.cpp PureCommentLexer.cpp Language.cpp URIStream.cpp srcmlapps.cpp libxml_archive_read.cpp libxml_archive_write.cpp srcMLParserTokenTypesMacro.hpp srcMLUtility.cpp SAX2ExtractUnitsSrc.cpp SAX2Utilities.cpp SAX2UnitDOM.cpp SAX2UnitDOMRelaxNG.cpp srcexfun.cpp)
       target_link_libraries(srcml_shared ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} ${LIBREGEX_LIBRARY})
       set_target_properties(srcml_shared PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ../bin LIBRARY_OUTPUT_DIRECTORY ../bin OUTPUT_NAME srcml)
endif()

# srcMLParserTokenTypesMacro
add_custom_target(srcMLParserTokenTypesMacro.hpp DEPENDS srcMLParserTokenTypes.hpp)

# srcml
add_executable(srcml EXCLUDE_FROM_ALL srcml.cpp)
target_link_libraries(srcml ${Boost_PROGRAM_OPTIONS_LIBRARY})

# src2srcml
add_executable(src2srcml src2srcml.cpp)
target_link_libraries(src2srcml ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB} srcml_static)

# srcml2src
add_executable(srcml2src srcml2src.cpp)
target_link_libraries(srcml2src  ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} srcml_static)

set_target_properties(srcml src2srcml srcml2src PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin)

