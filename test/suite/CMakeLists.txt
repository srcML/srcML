#
# @file CMakeLists.txt
# @copyright
# 
# Copyright (C) 2013 srcML, LLC. (www.srcML.org)
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


# Read suite.txt and extract names to be used in testing from it.
#
# Reads suite using a regular expression to extract information from the suite.txt
# file and create lists of things which need to be generated. It returns a list with
# the given name. Any of the items given as extra arguments are removed from the list
# after it has been read in.
# 
macro(readSuiteAndMakeList LIST_PREFIX OUTPUT)
    set(${OUTPUT} "")
    file(STRINGS suite.txt RESULTS_FROM_SUITE REGEX "^${LIST_PREFIX} ")
    foreach(SUITE_RESULT ${RESULTS_FROM_SUITE})
        string(REPLACE "${LIST_PREFIX} " "" RESULT ${SUITE_RESULT})
        string(STRIP ${RESULT} RESULT)
        list(APPEND ${OUTPUT} ${RESULT})
    endforeach()
    math(EXPR NUMBER_TO_REMOVE "${ARGC} - 2")
    if(${NUMBER_TO_REMOVE} GREATER 0)
        list(REMOVE_ITEM BASE_COMMON ${ARGN})
    endif()
endmacro()

readSuiteAndMakeList("LANGUAGE_ALL_BASE" BASE_COMMON "problem")
readSuiteAndMakeList("LANGUAGE_ALL_GEN"  GEN_BASE)
readSuiteAndMakeList("LANGUAGE_CXX_FAMILY"  GEN_BASE_CXX)
readSuiteAndMakeList("LANGUAGE_C_FAMILY" BASE_C_FAMILY)
readSuiteAndMakeList("LANGUAGE_C_ONLY" BASE_C)
readSuiteAndMakeList("LANGUAGE_OO" BASE_OO)
readSuiteAndMakeList("LANGUAGE_CXX" BASE_CXX)
readSuiteAndMakeList("LANGUAGE_CXX_GEN" BASE_CXX_GEN)
readSuiteAndMakeList("LANGUAGE_CSHARP" BASE_CS)
readSuiteAndMakeList("LANGUAGE_CSHARP_GEN" BASE_CS_GEN)
readSuiteAndMakeList("LANGUAGE_OBJECTIVE_C" BASE_OBJECTIVE_C)
readSuiteAndMakeList("LANGUAGE_OBJECTIVE_C_GEN" BASE_OBJECTIVE_C_GEN)
readSuiteAndMakeList("LANGUAGE_JAVA" BASE_JAVA)
readSuiteAndMakeList("LANGUAGE_JAVA_GEN" BASE_JAVA_GEN)

# Locating XSLT proc.
find_program(xsltproc REQUIRED)
message(STATUS "XSLTPROC Variable: ${XSLTPROC_EXECUTABLE}")

# Completely useless commands.
# add_custom_target(echobase COMMAND echo ${BASE_COMMON})
# add_custom_target(echogen COMMAND echo ${GEN_BASE})

# get_property(XSLTPROC GLOBAL PROPERTY XSLTPROC)

# # Merge separate srcML units into one
# set(XSLT_MERGEUNIT_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/mergeunit.xsl)
# set(XSLT_MERGEUNIT ${XSLTPROC} ${XSLT_MERGEUNIT_XSL})
# set(XSLT_MERGEUNIT_PROBLEM_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/mergeunitproblem.xsl)
# set(XSLT_MERGEUNIT_PROBLEM ${XSLTPROC} ${XSLT_MERGEUNIT_PROBLEM_XSL})
 
# # generate nested conditionals
# set(XSLT_NESTCOND_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/nestcond.xsl)
# set(XSLT_NESTCOND ${XSLTPROC} ${XSLT_NESTCOND_XSL})

# # if to while conversion
# set(XSLT_IF2WHILE_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/if2while.xsl)
# set(XSLT_IF2WHILE ${XSLTPROC} ${XSLT_IF2WHILE_XSL})

# # definition to declaration conversion
# set(XSLT_DEFN2DECL_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/defn2decl.xsl)
# set(XSLT_DEFN2DECL ${XSLTPROC} ${XSLT_DEFN2DECL_XSL})

# # wrap in a block
# set(XSLT_BLOCKUNIT ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertblock.xsl)

# set(XSLT_BLOCKUNIT_PROBLEM ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertblockproblem.xsl)

# set(XSLT_STRUCTUNIT ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertstruct.xsl)

# set(XSLT_BLOCKUNIT_JAVA ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertblock.java.xsl)
# set(XSLT_BLOCKUNIT_PROBLEM_JAVA ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertblockproblem.java.xsl)

# set(XSLT_ADDEXPLICIT ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/addexplicit.xsl)

# set(XSLT_ADDVIRTUAL ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/addvirtual.xsl)

# set(XSLT_ADDCONST ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/addconst.xsl)

# set(XSLT_IDENTITY ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/identity.xsl)

