#
# @file CMakeLists.txt
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

# message("Added file for Suite")

set(BASE_COMMON "comment string variable_names expression decl_simple if for do switch call function enum return_base problem literal operator_base whitespace ")
set(GEN_BASE "while ifnestcond return function_decl ifblock ")
set(GEN_BASE_CXX "throw constructor_explicit destructor_virtual function_const struct union ")
set(BASE_C_FAMILY "sm macro cstruct extern main function_pointer decl_struct typedef decl_pointer goto expression_pointer enum_cpp ")
set(BASE_C "function_c cast_c ")
set(BASE_OO "catch_form foreach try ")
set(BASE_CXX "throw constructor_explicit destructor_virtual function_const struct union friend namespace template typedef_template constructor destructor throw_base function_throw class function_operator typedef_ns variable_names_ns function_ns decl_ns try typedef_struct operator function_cpp call_ns expression_pointer enum_cpp operator_cpp call_cpp class_cpp decl_simple_cpp cast_cpp doxygen ")
set(BASE_CS "asm extern main decl_pointer goto throw namespace throw_base try operator expression_pointer interface_cs abstract_cs override_cs operator_cs finally_cs checked_cs delegate_cs event_cs explicit_cs fixed_cs foreach_cs implicit_cs internal_cs lock_cs function_operator_cs function_cs decl_cs sealed_cs unchecked_cs unsafe_cs generics_cs linq_cs accessor_cs partial_cs yield_cs class_cs struct_cs constructor_cs destructor_cs literal_cs indexers_cs attributes_cs string_cs lambda_cs enum_cs call_cs class_cs using_directive_cs using_statement_cs ")
set(BASE_CS_GEN "asm extern main decl_pointer goto throw namespace throw_base try operator expression_pointer ")
set(BASE_JAVA "class_java constructor_java keyword_java interface_java package_java finally_java generics_java literal_java operator_java anonymous_class_java synchronized_specifier_java synchronized_statement_java javadoc_java native_java strictfp_java catch_form_java try_java decl_java ")

add_custom_target(echobase COMMAND echo ${BASE_COMMON})
add_custom_target(echogen COMMAND echo ${GEN_BASE})
get_property(XSLTPROC GLOBAL PROPERTY XSLTPROC)

# Merge separate srcML units into one
set(XSLT_MERGEUNIT_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/mergeunit.xsl)
set(XSLT_MERGEUNIT ${XSLTPROC} ${XSLT_MERGEUNIT_XSL})
set(XSLT_MERGEUNIT_PROBLEM_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/mergeunitproblem.xsl)
set(XSLT_MERGEUNIT_PROBLEM ${XSLTPROC} ${XSLT_MERGEUNIT_PROBLEM_XSL})
 
# generate nested conditionals
set(XSLT_NESTCOND_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/nestcond.xsl)
set(XSLT_NESTCOND ${XSLTPROC} ${XSLT_NESTCOND_XSL})

# if to while conversion
set(XSLT_IF2WHILE_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/if2while.xsl)
set(XSLT_IF2WHILE ${XSLTPROC} ${XSLT_IF2WHILE_XSL})

# definition to declaration conversion
set(XSLT_DEFN2DECL_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/defn2decl.xsl)
set(XSLT_DEFN2DECL ${XSLTPROC} ${XSLT_DEFN2DECL_XSL})

# wrap in a block
set(XSLT_BLOCKUNIT ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertblock.xsl)

set(XSLT_BLOCKUNIT_PROBLEM ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertblockproblem.xsl)

set(XSLT_STRUCTUNIT ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertstruct.xsl)

set(XSLT_BLOCKUNIT_JAVA ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertblock.java.xsl)
set(XSLT_BLOCKUNIT_PROBLEM_JAVA ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertblockproblem.java.xsl)

set(XSLT_ADDEXPLICIT ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/addexplicit.xsl)

set(XSLT_ADDVIRTUAL ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/addvirtual.xsl)

set(XSLT_ADDCONST ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/addconst.xsl)

set(XSLT_IDENTITY ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/identity.xsl)

set(XSLT_INSERTEXPR ${XSLTPROC} --stringparam expr_filename "\"${CMAKE_CURRENT_SOURCE_DIR}/expression.cpp.xml\"" ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertexpr.xsl)
set(XSLT_INSERTEXPR_XSL ${CMAKE_CURRENT_SOURCE_DIR}/expression.cpp.xml ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertexpr.xsl)

set(XSLT_INSERTDECL ${XSLTPROC} --stringparam decl_filename "\"${CMAKE_CURRENT_SOURCE_DIR}/decl_simple.cpp.xml\"" ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertparam.xsl)
set(XSLT_INSERTDECL_XSL ${CMAKE_CURRENT_SOURCE_DIR}/decl_simple.cpp.xml ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertdecl.xsl)

set(XSLT_STRUCT2UNION_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/struct2union.xsl)
set(XSLT_STRUCT2UNION ${XSLTPROC} ${XSLT_STRUCT2UNION_XSL})

set(XSLT_CLASS2STRUCT_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/class2struct.xsl)
set(XSLT_CLASS2STRUCT ${XSLTPROC} ${XSLT_CLASS2STRUCT_XSL})

set(XSLT_CLASS2INTERFACE_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/class2interface.xsl)
set(XSLT_CLASS2INTERFACE ${XSLTPROC} ${XSLT_CLASS2INTERFACE_XSL})

set(XSLT_IF2IFBLOCK_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/if2ifblock.xsl)
set(XSLT_IF2IFBLOCK ${XSLTPROC} ${XSLT_IF2IFBLOCK_XSL})

##
# Comments and preprocessor statements are handled out of the normal grammar rules
# in src2srcml.  In order to test the handling of these tokens, special cases are
# generated from the non-comment and preprocessor cases.

# insert comments
set(XSLT_INSERTCOMMENT ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertcomments.xsl)

set(XSLT_INSERTCOMMENT_PROBLEM ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertcommentsproblem.xsl)

# insert preprocessor
set(XSLT_INSERTPREPROC ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertpreproc.xsl)

set(XSLT_INSERTPREPROC_PROBLEM ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertpreprocproblem.xsl)

# nest if
set(XSLT_INSERTIFTHENELSE ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/nestif.xsl)
set(XSLT_INSERTIFTHENELSE_JAVA ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/nestif.java.xsl)
set(XSLT_INSERTIFTHENELSE_PROBLEM ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/nestifproblem.xsl)
set(XSLT_INSERTIFTHENELSE_PROBLEM_JAVA ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/nestifproblem.java.xsl)

