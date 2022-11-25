# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
# 
# CMake files for building the parser

# TinySHA1 external include
include(FetchContent)
FetchContent_Declare(TinySHA1
  GIT_REPOSITORY https://github.com/mohaps/tinysha1
  GIT_TAG        2795aa8de91b1797defdfbff61ed93b22b5ced81
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(TinySHA1)
add_library(TinySHA1 INTERFACE IMPORTED)
target_include_directories(TinySHA1 INTERFACE ${tinysha1_SOURCE_DIR})

find_package(LibXml2 REQUIRED)
find_package(Iconv REQUIRED)
find_package(Java COMPONENTS Runtime REQUIRED)

# Setup ANTLR 2.7 library
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(antlrsrc
    URL https://www.antlr2.org/download/antlr-2.7.7.tar.gz
    PATCH_COMMAND patch -p 1 -i ${CMAKE_CURRENT_SOURCE_DIR}/antlr.patch
    URL_HASH MD5=01cc9a2a454dd33dcd8c856ec89af090
)
FetchContent_MakeAvailable(antlrsrc)

# antlr library
file(GLOB ANTLR_SOURCE ${antlrsrc_SOURCE_DIR}/lib/cpp/src/*.cpp)
list(REMOVE_ITEM ANTLR_SOURCE "${antlrsrc_SOURCE_DIR}/lib/cpp/src/dll.cpp")
add_library(antlr OBJECT "${ANTLR_SOURCE}")
target_include_directories(antlr SYSTEM PUBLIC "${antlrsrc_SOURCE_DIR}/lib/cpp/" SYSTEM PRIVATE "${antlrsrc_SOURCE_DIR}/lib/cpp/")

# With MSVC, interprocedural optimization leads to a 10x static library, with only a slight reduction in size for the dynamic library
set_target_properties(antlr PROPERTIES INTERPROCEDURAL_OPTIMIZATION $<IF:$<CXX_COMPILER_ID:MSVC>,OFF,ON> POSITION_INDEPENDENT_CODE ON
    OPTIMIZE_DEPENDENCIES YES)

# Generated source from antlr and the parser
set(CMAKE_GENERATED_SOURCE_DIR ${CMAKE_BINARY_DIR}/parser)

# Find antlr runtime
set(ANTLR_EXE ${Java_JAVA_EXECUTABLE} -classpath "${antlrsrc_SOURCE_DIR}/antlr.jar" antlr.Tool)
message("-- ANTLR: ${ANTLR_EXE}")

# parser library
file(GLOB PARSER_SOURCE *.hpp *.cpp)
add_library(parser OBJECT
    ${CMAKE_GENERATED_SOURCE_DIR}/srcMLParser.cpp
    ${CMAKE_GENERATED_SOURCE_DIR}/KeywordLexer.cpp
    ${CMAKE_GENERATED_SOURCE_DIR}/CommentTextLexer.cpp
    ${PARSER_SOURCE}
)
target_include_directories(parser SYSTEM PUBLIC ${antlrsrc_SOURCE_DIR}/lib/cpp)
target_include_directories(parser PRIVATE . ${CMAKE_GENERATED_SOURCE_DIR} PUBLIC . SYSTEM PUBLIC ${CMAKE_GENERATED_SOURCE_DIR})
target_link_libraries(parser PRIVATE LibXml2::LibXml2 antlr)
target_include_directories(parser PRIVATE $<TARGET_PROPERTY:TinySHA1,INTERFACE_INCLUDE_DIRECTORIES>)

# With MSVC, interprocedural optimization leads to a 10x static library, with only a slight reduction in size for the dynamic library
set_target_properties(parser PROPERTIES INTERPROCEDURAL_OPTIMIZATION $<IF:$<CXX_COMPILER_ID:MSVC>,OFF,ON> POSITION_INDEPENDENT_CODE ON
    OPTIMIZE_DEPENDENCIES YES)

# 
# RunAntlr
# Executes ANTLR executable on the provided files.
# 
# Takes 3 parameters OUTPUT_FILES INPUT_FILES DEPENDENCIES and 
#    1 optional parameter INCLUDE_GRAMMAR
# Use with named arguments.
#
macro(RunAntlr OUTPUT_FILES INPUT_FILES DEPENDENCIES INCLUDE_GRAMMAR)

    add_custom_command(OUTPUT  ${OUTPUT_FILES}
        DEPENDS ${INPUT_FILES} ${DEPENDENCIES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${ANTLR_EXE} -o \"${CMAKE_GENERATED_SOURCE_DIR}\" -glib \"${INCLUDE_GRAMMAR}\" ${INPUT_FILES}
        COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT_FILES}
    )

endmacro(RunAntlr)

macro(lexerFiles LEXER)
    set(${LEXER}LexerOutputFiles
        ${CMAKE_GENERATED_SOURCE_DIR}/${LEXER}Lexer.cpp
        ${CMAKE_GENERATED_SOURCE_DIR}/${LEXER}Lexer.hpp
        ${CMAKE_GENERATED_SOURCE_DIR}/${LEXER}LexerTokenTypes.hpp
        ${CMAKE_GENERATED_SOURCE_DIR}/${LEXER}LexerTokenTypes.txt
        ${CMAKE_GENERATED_SOURCE_DIR}/expanded${LEXER}Lexer.g
    )
endmacro()

# Running ANTLR on CommentTextLexer.g
lexerFiles(CommentText)
RunAntlr("${CommentTextLexerOutputFiles}"
    CommentTextLexer.g
    ""
    ""
)

# Running ANTLR on TextLexer.g
lexerFiles(Text)
RunAntlr("${TextLexerOutputFiles}"
    TextLexer.g
    ${CMAKE_GENERATED_SOURCE_DIR}/CommentTextLexer.cpp
    ""
)

# Running ANTLR on OperatorLexer.g
lexerFiles(Operator)
RunAntlr("${OperatorLexerOutputFiles}"
    OperatorLexer.g
    ${CMAKE_GENERATED_SOURCE_DIR}/TextLexer.cpp
    TextLexer.g
)

# Running ANTLR on KeywordLexer.g
lexerFiles(Keyword)
RunAntlr("${KeywordLexerOutputFiles}"
    KeywordLexer.g
    ${CMAKE_GENERATED_SOURCE_DIR}/OperatorLexer.cpp
    "OperatorLexer.g\;TextLexer.g"
)

# Running ANTLR on srcMLParser.g
set(srcMLParserOutputFiles
    ${CMAKE_GENERATED_SOURCE_DIR}/srcMLParser.cpp
    ${CMAKE_GENERATED_SOURCE_DIR}/srcMLParser.hpp
    ${CMAKE_GENERATED_SOURCE_DIR}/srcMLParserTokenTypes.hpp
    ${CMAKE_GENERATED_SOURCE_DIR}/srcMLParserTokenTypes.txt 
)
RunAntlr("${srcMLParserOutputFiles}"
    srcMLParser.g
    "srcMLParser.g;${CMAKE_GENERATED_SOURCE_DIR}/KeywordLexer.cpp"
    "OperatorLexer.g\;KeywordLexer.g\;TextLexer.g"
)

# set_target_properties(libsrcml PROPERTIES INTERPROCEDURAL_OPTIMIZATION $<NOT:$<CXX_COMPILER_ID:MSVC>,<UTFCharBuffer.hpp>>)

if(SRCML_BUILD_USEPCH)
    # Not sure why <UTF8CharBuffer> is non-MSVC. Perhaps an error when compiled
    target_precompile_headers(parser PRIVATE <srcMLParser.hpp> <CommentTextLexer.hpp> <ModeStack.hpp> <Language.hpp> $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:<UTF8CharBuffer.hpp$<ANGLE-R>>)
endif()

option(ENABLE_ANTLR_BUILD_WARNINGS "Enable disabled antlr library build warnings" OFF)
if(NOT ENABLE_ANTLR_BUILD_WARNINGS)
    target_compile_options(antlr PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wno-format>
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wno-misleading-indentation>
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wno-deprecated-declarations>
        $<$<CXX_COMPILER_ID:MSVC>:/wd4365> # conversion from 'int' to 'unsigned int',
        $<$<CXX_COMPILER_ID:MSVC>:/wd4267> # 'return': conversion from 'size_t' to 'unsigned int', possible loss of data
        $<$<CXX_COMPILER_ID:MSVC>:/wd4244> # 'argument': conversion from 'unsigned int' to '_Elem', possible loss of data
        $<$<CXX_COMPILER_ID:MSVC>:/wd4458> # declaration of 'index' hides class member
        $<$<CXX_COMPILER_ID:MSVC>:/wd4242> # 'argument': conversion from 'T' to '_Elem', possible loss of data
        $<$<CXX_COMPILER_ID:MSVC>:/wd4244> # 'argument': conversion from 'T' to '_Elem', possible loss of data
        $<$<CXX_COMPILER_ID:MSVC>:/wd4100> # unreferenced formal parameter
        $<$<CXX_COMPILER_ID:MSVC>:/wd4355> # this': used in base member initializer list
        $<$<CXX_COMPILER_ID:MSVC>:/wd4365> # 'argument': conversion from 'size_t' to 'const __int64', signed/unsigned mismatch
        $<$<CXX_COMPILER_ID:MSVC>:/wd4477> # 'sprintf' : format string '%u' requires an argument of type 'unsigned int', but variadic argument 1 has type 'size_t'
        $<$<CXX_COMPILER_ID:MSVC>:/wd5031> # #pragma warning(pop): likely mismatch, popping warning state pushed in different file
    )
    target_compile_options(parser PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wno-unused-but-set-variable>
        $<$<CXX_COMPILER_ID:MSVC>:/wd4365> # conversion from 'int' to 'unsigned int',
        $<$<CXX_COMPILER_ID:MSVC>:/wd4267> # 'return': conversion from 'size_t' to 'unsigned int', possible loss of data
        $<$<CXX_COMPILER_ID:MSVC>:/wd4244> # 'argument': conversion from 'unsigned int' to '_Elem', possible loss of data
        $<$<CXX_COMPILER_ID:MSVC>:/wd4458> # declaration of 'index' hides class member
        $<$<CXX_COMPILER_ID:MSVC>:/wd4242> # 'argument': conversion from 'T' to '_Elem', possible loss of data
        $<$<CXX_COMPILER_ID:MSVC>:/wd4244> # 'argument': conversion from 'T' to '_Elem', possible loss of data
        $<$<CXX_COMPILER_ID:MSVC>:/wd4100> # unreferenced formal parameter
        $<$<CXX_COMPILER_ID:MSVC>:/wd4355> # this': used in base member initializer list
        $<$<CXX_COMPILER_ID:MSVC>:/wd4365> # 'argument': conversion from 'size_t' to 'const __int64', signed/unsigned mismatch
        $<$<CXX_COMPILER_ID:MSVC>:/wd4477> # 'sprintf' : format string '%u' requires an argument of type 'unsigned int', but variadic argument 1 has type 'size_t'
        $<$<CXX_COMPILER_ID:MSVC>:/wd5031> # #pragma warning(pop): likely mismatch, popping warning state pushed in different file
    )
endif()
