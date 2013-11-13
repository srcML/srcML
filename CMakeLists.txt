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

#
#    The CMake project file for srcML.
#
cmake_minimum_required(VERSION 2.8)
project(srcML)
enable_testing()
# Setting up Project includes.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
include (srcml)

# Setting up source include directories.
include_directories(src/srcml src/libsrcml src/parser src/translator)

# Setting major and minor version numbers.
add_definitions(-DVERSION=\"1\")
add_definitions(-DREVISION=\"1\")

#MESSAGE(STATUS "${VERSION} ${REVISION}")
# This is being moved to be more compiler independent.
add_definitions(-fPIC)

# Adding global compiler definitions.
# This allows for compilation of a re-locatable execuatable on GCC I need to be sure that I
# can make this portable to compilers other than GCC.
if(CMAKE_COMPILER_IS_GNUXX)
    add_definitions(-fPIC)
endif()

get_property(SVN_ENABLED GLOBAL PROPERTY SVN_ENABLED)
if(${SVN_ENABLED})
    add_definitions(-DSVN)
endif()

get_property(SAX2_ENABLED GLOBAL PROPERTY SAX2_ENABLED)
if(${SAX2_ENABLED})
    add_definitions(-DSAX2)
endif()


# Including extra directories.
subdirs(src test timing)

# Remove this later.
# MESSAGE(STATUS "")
# MESSAGE(STATUS "")
# MESSAGE(STATUS "----------- Testing global properties --------------")
# 
# get_property(SAX2_ENABLED GLOBAL PROPERTY SAX2_ENABLED)
# get_property(LIBXML2_LIBS GLOBAL PROPERTY LIBXML2_LIBS)
# get_property(LIBXSLT_LIBS GLOBAL PROPERTY LIBXSLT_LIBS)
# get_property(BOOST_PROGRAM_OPTIONS_LIB GLOBAL PROPERTY BOOST_PROGRAM_OPTIONS_LIB)
# get_property(WINDOWS_DEP_PATH GLOBAL PROPERTY WINDOWS_DEP_PATH)
# get_property(ANTLR_LIB GLOBAL PROPERTY ANTLR_LIB)
# get_property(ANTLR_EXE GLOBAL PROPERTY ANTLR_EXE)
# get_property(SED_EXE GLOBAL PROPERTY SED_EXE)
# get_property(GREP_EXE GLOBAL PROPERTY GREP_EXE)
# get_property(LIBARCHIVE_LIBS  GLOBAL PROPERTY LIBARCHIVE_LIBS)
# 
# #  Printing global property values.
# MESSAGE(STATUS "SVN_ENABLED ${SVN_ENABLED}")
# MESSAGE(STATUS "SAX2_ENABLED ${SAX2_ENABLED}")
# MESSAGE(STATUS "LIBXML2_LIBS: ${LIBXML2_LIBS}")
# MESSAGE(STATUS "LIBXSLT_LIBS: ${LIBXSLT_LIBS}")
# MESSAGE(STATUS "BOOST_PROGRAM_OPTIONS_LIB: ${BOOST_PROGRAM_OPTIONS_LIB}")
# MESSAGE(STATUS "WINDOWS_DEP_PATH: ${WINDOWS_DEP_PATH}")
# MESSAGE(STATUS "ANTLR_LIB: ${ANTLR_LIB}")
# MESSAGE(STATUS "ANTLR_EXE: ${ANTLR_EXE}")
# MESSAGE(STATUS "SED_EXE: ${SED_EXE}")
# MESSAGE(STATUS "GREP_EXE: ${GREP_EXE}")
# MESSAGE(STATUS "LIBARCHIVE_LIBS: ${LIBARCHIVE_LIBS}")
# MESSAGE(STATUS "")
# MESSAGE(STATUS "")
# if(NOT DEFINED LIBXSLT_EXSLT_LIBRARY)
#     set(LIBXSLT_EXSLT_LIBRARY exslt)
# endif()
# 
# if(CMAKE_SYSTEM_NAME MATCHES "Windows")
#     include_directories(C:/antlr/277/include/antlr)
#     set(LIBREGEX_LIBRARY regex)
#     set(SRC2SRCML_LIBS srcml_shared)
#     set(SRCML2SRC_LIBS ${LIBXSLT_LIBRARIES};${LIBXSLT_EXSLT_LIBRARY};srcml_shared)
# else()
#     set(SRC2SRCML_LIBS dl;srcml_static)
#     set(SRCML2SRC_LIBS dl;srcml_static)
# endif()
# 
# set(LINK_EXTRA "")
# if(CMAKE_BUILD_TYPE MATCHES "DEBUG")
#     set(LINK_EXTRA ${LIBXSLT_LIBRARIES};${LIBXSLT_EXSLT_LIBRARY})
# endif()
# 
# 
# include_directories(src src/libsrcml src/oldclient src/parser src/srcml src/translator src/SAX2Framework)
# 
# if(SVN_ENABLED)
#     include_directories(/usr/include/apr-1 /usr/local/include/subversion-1)
#     add_definitions(-DSVN)
# endif()
# 
# find_library(LIBANTLR_LIB NAMES libantlr-pic.a libantlr.a libantlr2-0.dll PATHS /usr/lib /usr/local/lib ../dlls)
# 
# # define needed programs
# find_program(ANTLR NAMES antlr runantlr cantlr antlr2 antlr.bat PATHS /usr/bin /opt/local/bin /usr/local/bin C:/antlr/277/bin)
# find_program(SED NAMES gsed sed PATHS /opt/local/bin /usr/local /bin)
# find_program(GREP grep PATHS /bin /usr/bin)
# 
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY build)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY build)
# 
# # Run antlr
# add_custom_command(OUTPUT src/parser/CommentTextLexer.cpp src/parser/CommentTextLexer.hpp src/parser/CommentTextLexerTokenTypes.txt src/parser/CommentTextLexerTokenTypes.hpp 
# COMMAND ${ANTLR} -o src/parser src/parser/CommentTextLexer.g DEPENDS src/parser/CommentTextLexer.g
# COMMAND touch src/parser/CommentTextLexer.cpp src/parser/CommentTextLexer.hpp src/parser/CommentTextLexerTokenTypes.txt src/parser/CommentTextLexerTokenTypes.hpp src/parser/expandedCommentTextLexer.g 
# )
# 
# add_custom_command(OUTPUT src/parser/TextLexer.cpp src/parser/TextLexer.hpp src/parser/TextLexerTokenTypes.txt src/parser/TextLexerTokenTypes.hpp src/parser/expandedTextLexer.g 
# COMMAND ${ANTLR} -o src/parser src/parser/TextLexer.g DEPENDS src/parser/TextLexer.g src/parser/CommentTextLexer.cpp
# COMMAND touch src/parser/TextLexer.cpp src/parser/TextLexer.hpp src/parser/TextLexerTokenTypes.txt src/parser/TextLexerTokenTypes.hpp src/parser/expandedTextLexer.g 
# )
# 
# add_custom_command(OUTPUT src/parser/OperatorLexer.cpp src/parser/OperatorLexer.hpp src/parser/OperatorLexerTokenTypes.txt src/parser/OperatorLexerTokenTypes.hpp src/parser/expandedOperatorLexer.g 
# COMMAND ${ANTLR} -o src/parser -glib \"src/parser/TextLexer.g\" src/parser/OperatorLexer.g DEPENDS src/parser/OperatorLexer.g src/parser/TextLexer.cpp
# COMMAND touch src/parser/OperatorLexer.cpp src/parser/OperatorLexer.hpp src/parser/OperatorLexerTokenTypes.txt src/parser/OperatorLexerTokenTypes.hpp src/parser/expandedOperatorLexer.g
# )
# 
# add_custom_command(OUTPUT src/parser/KeywordLexer.cpp src/parser/KeywordLexer.hpp src/parser/KeywordLexerTokenTypes.txt src/parser/KeywordLexerTokenTypes.hpp src/parser/expandedKeywordLexer.g
# COMMAND ${ANTLR} -o src/parser -glib \"src/parser/OperatorLexer.g\;src/parser/TextLexer.g\" src/parser/KeywordLexer.g DEPENDS src/parser/KeywordLexer.g src/parser/OperatorLexer.cpp
# COMMAND touch src/parser/KeywordLexer.cpp src/parser/KeywordLexer.hpp src/parser/KeywordLexerTokenTypes.txt src/parser/KeywordLexerTokenTypes.hpp src/parser/expandedKeywordLexer.g
# #COMMAND rm -rf OperatorLexerTokenTypes.txt TextLexerTokenTypes.txt
# )
# 
# add_custom_command(OUTPUT src/parser/srcMLParser.cpp src/parser/srcMLParser.hpp src/parser/srcMLParserTokenTypes.txt src/parser/srcMLParserTokenTypes.hpp
# COMMAND ${ANTLR} -o src/parser -glib \"src/parser/OperatorLexer.g\;src/parser/TextLexer.g\;src/parser/KeywordLexer.g\;\" src/parser/srcMLParser.g DEPENDS src/parser/srcMLParser.g src/parser/KeywordLexer.cpp
# COMMAND touch src/parser/srcMLParser.cpp src/parser/srcMLParser.hpp src/parser/srcMLParserTokenTypes.txt src/parser/srcMLParserTokenTypes.hpp 
# )
# 
# # Run special command for macro
# add_custom_command(OUTPUT src/parser/srcMLParserTokenTypesMacro.hpp 
# COMMAND ${GREP} '=' src/parser/srcMLParserTokenTypes.hpp | ${SED} -e 's/,//g' -e 's/= //g' -e 's/^[ \t]*//' -e 's/^/\#define TOKEN_/g' > src/parser/srcMLParserTokenTypesMacro.hpp DEPENDS src/parser/srcMLParserTokenTypes.hpp
# )
# 
# set(PARSER_SRC src/parser/srcMLParser.cpp;src/parser/KeywordLexer.cpp;src/parser/srcMLOutput.cpp;src/parser/srcMLTranslatorOutput.cpp;src/parser/Mode.cpp;src/parser/CommentTextLexer.cpp;src/parser/srcMLParserTokenTypesMacro.hpp)
# file(GLOB TRANSLATOR_SRC src/translator/*.cpp)
# set(SRCMLAPPS_SRC src/srcml/srcmlapps.cpp)
# file(GLOB LIBSRCML_SRC src/libsrcml/*.cpp src/SAX2Framework/*.cpp)
# 
# if(LIBSRCML_SAX2_ENABLED)
#     add_definitions(-DSAX2)
# endif()
# 
# if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER 2.8.7)
# 
#     add_library(parser OBJECT ${PARSER_SRC})
#     add_library(translator OBJECT ${TRANSLATOR_SRC})
#     add_library(srcmlapps OBJECT ${SRCMLAPPS_SRC})
#     add_library(libsrcml OBJECT ${LIBSRCML_SRC})
# 
# <<<<<<< HEAD
# # Remove this later.
# # MESSAGE(STATUS "")
# # MESSAGE(STATUS "")
# # MESSAGE(STATUS "----------- Testing global properties --------------")
# # 
# # get_property(SAX2_ENABLED GLOBAL PROPERTY SAX2_ENABLED)
# # get_property(LIBXML2_LIBS GLOBAL PROPERTY LIBXML2_LIBS)
# # get_property(LIBXSLT_LIBS GLOBAL PROPERTY LIBXSLT_LIBS)
# # get_property(BOOST_PROGRAM_OPTIONS_LIB GLOBAL PROPERTY BOOST_PROGRAM_OPTIONS_LIB)
# # get_property(WINDOWS_DEP_PATH GLOBAL PROPERTY WINDOWS_DEP_PATH)
# # get_property(ANTLR_LIB GLOBAL PROPERTY ANTLR_LIB)
# # get_property(ANTLR_EXE GLOBAL PROPERTY ANTLR_EXE)
# # get_property(SED_EXE GLOBAL PROPERTY SED_EXE)
# # get_property(GREP_EXE GLOBAL PROPERTY GREP_EXE)
# # get_property(LIBARCHIVE_LIBS  GLOBAL PROPERTY LIBARCHIVE_LIBS)
# # 
# # #  Printing global property values.
# # MESSAGE(STATUS "SVN_ENABLED ${SVN_ENABLED}")
# # MESSAGE(STATUS "SAX2_ENABLED ${SAX2_ENABLED}")
# # MESSAGE(STATUS "LIBXML2_LIBS: ${LIBXML2_LIBS}")
# # MESSAGE(STATUS "LIBXSLT_LIBS: ${LIBXSLT_LIBS}")
# # MESSAGE(STATUS "BOOST_PROGRAM_OPTIONS_LIB: ${BOOST_PROGRAM_OPTIONS_LIB}")
# # MESSAGE(STATUS "WINDOWS_DEP_PATH: ${WINDOWS_DEP_PATH}")
# # MESSAGE(STATUS "ANTLR_LIB: ${ANTLR_LIB}")
# # MESSAGE(STATUS "ANTLR_EXE: ${ANTLR_EXE}")
# # MESSAGE(STATUS "SED_EXE: ${SED_EXE}")
# # MESSAGE(STATUS "GREP_EXE: ${GREP_EXE}")
# # MESSAGE(STATUS "LIBARCHIVE_LIBS: ${LIBARCHIVE_LIBS}")
# # MESSAGE(STATUS "")
# # MESSAGE(STATUS "")
# =======
#     # libsrcml static
#     add_library(srcml_static STATIC $<TARGET_OBJECTS:libsrcml> $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:translator> $<TARGET_OBJECTS:srcmlapps>)
# else()
#     add_library(srcml_static STATIC ${LIBSRCML_SRC} ${PARSER_SRC} ${TRANSLATOR_SRC} ${SRCMLAPPS_SRC})
# endif()
# 
# target_link_libraries(srcml_static ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB} ${LIBREGEX_LIBRARY} ${LINK_EXTRA})
# set_target_properties(srcml_static PROPERTIES OUTPUT_NAME srcml)
# 
# if(CMAKE_SYSTEM_NAME MATCHES "Windows")
# 
#     # libsrcml shared
#     if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER 2.8.7)
#         add_library(srcml_shared SHARED $<TARGET_OBJECTS:libsrcml> $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:translator> $<TARGET_OBJECTS:srcmlapps>)
#     else()
#         add_library(srcml_shared SHARED ${LIBSRCML_SRC} ${PARSER_SRC} ${TRANSLATOR_SRC} ${SRCMLAPPS_SRC})
#     endif()
#     target_link_libraries(srcml_shared ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} ${LIBREGEX_LIBRARY})
#     set_target_properties(srcml_shared PROPERTIES OUTPUT_NAME srcml)
# else()
# 
#     # libsrcml shared
#     if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER 2.8.7)
#         add_library(srcml_shared SHARED $<TARGET_OBJECTS:libsrcml> $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:translator> $<TARGET_OBJECTS:srcmlapps>)
# else()
#        add_library(srcml_shared SHARED ${LIBSRCML_SRC} ${PARSER_SRC} ${TRANSLATOR_SRC} ${SRCMLAPPS_SRC})
#     endif()
#     target_link_libraries(srcml_shared ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB} ${LINK_EXTRA})
#     set_target_properties(srcml_shared PROPERTIES OUTPUT_NAME srcml)
# endif()
# 
# # srcml
# add_executable(srcml EXCLUDE_FROM_ALL src/srcml/srcml.cpp src/srcml/srcmlCLI.cpp)
# target_link_libraries(srcml ${Boost_PROGRAM_OPTIONS_LIBRARY} ${LibArchive_LIBRARY} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} ${CURL_LIBRARIES} srcml_static)
# 
# # src2srcml
# if(NOT SVN_ENABLED)
#     add_executable(src2srcml src/oldclient/src2srcml.cpp)
# else()
#     add_executable(src2srcml src/oldclient/src2srcml.cpp src/oldclient/svn_io.cpp)
# endif()
# target_link_libraries(src2srcml ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LIBANTLR_LIB} ${SRC2SRCML_LIBS} ${LINK_EXTRA})
# 
# # srcml2src
# add_executable(srcml2src src/oldclient/srcml2src.cpp)
# target_link_libraries(srcml2src ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${SRCML2SRC_LIBS} ${LINK_EXTRA})
# 
# # install handling
# include(CMake/install.cmake)
# 
# # cpack
# include(CMake/packageConfig.cmake)
# >>>>>>> srcML/master
