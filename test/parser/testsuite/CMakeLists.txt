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

option(FORCE_REBUILD OFF "Force a rebuild of all parser tests")
option(ONEPASS OFF "One pass through any loop")

message(STATUS "Generating Parser Testfiles:")

set(XSLT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../xsl")

file(GLOB PARSE_TESTS *.xml)
foreach(PATH ${PARSE_TESTS})
    get_filename_component(F ${PATH} NAME)
    configure_file(${F} ${CMAKE_CURRENT_BINARY_DIR}/${F} COPYONLY)
endforeach()

set(ALLGEN)

# Sets the language of the result
macro(setlanguage INLANG LANG URL BASEEXT)

    # convert from language to extension
    if(LANG STREQUAL "Java")
        set(LANGEXT "java")
    elseif(LANG STREQUAL "Objective-C")
        set(LANGEXT "m")
    elseif(LANG STREQUAL "C#")
        set(LANGEXT "cs")
    elseif(LANG STREQUAL "C")
        set(LANGEXT "c")
    else()
        set(LANGEXT "cpp")
    endif()

    # assumes a C++ base
    set(BASE "${URL}.${BASEEXT}.xml")

    # strip _base from URL
#   string(REGEX REPLACE "_base" "" PRODURL "${URL}")
#   set(PRODUCT "${PRODURL}.${LANGEXT}.xml")
    set(PRODUCT "${URL}.${LANGEXT}.xml")

    add_custom_command(COMMAND srcml --output-xml --language=${LANG} ${BASE} -o ${PRODUCT}
                DEPENDS ${BASE}
                OUTPUT ${PRODUCT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    # record product so that variation are generated
    list(APPEND ALLGEN "${CMAKE_CURRENT_BINARY_DIR}/${PRODUCT}")
endmacro()

# Transforms BASE into PRODUCT with XSLT and applies the given URL
macro(transform BASE PRODUCT URL XSLT)

#   execute_process(COMMAND srcml --output-xml --xslt ${XSLT_DIR}/${XSLT} --url=${URL} ${BASE}
#                   OUTPUT_FILE ${PRODUCT}
#                       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    add_custom_command(COMMAND srcml --output-xml --xslt ${XSLT_DIR}/${XSLT} --url=${URL} ${BASE} -o ${PRODUCT}
                    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PRODUCT}
                    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${BASE}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    # record product so that variation are generated
    list(APPEND ALLGEN "${CMAKE_CURRENT_BINARY_DIR}/${PRODUCT}")
endmacro()

# Generate special transformations
message(STATUS "  Generating transformations")

# class -> struct
transform(class.cpp.xml struct.cpp.xml "struct" "class2struct.xsl")
transform(class_cpp.cpp.xml struct_cpp.cpp.xml "struct" "class2struct.xsl")
transform(class_cs.cs.xml struct_cs.cs.xml "struct" "class2struct.xsl")

# struct -> union
transform(struct.cpp.xml union.cpp.xml "union" "struct2union.xsl")
transform(struct_cpp.cpp.xml union_cpp.cpp.xml "union" "struct2union.xsl")
transform(struct_c.c.xml union_c.c.xml "union" "struct2union.xsl")

configure_file(${CMAKE_CURRENT_BINARY_DIR}/constructor_base.cpp.xml ${CMAKE_CURRENT_BINARY_DIR}/constructor.cpp.xml COPYONLY)

# if -> while
transform(if.cpp.xml while.cpp.xml "while" "if2while.xsl")

# C++ special transformations
transform(function.cpp.xml function_const.cpp.xml "const" "addconst.xsl")
transform(constructor.cpp.xml constructor_explicit.cpp.xml "constructor" "addexplicit.xsl")
transform(if.cpp.xml ifnestcond.cpp.xml "if" "nestcond.xsl")
transform(destructor.cpp.xml destructor_virtual.cpp.xml "destructor.virtual" "addvirtual.xsl")
transform(throw_base.cpp.xml throw.cpp.xml "throw" "insertexpr.xsl")
transform(class_java.java.xml interface_java.java.xml "interface" "class2interface.xsl")
transform(if.cpp.xml ifblock.cpp.xml "if.block" "if2ifblock.xsl")
#execute_process(COMMAND srcml --output-xml --xslt ${XSLT_DIR}/insertexpr.xsl --xslt-param=expr_filename="../testsuite/expression.cpp.xml" --url=return ${CMAKE_CURRENT_BINARY_DIR}/return_base.cpp.xml
#               OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/return.cpp.xml)

add_custom_command(COMMAND srcml --output-xml --xslt ${XSLT_DIR}/insertexpr.xsl --xslt-param=expr_filename="../testsuite/expression.cpp.xml" --url=return return_base.cpp.xml
                            -o return.cpp.xml
                   DEPENDS return_base.cpp.xml
                   OUTPUT return.cpp.xml
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

list(APPEND ALLGEN ${CMAKE_CURRENT_BINARY_DIR}/return.cpp.xml)

transform(function.cpp.xml function_decl.cpp.xml "function" "defn2decl.xsl")

# Was in Makefile.gen, but cannot find the file
#transform(function_asterisk.cpp.xml function_asterisk_decl.cpp.xml "function" "defn2decl.xsl")

# Objective-C special transformations
transform(class_interface_m.m.xml class_implementation_m.m.xml "function" interface2implementation.xsl)
transform(try.cpp.xml             try_m.m.xml                  "try"      keyword2mkeyword.xsl)
transform(catch_form.cpp.xml      catch_m.m.xml                "catch"    keyword2mkeyword.xsl)

# C# finally -> Java finally
#execute_process(COMMAND srcml --language=Java --output-xml finally_cs.cs.xml -o finally_java.java.xml
#                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(COMMAND srcml --language=Java --output-xml finally_cs.cs.xml -o finally_java.java.xml
                   DEPENDS finally_cs.cs.xml
                   OUTPUT finally_java.java.xml
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Objective-C try, catch, finally, throw from C++
transform(try.cpp.xml        try_m.m.xml        "try"     keyword2mkeyword.xsl)
transform(catch_form.cpp.xml catch_m.m.xml      "catch"   keyword2mkeyword.xsl)
transform(finally_cs.cs.xml  finally_m.m.xml    "finally" keyword2mkeyword.xsl)
transform(throw.cpp.xml      throw_m.m.xml      "throw"   keyword2mkeyword.xsl)
transform(throw_base.cpp.xml throw_base_m.m.xml "throw"   keyword2mkeyword.xsl)
transform(synthesize_m.m.xml dynamic_m.m.xml    "dynamic" synthesize2dynamic.xsl)

message(STATUS "  Generating from templates")

# Template-based C in C++ mode
function(template OUT_FILE NAME IN_FILES)
    list(TRANSFORM IN_FILES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
    foreach(FILE ${IN_FILES})
        file(READ ${FILE} S)
        set(RAW_CONTENTS "${RAW_CONTENTS}${S}")
    endforeach()
    string(CONFIGURE "${RAW_CONTENTS}" CONTENTS)
    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/${OUT_FILE} "${CONTENTS}")
    list(APPEND ALLGEN "${CMAKE_CURRENT_BINARY_DIR}/${OUT_FILE}")
endfunction()

template(c_cpp_mode_restrict.cpp.xml restrict "c_cpp_mode_header.template;c_cpp_mode_basic.template;c_cpp_mode_enum.template;c_cpp_mode_specifier.template;c_cpp_mode_call.template;c_cpp_mode_footer.template")
template(c_cpp_mode_mutable.cpp.xml  mutable  "c_cpp_mode_header.template;c_cpp_mode_basic.template;c_cpp_mode_enum.template;c_cpp_mode_specifier.template;c_cpp_mode_call.template;c_cpp_mode_footer.template")
template(c_cpp_mode_try.cpp.xml      try      "c_cpp_mode_header.template;c_cpp_mode_basic.template;c_cpp_mode_simple_decl.template;c_cpp_mode_decl.template;c_cpp_mode_single.template;c_cpp_mode_enum.template;c_cpp_mode_call.template;c_cpp_mode_footer.template")
template(c_cpp_mode_catch.cpp.xml    catch    "c_cpp_mode_header.template;c_cpp_mode_basic.template;c_cpp_mode_simple_decl.template;c_cpp_mode_decl.template;c_cpp_mode_single.template;c_cpp_mode_enum.template;c_cpp_mode_footer.template")
template(c_cpp_mode_class.cpp.xml    class    "c_cpp_mode_header.template;c_cpp_mode_basic.template;c_cpp_mode_decl.template;c_cpp_mode_single.template;c_cpp_mode_call.template;c_cpp_mode_footer.template")

message(STATUS "  Generating language variations")

# Process all tests in suite.txt according to their categories
file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/suite.txt SUITE REGEX "^LANGUAGE_")
foreach(GEN ${SUITE})

    # split into category and url
    separate_arguments(GEN)
    list(GET GEN 0 CATEGORY)
    list(GET GEN 1 URL)

    if(CATEGORY STREQUAL "LANGUAGE_ALL_BASE")

        # base is C++, so these just get added
        list(APPEND ALLGEN "${CMAKE_CURRENT_BINARY_DIR}/${URL}.cpp.xml")
        foreach(LANG "Java" "C#" "Objective-C" "C")
            setlanguage("C++" "${LANG}" "${URL}" "cpp")
        endforeach()

    elseif(CATEGORY STREQUAL "LANGUAGE_ALL_GEN")

        # explicitly generated
        list(APPEND ALLGEN "${CMAKE_CURRENT_BINARY_DIR}/${URL}.cpp.xml")
        foreach(LANG "Java" "C#" "Objective-C" "C")
            setlanguage("C++" "${LANG}" "${URL}" "cpp")
        endforeach()

    elseif(CATEGORY STREQUAL "LANGUAGE_CSHARP")

        # exist, just add to list
        list(APPEND ALLGEN "${CMAKE_CURRENT_BINARY_DIR}/${URL}.cs.xml")

    elseif(CATEGORY STREQUAL "LANGUAGE_CSHARP_GEN")

        # explicitly generated
        #list(APPEND ALLGEN "${CMAKE_CURRENT_BINARY_DIR}/${URL}.cs.xml")

    elseif(CATEGORY STREQUAL "LANGUAGE_CXX")

        # exist, just add to list
        list(APPEND ALLGEN "${CMAKE_CURRENT_BINARY_DIR}/${URL}.cpp.xml")

    elseif(CATEGORY STREQUAL "LANGUAGE_CXX_FAMILY")

        list(APPEND ALLGEN "${CMAKE_CURRENT_BINARY_DIR}/${URL}.cpp.xml")
        foreach(LANG "C#")
            setlanguage("C++" "${LANG}" "${URL}" "cpp")
        endforeach()

    elseif(CATEGORY STREQUAL "LANGUAGE_CXX_GEN")

        # explicitly generated
        #list(APPEND ALLGEN "${CMAKE_CURRENT_BINARY_DIR}/${URL}.cpp.xml")

    elseif(CATEGORY STREQUAL "LANGUAGE_C_FAMILY")

        list(APPEND ALLGEN "${CMAKE_CURRENT_BINARY_DIR}/${URL}.cpp.xml")
        foreach(LANG "C" "C#" "Objective-C")
            setlanguage("C++" "${LANG}" "${URL}" "cpp")
        endforeach()

    elseif(CATEGORY STREQUAL "LANGUAGE_C_FAMILY_NO_SHARP")

        list(APPEND ALLGEN "${CMAKE_CURRENT_BINARY_DIR}/${URL}.cpp.xml")
        foreach(LANG "C" "Objective-C")
            setlanguage("C++" "${LANG}" "${URL}" "cpp")
        endforeach()

    elseif(CATEGORY STREQUAL "LANGUAGE_C_ONLY")

        # exist, just add to list
        list(APPEND ALLGEN "${CMAKE_CURRENT_BINARY_DIR}/${URL}.c.xml")
        foreach(LANG "Objective-C")
            setlanguage("C" "${LANG}" "${URL}" "c")
        endforeach()

    elseif(CATEGORY STREQUAL "LANGUAGE_JAVA")

        # exist, just add to list
        list(APPEND ALLGEN "${CMAKE_CURRENT_BINARY_DIR}/${URL}.java.xml")

    elseif(CATEGORY STREQUAL "LANGUAGE_JAVA_GEN")

        # explicitly generated
        #list(APPEND ALLGEN "${CMAKE_CURRENT_BINARY_DIR}/${URL}.java.xml")

    elseif(CATEGORY STREQUAL "LANGUAGE_OBJECTIVE_C")

        # exist, just add to list
        list(APPEND ALLGEN "${CMAKE_CURRENT_BINARY_DIR}/${URL}.m.xml")

    elseif(CATEGORY STREQUAL "LANGUAGE_OBJECTIVE_C_GEN")

        # explicitly generated
        #list(APPEND ALLGEN "${CMAKE_CURRENT_BINARY_DIR}/${URL}.m.xml")

    elseif(CATEGORY STREQUAL "LANGUAGE_OO")

        # base is C++, so these just get added
        list(APPEND ALLGEN "${CMAKE_CURRENT_BINARY_DIR}/${URL}.cpp.xml")
        foreach(LANG "C#" "Java")
            setlanguage("C++" "${LANG}" "${URL}" "cpp")
        endforeach()

    endif()

    if(ONEPASS)
        break()
    endif()
endforeach()

list(SORT ALLGEN)
#file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/before.txt "${ALLGEN}")
# this should not be necessary, but without it there are some issues
# with empty srcML files
list(REMOVE_DUPLICATES ALLGEN)
#file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/after.txt "${ALLGEN}")


add_custom_target(singletests DEPENDS ${ALLGEN})

message(STATUS "  Generating context variations")

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
            DEPENDS ${GENURL}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            OUTPUT ${URL}.${VARIATION}.${LANGEXT}.xml
        )
        list(APPEND COMMANDS ${URL}.${VARIATION}.${LANGEXT}.xml)
    endforeach()

    # add_custom_command(
    #     COMMAND srcml --unit=1 --url=${URL}.all --output-xml ${GENURL} |
    #             sed "s|</unit>||g" > ${URL}.all.${LANGEXT}.xml.p1 VERBATIM
    #     COMMAND srcml --output-xml-inner ${GENURL} > ${URL}.all.${LANGEXT}.p2 VERBATIM
    #     COMMAND echo "</unit>" > ${URL}.all.${LANGEXT}.xml.p3 VERBATIM
    #     COMMAND cat ${URL}.all.${LANGEXT}.xml.p1 ${URL}.all.${LANGEXT}.p2 ${URL}.all.${LANGEXT}.xml.p3 >
    #             ${URL}.all.${LANGEXT}.xml VERBATIM
    #     #COMMAND rm -f ${URL}.all.${LANGEXT}.xml{.p1,.p2,.p3}
    #     DEPENDS ${GENURL}
    #     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    #     OUTPUT ${URL}.all.${LANGEXT}.xml
    # )
    # list(APPEND COMMANDS ${URL}.all.${LANGEXT}.xml)

    # file(READ ${CMAKE_CURRENT_BINARY_DIR}/${URL}.all.${LANGEXT}.template ALLHEADER)
    # string(LENGTH "${ALLHEADER}" SIZE)
    # math(EXPR NEWSIZE "${SIZE} - 8")
    # string(SUBSTRING "${ALLHEADER}" 0 ${NEWSIZE} ALLCONTENT)
    # file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${URL}.all.${LANGEXT}.xml "${ALLCONTENT}" "${ALL_INNER}" "</unit>")
    # file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/${URL}.all.${LANGEXT}.template)

    # list(APPEND ALLGEN ${URL}.all.${LANGEXT}.xml)
    # list(APPEND COMMANDS ${URL}.all.${LANGEXT}.xml)

    #message("DONE ${GENURL}")

    if(ONEPASS)
        break()
    endif()

endforeach()

#list(REMOVE_DUPLICATES COMMANDS)
list(TRANSFORM COMMANDS PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")

add_custom_target(wraptests DEPENDS ${COMMANDS})

file(GLOB TEST_FILES ${CMAKE_CURRENT_BINARY_DIR}/*.xml)
list(LENGTH TEST_FILES LEN)
message(STATUS "  Installed Parser Test files: ${LEN}")