# insert form feed
set(XSLT_INSERTFORMFEED ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertformfeed.xsl)
set(XSLT_INSERTFORMFEED_PROBLEM ${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertformfeedproblem.xsl)

#
# Test cases for one language are generated from test cases for another language.
# 

set(XSLT_SETLANGUAGE_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/setlanguage.xsl)
set(XSLT_SETLANGUAGE_PROBLEM_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/setlanguageproblem.xsl)
set(XSLT_REMOVECPP_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/removecpp.xsl)
set(XSLT_REMOVECPP_PROBLEM_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/removecppproblem.xsl)
set(XSLT_REMOVECPP_LITERAL_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/removecppliteral.xsl)
set(XSLT_REMOVECPP_OPERATOR_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/removecppoperator.xsl)

set(XSLT_SETLANGUAGE ${XSLTPROC} ${XSLT_SETLANGUAGE_XSL})

set(XSLT_SETLANGUAGE_C ${XSLTPROC} --stringparam language "\"C\"" ${XSLT_SETLANGUAGE_XSL})

set(XSLT_SETLANGUAGE_CS ${XSLTPROC} --stringparam language "\"C#\"" ${XSLT_SETLANGUAGE_XSL})

set(XSLT_SETLANGUAGE_CXX ${XSLTPROC} --stringparam language "\"CXX\"" ${XSLT_SETLANGUAGE_XSL})

set(XSLT_SETLANGUAGE_CXX_11 ${XSLTPROC} --stringparam language "\"C++11\"" ${XSLT_SETLANGUAGE_XSL})

set(XSLT_SETLANGUAGE_JAVA ${XSLTPROC} --stringparam language "\"Java\"" ${XSLT_SETLANGUAGE_XSL})
set(XSLT_SETLANGUAGE_ASPECTJ ${XSLTPROC} --stringparam language "\"AspectJ\"" ${XSLT_SETLANGUAGE_XSL})

set(XSLT_SETLANGUAGE_PROBLEM ${XSLTPROC} ${XSLT_SETLANGUAGE_PROBLEM_XSL})

set(XSLT_SETLANGUAGE_PROBLEM_C ${XSLTPROC} --stringparam language "\"C\"" ${XSLT_SETLANGUAGE_PROBLEM_XSL})

set(XSLT_SETLANGUAGE_PROBLEM_CS ${XSLTPROC} --stringparam language \""C#\"" ${XSLT_SETLANGUAGE_PROBLEM_XSL})

set(XSLT_SETLANGUAGE_PROBLEM_CXX ${XSLTPROC} --stringparam language "\"CXX\"" ${XSLT_SETLANGUAGE_PROBLEM_XSL})

set(XSLT_SETLANGUAGE_PROBLEM_CXX_11 ${XSLTPROC} --stringparam language "\"C++11\"" ${XSLT_SETLANGUAGE_PROBLEM_XSL})

set(XSLT_SETLANGUAGE_PROBLEM_JAVA ${XSLTPROC} --stringparam language "\"Java\"" ${XSLT_SETLANGUAGE_PROBLEM_XSL})
set(XSLT_SETLANGUAGE_PROBLEM_ASPECTJ ${XSLTPROC} --stringparam language "\"AspectJ\"" ${XSLT_SETLANGUAGE_PROBLEM_XSL})

set(XSLT_REMOVECPP ${XSLTPROC} ${XSLT_REMOVECPP_XSL})
set(XSLT_REMOVECPP_PROBLEM ${XSLTPROC} ${XSLT_REMOVECPP_PROBLEM_XSL})
set(XSLT_REMOVECPP_LITERAL ${XSLTPROC} ${XSLT_REMOVECPP_LITERAL_XSL})
set(XSLT_REMOVECPP_OPERATOR ${XSLTPROC} ${XSLT_REMOVECPP_OPERATOR_XSL})

set(SEP_BASE "${BASE_COMMON} ${BASE_C_FAMILY} ${BASE_CXX} ${BASE_OO}")


# basic generated C++ files
set(GEN_SEP "${GEN_BASE} ${GEN_BASE_CXX}")

# generated cases for C++ from base
stringListTofileNames(${GEN_SEP} ".cpp.xml" GEN_PART)

set(SEP_FILES "${SEP_BASE} preprocessor")

# base for all of Java
set(BASE_JAVA_ALL_GEN "${BASE_COMMON} ${GEN_BASE} ${BASE_OO}")
set(BASE_JAVA_ALL "${BASE_COMMON} ${GEN_BASE} ${BASE_OO} ${BASE_JAVA}")
stringListTofileNames(${BASE_JAVA_ALL_GEN} ".java.xml" GEN_JAVA)

# Base for all C.
set(BASE_C_ALL_GEN "${BASE_COMMON} ${GEN_BASE} ${BASE_C_FAMILY}")
set(BASE_C_ALL "${BASE_COMMON} ${GEN_BASE} ${BASE_C_FAMILY} ${BASE_C}")
stringListTofileNames(${BASE_C_ALL_GEN} ".c.xml" GEN_C)

# base for all C#
set(BASE_CS_ALL_GEN "${BASE_COMMON} ${GEN_BASE} ${BASE_CS_GEN}")
set(BASE_CS_ALL "${BASE_COMMON} ${GEN_BASE} ${BASE_CS} ${BASE_CS_GEN}")
stringListTofileNames(${BASE_CS_ALL_GEN} ".cs.xml" GEN_CS)

set(SEP_PLUS "${SEP_FILES} ${GEN_SEP}")
set(SEP_PLUSNOPREPROC "${SEP_BASE} ${GEN_SEP}")

set(SEP_STRUCT "function function_const function_cpp function_decl function_ns function_operator function_pointer function_throw ")

macro(generateFileGroup FILE_EXT_PREFIX OUTPUT)
    stringListTofileNames(${SEP_PLUS} ".${FILE_EXT_PREFIX}.cpp.xml" SEP_PLUS_NAMES)
    stringListTofileNames(${BASE_JAVA_ALL} ".${FILE_EXT_PREFIX}.java.xml" JAVA_NAMES)
    stringListTofileNames(${BASE_C_ALL} ".${FILE_EXT_PREFIX}.c.xml" C_NAMES)
    stringListTofileNames(${BASE_CS_ALL} ".${FILE_EXT_PREFIX}.cs.xml" CS_NAMES)    
    # stringListTofileNames(${BASE_ASPECTJ_ALL} ".${FILE_EXT_PREFIX}.aj.xml" AJ_NAMES)
    # stringListTofileNames(${BASE_CXX_11_ALL} ".${FILE_EXT_PREFIX}.cpp11.xml" CPP11_NAMES)
    set(${OUTPUT} "${SEP_PLUS_NAMES} ${JAVA_NAMES} ${C_NAMES} ${CS_NAMES} ${ARGN}")
    # Remember to add second statemement when and if these are added back into the test suite.
    # set(${OUTPUT} "${OUTPUT} ${AJ_NAMES} ${CPP11_NAMES}")
endmacro()

#
# generate cases for all
# 

# These are removed until they are defined again.
generateFileGroup("all" GEN_ALL "extra_cpp.all.cpp.xml extra_c.all.c.xml")

# generated cases for blocks
generateFileGroup("block" GEN_BLOCK "extra_c.block.c.xml extra_cpp.block.cpp.xml") 

# generated cases for structs
stringListTofileNames(${SEP_STRUCT} ".struct.cpp.xml" GEN_STRUCT)

# generated cases for comments
generateFileGroup("comment" GEN_COMMENT "extra_c.comment.c.xml extra_cpp.comment.cpp.xml")

# # generated cases for formfeed
generateFileGroup("formfeed" GEN_FORMFEED "extra_c.formfeed.c.xml extra_cpp.formfeed.cpp.xml")

# generated test cases for preproc
stringListTofileNames(${SEP_PLUSNOPREPROC} ".preproc.cpp.xml" CPP_SEP_PLUSNO_PREPROC_NAMES)
stringListTofileNames(${BASE_JAVA_ALL} ".preproc.java.xml" CPP_JAVA_NAMES)
stringListTofileNames(${BASE_C_ALL} ".preproc.c.xml" CPP_C_NAMES)
stringListTofileNames(${BASE_CS_ALL} ".preproc.cs.xml" CPP_CS_NAMES)
set(GEN_PREPROC "${CPP_SEP_PLUSNO_PREPROC_NAMES} ${CPP_JAVA_NAMES} ${CPP_C_NAMES} ${CPP_CS_NAMES}")

# # generated test cases for preproc
generateFileGroup("ifthenelse" GEN_IFTHENELSE "extra_c.ifthenelse.c.xml extra_cpp.ifthenelse.cpp.xml")


# TODO FIXME 
# file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/allinall.c.xml "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>
#   <unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\" dir=\"allinall.c\">
# ")
# file(GLOB allinallCXMLFiles "${CMAKE_CURRENT_BINARY_DIR}/*.all.c.xml" DEPENDS allinall)
# 
# file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/allinall.c.xml "</unit>")
# add_custom_target(allinall DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/allinall.c.xml ${allinallCXMLFiles})


# FIXME FIXME FIXME FIXME FIXME
# Not sure of any of this actually works correctly. It doesn't when
# I tried it using the original make file
# FIXME FIXME FIXME FIXME FIXME
# allinall.c.xml :
#         @echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > allinall.c.xml
#         @echo "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\" dir=\"allinall.c\">" >> allinall.c.xml
# 
#         find ../suite -name "*.all.c.xml" | xargs -i tail +3 {} | sed "s|</unit>||">> allinall.c.xml
#         @echo "</unit>" >> allinall.c.xml
# 
# allinall.java.xml :
#         @echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > allinall.java.xml
#         @echo "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"Java\" dir=\"allinall.java\">" >> allinall.java.xml
# 
#         find ../suite -name "*.all.java.xml" | xargs -i tail+3 {} | sed "s|</unit>||">> allinall.java.xml
#         @echo "</unit>" >> allinall.java.xml
# 
# allinall.1.cpp.xml :
#         @echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > allinall.1.cpp.xml
#         @echo "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"allinall.1.cpp\">" >> allinall.1.cpp.xml
# 
#         @find ../suite -name "*.all.cpp.xml" | sed "s|cstruct.all.cpp.xml||" | sed "s|functionkr.all.cpp.xml||" | head -30 | xargs -i tail +3 {} | sed "s|</unit>||" >> allinall.1.cpp.xml
#         @echo "</unit>" >> allinall.1.cpp.xml
# 
# allinall.2.cpp.xml :
#         @echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > allinall.2.cpp.xml
#         @echo "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"allinall.2.cpp\">" >> allinall.2.cpp.xml
# 
#         @find ../suite -name "*.all.cpp.xml" | sed "s|cstruct.all.cpp.xml||" | sed "s|functionkr.all.cpp.xml||" | tail -18 | xargs -i tail +3 {} | sed "s|</unit>||" >> allinall.2.cpp.xml
#         @echo "</unit>" >> allinall.2.cpp.xml
# 
# allinone : allinonecpp allinonec allinonejava
#         #@make cleanall
# 
# allinonec : allinall.c.xml
#         -${SRCML2SRC} ${SRCENCODING} allinall.c.xml | ${SRC2SRCML} -l C ${SRCENCODING} --directory="allinall.c" | diff -w  - allinall.c.xml
# 
# allinonecpp : allinall.1.cpp.xml allinall.2.cpp.xml
#         -${SRCML2SRC} ${SRCENCODING} allinall.1.cpp.xml | ${SRC2SRCML} -l C++ ${SRCENCODING} --directory="allinall.1.cpp" | diff -w  - allinall.1.cpp.xml
#         -${SRCML2SRC} ${SRCENCODING} allinall.2.cpp.xml | ${SRC2SRCML} -l C++ ${SRCENCODING} --directory="allinall.2.cpp" | diff -w  - allinall.2.cpp.xml
# 
# allinonejava : allinall.java.xml
#         -${SRCML2SRC} ${SRCENCODING} allinall.java.xml | ${SRC2SRCML} -l Java ${SRCENCODING} --directory="allinall.java" | diff -w  - allinall.java.xml
# 
# cleanall :
#         rm -f allinall.*

# FIXME End of all in one file generation commands FIXME


# problem.java.xml : problem.cpp.xml
#         ${XSLT_SETLANGUAGE_PROBLEM_JAVA} $< | ${XSLT_REMOVECPP_PROBLEM} - > $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/problem.java.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/problem.cpp.xml
    COMMAND ${XSLT_SETLANGUAGE_JAVA} ${CMAKE_CURRENT_SOURCE_DIR}/problem.cpp.xml | ${XSLT_REMOVECPP_PROBLEM} - > ${CMAKE_CURRENT_SOURCE_DIR}/problem.java.xml
    COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/problem.java.xml
)

# problem.aj.xml : problem.java.xml
#         ${XSLT_SETLANGUAGE_PROBLEM_ASPECTJ} $< > $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/problem.aj.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/problem.java.xml
    COMMAND ${XSLT_SETLANGUAGE_PROBLEM_ASPECTJ} ${CMAKE_CURRENT_SOURCE_DIR}/problem.java.xml > ${CMAKE_CURRENT_SOURCE_DIR}/problem.aj.xml
    COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/problem.aj.xml
)

