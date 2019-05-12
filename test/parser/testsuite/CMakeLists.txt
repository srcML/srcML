##
# @file CMakeLists.txt
#
# @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
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

message(STATUS "Generating Parser Testfiles:")

set(XSLT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../xsl")

file(GLOB PARSE_TESTS *.xml)
foreach(PATH ${PARSE_TESTS})
    get_filename_component(F ${PATH} NAME)
    configure_file(${F} ${CMAKE_CURRENT_BINARY_DIR}/${F} COPYONLY)
endforeach()

set(ALLGEN)

# Language name to extension mapping
set(LANGLIST "C++" "cpp" "C" "c" "Java" "java" "C#" "cs" "Objective-C" "m")

# Sets the language of the result
macro(setlanguage INLANG URL)

    # convert from input language to input extension
    list(FIND LANGLIST "${INLANG}" INLANGPOS)
    math(EXPR INLANGEXTPOS "${INLANGPOS} + 1")
    list(GET LANGLIST "${INLANGEXTPOS}" BASEEXT)

    # assumes a C++ base
    set(BASE "${URL}.${BASEEXT}.xml")

    set(PRODUCT_LANGS ${ARGN})
    foreach(LANG ${PRODUCT_LANGS})

        # convert from language to extension
        list(FIND LANGLIST "${LANG}" LANGPOS)
        math(EXPR LANGEXTPOS "${LANGPOS} + 1")
        list(GET LANGLIST "${LANGEXTPOS}" LANGEXT)

        # strip _base from URL
        # string(REGEX REPLACE "_base" "" PRODURL "${URL}")
        # set(PRODUCT "${PRODURL}.${LANGEXT}.xml")
        set(PRODUCT "${URL}.${LANGEXT}.xml")

        add_custom_command(COMMAND srcml --output-xml --language=${LANG} ${BASE} -o ${PRODUCT}
                    DEPENDS ${BASE}
                    OUTPUT ${PRODUCT}
                    COMMENT "Generating parser testfile ${PRODUCT}"
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

        list(APPEND ALLGEN "${PRODUCT}")
    endforeach()
endmacro()

# Transforms BASE into PRODUCT with XSLT and applies the given URL
macro(transform PRODUCT BASE URL XSLT)

    # set(EXTRA)
    # set(IN_FILES ${ARGN})
    # foreach(FILE ${IN_FILES})
    #     set(EXTRA "${EXTRA} --xslt-param=${FILE}")
    # endforeach()

    # Passing xslt params is not working, so set here for insertexpr
    set(EXTRA --xslt-param=expr_filename=${CMAKE_CURRENT_BINARY_DIR}/expression.cpp.xml)

    add_custom_command(COMMAND srcml --output-xml --xslt ${XSLT_DIR}/${XSLT} ${EXTRA} --url=${URL} ${BASE} -o ${PRODUCT}
                    OUTPUT ${PRODUCT}
                    DEPENDS ${BASE}
                    COMMENT "Generating parser testfile ${PRODUCT}"
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    list(APPEND ALLGEN "${PRODUCT}")
endmacro()

# Generate special transformations
message(STATUS "  Generating special transformation targets")

configure_file(${CMAKE_CURRENT_BINARY_DIR}/constructor_base.cpp.xml ${CMAKE_CURRENT_BINARY_DIR}/constructor.cpp.xml COPYONLY)

add_custom_command(COMMAND srcml --language=Java --output-xml finally_cs.cs.xml -o finally_java.java.xml
                   DEPENDS finally_cs.cs.xml
                   OUTPUT finally_java.java.xml
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Template-based test cases
function(template OUT_FILE NAME)

    # cat all the files into one string
    set(IN_FILES ${ARGN})
    foreach(FILE ${IN_FILES})
        file(READ ${CMAKE_CURRENT_SOURCE_DIR}/${FILE} S)
        set(RAWCONTENTS "${RAWCONTENTS}${S}")
    endforeach()

    # replace the placeholder (in configure_file() format)
    string(REPLACE "\$$\{NAME\}" "${NAME}" CONTENTS "${RAWCONTENTS}")

    # write the substituted string
    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/${OUT_FILE} "${CONTENTS}")

    list(APPEND ALLGEN "${OUT_FILE}")
endfunction()

message(STATUS "  Generating suite targets")

# Process all tests in suite.txt according to their categories
file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/suite.txt SUITE REGEX "^LANGUAGE_")
foreach(GEN ${SUITE})

    # split into category and url
    separate_arguments(GEN)
    list(GET GEN 0 CATEGORY)
    list(GET GEN 1 URL)
    list(SUBLIST GEN 1 -1 FULLURL)

    if(CATEGORY STREQUAL "LANGUAGE_ALL_BASE")

        # base is C++, so these just get added
        list(APPEND ALLGEN "${URL}.cpp.xml")
        setlanguage("C++" "${URL}" "Java" "C#" "Objective-C" "C")

    elseif(CATEGORY STREQUAL "LANGUAGE_ALL_GEN")

        # explicitly generated
        list(APPEND ALLGEN "${URL}.cpp.xml")
        setlanguage("C++" "${URL}" "Java" "C#" "Objective-C" "C")

    elseif(CATEGORY STREQUAL "LANGUAGE_CSHARP")

        # exist, just add to list
        list(APPEND ALLGEN "${URL}.cs.xml")

    elseif(CATEGORY STREQUAL "LANGUAGE_CSHARP_GEN")

        # explicitly generated
        #list(APPEND ALLGEN "${URL}.cs.xml")

    elseif(CATEGORY STREQUAL "LANGUAGE_CXX")

        # exist, just add to list
        list(APPEND ALLGEN "${URL}.cpp.xml")

    elseif(CATEGORY STREQUAL "LANGUAGE_CXX_FAMILY")

        list(APPEND ALLGEN "${URL}.cpp.xml")
        setlanguage("C++" "${URL}" "C#")

    elseif(CATEGORY STREQUAL "LANGUAGE_CXX_GEN")

    elseif(CATEGORY STREQUAL "LANGUAGE_C_FAMILY")

        list(APPEND ALLGEN "${URL}.cpp.xml")
        setlanguage("C++" "${URL}" "C" "C#" "Objective-C")

    elseif(CATEGORY STREQUAL "LANGUAGE_C_FAMILY_NO_SHARP")

        list(APPEND ALLGEN "${URL}.cpp.xml")
        setlanguage("C++" "${URL}" "C" "Objective-C")
 
    elseif(CATEGORY STREQUAL "LANGUAGE_C_ONLY")

        list(APPEND ALLGEN "${URL}.c.xml")
        setlanguage("C" "${URL}" "Objective-C")

    elseif(CATEGORY STREQUAL "LANGUAGE_JAVA")

        list(APPEND ALLGEN "${URL}.java.xml")

    elseif(CATEGORY STREQUAL "LANGUAGE_JAVA_GEN")

    elseif(CATEGORY STREQUAL "LANGUAGE_OBJECTIVE_C")

        list(APPEND ALLGEN "${URL}.m.xml")

    elseif(CATEGORY STREQUAL "LANGUAGE_OBJECTIVE_C_GEN")

    elseif(CATEGORY STREQUAL "LANGUAGE_OO")

        # base is C++, so these just get added
        list(APPEND ALLGEN "${URL}.cpp.xml")
        setlanguage("C++" "${URL}" "C#" "Java")

    elseif(CATEGORY STREQUAL "LANGUAGE_TEMPLATE")

        template(${FULLURL})

    elseif(CATEGORY STREQUAL "LANGUAGE_TRANSFORM")

        transform(${FULLURL})

    endif()

endforeach()

list(SORT ALLGEN)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/before.txt "${ALLGEN}")
# this should not be necessary, but without it there are some issues
# with empty srcML files
list(REMOVE_DUPLICATES ALLGEN)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/after.txt "${ALLGEN}")

