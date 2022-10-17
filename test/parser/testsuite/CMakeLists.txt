# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
# 
# CMake files for the parser tests

message(STATUS "Generating Parser Testfiles:")

set(XSLT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../xsl")

# copy starting set of test cases
file(GLOB PARSE_TESTS *.xml)
foreach(PATH IN ITEMS ${PARSE_TESTS})
    get_filename_component(FILENAME ${PATH} NAME)

    # Set newline at LF since these are srcML files and srcML uses LF (as does XML)
    # Do not use file(COPY) as it changes the line endings
    # Do not use COPYFILE_ONLY as it changes the line endings
    configure_file(${FILENAME} ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME} NEWLINE_STYLE LF)
endforeach()

set(TESTFILES)

# convert from input language to input extension
set(LANGLIST "C++" "cpp" "C" "c" "Java" "java" "C#" "cs" "Objective-C" "m")
function(lang2ext LANG EXT)
    list(FIND LANGLIST "${LANG}" LANG_POS)
    math(EXPR EXT_POS "${LANG_POS} + 1")
    list(GET LANGLIST "${EXT_POS}" INFILE_EXT)
    set(${EXT} "${INFILE_EXT}" PARENT_SCOPE)
endfunction()

# Sets the language of the result
macro(setlanguage INLANG URL)

    # convert from input language to input extension
    lang2ext("${INLANG}" INFILE_EXT)

    # assumes a C++ base
    set(BASEFILE "${URL}.${INFILE_EXT}.xml")

    set(PRODUCT_LANGS ${ARGN})
    foreach(LANG IN ITEMS ${ARGN})

        # convert from language to extension
        lang2ext("${LANG}" LANGEXT)

        # strip _base from URL
        # string(REGEX REPLACE "_base" "" PRODURL "${URL}")
        # set(PRODUCT "${PRODURL}.${LANGEXT}.xml")
        set(OUTFILE "${URL}.${LANGEXT}.xml")

        add_custom_command(COMMAND ${SRCML_EXE} --output-srcml --language=${LANG} ${BASEFILE} -o ${OUTFILE}
                    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${BASEFILE}
                    OUTPUT ${OUTFILE}
                    COMMENT "Generating parser testfile ${OUTFILE} from ${BASEFILE}"
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

        list(APPEND TESTFILES "${OUTFILE}")
    endforeach()
endmacro()

# Transforms BASE into PRODUCT with XSLT and applies the given URL
macro(transform OUTFILE INFILE URL XSLT)

    # set(EXTRA)
    # set(IN_FILES ${ARGN})
    # foreach(FILE ${IN_FILES})
    #     set(EXTRA "${EXTRA} --xslt-param=${FILE}")
    # endforeach()

    # Passing xslt params is not working, so set here for insertexpr
    set(EXTRA --xslt-param=expr_filename=${CMAKE_CURRENT_BINARY_DIR}/expression.cpp.xml)

    add_custom_command(COMMAND ${SRCML_EXE} --output-srcml --xslt ${XSLT_DIR}/${XSLT} ${EXTRA} --url=${URL} ${INFILE} -o ${OUTFILE}
                       OUTPUT ${OUTFILE}
                       DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${INFILE}
                       COMMENT "Generating parser testfile ${OUTFILE} from ${INFILE}"
                       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    list(APPEND TESTFILES "${OUTFILE}")
endmacro()

# Generate special transformations
message(STATUS "  Generating special transformation targets")

# Set newline at LF since these are srcML files and srcML uses LF (as does XML)
# Do not use file(COPY) as it changes the line endings
# Do not use COPYFILE_ONLY as it changes the line endings
configure_file(${CMAKE_CURRENT_BINARY_DIR}/constructor_base.cpp.xml ${CMAKE_CURRENT_BINARY_DIR}/constructor.cpp.xml NEWLINE_STYLE LF)

add_custom_command(COMMAND ${SRCML_EXE} --language=Java --output-srcml finally_cs.cs.xml -o finally_java.java.xml
                   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/finally_cs.cs.xml
                   OUTPUT finally_java.java.xml
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

message(STATUS "  Generating suite targets")

# Process all tests in suite.txt according to their categories
file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/suite.txt SUITE REGEX "^LANGUAGE_")
foreach(GEN IN ITEMS ${SUITE})

    # split into category and url
    separate_arguments(GEN)
    list(GET GEN 0 CATEGORY)
    list(GET GEN 1 URL)
    list(SUBLIST GEN 1 -1 FULLURL)

    if(CATEGORY STREQUAL "LANGUAGE_ALL_BASE")

        # base is C++, so these just get added
        list(APPEND TESTFILES "${URL}.cpp.xml")
        setlanguage("C++" "${URL}" "Java" "C#" "Objective-C" "C")

    elseif(CATEGORY STREQUAL "LANGUAGE_ALL_GEN")

        # explicitly generated
        list(APPEND TESTFILES "${URL}.cpp.xml")
        setlanguage("C++" "${URL}" "Java" "C#" "Objective-C" "C")

    elseif(CATEGORY STREQUAL "LANGUAGE_CSHARP")

        # exist, just add to list
        list(APPEND TESTFILES "${URL}.cs.xml")

    elseif(CATEGORY STREQUAL "LANGUAGE_CSHARP_GEN")

        # explicitly generated
        #list(APPEND TESTFILES "${URL}.cs.xml")

    elseif(CATEGORY STREQUAL "LANGUAGE_CXX")

        # exist, just add to list
        list(APPEND TESTFILES "${URL}.cpp.xml")

    elseif(CATEGORY STREQUAL "LANGUAGE_CXX_FAMILY")

        list(APPEND TESTFILES "${URL}.cpp.xml")
        setlanguage("C++" "${URL}" "C#")

    elseif(CATEGORY STREQUAL "LANGUAGE_CXX_GEN")

    elseif(CATEGORY STREQUAL "LANGUAGE_NOP")

    elseif(CATEGORY STREQUAL "LANGUAGE_C_FAMILY")

        list(APPEND TESTFILES "${URL}.cpp.xml")
        setlanguage("C++" "${URL}" "C" "C#" "Objective-C")

    elseif(CATEGORY STREQUAL "LANGUAGE_C_FAMILY_NO_SHARP")

        list(APPEND TESTFILES "${URL}.cpp.xml")
        setlanguage("C++" "${URL}" "C" "Objective-C")
 
    elseif(CATEGORY STREQUAL "LANGUAGE_C_ONLY")

        list(APPEND TESTFILES "${URL}.c.xml")
        setlanguage("C" "${URL}" "Objective-C")

    elseif(CATEGORY STREQUAL "LANGUAGE_JAVA")

        list(APPEND TESTFILES "${URL}.java.xml")

    elseif(CATEGORY STREQUAL "LANGUAGE_JAVA_GEN")

    elseif(CATEGORY STREQUAL "LANGUAGE_OBJECTIVE_C")

        list(APPEND TESTFILES "${URL}.m.xml")

    elseif(CATEGORY STREQUAL "LANGUAGE_OBJECTIVE_C_GEN")

    elseif(CATEGORY STREQUAL "LANGUAGE_OO")

        # base is C++, so these just get added
        list(APPEND TESTFILES "${URL}.cpp.xml")
        setlanguage("C++" "${URL}" "C#" "Java")


    elseif(CATEGORY STREQUAL "LANGUAGE_TRANSFORM")

        transform(${FULLURL})

    endif()

endforeach()

list(SORT TESTFILES)

# this should not be necessary, but without it there are some issues
# with empty srcML files
list(REMOVE_DUPLICATES TESTFILES)

message(STATUS "  Generating context variation targets")

set(CONTEXT_FILES)

# Generate variations: formfeed, comment, block, etc.
foreach(GENURL IN ITEMS ${TESTFILES})

    # ${URL}.${LANGEXT}.xml
    get_filename_component(SRCFILENAME "${GENURL}" NAME_WLE)
    get_filename_component(URL "${SRCFILENAME}" NAME_WLE)
    get_filename_component(EXTENSION "${SRCFILENAME}" LAST_EXT)
    # remove '.'
    string(SUBSTRING "${EXTENSION}" 1 -1 LANGEXT)

    # Generate "formfeed" "comment" "block" "struct" "ifthenelse"
    foreach(VARIATION IN ITEMS "formfeed" "comment" "block" "struct" "preproc" "ifthenelse")
        # preproc and struct do not work with Java
        if((LANGEXT STREQUAL "java" AND (VARIATION STREQUAL "struct" OR VARIATION STREQUAL "preproc")) OR
           (LANGEXT STREQUAL "cs" AND VARIATION STREQUAL "struct"))
            continue()
        endif()

        add_custom_command(
            COMMAND ${SRCML_EXE} --output-srcml --xslt ${XSLT_DIR}/insert${VARIATION}.xsl --url=${URL}.${VARIATION} ${GENURL}
                     -o ${URL}.${VARIATION}.${LANGEXT}.xml
            COMMENT "Generating parser testfile ${URL}.${VARIATION}.${LANGEXT}.xml from ${GENURL}"
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${GENURL}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            OUTPUT ${URL}.${VARIATION}.${LANGEXT}.xml
        )
        list(APPEND CONTEXT_FILES ${URL}.${VARIATION}.${LANGEXT}.xml)
    endforeach()

    # all is generated separately because it does not use an XSLT transformation
    add_custom_command(
        COMMAND ${SRCML_EXE} --cat --url=${URL}.all ${GENURL} -o ${URL}.all.${LANGEXT}.xml
        COMMENT "Generating parser testfile ${URL}.all.${LANGEXT}.xml from ${GENURL}"
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${GENURL}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        OUTPUT ${URL}.all.${LANGEXT}.xml
    )
    list(APPEND CONTEXT_FILES ${URL}.all.${LANGEXT}.xml)

endforeach()

# specify the build directory, as the WORKING_DIRECTORY does not always seem to
# be used in the DEPENDS
list(TRANSFORM CONTEXT_FILES PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")

# display number of parser testfiles
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/.count "echo '-- Total Parser Testfiles: ' | tr -d '\n'; ls *.xml | wc -l | tr -d ' '")
add_custom_command(
    COMMAND . ./.count
    COMMENT ""
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${CONTEXT_FILES}
    OUTPUT parser_testfile_count
)

# generate the parser tests
if(NOT WIN32)
add_custom_target(gen_parser_tests DEPENDS parser_testfile_count
                  COMMENT "Generated parser testfile target"
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
else()
add_custom_target(gen_parser_tests DEPENDS ${CONTEXT_FILES}
                  COMMENT "Generated parser testfile target"
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

# clean all parser tests, original and generated
add_custom_target(clean_parser_tests
                  COMMAND ${CMAKE_COMMAND} -E remove *.xml
                  COMMENT "Clean all generated parser testfiles"
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set(TESTSUITE *.xml)

set(FLAGS "")
if(WIN32)
     set(FLAGS "--no-color")
endif()

# targets to manage parser testing
add_custom_target(run_parser_tests        COMMENT "Run all parser test cases"
                         COMMAND ${SRCML_EXE} --parser-test ${FLAGS} ${TESTSUITE}
                         DEPENDS gen_parser_tests
                         USES_TERMINAL
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_target(run_parser_tests_c      COMMENT "Run C parser test cases"
                         COMMAND ${SRCML_EXE} --parser-test ${FLAGS} --language=C    ${TESTSUITE}
                         DEPENDS gen_parser_tests
                         USES_TERMINAL
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_target(run_parser_tests_cpp    COMMENT "Run C++ parser test cases"
                         COMMAND ${SRCML_EXE} --parser-test ${FLAGS} --language=C++  ${TESTSUITE}
                         DEPENDS gen_parser_tests
                         USES_TERMINAL
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_target(run_parser_tests_C++    COMMENT "Run C++ parser test cases"
                         COMMAND ${SRCML_EXE} --parser-test ${FLAGS} --language=C++  ${TESTSUITE}
                         DEPENDS gen_parser_tests
                         USES_TERMINAL
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_target(run_parser_tests_java   COMMENT "Run Java parser test cases"
                         COMMAND ${SRCML_EXE} --parser-test ${FLAGS} --language=Java ${TESTSUITE}
                         DEPENDS gen_parser_tests
                         USES_TERMINAL
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_target(run_parser_tests_cs     COMMENT "Run C\# language parser test cases"
                         COMMAND ${SRCML_EXE} --parser-test ${FLAGS} --language=C\#  ${TESTSUITE}
                         DEPENDS gen_parser_tests
                         USES_TERMINAL
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_target(run_parser_tests_CSHARP COMMENT "Run C\# language parser test cases"
                         COMMAND ${SRCML_EXE} --parser-test ${FLAGS} --language=C\#  ${TESTSUITE}
                         DEPENDS gen_parser_tests
                         USES_TERMINAL
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