# problem.c.xml : problem.cpp.xml
#         ${XSLT_SETLANGUAGE_PROBLEM_C} $< > $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/problem.c.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/problem.cpp.xml
    COMMAND ${XSLT_SETLANGUAGE_PROBLEM_C} ${CMAKE_CURRENT_SOURCE_DIR}/problem.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/problem.c.xml
    COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/problem.c.xml
)

# problem.cs.xml : problem.cpp.xml
#         ${XSLT_SETLANGUAGE_PROBLEM_CS} $< > $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/problem.cs.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/problem.cpp.xml
    COMMAND ${XSLT_SETLANGUAGE_PROBLEM_C} ${CMAKE_CURRENT_SOURCE_DIR}/problem.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/problem.cs.xml
    COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/problem.cs.xml
)

# This is used to simplfy the handling of each of the different programming languages
# so that it could be done more easily using macros.
set(PROGRAMMING_LANGUAGES cpp c java aj)

# Handling the problem.all files.

# 
# This function assists with the creation of dependencies with targets being built
# by other functions within this file. This allows individual targets to be invoked
# with make using the file name as the target name.
# 
macro(createProbDep PROBLEM_TYPE LANGUAGE_EXT)
    add_custom_target(problem.${PROBLEM_TYPE}.${LANGUAGE_EXT}.xml
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/problem.${PROBLEM_TYPE}.${LANGUAGE_EXT}.xml
        COMMAND echo "Built problem.${PROBLEM_TYPE}.${LANGUAGE_EXT}.xml"
    )    