# set(XSLT_INSERTEXPR ${XSLTPROC} --stringparam expr_filename "\"${CMAKE_CURRENT_SOURCE_DIR}/expression.cpp.xml\"" ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertexpr.xsl)
# set(XSLT_INSERTEXPR_XSL ${CMAKE_CURRENT_SOURCE_DIR}/expression.cpp.xml ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertexpr.xsl)

# set(XSLT_INSERTDECL ${XSLTPROC} --stringparam decl_filename "\"${CMAKE_CURRENT_SOURCE_DIR}/decl_simple.cpp.xml\"" ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertparam.xsl)
# set(XSLT_INSERTDECL_XSL ${CMAKE_CURRENT_SOURCE_DIR}/decl_simple.cpp.xml ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertdecl.xsl)

# set(XSLT_STRUCT2UNION_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/struct2union.xsl)
# set(XSLT_STRUCT2UNION ${XSLTPROC} ${XSLT_STRUCT2UNION_XSL})

# set(XSLT_CLASS2STRUCT_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/class2struct.xsl)
# set(XSLT_CLASS2STRUCT ${XSLTPROC} ${XSLT_CLASS2STRUCT_XSL})

# set(XSLT_CLASS2INTERFACE_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/class2interface.xsl)
# set(XSLT_CLASS2INTERFACE ${XSLTPROC} ${XSLT_CLASS2INTERFACE_XSL})

# set(XSLT_IF2IFBLOCK_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/if2ifblock.xsl)
# set(XSLT_IF2IFBLOCK ${XSLTPROC} ${XSLT_IF2IFBLOCK_XSL})

# ##
# # Comments and preprocessor statements are handled out of the normal grammar rules
# # in src2srcml.  In order to test the handling of these tokens, special cases are
# # generated from the non-comment and preprocessor cases.

# # insert comments
# set(XSLT_INSERTCOMMENT ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertcomments.xsl)

# set(XSLT_INSERTCOMMENT_PROBLEM ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertcommentsproblem.xsl)

# # insert preprocessor
# set(XSLT_INSERTPREPROC ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertpreproc.xsl)

# set(XSLT_INSERTPREPROC_PROBLEM ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertpreprocproblem.xsl)

# # nest if
# set(XSLT_INSERTIFTHENELSE ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/nestif.xsl)
# set(XSLT_INSERTIFTHENELSE_JAVA ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/nestif.java.xsl)
# set(XSLT_INSERTIFTHENELSE_PROBLEM ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/nestifproblem.xsl)
# set(XSLT_INSERTIFTHENELSE_PROBLEM_JAVA ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/nestifproblem.java.xsl)

# # insert form feed
# set(XSLT_INSERTFORMFEED ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertformfeed.xsl)
# set(XSLT_INSERTFORMFEED_PROBLEM ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertformfeedproblem.xsl)

# #
# # Test cases for one language are generated from test cases for another language.
# # 
# set(XSLT_SETLANGUAGE_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/setlanguage.xsl)
# set(XSLT_SETLANGUAGE_PROBLEM_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/setlanguageproblem.xsl)
# set(XSLT_REMOVECPP_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/removecpp.xsl)
# set(XSLT_REMOVECPP_PROBLEM_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/removecppproblem.xsl)
# set(XSLT_REMOVECPP_LITERAL_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/removecppliteral.xsl)
# set(XSLT_REMOVECPP_OPERATOR_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/removecppoperator.xsl)

# set(XSLT_SETLANGUAGE ${XSLTPROC} ${XSLT_SETLANGUAGE_XSL})

# set(XSLT_SETLANGUAGE_C ${XSLTPROC} --stringparam language "\"C\"" ${XSLT_SETLANGUAGE_XSL})

# set(XSLT_SETLANGUAGE_CS ${XSLTPROC} --stringparam language "\"C#\"" ${XSLT_SETLANGUAGE_XSL})

# set(XSLT_SETLANGUAGE_CXX ${XSLTPROC} --stringparam language "\"CXX\"" ${XSLT_SETLANGUAGE_XSL})

# set(XSLT_SETLANGUAGE_CXX_11 ${XSLTPROC} --stringparam language "\"C++11\"" ${XSLT_SETLANGUAGE_XSL})

# set(XSLT_SETLANGUAGE_JAVA ${XSLTPROC} --stringparam language "\"Java\"" ${XSLT_SETLANGUAGE_XSL})
# set(XSLT_SETLANGUAGE_ASPECTJ ${XSLTPROC} --stringparam language "\"AspectJ\"" ${XSLT_SETLANGUAGE_XSL})

# set(XSLT_SETLANGUAGE_PROBLEM ${XSLTPROC} ${XSLT_SETLANGUAGE_PROBLEM_XSL})

# set(XSLT_SETLANGUAGE_PROBLEM_C ${XSLTPROC} --stringparam language "\"C\"" ${XSLT_SETLANGUAGE_PROBLEM_XSL})

# set(XSLT_SETLANGUAGE_PROBLEM_CS ${XSLTPROC} --stringparam language "\"C#\"" ${XSLT_SETLANGUAGE_PROBLEM_XSL})

