###
#    CMakeLists.txt
#
#
#    The main way to build srcML.
#    Makefile.old is provided as an alternative.


cmake_minimum_required(VERSION 2.8)
project(srcML)

option(SVN_ENABLED "Build with svn" off)

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
add_definitions(-s)
add_definitions(-fPIC)

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

if(SVN_ENABLED)
include_directories(/usr/include/apr-1)
include_directories(/usr/local/include/subversion-1)
add_definitions(-DSVN)
endif()

find_library(LIBANTLR_LIB NAMES libantlr-pic.a libantlr.a libantlr2-0.dll PATHS /usr/lib /usr/local/lib ../dlls)

# define needed programs
find_program(ANTLR NAMES antlr runantlr cantlr antlr2 antlr.bat PATHS /usr/bin /opt/local/bin /usr/local/bin C:/antlr/277/bin)
find_program(SED NAMES gsed sed PATHS /opt/local/bin /usr/local /bin)
find_program(GREP grep PATHS /bin /usr/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../bin )
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../bin )

# Run antlr
add_custom_command(OUTPUT CommentTextLexer.cpp CommentTextLexer.hpp CommentTextLexerTokenTypes.txt CommentTextLexerTokenTypes.hpp 
COMMAND ${ANTLR} CommentTextLexer.g DEPENDS CommentTextLexer.g
COMMAND touch CommentTextLexer.cpp CommentTextLexer.hpp CommentTextLexerTokenTypes.txt CommentTextLexerTokenTypes.hpp expandedCommentTextLexer.g 
)

add_custom_command(OUTPUT TextLexer.cpp TextLexer.hpp TextLexerTokenTypes.txt TextLexerTokenTypes.hpp expandedTextLexer.g COMMAND ${ANTLR} TextLexer.g DEPENDS TextLexer.g CommentTextLexer.cpp
COMMAND touch TextLexer.cpp TextLexer.hpp TextLexerTokenTypes.txt TextLexerTokenTypes.hpp expandedTextLexer.g 
)

add_custom_command(OUTPUT OperatorLexer.cpp OperatorLexer.hpp OperatorLexerTokenTypes.txt OperatorLexerTokenTypes.hpp expandedOperatorLexer.g 
COMMAND ${ANTLR} -glib \"TextLexer.g\" OperatorLexer.g DEPENDS OperatorLexer.g TextLexer.cpp
COMMAND touch OperatorLexer.cpp OperatorLexer.hpp OperatorLexerTokenTypes.txt OperatorLexerTokenTypes.hpp expandedOperatorLexer.g
)

add_custom_command(OUTPUT KeywordLexer.cpp KeywordLexer.hpp KeywordLexerTokenTypes.txt KeywordLexerTokenTypes.hpp expandedKeywordLexer.g
COMMAND ${ANTLR} -glib \"OperatorLexer.g\;TextLexer.g\" KeywordLexer.g DEPENDS KeywordLexer.g OperatorLexer.cpp
COMMAND touch KeywordLexer.cpp KeywordLexer.hpp KeywordLexerTokenTypes.txt KeywordLexerTokenTypes.hpp expandedKeywordLexer.g 
)

add_custom_command(OUTPUT srcMLParser.cpp srcMLParser.hpp srcMLParserTokenTypes.txt srcMLParserTokenTypes.hpp
COMMAND ${ANTLR} -glib \"OperatorLexer.g\;TextLexer.g\;KeywordLexer.g\;\" srcMLParser.g DEPENDS srcMLParser.g KeywordLexer.cpp
COMMAND touch srcMLParser.cpp srcMLParser.hpp srcMLParserTokenTypes.txt srcMLParserTokenTypes.hpp 
)

# Run special command for macro
add_custom_command(OUTPUT srcMLParserTokenTypesMacro.hpp COMMAND ${GREP} '=' srcMLParserTokenTypes.hpp | ${SED} -e 's/,//g' -e 's/= //g' -e 's/^[ \t]*//' -e 's/^/\#define TOKEN_/g' > srcMLParserTokenTypesMacro.hpp DEPENDS srcMLParserTokenTypes.hpp)

if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER 2.8.7)
add_library(parser OBJECT srcMLParser.cpp KeywordLexer.cpp srcMLOutput.cpp srcMLTranslatorOutput.cpp Mode.cpp CommentTextLexer.cpp srcMLParserTokenTypesMacro.hpp)
add_library(srcmlutility OBJECT srcMLUtility.cpp SAX2ExtractUnitsSrc.cpp SAX2Utilities.cpp SAX2UnitDOM.cpp SAX2UnitDOMRelaxNG.cpp srcexfun.cpp)
add_library(translator OBJECT srcMLTranslator.cpp srcMLTranslatorCore.cpp UTF8CharBuffer.cpp Language.cpp URIStream.cpp libxml_archive_read.cpp libxml_archive_write.cpp)
add_library(srcmlapps OBJECT srcmlapps.cpp)
add_library(libsrcml OBJECT libsrcml/libsrcml.cpp libsrcml/srcMLReader.cpp libsrcml/srcml_transform.cpp libsrcml/srcml_info.cpp libsrcml/srcml_archive.cpp libsrcml/srcml_unit.cpp)