endmacro()

# 
# This macro helps build a custom command based on the supplied language which is
# performs the merge unit command for each of the different languages.
# 
macro(problemAll LANGUAGE_EXT)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/problem.all.${LANGUAGE_EXT}.xml
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANGUAGE_EXT}.xml
        COMMAND ${XSLT_MERGEUNIT_PROBLEM} ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANGUAGE_EXT}.xml > ${CMAKE_CURRENT_SOURCE_DIR}/problem.all.${LANGUAGE_EXT}.xml
        COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/problem.all.${LANGUAGE_EXT}.xml
    )    
endmacro()


set(PROBLEM_ALL_TARGETS "")
foreach(LANG ${PROGRAMMING_LANGUAGES})
    problemAll(${LANG})
    createProbDep(all ${LANG})
    list(APPEND PROBLEM_ALL_TARGETS problem.all.${LANG}.xml)
endforeach()

add_custom_target(problem_all
    DEPENDS ${PROBLEM_ALL_TARGETS}
    COMMAND echo "Built all PROBLEM_ALL_TARGETS"
)


# Handling problem.block files.

#
# This function assists with the processing of the problem.block targets
# 
macro(problemBlock LANGUAGE_EXT)
    if((${LANGUAGE_EXT} MATCHES "java") OR (${LANGUAGE_EXT} MATCHES "aj"))
        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/problem.block.${LANGUAGE_EXT}.xml
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANGUAGE_EXT}.xml
            COMMAND ${XSLT_BLOCKUNIT_PROBLEM_JAVA} ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANGUAGE_EXT}.xml > ${CMAKE_CURRENT_SOURCE_DIR}/problem.block.${LANGUAGE_EXT}.xml
            COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/problem.block.${LANGUAGE_EXT}.xml
        )
    else()
        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/problem.block.${LANGUAGE_EXT}.xml
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANGUAGE_EXT}.xml
            COMMAND ${XSLT_BLOCKUNIT_PROBLEM} ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANGUAGE_EXT}.xml > ${CMAKE_CURRENT_SOURCE_DIR}/problem.block.${LANGUAGE_EXT}.xml
            COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/problem.block.${LANGUAGE_EXT}.xml
        )
    endif()
endmacro()


set(PROBLEM_BLOCK_TARGETS "")
foreach(LANG ${PROGRAMMING_LANGUAGES})
    problemBlock(${LANG})
    createProbDep(block ${LANG})
    list(APPEND PROBLEM_BLOCK_TARGETS problem.block.${LANG}.xml)
endforeach()

add_custom_target(problem_block
    DEPENDS ${PROBLEM_BLOCK_TARGETS}
    COMMAND echo "Built all PROBLEM_BLOCK_TARGETS"
)

 
# Handling problem.comment files..

# 
# This function handles the processing of language extensions so
# they can be run through XSLTPROC and comments inseted.
# 
macro(problemComment LANGUAGE_EXT)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/problem.comment.${LANGUAGE_EXT}.xml
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANGUAGE_EXT}.xml
        COMMAND ${XSLT_INSERTCOMMENT_PROBLEM} ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANGUAGE_EXT}.xml > ${CMAKE_CURRENT_SOURCE_DIR}/problem.comment.${LANGUAGE_EXT}.xml
        COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/problem.comment.${LANGUAGE_EXT}.xml
    )
endmacro()

set(PROBLEM_COMMENT_TARGETS "")
foreach(LANG ${PROGRAMMING_LANGUAGES})
    problemComment(${LANG})
    createProbDep(comment ${LANG})
    list(APPEND PROBLEM_COMMENT_TARGETS problem.comment.${LANG}.xml)
endforeach()

add_custom_target(problem_comment
    DEPENDS ${PROBLEM_COMMENT_TARGETS}
    COMMAND echo "Built all PROBLEM_COMMENT_TARGETS"
)

# Handling the processing of insertion of preprocessor nodes into the files.
macro(problemPreproc LANGUAGE_EXT)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/problem.preproc.${LANGUAGE_EXT}.xml
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANGUAGE_EXT}.xml
        COMMAND ${XSLT_INSERTPREPROC_PROBLEM} ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANGUAGE_EXT}.xml > ${CMAKE_CURRENT_SOURCE_DIR}/problem.preproc.${LANGUAGE_EXT}.xml
        COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/problem.preproc.${LANGUAGE_EXT}.xml
    )
endmacro()

set(PROBLEM_INSERT_PREPROC_TARGETS "")
foreach(LANG ${PROGRAMMING_LANGUAGES})
    problemPreproc(${LANG})
    createProbDep(preproc ${LANG})
    list(APPEND PROBLEM_INSERT_PREPROC_TARGETS problem.preproc.${LANG}.xml)
endforeach()

add_custom_target(problem_preproc
    DEPENDS ${PROBLEM_INSERT_PREPROC_TARGETS}
    COMMAND echo "Built all PROBLEM_INSERT_PREPROC_TARGETS"
)

# Handling the insertion of ifthanelse into the files.
macro(problemIfThenElse LANGUAGE_EXT)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/problem.ifthenelse.${LANGUAGE_EXT}.xml
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANGUAGE_EXT}.xml
        COMMAND ${XSLT_INSERTIFTHENELSE_PROBLEM} ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANGUAGE_EXT}.xml > ${CMAKE_CURRENT_SOURCE_DIR}/problem.ifthenelse.${LANGUAGE_EXT}.xml
        COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/problem.ifthenelse.${LANGUAGE_EXT}.xml
    )
endmacro()

set(PROBLEM_IFTHENELSE_TARGETS "")
foreach(LANG ${PROGRAMMING_LANGUAGES})
    problemIfThenElse(${LANG})
    createProbDep(ifthenelse ${LANG})
    list(APPEND PROBLEM_IFTHENELSE_TARGETS problem.ifthenelse.${LANG}.xml)
endforeach()

add_custom_target(problem_ifthenelse
    DEPENDS ${PROBLEM_IFTHENELSE_TARGETS}
    COMMAND echo "Built all PROBLEM_IFTHENELSE_TARGETS"
)

