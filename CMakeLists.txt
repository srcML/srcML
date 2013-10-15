###
#    CMakeLists.txt
#
#
#    The main way to build srcML.
#    Makefile.old is provided as an alternative.
#    However, Makefile.old may not be maintained.

cmake_minimum_required(VERSION 2.8)
project(srcML)

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_FLAGS_DEBUG "-g --coverage -O0 -fprofile-arcs -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE -O3)

option(SVN_ENABLED "Build with svn" off)
option(LIBSRCML_SAX2_ENABLED "Build with SAX2Framework for srcML" off)

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
include_directories(. src src/libsrcml)

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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY bin)

# Run antlr
add_custom_command(OUTPUT src/CommentTextLexer.cpp src/CommentTextLexer.hpp src/CommentTextLexerTokenTypes.txt src/CommentTextLexerTokenTypes.hpp 
COMMAND ${ANTLR} -o src src/CommentTextLexer.g DEPENDS src/CommentTextLexer.g
COMMAND touch src/CommentTextLexer.cpp src/CommentTextLexer.hpp src/CommentTextLexerTokenTypes.txt src/CommentTextLexerTokenTypes.hpp src/expandedCommentTextLexer.g 
)

add_custom_command(OUTPUT src/TextLexer.cpp src/TextLexer.hpp src/TextLexerTokenTypes.txt src/TextLexerTokenTypes.hpp src/expandedTextLexer.g 
COMMAND ${ANTLR} -o src src/TextLexer.g DEPENDS src/TextLexer.g src/CommentTextLexer.cpp
COMMAND touch src/TextLexer.cpp src/TextLexer.hpp src/TextLexerTokenTypes.txt src/TextLexerTokenTypes.hpp src/expandedTextLexer.g 
)

add_custom_command(OUTPUT src/OperatorLexer.cpp src/OperatorLexer.hpp src/OperatorLexerTokenTypes.txt src/OperatorLexerTokenTypes.hpp src/expandedOperatorLexer.g 
COMMAND ${ANTLR} -o src -glib \"src/TextLexer.g\" src/OperatorLexer.g DEPENDS src/OperatorLexer.g src/TextLexer.cpp
COMMAND touch src/OperatorLexer.cpp src/OperatorLexer.hpp src/OperatorLexerTokenTypes.txt src/OperatorLexerTokenTypes.hpp src/expandedOperatorLexer.g
)

add_custom_command(OUTPUT src/KeywordLexer.cpp src/KeywordLexer.hpp src/KeywordLexerTokenTypes.txt src/KeywordLexerTokenTypes.hpp src/expandedKeywordLexer.g
COMMAND ${ANTLR} -o src -glib \"src/OperatorLexer.g\;src/TextLexer.g\" src/KeywordLexer.g DEPENDS src/KeywordLexer.g src/OperatorLexer.cpp
COMMAND touch src/KeywordLexer.cpp src/KeywordLexer.hpp src/KeywordLexerTokenTypes.txt src/KeywordLexerTokenTypes.hpp src/expandedKeywordLexer.g
COMMAND rm -rf OperatorLexerTokenTypes.txt TextLexerTokenTypes.txt
)

add_custom_command(OUTPUT src/srcMLParser.cpp src/srcMLParser.hpp src/srcMLParserTokenTypes.txt src/srcMLParserTokenTypes.hpp
COMMAND ${ANTLR} -o src -glib \"src/OperatorLexer.g\;src/TextLexer.g\;src/KeywordLexer.g\;\" src/srcMLParser.g DEPENDS src/srcMLParser.g src/KeywordLexer.cpp
COMMAND touch src/srcMLParser.cpp src/srcMLParser.hpp src/srcMLParserTokenTypes.txt src/srcMLParserTokenTypes.hpp 
)


# Run special command for macro
add_custom_command(OUTPUT src/srcMLParserTokenTypesMacro.hpp 
COMMAND ${GREP} '=' src/srcMLParserTokenTypes.hpp | ${SED} -e 's/,//g' -e 's/= //g' -e 's/^[ \t]*//' -e 's/^/\#define TOKEN_/g' > src/srcMLParserTokenTypesMacro.hpp DEPENDS src/srcMLParserTokenTypes.hpp
)