# set(XSLT_SETLANGUAGE_PROBLEM_CXX ${XSLTPROC} --stringparam language "\"CXX\"" ${XSLT_SETLANGUAGE_PROBLEM_XSL})

# set(XSLT_SETLANGUAGE_PROBLEM_CXX_11 ${XSLTPROC} --stringparam language "\"C++11\"" ${XSLT_SETLANGUAGE_PROBLEM_XSL})

# set(XSLT_SETLANGUAGE_PROBLEM_JAVA ${XSLTPROC} --stringparam language "\"Java\"" ${XSLT_SETLANGUAGE_PROBLEM_XSL})
# set(XSLT_SETLANGUAGE_PROBLEM_ASPECTJ ${XSLTPROC} --stringparam language "\"AspectJ\"" ${XSLT_SETLANGUAGE_PROBLEM_XSL})

# set(XSLT_REMOVECPP ${XSLTPROC} ${XSLT_REMOVECPP_XSL})
# set(XSLT_REMOVECPP_PROBLEM ${XSLTPROC} ${XSLT_REMOVECPP_PROBLEM_XSL})
# set(XSLT_REMOVECPP_LITERAL ${XSLTPROC} ${XSLT_REMOVECPP_LITERAL_XSL})
# set(XSLT_REMOVECPP_OPERATOR ${XSLTPROC} ${XSLT_REMOVECPP_OPERATOR_XSL})

# # This is all of the XML targets generated in this file (well most of them any way)
# # TODO fix this so that the name is better and that it makes more sense.
# set(ALL_XML_TARGETS "")
# set(SEP_BASE ${BASE_COMMON} ${BASE_C_FAMILY} ${BASE_CXX} ${BASE_OO})

# # basic generated C++ files
# set(GEN_SEP ${GEN_BASE} ${GEN_BASE_CXX})

# # generated cases for C++ from base
# # appendFileExtToList(.cpp.xml GEN_PART ${GEN_SEP})


# set(SEP_FILES ${SEP_BASE} preprocessor)
# set(SEP_PLUS ${SEP_FILES} ${GEN_SEP})
# set(SEP_PLUSNOPREPROC ${SEP_BASE} ${GEN_SEP})
# set(SEP_STRUCT function function_const function_cpp function_decl function_ns function_operator function_pointer function_throw)

# # 
# # This macro helps create rules to be built to create all of the
# # .cpp.xml files.
# # 
# macro(buildXMLTarget COMMAND_NAME INPUT_FILE OUTPUT_FILE_TITLE SUFFIX) 
#     set(OUTPUT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT_FILE_TITLE}${SUFFIX})
#     add_custom_command(
#         OUTPUT ${OUTPUT_FILE_NAME}
#         COMMAND ${${COMMAND_NAME}} ${INPUT_FILE} > ${OUTPUT_FILE_NAME}
#         DEPENDS ${INPUT_FILE}
#     )
#     foreach(LIST_TO_ADD_TO ${ARGN})
#         list(APPEND ${LIST_TO_ADD_TO} ${OUTPUT_FILE_NAME})
#     endforeach()
# endmacro()


# # base for all of Java
# createUniqueList(BASE_JAVA_ALL_GEN ${BASE_COMMON} ${GEN_BASE} ${BASE_OO})
# createUniqueList(BASE_JAVA_ALL     ${BASE_COMMON} ${GEN_BASE} ${BASE_OO} ${BASE_JAVA})
# foreach(JAVA_BASE_FILE_NAME ${BASE_JAVA_ALL_GEN})
#     set(INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${JAVA_BASE_FILE_NAME}.cpp.xml)
#     set(OUTPUT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/${JAVA_BASE_FILE_NAME}.java.xml)
#     if(${JAVA_BASE_FILE_NAME} MATCHES "operator_base")
#         add_custom_command(
#             OUTPUT ${OUTPUT_FILE_NAME}
#             COMMAND ${XSLT_SETLANGUAGE_JAVA} ${INPUT_FILE} | ${XSLT_REMOVECPP_OPERATOR} - > ${OUTPUT_FILE_NAME}
#             DEPENDS ${INPUT_FILE}
#         )
#     elseif(${JAVA_BASE_FILE_NAME} MATCHES "literal")
#         add_custom_command(
#             OUTPUT ${OUTPUT_FILE_NAME}
#             COMMAND ${XSLT_SETLANGUAGE_JAVA} ${INPUT_FILE} | ${XSLT_REMOVECPP_LITERAL} - > ${OUTPUT_FILE_NAME}
#             DEPENDS ${INPUT_FILE}
#         )
#     else()
#         add_custom_command(
#             OUTPUT ${OUTPUT_FILE_NAME}
#             COMMAND ${XSLT_SETLANGUAGE_JAVA} ${INPUT_FILE} | ${XSLT_REMOVECPP} - > ${OUTPUT_FILE_NAME}
#             DEPENDS ${INPUT_FILE}
#         )
#     endif()

#     list(APPEND ALL_XML_TARGETS ${OUTPUT_FILE_NAME})
# endforeach()