# Handling the insertion of the form feed into the files.
macro(problemFormFeed LANGUAGE_EXT)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/problem.formfeed.${LANGUAGE_EXT}.xml
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANGUAGE_EXT}.xml
        COMMAND ${XSLT_INSERTFORMFEED_PROBLEM} ${CMAKE_CURRENT_SOURCE_DIR}/problem.${LANGUAGE_EXT}.xml > ${CMAKE_CURRENT_SOURCE_DIR}/problem.formfeed.${LANGUAGE_EXT}.xml
        COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/problem.formfeed.${LANGUAGE_EXT}.xml
    )
endmacro()

set(PROBLEM_FORMFEED_TARGETS "")
foreach(LANG ${PROGRAMMING_LANGUAGES})
    problemFormFeed(${LANG})
    createProbDep(formfeed ${LANG})
    list(APPEND PROBLEM_FORMFEED_TARGETS problem.formfeed.${LANG}.xml)
endforeach()

add_custom_target(problem_formfeed
    DEPENDS ${PROBLEM_FORMFEED_TARGETS}
    COMMAND echo "Built all PROBLEM_FORMFEED_TARGETS"
)

# Testing command which forces ALL depencies to be built which start with "problem_"
add_custom_target(build_all_problems
    DEPENDS problem_comment problem_block problem_all problem_preproc problem_ifthenelse problem_formfeed
    COMMAND echo "Built all."
)

# Xml building stuff.
set(ALL_CPP_FILES "")
file(GLOB ALL_CPP_XML_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp.xml)

# Adding generated dependent files because they don't get picked up by the file GLOB.
list(APPEND ALL_CPP_XML_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/function_decl.cpp.xml
    #     Removed because the test file doesn't exist at the moment.
    #     ${CMAKE_CURRENT_SOURCE_DIR}/function_asterisk_decl.cpp.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/function_const.cpp.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/while.cpp.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/ifnestcond.cpp.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/return.cpp.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/constructor.cpp.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/constructor_explicit.cpp.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/destructor_virtual.cpp.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/throw.cpp.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/struct.cpp.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/union.cpp.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/struct_cpp.cpp.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/union_cpp.cpp.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/ifblock.cpp.xml
)

foreach(CPP_XML_FILE ${ALL_CPP_XML_FILES})
    stripFileExt(${CPP_XML_FILE} FILE_EXT_REMOVED)
    add_custom_command(
        OUTPUT ${FILE_EXT_REMOVED}
        DEPENDS ${CPP_XML_FILE}
        COMMAND ${XSLT_IDENTITY} ${CPP_XML_FILE} > ${FILE_EXT_REMOVED}
        COMMAND touch ${FILE_EXT_REMOVED}
    )
    list(APPEND ALL_CPP_FILES ${FILE_EXT_REMOVED})
endforeach()

add_custom_target(identity_cpp
    DEPENDS ${ALL_CPP_FILES}
    COMMAND echo "Building all CPP files"
)

# Modified list of all cpp.xml files to process (this excludes those which don't
# end with the extension .cpp.xml (i.e. .all.cpp.xml is excluded)).
set(ALL_NON_GENERATED_CPP_XML_FILES "")
foreach(CURRENT_CPP ${ALL_CPP_XML_FILES})
    string(REGEX MATCH "[/|\\|^][^/|\\|\\.]*\\.cpp\\.xml$" FILE_NAME ${CURRENT_CPP})
    if(NOT ${FILE_NAME} EQUAL "")
        string(REGEX REPLACE "[/|\\]" "" FILE_NAME ${FILE_NAME})
        string(REPLACE ".cpp.xml" "" FILE_NAME ${FILE_NAME})
        list(APPEND ALL_NON_GENERATED_CPP_XML_FILES ${FILE_NAME})
    endif()
endforeach()

# 
# This macro helps create tules to be built to create all of the
# .cpp.xml files.
# 
macro(buildXMLTarget COMMAND_NAME INPUT_FILE OUTPUT_FILE_TITLE SUFFIX) 
    set(OUTPUT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT_FILE_TITLE}${SUFFIX})
    add_custom_command(
        OUTPUT ${OUTPUT_FILE_NAME}
        COMMAND ${${COMMAND_NAME}} ${INPUT_FILE} > ${OUTPUT_FILE_NAME}
        DEPENDS ${INPUT_FILE}
    )
    foreach(LIST_TO_ADD_TO ${ARGN})
        list(APPEND ${LIST_TO_ADD_TO} ${OUTPUT_FILE_NAME})
    endforeach()
endmacro()

# 
# This is a macro to assist with the creation of a .java.xml file from a .cpp.xml
# file.
# 
macro(buildJavaXmlTarget INPUT_FILE OUTPUT_FILE_TITLE)
    set(OUTPUT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT_FILE_TITLE}.java.xml)
    if(${OUTPUT_FILE_TITLE} EQUAL "operator_base")
        add_custom_command(
            OUTPUT ${OUTPUT_FILE_NAME}
            COMMAND ${XSLT_SETLANGUAGE_JAVA} ${INPUT_FILE} | ${XSLT_REMOVECPP_OPERATOR} - > ${OUTPUT_FILE_NAME}
            DEPENDS ${INPUT_FILE}
        )
    elseif(${OUTPUT_FILE_TITLE} EQUAL "literal")
        add_custom_command(
            OUTPUT ${OUTPUT_FILE_NAME}
            COMMAND ${XSLT_SETLANGUAGE_JAVA} ${INPUT_FILE} | ${XSLT_REMOVECPP_LITERAL} - > ${OUTPUT_FILE_NAME}
            DEPENDS ${INPUT_FILE}
        )
    else()
        add_custom_command(
            OUTPUT ${OUTPUT_FILE_NAME}
            COMMAND ${XSLT_SETLANGUAGE_JAVA} ${INPUT_FILE} | ${XSLT_REMOVECPP} - > ${OUTPUT_FILE_NAME}
            DEPENDS ${INPUT_FILE}
        )
    endif()

    foreach(LIST_TO_ADD_TO ${ARGN})
        list(APPEND ${LIST_TO_ADD_TO} ${OUTPUT_FILE_NAME})
    endforeach()
endmacro()