if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER 2.8.7)
    add_library(parser OBJECT src/srcMLParser.cpp src/KeywordLexer.cpp src/srcMLOutput.cpp src/srcMLTranslatorOutput.cpp src/Mode.cpp src/CommentTextLexer.cpp src/srcMLParserTokenTypesMacro.hpp)
    add_library(srcmlutility OBJECT src/srcMLUtility.cpp src/SAX2ExtractUnitsSrc.cpp src/SAX2Utilities.cpp src/SAX2UnitDOM.cpp src/SAX2UnitDOMRelaxNG.cpp src/srcexfun.cpp)
    add_library(translator OBJECT src/srcMLTranslator.cpp src/srcMLTranslatorCore.cpp src/UTF8CharBuffer.cpp src/Language.cpp src/URIStream.cpp src/libxml_archive_read.cpp src/libxml_archive_write.cpp)
    add_library(srcmlapps OBJECT src/srcmlapps.cpp)

    if(LIBSRCML_SAX2_ENABLED)
        file(GLOB LIBSRCML_SRC src/libsrcml/*.cpp)
        add_definitions(-DSAX2)
    else()
        file(GLOB LIBSRCML_SRC src/libsrcml/srcml_*.cpp src/libsrcml/libsrcml.cpp src/libsrcml/srcMLReader.cpp)
    endif()
    add_library(libsrcml OBJECT ${LIBSRCML_SRC})

    # libsrcml static
    add_library(srcml_static STATIC $<TARGET_OBJECTS:libsrcml> $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:translator> $<TARGET_OBJECTS:srcmlapps> $<TARGET_OBJECTS:srcmlutility>)
else()
    add_library(srcml_static STATIC src/libsrcml/libsrcml.cpp src/libsrcml/srcMLReader.cpp src/libsrcml/srcml_transform.cpp src/libsrcml/srcml_info.cpp src/libsrcml/srcml_archive.cpp src/libsrcml/srcml_unit.cpp src/libsrcml/srcml_sax2_utilities.cpp src/srcMLParser.cpp src/KeywordLexer.cpp src/srcMLOutput.cpp src/srcMLTranslatorOutput.cpp src/Mode.cpp src/CommentTextLexer.cpp src/srcMLParserTokenTypesMacro.hpp src/srcMLUtility.cpp src/SAX2ExtractUnitsSrc.cpp src/SAX2Utilities.cpp src/SAX2UnitDOM.cpp src/SAX2UnitDOMRelaxNG.cpp src/srcexfun.cpp src/srcMLTranslator.cpp src/srcMLTranslatorCore.cpp src/UTF8CharBuffer.cpp src/Language.cpp src/URIStream.cpp src/libxml_archive_read.cpp src/libxml_archive_write.cpp src/srcmlapps.cpp)
endif()

target_link_libraries(srcml_static ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB} ${LIBREGEX_LIBRARY})
set_target_properties(srcml_static PROPERTIES OUTPUT_NAME srcml)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    # libsrcml shared
    if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER 2.8.7)
        add_library(srcml_shared SHARED $<TARGET_OBJECTS:libsrcml> $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:srcmlutility> $<TARGET_OBJECTS:translator> $<TARGET_OBJECTS:srcmlapps>)
    else()
        add_library(srcml_shared SHARED src/libsrcml/libsrcml.cpp src/libsrcml/srcMLReader.cpp src/libsrcml/srcml_transform.cpp src/libsrcml/srcml_info.cpp src/libsrcml/srcml_archive.cpp src/libsrcml/srcml_unit.cpp src/libsrcml/srcml_sax2_utilities.cpp src/srcMLParser.cpp src/KeywordLexer.cpp src/srcMLOutput.cpp src/srcMLTranslatorOutput.cpp src/Mode.cpp src/CommentTextLexer.cpp src/srcMLParserTokenTypesMacro.hpp src/srcMLUtility.cpp src/SAX2ExtractUnitsSrc.cpp src/SAX2Utilities.cpp src/SAX2UnitDOM.cpp src/SAX2UnitDOMRelaxNG.cpp src/srcexfun.cpp src/srcMLTranslator.cpp src/srcMLTranslatorCore.cpp src/UTF8CharBuffer.cpp src/Language.cpp src/URIStream.cpp src/libxml_archive_read.cpp src/libxml_archive_write.cpp src/srcmlapps.cpp)
    endif()
    target_link_libraries(srcml_shared ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} ${LIBREGEX_LIBRARY})
    set_target_properties(srcml_shared PROPERTIES OUTPUT_NAME srcml)
else()
    # libsrcml shared
    if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER 2.8.7)
        add_library(srcml_shared SHARED $<TARGET_OBJECTS:libsrcml> $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:srcmlutility> $<TARGET_OBJECTS:translator> $<TARGET_OBJECTS:srcmlapps>)
else()
       add_library(srcml_shared SHARED src/libsrcml/libsrcml.cpp src/libsrcml/srcMLReader.cpp src/libsrcml/srcml_transform.cpp src/libsrcml/srcml_info.cpp src/libsrcml/srcml_archive.cpp src/libsrcml/srcml_unit.cpp src/libsrcml/srcml_sax2_utilities.cpp src/srcMLParser.cpp src/KeywordLexer.cpp src/srcMLOutput.cpp src/srcMLTranslatorOutput.cpp src/Mode.cpp src/CommentTextLexer.cpp src/srcMLParserTokenTypesMacro.hpp src/srcMLUtility.cpp src/SAX2ExtractUnitsSrc.cpp src/SAX2Utilities.cpp src/SAX2UnitDOM.cpp src/SAX2UnitDOMRelaxNG.cpp src/srcexfun.cpp src/srcMLTranslator.cpp src/srcMLTranslatorCore.cpp src/UTF8CharBuffer.cpp src/Language.cpp src/URIStream.cpp src/libxml_archive_read.cpp src/libxml_archive_write.cpp src/srcmlapps.cpp)
    endif()
    target_link_libraries(srcml_shared ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB})
    set_target_properties(srcml_shared PROPERTIES OUTPUT_NAME srcml)
endif()

# srcml
add_executable(srcml EXCLUDE_FROM_ALL src/srcml.cpp)
target_link_libraries(srcml ${Boost_PROGRAM_OPTIONS_LIBRARY})

# src2srcml
if(NOT SVN_ENABLED)
    add_executable(src2srcml src/src2srcml.cpp)
else()
    add_executable(src2srcml src/src2srcml.cpp src/svn_io.cpp)
endif()

if(NOT CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(src2srcml ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB} dl srcml_static)
else()
    target_link_libraries(src2srcml ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB} srcml_shared)
endif()

# srcml2src
if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER 2.8.7)
    add_executable(srcml2src src/srcml2src.cpp $<TARGET_OBJECTS:srcmlutility>)
else()
    add_executable(srcml2src src/srcml2src.cpp)
endif()

if(NOT CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(srcml2src ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} dl srcml_static)
else()
    target_link_libraries(srcml2src ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} srcml_shared)

endif()

# install handling
include(CMakeInstall.inc)

# cpack
include(CPackConfig.inc)
