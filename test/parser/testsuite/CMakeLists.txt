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
file(GLOB DEFINED_TEST_FILES *.xml)
foreach(PATH IN ITEMS ${DEFINED_TEST_FILES})
    get_filename_component(FILENAME ${PATH} NAME)

    # Set newline at LF since these are srcML files and srcML uses LF (as does XML)
    # Do not use file(COPY) as it changes the line endings
    # Do not use COPYFILE_ONLY as it changes the line endings
    configure_file(${FILENAME} ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME} NEWLINE_STYLE LF)
endforeach()

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

    # assume a C++ base
    set(BASEFILE "${URL}.${INFILE_EXT}.xml")

    set(PRODUCT_LANGS ${ARGN})
    foreach(LANG IN ITEMS ${ARGN})

        # convert from language to extension
        lang2ext("${LANG}" LANGEXT)

        # strip _base from URL
        # string(REGEX REPLACE "_base" "" PRODURL "${URL}")
        # set(PRODUCT "${PRODURL}.${LANGEXT}.xml")
        set(OUTFILE "${URL}.${LANGEXT}.xml")

        # generate new language file
        add_custom_command(COMMAND ${SRCML_EXE} --output-srcml --language=${LANG} ${BASEFILE} -o ${OUTFILE}
                    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${BASEFILE}
                    OUTPUT ${OUTFILE}
                    COMMENT "Generating parser testfile ${OUTFILE} from ${BASEFILE}"
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

        track("${OUTFILE}")
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

    track("${OUTFILE}")
endmacro()

# Track all generated files
set(GENERATED_TEST_FILES)
macro(track URL)
    list(APPEND GENERATED_TEST_FILES "${URL}")
    set(GENERATED_TEST_FILES "${GENERATED_TEST_FILES}" PARENT_SCOPE)
endmacro()

# Generate special transformations
message(STATUS "  Generating special transformation targets")

# Set newline at LF since these are srcML files and srcML uses LF (as does XML)
# Do not use file(COPY) as it changes the line endings
# Do not use COPYFILE_ONLY as it changes the line endings
configure_file(${CMAKE_CURRENT_BINARY_DIR}/constructor_base.cpp.xml ${CMAKE_CURRENT_BINARY_DIR}/constructor.cpp.xml NEWLINE_STYLE LF)
track(constructor.cpp.xml)

add_custom_command(COMMAND ${SRCML_EXE} --language=Java --output-srcml finally_cs.cs.xml -o finally_java.java.xml
                   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/finally_cs.cs.xml
                   OUTPUT finally_java.java.xml
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
track(finally_java.java.xml)

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

        setlanguage("C++" "${URL}" "Java" "C#" "Objective-C" "C")

    elseif(CATEGORY STREQUAL "LANGUAGE_ALL_GEN")

        setlanguage("C++" "${URL}" "Java" "C#" "Objective-C" "C")

    elseif(CATEGORY STREQUAL "LANGUAGE_CXX_FAMILY")

        setlanguage("C++" "${URL}" "C#")

    elseif(CATEGORY STREQUAL "LANGUAGE_C_FAMILY")

        setlanguage("C++" "${URL}" "C" "C#" "Objective-C")

    elseif(CATEGORY STREQUAL "LANGUAGE_C_FAMILY_NO_SHARP")

        setlanguage("C++" "${URL}" "C" "Objective-C")

    elseif(CATEGORY STREQUAL "LANGUAGE_C_ONLY")

        setlanguage("C" "${URL}" "Objective-C")

    elseif(CATEGORY STREQUAL "LANGUAGE_OO")

        setlanguage("C++" "${URL}" "C#" "Java")

    elseif(CATEGORY STREQUAL "LANGUAGE_TRANSFORM")

        transform(${FULLURL})

    endif()

endforeach()

set(ALLFILES ${DEFINED_TEST_FILES})
list(APPEND ALLFILES ${GENERATED_TEST_FILES})
list(SORT ALLFILES)

message(STATUS "  Generating context variation targets")

# Generate variations: formfeed, comment, block, etc.
foreach(GENURL IN ITEMS ${ALLFILES})

    # ${URL}.${LANGEXT}.xml
    get_filename_component(SRCFILENAME "${GENURL}" NAME_WLE)
    get_filename_component(URL "${SRCFILENAME}" NAME_WLE)
    get_filename_component(EXTENSION "${SRCFILENAME}" LAST_EXT)

    if(URL STREQUAL "comment_eof" OR URL STREQUAL "puretypedecl" OR URL STREQUAL "throws_java")
        continue()
    endif()

    # remove '.'
    string(SUBSTRING "${EXTENSION}" 1 -1 LANGEXT)

    # Generate "formfeed" "comment" "block" "struct" "ifthenelse"
    foreach(VARIATION IN ITEMS "formfeed" "comment" "block" "struct" "preproc" "ifthenelse")
        # preproc and struct do not work with Java
        # struct does not work with C#
        # visual_cxx_asm and openmp do not work with preproc
        if((LANGEXT STREQUAL "java" AND (VARIATION STREQUAL "struct" OR VARIATION STREQUAL "preproc")) OR
           (LANGEXT STREQUAL "cs" AND VARIATION STREQUAL "struct") OR
           URL STREQUAL "visual_cxx_asm" AND VARIATION STREQUAL "preproc" OR
           URL STREQUAL "openmp" AND VARIATION STREQUAL "preproc")
            continue()
        endif()

        add_custom_command(
            COMMAND ${SRCML_EXE} --output-srcml --xslt ${XSLT_DIR}/insert${VARIATION}.xsl --url=${URL}.${VARIATION} ${GENURL}
                     -o ${URL}.${VARIATION}.${LANGEXT}.xml
            COMMENT "Generating parser testfile ${URL}.${VARIATION}.${LANGEXT}.xml from ${GENURL}"
            DEPENDS ${GENURL}
            # DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${GENURL}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            OUTPUT ${URL}.${VARIATION}.${LANGEXT}.xml
        )
        track(${URL}.${VARIATION}.${LANGEXT}.xml)
    endforeach()

    # No all for elseif
    if(URL STREQUAL "elseif")
        continue()
    endif()

    # all is generated separately because it does not use an XSLT transformation
    add_custom_command(
        COMMAND ${SRCML_EXE} --cat --url=${URL}.all ${GENURL} -o ${URL}.all.${LANGEXT}.xml
        COMMENT "Generating parser testfile ${URL}.all.${LANGEXT}.xml from ${GENURL}"
        DEPENDS ${GENURL}
        # DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${GENURL}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        OUTPUT ${URL}.all.${LANGEXT}.xml
    )
    track(${URL}.all.${LANGEXT}.xml)

endforeach()

list(LENGTH GENERATED_TEST_FILES LENGTH)
message(STATUS "  Generated test files: ${LENGTH}")

# generate parser tests
add_custom_target(gen_parser_tests
                  DEPENDS ${GENERATED_TEST_FILES}
                  COMMENT "Generated parser testfile target"
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# clean parser tests
add_custom_target(clean_parser_tests
                  COMMAND ${CMAKE_COMMAND} -E remove ${GENERATED_TEST_FILES}
                  COMMENT "Clean all generated parser testfiles"
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# run parser tests
set(SRCML_PARSER_TEST_TESTSUITE *.xml)
find_program(BASH_EXE bash)
if(${BASH_EXE_FOUND})
    # If the SRCML_PARSER_TEST_LANGUAGE exists, use the current value to limit to that language
    # If not, then no language is specified
    add_custom_target(run_parser_tests
                      COMMAND bash -c "${SRCML_EXE} --parser-test $([[ -z $$SRCML_PARSER_TEST_LANGUAGE ]] || echo \"--language $$SRCML_PARSER_TEST_LANGUAGE\") ${SRCML_PARSER_TEST_FLAGS} ${SRCML_PARSER_TEST_TESTSUITE}"
                      COMMENT "Run all parser test cases"
                      DEPENDS gen_parser_tests
                      USES_TERMINAL
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

elseif(DEFINED ENV{SRCML_PARSER_TEST_LANGUAGE})
    # If the SRCML_PARSER_TEST_LANGUAGE exists, use the current value when cmake is run to limit to that language
    add_custom_target(run_parser_tests
                      COMMAND ${SRCML_EXE} --parser-test --language $$SRCML_PARSER_TEST_LANGUAGE ${SRCML_PARSER_TEST_FLAGS} ${SRCML_PARSER_TEST_TESTSUITE}
                      COMMENT "Run all parser test cases"
                      DEPENDS gen_parser_tests
                      USES_TERMINAL
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
else()
    # Do not limit to a language
    add_custom_target(run_parser_tests
                      COMMAND ${SRCML_EXE} --parser-test ${SRCML_PARSER_TEST_FLAGS} ${SRCML_PARSER_TEST_TESTSUITE}
                      COMMENT "Run all parser test cases"
                      DEPENDS gen_parser_tests
                      USES_TERMINAL
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()