# libsrcml static
add_library(srcml_static STATIC $<TARGET_OBJECTS:libsrcml> $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:translator> $<TARGET_OBJECTS:srcmlapps> $<TARGET_OBJECTS:srcmlutility>)
else()
add_library(srcml_static STATIC libsrcml/libsrcml.cpp libsrcml/srcMLReader.cpp libsrcml/srcml_transform.cpp libsrcml/srcml_info.cpp libsrcml/srcml_archive.cpp libsrcml/srcml_unit.cpp srcMLParser.cpp KeywordLexer.cpp srcMLOutput.cpp srcMLTranslatorOutput.cpp Mode.cpp CommentTextLexer.cpp srcMLParserTokenTypesMacro.hpp srcMLUtility.cpp SAX2ExtractUnitsSrc.cpp SAX2Utilities.cpp SAX2UnitDOM.cpp SAX2UnitDOMRelaxNG.cpp srcexfun.cpp srcMLTranslator.cpp srcMLTranslatorCore.cpp UTF8CharBuffer.cpp Language.cpp URIStream.cpp libxml_archive_read.cpp libxml_archive_write.cpp srcmlapps.cpp srcml.c)
endif()

target_link_libraries(srcml_static ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB} ${LIBREGEX_LIBRARY})
set_target_properties(srcml_static PROPERTIES OUTPUT_NAME srcml)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
       # libsrcml shared
if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER 2.8.7)
       add_library(srcml_shared SHARED $<TARGET_OBJECTS:libsrcml> $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:srcmlutility> $<TARGET_OBJECTS:translator> $<TARGET_OBJECTS:srcmlapps>)
else()
       add_library(srcml_shared SHARED libsrcml/libsrcml.cpp libsrcml/srcMLReader.cpp libsrcml/srcml_transform.cpp libsrcml/srcml_info.cpp libsrcml/srcml_archive.cpp libsrcml/srcml_unit.cpp srcMLParser.cpp KeywordLexer.cpp srcMLOutput.cpp srcMLTranslatorOutput.cpp Mode.cpp CommentTextLexer.cpp srcMLParserTokenTypesMacro.hpp srcMLUtility.cpp SAX2ExtractUnitsSrc.cpp SAX2Utilities.cpp SAX2UnitDOM.cpp SAX2UnitDOMRelaxNG.cpp srcexfun.cpp srcMLTranslator.cpp srcMLTranslatorCore.cpp UTF8CharBuffer.cpp Language.cpp URIStream.cpp libxml_archive_read.cpp libxml_archive_write.cpp srcmlapps.cpp srcml.c)
endif()
       target_link_libraries(srcml_shared ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} ${LIBREGEX_LIBRARY})
       set_target_properties(srcml_shared PROPERTIES OUTPUT_NAME srcml)
else()
       # libsrcml shared
if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER 2.8.7)
       add_library(srcml_shared SHARED $<TARGET_OBJECTS:libsrcml> $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:srcmlutility> $<TARGET_OBJECTS:translator> $<TARGET_OBJECTS:srcmlapps>)
else()
       add_library(srcml_shared SHARED libsrcml/libsrcml.cpp libsrcml/srcMLReader.cpp libsrcml/srcml_transform.cpp libsrcml/srcml_info.cpp libsrcml/srcml_archive.cpp libsrcml/srcml_unit.cpp srcMLParser.cpp KeywordLexer.cpp srcMLOutput.cpp srcMLTranslatorOutput.cpp Mode.cpp CommentTextLexer.cpp srcMLParserTokenTypesMacro.hpp srcMLUtility.cpp SAX2ExtractUnitsSrc.cpp SAX2Utilities.cpp SAX2UnitDOM.cpp SAX2UnitDOMRelaxNG.cpp srcexfun.cpp srcMLTranslator.cpp srcMLTranslatorCore.cpp UTF8CharBuffer.cpp Language.cpp URIStream.cpp libxml_archive_read.cpp libxml_archive_write.cpp srcmlapps.cpp srcml.c)
endif()
       target_link_libraries(srcml_shared ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB})
       set_target_properties(srcml_shared PROPERTIES OUTPUT_NAME srcml)
endif()

# srcMLParserTokenTypesMacro
add_custom_target(srcMLParserTokenTypesMacro.hpp DEPENDS srcMLParserTokenTypes.hpp)

# srcml
add_executable(srcml EXCLUDE_FROM_ALL srcml.cpp)
target_link_libraries(srcml ${Boost_PROGRAM_OPTIONS_LIBRARY})

# src2srcml
if(NOT SVN_ENABLED)
add_executable(src2srcml src2srcml.cpp)
else()
add_executable(src2srcml src2srcml.cpp svn_io.cpp)
endif()

if(NOT CMAKE_SYSTEM_NAME MATCHES "Windows")
       target_link_libraries(src2srcml ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB} dl srcml_static)
else()
        target_link_libraries(src2srcml ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB} srcml_shared)
endif()

# srcml2src
if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER 2.8.7)
add_executable(srcml2src srcml2src.cpp $<TARGET_OBJECTS:srcmlutility>)
else()
add_executable(srcml2src srcml2src.cpp)
endif()

if(NOT CMAKE_SYSTEM_NAME MATCHES "Windows")
       target_link_libraries(srcml2src  ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} dl srcml_static)
else()
       target_link_libraries(srcml2src  ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} srcml_shared)

endif()