# # Base for all C.
# set(BASE_C_ALL_GEN ${BASE_COMMON} ${GEN_BASE} ${BASE_C_FAMILY})
# set(BASE_C_ALL     ${BASE_COMMON} ${GEN_BASE} ${BASE_C_FAMILY} ${BASE_C})
# foreach(C_BASE_FILE_NAME ${BASE_C_ALL_GEN})
#     buildXMLTarget(
#         XSLT_SETLANGUAGE_C
#         ${CMAKE_CURRENT_SOURCE_DIR}/${C_BASE_FILE_NAME}.cpp.xml
#         ${C_BASE_FILE_NAME}
#         .c.xml
#         ALL_XML_TARGETS
#     )
# endforeach()


# # base for all C#
# set(BASE_CS_ALL_GEN ${BASE_COMMON} ${GEN_BASE} ${BASE_CS_GEN})
# set(BASE_CS_ALL     ${BASE_COMMON} ${GEN_BASE} ${BASE_CS_GEN} ${BASE_CS})
# foreach(CS_BASE_FILE_NAME ${BASE_CS_ALL_GEN})
#     buildXMLTarget(
#         XSLT_SETLANGUAGE_CS
#         ${CMAKE_CURRENT_SOURCE_DIR}/${CS_BASE_FILE_NAME}.cpp.xml
#         ${CS_BASE_FILE_NAME}
#         .cs.xml
#         ALL_XML_TARGETS
#     )
# endforeach()


# # 
# # generateRulesSet
# # This macro assists with the creation of rules for each of the titles
# # which reqire a different suffix but similar file conversion.
# # 
# macro(generateRulesSet SUFFIX C_CMD JAVA_CMD)
#     # .${SUFFIX}.cpp.xml
#     foreach(CURRENT_TITLE ${SEP_PLUS} extra_cpp)
#         buildXMLTarget(
#             ${C_CMD}
#             ${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_TITLE}.cpp.xml
#             ${CURRENT_TITLE}
#             .${SUFFIX}.cpp.xml
#             ALL_XML_TARGETS
#         )
#     endforeach()

#     # Generating .${SUFFIX}.java.xml
#     foreach(CURRENT_TITLE ${BASE_JAVA_ALL})
#         buildXMLTarget(
#             ${JAVA_CMD}
#             ${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_TITLE}.java.xml
#             ${CURRENT_TITLE}
#             .${SUFFIX}.java.xml
#             ALL_XML_TARGETS
#         )
#     endforeach()

#     # Generating .${SUFFIX}.c.xml
#     foreach(CURRENT_TITLE ${BASE_C_ALL} extra_c)
#         buildXMLTarget(
#             ${C_CMD}
#             ${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_TITLE}.c.xml
#             ${CURRENT_TITLE}
#             .${SUFFIX}.c.xml
#             ALL_XML_TARGETS
#         )
#     endforeach()
    
#     # Generating .${SUFFIX}.cs.xml
#     foreach(CURRENT_TITLE ${BASE_CS_ALL})
#         buildXMLTarget(
#             ${C_CMD}
#             ${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_TITLE}.cs.xml
#             ${CURRENT_TITLE}
#             .${SUFFIX}.cs.xml
#             ALL_XML_TARGETS
#         )
#     endforeach()
# endmacro()

# # generate cases for all
# generateRulesSet(all XSLT_MERGEUNIT XSLT_MERGEUNIT)

# # generated cases for blocks
# generateRulesSet(block XSLT_BLOCKUNIT XSLT_BLOCKUNIT_JAVA)

# # generated cases for comments
# generateRulesSet(comment XSLT_INSERTCOMMENT XSLT_INSERTCOMMENT)

# # generated cases for formfeed
# generateRulesSet(formfeed XSLT_INSERTFORMFEED XSLT_INSERTFORMFEED)

# # generated cases for ifthenelse
# generateRulesSet(ifthenelse XSLT_INSERTIFTHENELSE XSLT_INSERTIFTHENELSE_JAVA)

# # generated cases for structs
# foreach(CURRENT_TITLE ${SEP_STRUCT})
#     buildXMLTarget(
#         XSLT_STRUCTUNIT
#         ${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_TITLE}.cpp.xml
#         ${CURRENT_TITLE}
#         .struct.cpp.xml
#         ALL_XML_TARGETS
#     )
# endforeach()

# # generated test cases for preproc
# foreach(CURRENT_TITLE ${SEP_PLUSNOPREPROC} extra_cpp)
#     buildXMLTarget(
#         XSLT_INSERTPREPROC
#         ${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_TITLE}.cpp.xml
#         ${CURRENT_TITLE}
#         .preproc.cpp.xml
#         ALL_XML_TARGETS
#     )
# endforeach()
# foreach(CURRENT_TITLE ${BASE_C_ALL} extra_c)
#     buildXMLTarget(
#         XSLT_INSERTPREPROC
#         ${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_TITLE}.c.xml
#         ${CURRENT_TITLE}
#         .preproc.c.xml
#         ALL_XML_TARGETS
#     )
# endforeach()
# foreach(CURRENT_TITLE ${BASE_CS_ALL})
#     buildXMLTarget(
#         XSLT_INSERTPREPROC
#         ${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_TITLE}.cs.xml
#         ${CURRENT_TITLE}
#         .preproc.cs.xml
#         ALL_XML_TARGETS
#     )
# endforeach()