set(ALL_XML_TARGETS "")
set(C_PROCESSING_COLLECTION
    ".all.c.xml\;XSLT_MERGEUNIT"
    ".block.c.xml\;XSLT_BLOCKUNIT"
    ".comment.c.xml\;XSLT_INSERTCOMMENT"
    ".preproc.c.xml\;XSLT_INSERTPREPROC"
    ".ifthenelse.c.xml\;XSLT_INSERTIFTHENELSE"
    ".formfeed.c.xml\;XSLT_INSERTFORMFEED"
)
set(CS_PROCESSING_COLLECTION
    ".all.cs.xml\;XSLT_MERGEUNIT"
    ".block.cs.xml\;XSLT_BLOCKUNIT"
    ".comment.cs.xml\;XSLT_INSERTCOMMENT"
    ".preproc.cs.xml\;XSLT_INSERTPREPROC"
    ".ifthenelse.cs.xml\;XSLT_INSERTIFTHENELSE"
    ".formfeed.cs.xml\;XSLT_INSERTFORMFEED"
)
set(JAVA_PROCESSING_COLLECTION
    ".all.java.xml\;XSLT_MERGEUNIT"
    ".block.java.xml\;XSLT_BLOCKUNIT_JAVA"
    ".comment.java.xml\;XSLT_INSERTCOMMENT"
    ".preproc.java.xml\;XSLT_INSERTPREPROC"
    ".ifthenelse.java.xml\;XSLT_INSERTIFTHENELSE_JAVA"
    ".formfeed.java.xml\;XSLT_INSERTFORMFEED"
)
set(ASPECTJ_PROCESSING_COLLECTION
    ".all.aj.xml\;XSLT_MERGEUNIT"
    ".block.aj.xml\;XSLT_BLOCKUNIT_JAVA"
    ".comment.aj.xml\;XSLT_INSERTCOMMENT"
    ".preproc.aj.xml\;XSLT_INSERTPREPROC"
    ".ifthenelse.aj.xml\;XSLT_INSERTIFTHENELSE_JAVA"
    ".formfeed.aj.xml\;XSLT_INSERTFORMFEED"
)
foreach(CURRENT_FILE_TITLE ${ALL_NON_GENERATED_CPP_XML_FILES})

    # Handling special case for problem.cpp.xml
    set(INPUT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_FILE_TITLE}.cpp.xml)
    if(${CURRENT_FILE_TITLE} MATCHES "problem")
        set(CURRENT_FILE_TITLE ${CURRENT_FILE_TITLE}_2)
    endif()
    
    
    # %.all.cpp.xml : %.cpp.xml
    #         ${XSLT_MERGEUNIT} $< > $@
    buildXMLTarget(XSLT_MERGEUNIT
        ${INPUT_FILE_NAME}
        ${CURRENT_FILE_TITLE}
        .all.cpp.xml
        ALL_XML_TARGETS
    )
    
    # %.comment.cpp.xml : %.cpp.xml
    #         ${XSLT_INSERTCOMMENT} $< > $@
    buildXMLTarget(XSLT_INSERTCOMMENT
        ${INPUT_FILE_NAME}
        ${CURRENT_FILE_TITLE}
        .comment.cpp.xml
        ALL_XML_TARGETS
    )
    
    # %.preproc.cpp.xml : %.cpp.xml
    #         ${XSLT_INSERTPREPROC} $< > $@
    buildXMLTarget(XSLT_INSERTPREPROC
        ${INPUT_FILE_NAME}
        ${CURRENT_FILE_TITLE}
        .preproc.cpp.xml
        ALL_XML_TARGETS
    )
    
    # %.block.cpp.xml : %.cpp.xml
    #         ${XSLT_BLOCKUNIT} $< > $@
    buildXMLTarget(XSLT_BLOCKUNIT
        ${INPUT_FILE_NAME}
        ${CURRENT_FILE_TITLE}
        .block.cpp.xml
        ALL_XML_TARGETS
    )

    # %.struct.cpp.xml : %.cpp.xml
    #         ${XSLT_STRUCTUNIT} $< > $@
    buildXMLTarget(XSLT_STRUCTUNIT
        ${INPUT_FILE_NAME}
        ${CURRENT_FILE_TITLE}
        .struct.cpp.xml
        ALL_XML_TARGETS
    )

    # %.ifthenelse.cpp.xml : %.cpp.xml
    #         ${XSLT_INSERTIFTHENELSE} $< > $@
    buildXMLTarget(XSLT_INSERTIFTHENELSE
        ${INPUT_FILE_NAME}
        ${CURRENT_FILE_TITLE}
        .ifthenelse.cpp.xml
        ALL_XML_TARGETS
    )

    # %.formfeed.cpp.xml : %.cpp.xml
    #         ${XSLT_INSERTFORMFEED} $< > $@
    buildXMLTarget(XSLT_INSERTFORMFEED
        ${INPUT_FILE_NAME}
        ${CURRENT_FILE_TITLE}
        .formfeed.cpp.xml
        ALL_XML_TARGETS
    )

    # %.cpp11.xml : %.cpp.xml
    #         ${XSLT_SETLANGUAGE_CXX_11} $< > $@
    buildXMLTarget(XSLT_SETLANGUAGE_CXX_11
        ${INPUT_FILE_NAME}
        ${CURRENT_FILE_TITLE}
        .cpp11.xml
        ALL_XML_TARGETS
    )

    # Handling all of the C file conversions and depencies.
    # %.c.xml : %.cpp.xml
    #         ${XSLT_SETLANGUAGE_C} $< > $@
    buildXMLTarget(XSLT_SETLANGUAGE_C
        ${INPUT_FILE_NAME}
        ${CURRENT_FILE_TITLE}
        .c.xml
        ALL_XML_TARGETS
    )
    
    set(CURRENT_C_INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_FILE_TITLE}.c.xml)
    
    # %.all.c.xml : %.c.xml
    #         ${XSLT_MERGEUNIT} $< > $@
    # %.block.c.xml : %.c.xml
    #         ${XSLT_BLOCKUNIT} $< > $@
    # %.comment.c.xml : %.c.xml
    #         ${XSLT_INSERTCOMMENT} $< > $@
    # %.preproc.c.xml : %.c.xml
    #         ${XSLT_INSERTPREPROC} $< > $@
    # %.ifthenelse.c.xml : %.c.xml
    #         ${XSLT_INSERTIFTHENELSE} $< > $@
    # %.formfeed.c.xml : %.c.xml
    #         ${XSLT_INSERTFORMFEED} $< > $@
    foreach(PROC_INSTRUCTIONS ${C_PROCESSING_COLLECTION})
        list(GET PROC_INSTRUCTIONS 0 SUFFIX)
        list(GET PROC_INSTRUCTIONS 1 CMD)
        buildXMLTarget(
            ${CMD}
            ${CURRENT_C_INPUT_FILE}
            ${CURRENT_FILE_TITLE}
            ${SUFFIX}
            ALL_XML_TARGETS
        )
    endforeach()
    
    # Processing all of the C# files
    # %.cs.xml : %.cpp.xml
    #         ${XSLT_SETLANGUAGE_CS} $< > $@
    buildXMLTarget(XSLT_SETLANGUAGE_CS
        ${INPUT_FILE_NAME}
        ${CURRENT_FILE_TITLE}
        .cs.xml
        ALL_XML_TARGETS
    )
    
    set(CURRENT_CS_INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_FILE_TITLE}.cs.xml)
    # %.all.cs.xml : %.cs.xml
    #         ${XSLT_MERGEUNIT} $< > $@
    # %.block.cs.xml : %.cs.xml
    #         ${XSLT_BLOCKUNIT} $< > $@ 
    # %.comment.cs.xml : %.cs.xml
    #         ${XSLT_INSERTCOMMENT} $< > $@
    # %.preproc.cs.xml : %.cs.xml
    #         ${XSLT_INSERTPREPROC} $< > $@
    # %.ifthenelse.cs.xml : %.cs.xml
    #         ${XSLT_INSERTIFTHENELSE} $< > $@
    # %.formfeed.cs.xml : %.cs.xml
    #         ${XSLT_INSERTFORMFEED} $< > $@
    foreach(PROC_INSTRUCTIONS ${CS_PROCESSING_COLLECTION})
        list(GET PROC_INSTRUCTIONS 0 SUFFIX)
        list(GET PROC_INSTRUCTIONS 1 CMD)
        buildXMLTarget(
            ${CMD}
            ${CURRENT_CS_INPUT_FILE}
            ${CURRENT_FILE_TITLE}
            ${SUFFIX}
            ALL_XML_TARGETS
        )
    endforeach()
    
    # Handling java conversion.
    buildJavaXmlTarget(
        ${INPUT_FILE_NAME}
        ${CURRENT_FILE_TITLE}
        ALL_XML_TARGETS
    )
    set(CURRENT_JAVA_INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_FILE_TITLE}.java.xml)
    # Java
    # %.all.java.xml : %.java.xml
    #         ${XSLT_MERGEUNIT} $< > $@
    # %.block.java.xml : %.java.xml
    #         ${XSLT_BLOCKUNIT_JAVA} $< > $@
    # %.comment.java.xml : %.java.xml
    #         ${XSLT_INSERTCOMMENT} $< > $@
    # %.preproc.java.xml : %.java.xml
    #         ${XSLT_INSERTPREPROC} $< > $@
    # %.ifthenelse.java.xml : %.java.xml
    #         ${XSLT_INSERTIFTHENELSE_JAVA} $< > $@
    # %.formfeed.java.xml : %.java.xml
    #         ${XSLT_INSERTFORMFEED} $< > $@
    foreach(PROC_INSTRUCTIONS ${JAVA_PROCESSING_COLLECTION})
        list(GET PROC_INSTRUCTIONS 0 SUFFIX)
        list(GET PROC_INSTRUCTIONS 1 CMD)
        buildXMLTarget(
            ${CMD}
            ${CURRENT_JAVA_INPUT_FILE}
            ${CURRENT_FILE_TITLE}
            ${SUFFIX}
            ALL_XML_TARGETS
        )
    endforeach()
    
    # Processing all java files into AspectJ
    # %.aj.xml : %.java.xml
    #         ${XSLT_SETLANGUAGE_ASPECTJ} $< > $@
    buildXMLTarget(XSLT_SETLANGUAGE_ASPECTJ
        ${CURRENT_JAVA_INPUT_FILE}
        ${CURRENT_FILE_TITLE}
        .aj.xml
        ALL_XML_TARGETS
    )
    set(CURRENT_ASPECTJ_INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_FILE_TITLE}.aj.xml)
    # AspectJ
    # %.all.aj.xml : %.aj.xml
    #         ${XSLT_MERGEUNIT} $< > $@
    # %.block.aj.xml : %.aj.xml
    #         ${XSLT_BLOCKUNIT_JAVA} $< > $@
    # %.comment.aj.xml : %.aj.xml
    #         ${XSLT_INSERTCOMMENT} $< > $@
    # %.preproc.aj.xml : %.aj.xml
    #         ${XSLT_INSERTPREPROC} $< > $@
    # %.ifthenelse.aj.xml : %.aj.xml
    #         ${XSLT_INSERTIFTHENELSE_JAVA} $< > $@ 
    # %.formfeed.aj.xml : %.aj.xml
    #         ${XSLT_INSERTFORMFEED} $< > $@ 
    foreach(PROC_INSTRUCTIONS ${ASPECTJ_PROCESSING_COLLECTION})
        list(GET PROC_INSTRUCTIONS 0 SUFFIX)
        list(GET PROC_INSTRUCTIONS 1 CMD)
        buildXMLTarget(
            ${CMD}
            ${CURRENT_ASPECTJ_INPUT_FILE}
            ${CURRENT_FILE_TITLE}
            ${SUFFIX}
            ALL_XML_TARGETS
        )
    endforeach()
