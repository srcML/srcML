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
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG -DNO_DLLOAD")
#set(CMAKE_CXX_FLAGS_DEBUG "-g --coverage -O0 -fprofile-arcs -DDEBUG -DNO_DLLOAD")
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
find_package(LibArchive REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(LibXslt)
find_package(Boost COMPONENTS program_options)

include_directories(${LibArchive_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIR})

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
    include_directories(C:/antlr/277/include/antlr)
    set(LIBREGEX_LIBRARY regex)
    set(SRC2SRCML_LIBS srcml_shared)
    set(SRCML2SRC_LIBS ${LIBXSLT_LIBRARIES};${LIBXSLT_EXSLT_LIBRARY};srcml_shared)
else()
    set(SRC2SRCML_LIBS dl;srcml_static)
    set(SRCML2SRC_LIBS dl;srcml_static)
endif()

set(LINK_EXTRA "")
if(CMAKE_BUILD_TYPE MATCHES "DEBUG")
    set(LINK_EXTRA ${LIBXSLT_LIBRARIES};${LIBXSLT_EXSLT_LIBRARY})
endif()


include_directories(src src/libsrcml src/oldclient src/parser src/srcml src/translator src/SAX2Framework)

if(SVN_ENABLED)
    include_directories(/usr/include/apr-1 /usr/local/include/subversion-1)
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
add_custom_command(OUTPUT src/parser/CommentTextLexer.cpp src/parser/CommentTextLexer.hpp src/parser/CommentTextLexerTokenTypes.txt src/parser/CommentTextLexerTokenTypes.hpp 
COMMAND ${ANTLR} -o src/parser src/parser/CommentTextLexer.g DEPENDS src/parser/CommentTextLexer.g
COMMAND touch src/parser/CommentTextLexer.cpp src/parser/CommentTextLexer.hpp src/parser/CommentTextLexerTokenTypes.txt src/parser/CommentTextLexerTokenTypes.hpp src/parser/expandedCommentTextLexer.g 
)

add_custom_command(OUTPUT src/parser/TextLexer.cpp src/parser/TextLexer.hpp src/parser/TextLexerTokenTypes.txt src/parser/TextLexerTokenTypes.hpp src/parser/expandedTextLexer.g 
COMMAND ${ANTLR} -o src/parser src/parser/TextLexer.g DEPENDS src/parser/TextLexer.g src/parser/CommentTextLexer.cpp
COMMAND touch src/parser/TextLexer.cpp src/parser/TextLexer.hpp src/parser/TextLexerTokenTypes.txt src/parser/TextLexerTokenTypes.hpp src/parser/expandedTextLexer.g 
)

add_custom_command(OUTPUT src/parser/OperatorLexer.cpp src/parser/OperatorLexer.hpp src/parser/OperatorLexerTokenTypes.txt src/parser/OperatorLexerTokenTypes.hpp src/parser/expandedOperatorLexer.g 
COMMAND ${ANTLR} -o src/parser -glib \"src/parser/TextLexer.g\" src/parser/OperatorLexer.g DEPENDS src/parser/OperatorLexer.g src/parser/TextLexer.cpp
COMMAND touch src/parser/OperatorLexer.cpp src/parser/OperatorLexer.hpp src/parser/OperatorLexerTokenTypes.txt src/parser/OperatorLexerTokenTypes.hpp src/parser/expandedOperatorLexer.g
)

add_custom_command(OUTPUT src/parser/KeywordLexer.cpp src/parser/KeywordLexer.hpp src/parser/KeywordLexerTokenTypes.txt src/parser/KeywordLexerTokenTypes.hpp src/parser/expandedKeywordLexer.g
COMMAND ${ANTLR} -o src/parser -glib \"src/parser/OperatorLexer.g\;src/parser/TextLexer.g\" src/parser/KeywordLexer.g DEPENDS src/parser/KeywordLexer.g src/parser/OperatorLexer.cpp
COMMAND touch src/parser/KeywordLexer.cpp src/parser/KeywordLexer.hpp src/parser/KeywordLexerTokenTypes.txt src/parser/KeywordLexerTokenTypes.hpp src/parser/expandedKeywordLexer.g
#COMMAND rm -rf OperatorLexerTokenTypes.txt TextLexerTokenTypes.txt
)

add_custom_command(OUTPUT src/parser/srcMLParser.cpp src/parser/srcMLParser.hpp src/parser/srcMLParserTokenTypes.txt src/parser/srcMLParserTokenTypes.hpp
COMMAND ${ANTLR} -o src/parser -glib \"src/parser/OperatorLexer.g\;src/parser/TextLexer.g\;src/parser/KeywordLexer.g\;\" src/parser/srcMLParser.g DEPENDS src/parser/srcMLParser.g src/parser/KeywordLexer.cpp
COMMAND touch src/parser/srcMLParser.cpp src/parser/srcMLParser.hpp src/parser/srcMLParserTokenTypes.txt src/parser/srcMLParserTokenTypes.hpp 
)

# Run special command for macro
add_custom_command(OUTPUT src/parser/srcMLParserTokenTypesMacro.hpp 
COMMAND ${GREP} '=' src/parser/srcMLParserTokenTypes.hpp | ${SED} -e 's/,//g' -e 's/= //g' -e 's/^[ \t]*//' -e 's/^/\#define TOKEN_/g' > src/parser/srcMLParserTokenTypesMacro.hpp DEPENDS src/parser/srcMLParserTokenTypes.hpp
)

set(PARSER_SRC src/parser/srcMLParser.cpp;src/parser/KeywordLexer.cpp;src/parser/srcMLOutput.cpp;src/parser/srcMLTranslatorOutput.cpp;src/parser/Mode.cpp;src/parser/CommentTextLexer.cpp;src/parser/srcMLParserTokenTypesMacro.hpp)
file(GLOB TRANSLATOR_SRC src/translator/*.cpp)
set(SRCMLAPPS_SRC src/srcml/srcmlapps.cpp)
file(GLOB LIBSRCML_SRC src/libsrcml/*.cpp src/SAX2Framework/*.cpp)

if(LIBSRCML_SAX2_ENABLED)
    add_definitions(-DSAX2)
endif()

if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER 2.8.7)

    add_library(parser OBJECT ${PARSER_SRC})
    add_library(translator OBJECT ${TRANSLATOR_SRC})
    add_library(srcmlapps OBJECT ${SRCMLAPPS_SRC})
    add_library(libsrcml OBJECT ${LIBSRCML_SRC})

    # libsrcml static
    add_library(srcml_static STATIC $<TARGET_OBJECTS:libsrcml> $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:translator> $<TARGET_OBJECTS:srcmlapps>)
else()
    add_library(srcml_static STATIC ${LIBSRCML_SRC} ${PARSER_SRC} ${TRANSLATOR_SRC} ${SRCMLAPPS_SRC})
endif()

target_link_libraries(srcml_static ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB} ${LIBREGEX_LIBRARY} ${LINK_EXTRA})
set_target_properties(srcml_static PROPERTIES OUTPUT_NAME srcml)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")

    # libsrcml shared
    if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER 2.8.7)
        add_library(srcml_shared SHARED $<TARGET_OBJECTS:libsrcml> $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:translator> $<TARGET_OBJECTS:srcmlapps>)
    else()
        add_library(srcml_shared SHARED ${LIBSRCML_SRC} ${PARSER_SRC} ${TRANSLATOR_SRC} ${SRCMLAPPS_SRC})
    endif()
    target_link_libraries(srcml_shared ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} ${LIBREGEX_LIBRARY})
    set_target_properties(srcml_shared PROPERTIES OUTPUT_NAME srcml)
else()

    # libsrcml shared
    if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER 2.8.7)
        add_library(srcml_shared SHARED $<TARGET_OBJECTS:libsrcml> $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:translator> $<TARGET_OBJECTS:srcmlapps>)
else()
       add_library(srcml_shared SHARED ${LIBSRCML_SRC} ${PARSER_SRC} ${TRANSLATOR_SRC} ${SRCMLAPPS_SRC})
    endif()
    target_link_libraries(srcml_shared ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB} ${LINK_EXTRA})
    set_target_properties(srcml_shared PROPERTIES OUTPUT_NAME srcml)
endif()

# srcml
add_executable(srcml EXCLUDE_FROM_ALL src/srcml/srcml.cpp src/srcml/srcmlCLI.cpp)
target_link_libraries(srcml ${Boost_PROGRAM_OPTIONS_LIBRARY} ${LibArchive_LIBRARY} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} ${CURL_LIBRARIES} srcml_static)

# src2srcml
if(NOT SVN_ENABLED)
    add_executable(src2srcml src/oldclient/src2srcml.cpp)
else()
    add_executable(src2srcml src/oldclient/src2srcml.cpp src/oldclient/svn_io.cpp)
endif()
target_link_libraries(src2srcml ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB} ${SRC2SRCML_LIBS} ${LINK_EXTRA})

# srcml2src
add_executable(srcml2src src/oldclient/srcml2src.cpp)
target_link_libraries(srcml2src ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${SRCML2SRC_LIBS} ${LINK_EXTRA})

# install handling
include(CMake/install.cmake)

# cpack
include(CMake/packageConfig.cmake)