# # TODO FIXME 
# # file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/allinall.c.xml "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>
# #   <unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\" dir=\"allinall.c\">
# # ")
# # file(GLOB allinallCXMLFiles "${CMAKE_CURRENT_BINARY_DIR}/*.all.c.xml" DEPENDS allinall)
# # 
# # file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/allinall.c.xml "</unit>")
# # add_custom_target(allinall DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/allinall.c.xml ${allinallCXMLFiles})


# # FIXME FIXME FIXME FIXME FIXME
# # Not sure of any of this actually works correctly. It doesn't when
# # I tried it using the original make file
# # FIXME FIXME FIXME FIXME FIXME
# # allinall.c.xml :
# #         @echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > allinall.c.xml
# #         @echo "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\" dir=\"allinall.c\">" >> allinall.c.xml
# # 
# #         find ../suite -name "*.all.c.xml" | xargs -i tail +3 {} | sed "s|</unit>||">> allinall.c.xml
# #         @echo "</unit>" >> allinall.c.xml
# # 
# # allinall.java.xml :
# #         @echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > allinall.java.xml
# #         @echo "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"Java\" dir=\"allinall.java\">" >> allinall.java.xml
# # 
# #         find ../suite -name "*.all.java.xml" | xargs -i tail+3 {} | sed "s|</unit>||">> allinall.java.xml
# #         @echo "</unit>" >> allinall.java.xml
# # 
# # allinall.1.cpp.xml :
# #         @echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > allinall.1.cpp.xml
# #         @echo "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"allinall.1.cpp\">" >> allinall.1.cpp.xml
# # 
# #         @find ../suite -name "*.all.cpp.xml" | sed "s|cstruct.all.cpp.xml||" | sed "s|functionkr.all.cpp.xml||" | head -30 | xargs -i tail +3 {} | sed "s|</unit>||" >> allinall.1.cpp.xml
# #         @echo "</unit>" >> allinall.1.cpp.xml
# # 
# # allinall.2.cpp.xml :
# #         @echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > allinall.2.cpp.xml
# #         @echo "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"allinall.2.cpp\">" >> allinall.2.cpp.xml
# # 
# #         @find ../suite -name "*.all.cpp.xml" | sed "s|cstruct.all.cpp.xml||" | sed "s|functionkr.all.cpp.xml||" | tail -18 | xargs -i tail +3 {} | sed "s|</unit>||" >> allinall.2.cpp.xml
# #         @echo "</unit>" >> allinall.2.cpp.xml
# # 
# # allinone : allinonecpp allinonec allinonejava
# #         #@make cleanall
# # 
# # allinonec : allinall.c.xml
# #         -${SRCML2SRC} ${SRCENCODING} allinall.c.xml | ${SRC2SRCML} -l C ${SRCENCODING} --directory="allinall.c" | diff -w  - allinall.c.xml
# # 
# # allinonecpp : allinall.1.cpp.xml allinall.2.cpp.xml
# #         -${SRCML2SRC} ${SRCENCODING} allinall.1.cpp.xml | ${SRC2SRCML} -l C++ ${SRCENCODING} --directory="allinall.1.cpp" | diff -w  - allinall.1.cpp.xml
# #         -${SRCML2SRC} ${SRCENCODING} allinall.2.cpp.xml | ${SRC2SRCML} -l C++ ${SRCENCODING} --directory="allinall.2.cpp" | diff -w  - allinall.2.cpp.xml
# # 
# # allinonejava : allinall.java.xml
# #         -${SRCML2SRC} ${SRCENCODING} allinall.java.xml | ${SRC2SRCML} -l Java ${SRCENCODING} --directory="allinall.java" | diff -w  - allinall.java.xml
# # 
# # cleanall :
# #         rm -f allinall.*

# # FIXME End of all in one file generation commands FIXME


# set(PROBLEM_ALL_TARGETS "")
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/problem.java.xml
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/problem.cpp.xml
#     COMMAND ${XSLT_SETLANGUAGE_JAVA} ${CMAKE_CURRENT_SOURCE_DIR}/problem.cpp.xml | ${XSLT_REMOVECPP_PROBLEM} - > ${CMAKE_CURRENT_SOURCE_DIR}/problem.java.xml
#     COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/problem.java.xml
# )

# # # problem.aj.xml : problem.java.xml
# # #         ${XSLT_SETLANGUAGE_PROBLEM_ASPECTJ} $< > $@
# # add_custom_command(
# #     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/problem.aj.xml
# #     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/problem.java.xml
# #     COMMAND ${XSLT_SETLANGUAGE_PROBLEM_ASPECTJ} ${CMAKE_CURRENT_SOURCE_DIR}/problem.java.xml > ${CMAKE_CURRENT_SOURCE_DIR}/problem.aj.xml
# #     COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/problem.aj.xml
# # )