add_custom_target(parser_solitary DEPENDS ${ALLGEN}
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

message(STATUS "  Generating context variation targets")

set(COMMANDS)

# Generate variations: formfeed, comment, block, etc.
foreach(GENURL ${ALLGEN})

    # ${URL}.${LANGEXT}.xml
    get_filename_component(SRCFILENAME "${GENURL}" NAME_WLE)
    get_filename_component(URL "${SRCFILENAME}" NAME_WLE)
    get_filename_component(EXTENSION "${SRCFILENAME}" LAST_EXT)
    # remove '.'
    string(SUBSTRING "${EXTENSION}" 1 -1 LANGEXT)

    # Generate "formfeed" "comment" "block" "struct" "ifthenelse"
    foreach(VARIATION "formfeed" "comment" "block" "struct" "preproc" "ifthenelse")
        # preproc and struct do not work with Java
        if(LANGEXT STREQUAL "java" AND (VARIATION STREQUAL "struct" OR VARIATION STREQUAL "preproc"))
            continue()
        endif()

        add_custom_command(
            COMMAND srcml --output-xml --xslt ${XSLT_DIR}/insert${VARIATION}.xsl --url=${URL}.${VARIATION} ${GENURL}
                     -o ${URL}.${VARIATION}.${LANGEXT}.xml
            COMMENT "Generating parser testfile ${URL}.${VARIATION}.${LANGEXT}.xml"
            DEPENDS ${GENURL}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            OUTPUT ${URL}.${VARIATION}.${LANGEXT}.xml
        )
        list(APPEND COMMANDS ${URL}.${VARIATION}.${LANGEXT}.xml)
    endforeach()

    # all is generated separately because it does not use an XSLT transformation
    add_custom_command(
        COMMAND srcml --cat --url=${URL}.all ${GENURL} -o ${URL}.all.${LANGEXT}.xml
        COMMENT "Generating parser testfile ${URL}.all.${LANGEXT}.xml"
        DEPENDS ${GENURL}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        OUTPUT ${URL}.all.${LANGEXT}.xml
    )
    list(APPEND COMMANDS ${URL}.all.${LANGEXT}.xml)

endforeach()

# shows number of parser testfiles
add_custom_command(
    COMMAND ls *.xml | wc -l
    COMMENT "Total Parser Testfiles: "
    DEPENDS ${COMMANDS}
    OUTPUT parser_testfile_count
)

add_custom_target(parser_tests DEPENDS ${COMMANDS} parser_testfile_count
                  COMMENT "Generated parser testfile target"
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB TEST_FILES ${CMAKE_CURRENT_BINARY_DIR}/*.xml)
list(LENGTH TEST_FILES LEN)
message(STATUS "  Installed Parser Testfiles: ${LEN}")