endforeach()


# #  Test cases generated from other test cases
# 
# # function declaration from function
# function_decl.cpp.xml : function.cpp.xml ${XSLT_DEFN2DECL_XSL}
#         ${XSLT_DEFN2DECL} $< > $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/function_decl.cpp.xml
    COMMAND ${XSLT_DEFN2DECL} ${CMAKE_CURRENT_SOURCE_DIR}/function.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/function_decl.cpp.xml 
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/function.cpp.xml ${XSLT_DEFN2DECL_XSL}
)
list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/function_decl.cpp.xml)

# Missing depency function_asterisk.cpp.xml
# function_asterisk_decl.cpp.xml : function_asterisk.cpp.xml ${XSLT_DEFN2DECL_XSL}
#         ${XSLT_DEFN2DECL} $< > $@
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/function_asterisk_decl.cpp.xml
#     COMMAND ${XSLT_DEFN2DECL} ${CMAKE_CURRENT_SOURCE_DIR}/function_asterisk.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/function_asterisk_decl.cpp.xml 
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/function_asterisk.cpp.xml ${XSLT_DEFN2DECL_XSL}
# )
# list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/function_asterisk_decl.cpp.xml)

# # const function from function
# function_const.cpp.xml : function.cpp.xml ${XSLT_DEFN2DECL_XSL}
#         ${XSLT_ADDCONST} $< > $@

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/function_const.cpp.xml
    COMMAND ${XSLT_DEFN2DECL} ${CMAKE_CURRENT_SOURCE_DIR}/function.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/function_const.cpp.xml 
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/function.cpp.xml ${XSLT_DEFN2DECL_XSL}
)
list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/function_const.cpp.xml)

# while from if
# while.cpp.xml : if.cpp.xml ${XSLT_IF2WHILE_XSL}
#         ${XSLT_IF2WHILE} $< > $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/while.cpp.xml
    COMMAND ${XSLT_IF2WHILE} ${CMAKE_CURRENT_SOURCE_DIR}/if.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/while.cpp.xml 
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/if.cpp.xml ${XSLT_IF2WHILE_XSL}
)
list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/while.cpp.xml)

# if with a nested conditional from if
# ifnestcond.cpp.xml : if.cpp.xml ${XSLT_NESTCOND_XSL}
#         ${XSLT_NESTCOND} $< > $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/ifnestcond.cpp.xml
    COMMAND ${XSLT_NESTCOND} ${CMAKE_CURRENT_SOURCE_DIR}/if.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/ifnestcond.cpp.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/if.cpp.xml ${XSLT_NESTCOND_XSL}
)
list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/ifnestcond.cpp.xml)