# # problem.c.xml : problem.cpp.xml
# #         ${XSLT_SETLANGUAGE_PROBLEM_C} $< > $@
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/problem.c.xml
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/problem.cpp.xml
#     COMMAND ${XSLT_SETLANGUAGE_PROBLEM_C} ${CMAKE_CURRENT_SOURCE_DIR}/problem.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/problem.c.xml
#     COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/problem.c.xml
# )

# # problem.cs.xml : problem.cpp.xml
# #         ${XSLT_SETLANGUAGE_PROBLEM_CS} $< > $@
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/problem.cs.xml
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/problem.cpp.xml
#     COMMAND ${XSLT_SETLANGUAGE_PROBLEM_CS} ${CMAKE_CURRENT_SOURCE_DIR}/problem.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/problem.cs.xml
#     COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/problem.cs.xml
# )

# # This is used to simplfy the handling of each of the different programming languages
# # so that it could be done more easily using macros.
# set(PROGRAMMING_LANGUAGES cpp c cs java) #Removed  aj
# set(PROGRAMMING_LANGUAGES_BUT_JAVA_AND_AJ cpp c cs)

# # Handling the problem.all files.
# foreach(LANG ${PROGRAMMING_LANGUAGES})
#     buildXMLTarget(
#         XSLT_MERGEUNIT_PROBLEM
#         ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANG}.xml
#         "problem"
#         .all.${LANG}.xml
#         ALL_XML_TARGETS
#     )
# endforeach()

# # Handling problem.block files.
# foreach(LANG ${PROGRAMMING_LANGUAGES_BUT_JAVA_AND_AJ})
#     buildXMLTarget(
#         XSLT_BLOCKUNIT_PROBLEM
#         ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANG}.xml
#         "problem"
#         .block.${LANG}.xml
#         ALL_XML_TARGETS
#     )
# endforeach()

# # Handling problem.block.java.xml
# buildXMLTarget(
#     XSLT_BLOCKUNIT_PROBLEM_JAVA
#     ${CMAKE_CURRENT_SOURCE_DIR}/problem.java.xml
#     "problem"
#     .block.java.xml
#     ALL_XML_TARGETS
# )

# # Handling problem.comment files.
# foreach(LANG ${PROGRAMMING_LANGUAGES})
#     buildXMLTarget(
#         XSLT_INSERTCOMMENT_PROBLEM
#         ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANG}.xml
#         "problem"
#         .comment.${LANG}.xml
#         ALL_XML_TARGETS
#     )
# endforeach()

# # Handling the processing of insertion of preprocessor nodes into the files.
# foreach(LANG ${PROGRAMMING_LANGUAGES_BUT_JAVA_AND_AJ})
#     buildXMLTarget(
#         XSLT_INSERTPREPROC_PROBLEM
#         ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANG}.xml
#         "problem"
#         .preproc.${LANG}.xml
#         ALL_XML_TARGETS
#     )
# endforeach()

# # Handling the insertion of ifthanelse into the files.
# foreach(LANG ${PROGRAMMING_LANGUAGES_BUT_JAVA_AND_AJ})
#     buildXMLTarget(
#         XSLT_INSERTIFTHENELSE_PROBLEM
#         ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANG}.xml
#         "problem"
#         .ifthenelse.${LANG}.xml
#         ALL_XML_TARGETS
#     )
# endforeach()
# buildXMLTarget(
#     XSLT_INSERTIFTHENELSE_PROBLEM_JAVA
#     ${CMAKE_CURRENT_SOURCE_DIR}/problem.java.xml
#     "problem"
#     .ifthenelse.java.xml
#     ALL_XML_TARGETS
# )


# # Handling the insertion of the form feed into the files.
# foreach(LANG ${PROGRAMMING_LANGUAGES})
#     buildXMLTarget(
#         XSLT_INSERTFORMFEED_PROBLEM
#         ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANG}.xml
#         "problem"
#         .formfeed.${LANG}.xml
#         ALL_XML_TARGETS
#     )
# endforeach()


# # Xml building stuff.
# set(ALL_CPP_FILES "")
# file(GLOB ALL_CPP_XML_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp.xml)

# foreach(CPP_XML_FILE ${ALL_CPP_XML_FILES})
#     stripFileExt(${CPP_XML_FILE} FILE_EXT_REMOVED)
#     add_custom_command(
#         OUTPUT ${FILE_EXT_REMOVED}
#         DEPENDS ${CPP_XML_FILE}
#         COMMAND ${XSLT_IDENTITY} ${CPP_XML_FILE} > ${FILE_EXT_REMOVED}
#         COMMAND touch ${FILE_EXT_REMOVED}
#     )
#     list(APPEND ALL_CPP_FILES ${FILE_EXT_REMOVED})
# endforeach()

# add_custom_target(identity_cpp
#     DEPENDS ${ALL_CPP_FILES}
#     COMMAND echo "Building all CPP files"
# )

# # #  Test cases generated from other test cases
# # 
# # # function declaration from function
# # function_decl.cpp.xml : function.cpp.xml ${XSLT_DEFN2DECL_XSL}
# #         ${XSLT_DEFN2DECL} $< > $@
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/function_decl.cpp.xml
#     COMMAND ${XSLT_DEFN2DECL} ${CMAKE_CURRENT_SOURCE_DIR}/function.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/function_decl.cpp.xml 
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/function.cpp.xml ${XSLT_DEFN2DECL_XSL}
# )
# list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/function_decl.cpp.xml)

# # Missing depency function_asterisk.cpp.xml
# # function_asterisk_decl.cpp.xml : function_asterisk.cpp.xml ${XSLT_DEFN2DECL_XSL}
# #         ${XSLT_DEFN2DECL} $< > $@
# # add_custom_command(
# #     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/function_asterisk_decl.cpp.xml
# #     COMMAND ${XSLT_DEFN2DECL} ${CMAKE_CURRENT_SOURCE_DIR}/function_asterisk.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/function_asterisk_decl.cpp.xml 
# #     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/function_asterisk.cpp.xml ${XSLT_DEFN2DECL_XSL}
# # )
# # list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/function_asterisk_decl.cpp.xml)

# # # const function from function
# # function_const.cpp.xml : function.cpp.xml ${XSLT_DEFN2DECL_XSL}
# #         ${XSLT_ADDCONST} $< > $@
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/function_const.cpp.xml
#     COMMAND ${XSLT_ADDCONST} ${CMAKE_CURRENT_SOURCE_DIR}/function.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/function_const.cpp.xml 
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/function.cpp.xml ${XSLT_DEFN2DECL_XSL}
# )
# list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/function_const.cpp.xml)

# # return from return base
# # return.cpp.xml : return_base.cpp.xml ${XSLT_INSRT_XSL}
# #         ${XSLT_INSERTEXPR} $< > $@
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/return.cpp.xml
#     COMMAND ${XSLT_INSERTEXPR} ${CMAKE_CURRENT_SOURCE_DIR}/return_base.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/return.cpp.xml
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/return_base.cpp.xml ${XSLT_INSRT_XSL}
# )
# list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/return.cpp.xml)


# # while from if
# # while.cpp.xml : if.cpp.xml ${XSLT_IF2WHILE_XSL}
# #         ${XSLT_IF2WHILE} $< > $@
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/while.cpp.xml
#     COMMAND ${XSLT_IF2WHILE} ${CMAKE_CURRENT_SOURCE_DIR}/if.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/while.cpp.xml 
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/if.cpp.xml ${XSLT_IF2WHILE_XSL}
# )
# list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/while.cpp.xml)

# # if with a nested conditional from if
# # ifnestcond.cpp.xml : if.cpp.xml ${XSLT_NESTCOND_XSL}
# #         ${XSLT_NESTCOND} $< > $@
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/ifnestcond.cpp.xml
#     COMMAND ${XSLT_NESTCOND} ${CMAKE_CURRENT_SOURCE_DIR}/if.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/ifnestcond.cpp.xml
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/if.cpp.xml ${XSLT_NESTCOND_XSL}
# )
# list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/ifnestcond.cpp.xml)



# # constructor from constructor base
# # constructor.cpp.xml : constructor_base.cpp.xml ${XSLT_DEFN2DECL_XSL}
# #         cp $< $@
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/constructor.cpp.xml
#     COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/constructor_base.cpp.xml ${CMAKE_CURRENT_SOURCE_DIR}/constructor.cpp.xml
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/constructor_base.cpp.xml ${XSLT_DEFN2DECL_XSL}
# )
# list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/constructor.cpp.xml)

# # explicit constructor from constructor
# # constructor_explicit.cpp.xml : constructor.cpp.xml ${XSLT_INSRT_XSL}
# #         ${XSLT_ADDEXPLICIT} $< > $@
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/constructor_explicit.cpp.xml
#     COMMAND ${XSLT_ADDEXPLICIT} ${CMAKE_CURRENT_SOURCE_DIR}/constructor_base.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/constructor_explicit.cpp.xml
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/constructor_base.cpp.xml ${XSLT_INSRT_XSL}
# )
# list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/constructor_explicit.cpp.xml)


# # virtual destructor from destructor
# # destructor_virtual.cpp.xml : destructor.cpp.xml ${XSLT_INSRT_XSL}
# #         ${XSLT_ADDVIRTUAL} $< > $@
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/destructor_virtual.cpp.xml
#     COMMAND ${XSLT_ADDVIRTUAL} ${CMAKE_CURRENT_SOURCE_DIR}/destructor.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/destructor_virtual.cpp.xml
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/destructor.cpp.xml ${XSLT_INSRT_XSL}
# )
# list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/destructor_virtual.cpp.xml)