# return from return base
# return.cpp.xml : return_base.cpp.xml ${XSLT_INSRT_XSL}
#         ${XSLT_INSERTEXPR} $< > $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/return.cpp.xml
    COMMAND ${XSLT_INSERTEXPR} ${CMAKE_CURRENT_SOURCE_DIR}/return_base.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/return.cpp.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/return_base.cpp.xml ${XSLT_INSRT_XSL}
)
list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/return.cpp.xml)

# constructor from constructor base
# constructor.cpp.xml : constructor_base.cpp.xml ${XSLT_DEFN2DECL_XSL}
#         cp $< $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/constructor.cpp.xml
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/constructor_base.cpp.xml ${CMAKE_CURRENT_SOURCE_DIR}/constructor.cpp.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/constructor_base.cpp.xml ${XSLT_DEFN2DECL_XSL}
)
list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/constructor.cpp.xml)

# explicit constructor from constructor
# constructor_explicit.cpp.xml : constructor.cpp.xml ${XSLT_INSRT_XSL}
#         ${XSLT_ADDEXPLICIT} $< > $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/constructor_explicit.cpp.xml
    COMMAND ${XSLT_ADDEXPLICIT} ${CMAKE_CURRENT_SOURCE_DIR}/constructor_base.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/constructor_explicit.cpp.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/constructor_base.cpp.xml ${XSLT_INSRT_XSL}
)
list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/constructor_explicit.cpp.xml)


# virtual destructor from destructor
# destructor_virtual.cpp.xml : destructor.cpp.xml ${XSLT_INSRT_XSL}
#         ${XSLT_ADDVIRTUAL} $< > $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/destructor_virtual.cpp.xml
    COMMAND ${XSLT_ADDVIRTUAL} ${CMAKE_CURRENT_SOURCE_DIR}/destructor.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/destructor_virtual.cpp.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/destructor.cpp.xml ${XSLT_INSRT_XSL}
)
list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/destructor_virtual.cpp.xml)

# throw from throw base
# throw.cpp.xml : throw_base.cpp.xml ${XSLT_INSRT_XSL}
#         ${XSLT_INSERTEXPR} $< > $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/throw.cpp.xml
    COMMAND ${XSLT_INSERTEXPR} ${CMAKE_CURRENT_SOURCE_DIR}/throw_base.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/throw.cpp.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/throw_base.cpp.xml ${XSLT_INSRT_XSL}
)
list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/throw.cpp.xml)

# struct from class
# struct.cpp.xml : class.cpp.xml ${XSLT_CLASS2STRUCT_XSL}
#         ${XSLT_CLASS2STRUCT} $< > $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/struct.cpp.xml
    COMMAND ${XSLT_CLASS2STRUCT} ${CMAKE_CURRENT_SOURCE_DIR}/class.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/struct.cpp.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/class.cpp.xml ${XSLT_CLASS2STRUCT_XSL}
)
list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/struct.cpp.xml)
 
# union from struct
# union.cpp.xml : struct.cpp.xml ${XSLT_STRUCT2UNION_XSL}
#         ${XSLT_STRUCT2UNION} $< > $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/union.cpp.xml
    COMMAND ${XSLT_STRUCT2UNION} ${CMAKE_CURRENT_SOURCE_DIR}/struct.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/union.cpp.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/struct.cpp.xml ${XSLT_STRUCT2UNION_XSL}
)
list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/union.cpp.xml)

# struct from class
# struct_cpp.cpp.xml : class_cpp.cpp.xml ${XSLT_CLASS2STRUCT_XSL}
#         ${XSLT_CLASS2STRUCT} $< > $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/struct_cpp.cpp.xml
    COMMAND ${XSLT_CLASS2STRUCT} ${CMAKE_CURRENT_SOURCE_DIR}/class_cpp.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/struct_cpp.cpp.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/class_cpp.cpp.xml ${XSLT_CLASS2STRUCT_XSL}
)
list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/struct_cpp.cpp.xml)

# union from struct
# union_cpp.cpp.xml : struct_cpp.cpp.xml ${XSLT_STRUCT2UNION_XSL}
#         ${XSLT_STRUCT2UNION} $< > $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/union_cpp.cpp.xml
    COMMAND ${XSLT_STRUCT2UNION} ${CMAKE_CURRENT_SOURCE_DIR}/struct_cpp.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/union_cpp.cpp.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/struct_cpp.cpp.xml ${XSLT_STRUCT2UNION_XSL}
)
list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/union_cpp.cpp.xml)

# struct from class
# struct_cs.cs.xml : class_cs.cs.xml ${XSLT_CLASS2STRUCT_XSL}
#         ${XSLT_CLASS2STRUCT} $< > $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/struct_cs.cs.xml
    COMMAND ${XSLT_CLASS2STRUCT} ${CMAKE_CURRENT_SOURCE_DIR}/class_cs.cs.xml > ${CMAKE_CURRENT_SOURCE_DIR}/struct_cs.cs.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/class_cs.cs.xml ${XSLT_CLASS2STRUCT_XSL}
)
list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/struct_cs.cs.xml)

# union from struct
# union_cs.cs.xml : struct_cs.cs.xml ${XSLT_STRUCT2UNION_XSL}
#         ${XSLT_STRUCT2UNION} $< > $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/union_cs.cs.xml
    COMMAND ${XSLT_STRUCT2UNION} ${CMAKE_CURRENT_SOURCE_DIR}/struct_cs.cs.xml > ${CMAKE_CURRENT_SOURCE_DIR}/union_cs.cs.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/struct_cs.cs.xml ${XSLT_STRUCT2UNION_XSL}
)
list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/union_cs.cs.xml)

# interface from class
# interface_java.java.xml : class_java.java.xml ${XSLT_CLASS2INTERFACE_XSL}
#         ${XSLT_CLASS2INTERFACE} $< > $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/interface_java.java.xml
    COMMAND ${XSLT_CLASS2INTERFACE} ${CMAKE_CURRENT_SOURCE_DIR}/class_java.java.xml > ${CMAKE_CURRENT_SOURCE_DIR}/interface_java.java.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/class_java.java.xml ${XSLT_CLASS2INTERFACE_XSL}
)
list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/interface_java.java.xml)

# if with blocks from if
# ifblock.cpp.xml : if.cpp.xml ${XSLT_IF2IFBLOCK_XSL}
#         ${XSLT_IF2IFBLOCK} $< > $@
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/ifblock.cpp.xml
    COMMAND ${XSLT_IF2IFBLOCK} ${CMAKE_CURRENT_SOURCE_DIR}/if.cpp.xml > ${CMAKE_CURRENT_SOURCE_DIR}/ifblock.cpp.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/if.cpp.xml ${XSLT_IF2IFBLOCK_XSL}
)
list(APPEND ALL_XML_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/ifblock.cpp.xml)


add_custom_target(all_xml_targets
    DEPENDS ${ALL_XML_TARGETS}
    COMMAND echo "Built all ALL_ALL_CPP_XML_TARGETS"
)