# # throw from throw base
# # throw.cpp.xml : throw_base.cpp.xml ${XSLT_INSRT_XSL}
# #         ${XSLT_INSERTEXPR} $< > $@
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/throw.cpp.xml
#     COMMAND ${XSLT_INSERTEXPR} ${CMAKE_CURRENT_SOURCE_DIR}/throw_base.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/throw.cpp.xml
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/throw_base.cpp.xml ${XSLT_INSRT_XSL}
# )
# list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/throw.cpp.xml)

# # struct from class
# # struct.cpp.xml : class.cpp.xml ${XSLT_CLASS2STRUCT_XSL}
# #         ${XSLT_CLASS2STRUCT} $< > $@
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/struct.cpp.xml
#     COMMAND ${XSLT_CLASS2STRUCT} ${CMAKE_CURRENT_SOURCE_DIR}/class.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/struct.cpp.xml
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/class.cpp.xml ${XSLT_CLASS2STRUCT_XSL}
# )
# list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/struct.cpp.xml)
 
# # union from struct
# # union.cpp.xml : struct.cpp.xml ${XSLT_STRUCT2UNION_XSL}
# #         ${XSLT_STRUCT2UNION} $< > $@
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/union.cpp.xml
#     COMMAND ${XSLT_STRUCT2UNION} ${CMAKE_CURRENT_SOURCE_DIR}/struct.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/union.cpp.xml
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/struct.cpp.xml ${XSLT_STRUCT2UNION_XSL}
# )
# list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/union.cpp.xml)

# # struct from class
# # struct_cpp.cpp.xml : class_cpp.cpp.xml ${XSLT_CLASS2STRUCT_XSL}
# #         ${XSLT_CLASS2STRUCT} $< > $@
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/struct_cpp.cpp.xml
#     COMMAND ${XSLT_CLASS2STRUCT} ${CMAKE_CURRENT_SOURCE_DIR}/class_cpp.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/struct_cpp.cpp.xml
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/class_cpp.cpp.xml ${XSLT_CLASS2STRUCT_XSL}
# )
# list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/struct_cpp.cpp.xml)

# # union from struct
# # union_cpp.cpp.xml : struct_cpp.cpp.xml ${XSLT_STRUCT2UNION_XSL}
# #         ${XSLT_STRUCT2UNION} $< > $@
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/union_cpp.cpp.xml
#     COMMAND ${XSLT_STRUCT2UNION} ${CMAKE_CURRENT_SOURCE_DIR}/struct_cpp.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/union_cpp.cpp.xml
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/struct_cpp.cpp.xml ${XSLT_STRUCT2UNION_XSL}
# )
# list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/union_cpp.cpp.xml)

# # struct from class
# # struct_cs.cs.xml : class_cs.cs.xml ${XSLT_CLASS2STRUCT_XSL}
# #         ${XSLT_CLASS2STRUCT} $< > $@
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/struct_cs.cs.xml
#     COMMAND ${XSLT_CLASS2STRUCT} ${CMAKE_CURRENT_SOURCE_DIR}/class_cs.cs.xml > ${CMAKE_CURRENT_SOURCE_DIR}/struct_cs.cs.xml
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/class_cs.cs.xml ${XSLT_CLASS2STRUCT_XSL}
# )
# list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/struct_cs.cs.xml)

# # union from struct
# # union_cs.cs.xml : struct_cs.cs.xml ${XSLT_STRUCT2UNION_XSL}
# #         ${XSLT_STRUCT2UNION} $< > $@
# # add_custom_command(
# #     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/union_cs.cs.xml
# #     COMMAND ${XSLT_STRUCT2UNION} ${CMAKE_CURRENT_SOURCE_DIR}/struct_cs.cs.xml > ${CMAKE_CURRENT_SOURCE_DIR}/union_cs.cs.xml
# #     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/struct_cs.cs.xml ${XSLT_STRUCT2UNION_XSL}
# # )
# # list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/union_cs.cs.xml)

# # interface from class
# # interface_java.java.xml : class_java.java.xml ${XSLT_CLASS2INTERFACE_XSL}
# #         ${XSLT_CLASS2INTERFACE} $< > $@
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/interface_java.java.xml
#     COMMAND ${XSLT_CLASS2INTERFACE} ${CMAKE_CURRENT_SOURCE_DIR}/class_java.java.xml > ${CMAKE_CURRENT_SOURCE_DIR}/interface_java.java.xml
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/class_java.java.xml ${XSLT_CLASS2INTERFACE_XSL}
# )
# list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/interface_java.java.xml)

# # if with blocks from if
# # ifblock.cpp.xml : if.cpp.xml ${XSLT_IF2IFBLOCK_XSL}
# #         ${XSLT_IF2IFBLOCK} $< > $@
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/ifblock.cpp.xml
#     COMMAND ${XSLT_IF2IFBLOCK} ${CMAKE_CURRENT_SOURCE_DIR}/if.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/ifblock.cpp.xml
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/if.cpp.xml ${XSLT_IF2IFBLOCK_XSL}
# )
# list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/ifblock.cpp.xml)


# add_custom_target(all_xml_targets
#     DEPENDS ${ALL_XML_TARGETS}
#     COMMAND echo "Built all ALL_ALL_CPP_XML_TARGETS"
# )
