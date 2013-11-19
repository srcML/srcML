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
set(XSLT_MERGEUNIT "${XSLTPROC} ${XSLT_MERGEUNIT_XSL}")
set(XSLT_MERGEUNIT_PROBLEM_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/mergeunitproblem.xsl)
set(XSLT_MERGEUNIT_PROBLEM "${XSLTPROC} ${XSLT_MERGEUNIT_PROBLEM_XSL}")
 
# generate nested conditionals
set(XSLT_NESTCOND_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/nestcond.xsl)
set(XSLT_NESTCOND "${XSLTPROC} ${XSLT_NESTCOND_XSL}")

# if to while conversion
set(XSLT_IF2WHILE_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/if2while.xsl)
set(XSLT_IF2WHILE "${XSLTPROC} ${XSLT_IF2WHILE_XSL}")

# definition to declaration conversion
set(XSLT_DEFN2DECL_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/defn2decl.xsl)
set(XSLT_DEFN2DECL "${XSLTPROC} ${XSLT_DEFN2DECL_XSL}")

# wrap in a block
set(XSLT_BLOCKUNIT "${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertblock.xsl")

set(XSLT_BLOCKUNIT_PROBLEM "${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertblockproblem.xsl")

set(XSLT_STRUCTUNIT "${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertstruct.xsl")

set(XSLT_BLOCKUNIT_JAVA "${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertblock.java.xsl")
set(XSLT_BLOCKUNIT_PROBLEM_JAVA "${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertblockproblem.java.xsl")

set(XSLT_ADDEXPLICIT "${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/addexplicit.xsl")

set(XSLT_ADDVIRTUAL "${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/addvirtual.xsl")

set(XSLT_ADDCONST "${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/addconst.xsl")

set(XSLT_IDENTITY "${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/identity.xsl")

set(XSLT_INSERTEXPR "${XSLTPROC} --stringparam expr_filename \"${CMAKE_CURRENT_SOURCE_DIR}/expression.cpp.xml\" ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertexpr.xsl")
set(XSLT_INSERTEXPR_XSL "${CMAKE_CURRENT_SOURCE_DIR}/expression.cpp.xml ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertexpr.xsl")

set(XSLT_INSERTDECL "${XSLTPROC} --stringparam decl_filename \"${CMAKE_CURRENT_SOURCE_DIR}/decl_simple.cpp.xml\" ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertparam.xsl")
set(XSLT_INSERTDECL_XSL "${CMAKE_CURRENT_SOURCE_DIR}/decl_simple.cpp.xml ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertdecl.xsl")

set(XSLT_STRUCT2UNION_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/struct2union.xsl)
set(XSLT_STRUCT2UNION "${XSLTPROC} ${XSLT_STRUCT2UNION_XSL}")

set(XSLT_CLASS2STRUCT_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/class2struct.xsl)
set(XSLT_CLASS2STRUCT "${XSLTPROC} ${XSLT_CLASS2STRUCT_XSL}")

set(XSLT_CLASS2INTERFACE_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/class2interface.xsl)
set(XSLT_CLASS2INTERFACE "${XSLTPROC} ${XSLT_CLASS2INTERFACE_XSL}")

set(XSLT_IF2IFBLOCK_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/if2ifblock.xsl)
set(XSLT_IF2IFBLOCK "${XSLTPROC} ${XSLT_IF2IFBLOCK_XSL}")

##
# Comments and preprocessor statements are handled out of the normal grammar rules
# in src2srcml.  In order to test the handling of these tokens, special cases are
# generated from the non-comment and preprocessor cases.

# insert comments
set(XSLT_INSERTCOMMENT "${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertcomments.xsl")

set(XSLT_INSERTCOMMENT_PROBLEM "${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertcommentsproblem.xsl")

# insert preprocessor
set(XSLT_INSERTPREPROC "${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertpreproc.xsl")

set(XSLT_INSERTPREPROC_PROBLEM "${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertpreprocproblem.xsl")

# nest if
set(XSLT_INSERTIFTHENELSE "${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/nestif.xsl")
set(XSLT_INSERTIFTHENELSE_JAVA "${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/nestif.java.xsl")
set(XSLT_INSERTIFTHENELSE_PROBLEM "${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/nestifproblem.xsl")
set(XSLT_INSERTIFTHENELSE_PROBLEM_JAVA "${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/nestifproblem.java.xsl")

# insert form feed
set(XSLT_INSERTFORMFEED "${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertformfeed.xsl")
set(XSLT_INSERTFORMFEED_PROBLEM "${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/xsl/insertformfeedproblem.xsl")

#
# Test cases for one language are generated from test cases for another language.
# 

set(XSLT_SETLANGUAGE_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/setlanguage.xsl)
set(XSLT_SETLANGUAGE_PROBLEM_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/setlanguageproblem.xsl)

set(XSLT_REMOVECPP_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/removecpp.xsl)
set(XSLT_REMOVECPP_PROBLEM_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/removecppproblem.xsl)
set(XSLT_REMOVECPP_LITERAL_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/removecppliteral.xsl)
set(XSLT_REMOVECPP_OPERATOR_XSL ${CMAKE_CURRENT_SOURCE_DIR}/xsl/removecppoperator.xsl)

set(XSLT_SETLANGUAGE "${XSLTPROC} ${XSLT_SETLANGUAGE_XSL}")

set(XSLT_SETLANGUAGE_C "${XSLTPROC} --stringparam language \"C\" ${XSLT_SETLANGUAGE_XSL}")

set(XSLT_SETLANGUAGE_CS "${XSLTPROC} --stringparam language \"C#\" ${XSLT_SETLANGUAGE_XSL}")

set(XSLT_SETLANGUAGE_CXX "${XSLTPROC} --stringparam language \"CXX\" ${XSLT_SETLANGUAGE_XSL}")

set(XSLT_SETLANGUAGE_CXX_11 "${XSLTPROC} --stringparam language \"C++11\" ${XSLT_SETLANGUAGE_XSL}")

set(XSLT_SETLANGUAGE_JAVA "${XSLTPROC} --stringparam language \"Java\" ${XSLT_SETLANGUAGE_XSL}")
set(XSLT_SETLANGUAGE_ASPECTJ "${XSLTPROC} --stringparam language \"AspectJ\" ${XSLT_SETLANGUAGE_XSL}")

set(XSLT_SETLANGUAGE_PROBLEM "${XSLTPROC} ${XSLT_SETLANGUAGE_PROBLEM_XSL}")

set(XSLT_SETLANGUAGE_PROBLEM_C "${XSLTPROC} --stringparam language \"C\" ${XSLT_SETLANGUAGE_PROBLEM_XSL}")

set(XSLT_SETLANGUAGE_PROBLEM_CS "${XSLTPROC} --stringparam language \"C#\" ${XSLT_SETLANGUAGE_PROBLEM_XSL}")

set(XSLT_SETLANGUAGE_PROBLEM_CXX "${XSLTPROC} --stringparam language \"CXX\" ${XSLT_SETLANGUAGE_PROBLEM_XSL}")

set(XSLT_SETLANGUAGE_PROBLEM_CXX_11 "${XSLTPROC} --stringparam language \"C++11\" ${XSLT_SETLANGUAGE_PROBLEM_XSL}")

set(XSLT_SETLANGUAGE_PROBLEM_JAVA "${XSLTPROC} --stringparam language \"Java\" ${XSLT_SETLANGUAGE_PROBLEM_XSL}")
set(XSLT_SETLANGUAGE_PROBLEM_ASPECTJ "${XSLTPROC} --stringparam language \"AspectJ\" ${XSLT_SETLANGUAGE_PROBLEM_XSL}")

set(XSLT_REMOVECPP "${XSLTPROC} ${XSLT_REMOVECPP_XSL}")
set(XSLT_REMOVECPP_PROBLEM "${XSLTPROC} ${XSLT_REMOVECPP_PROBLEM_XSL}")
set(XSLT_REMOVECPP_LITERAL "${XSLTPROC} ${XSLT_REMOVECPP_LITERAL_XSL}")
set(XSLT_REMOVECPP_OPERATOR "${XSLTPROC} ${XSLT_REMOVECPP_OPERATOR_XSL}")

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

add_custom_target(
    allinall
    COMMAND @echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" 
#     COMMAND echo "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\" dir=\"allinall.c\">" >> allinall.c.xml
#     COMMAND find ../suite -name "*.all.c.xml" | xargs -i tail +3 {} | sed "s|</unit>||" >> allinall.c.xml
#     COMMAND echo "</unit>" >> allinall.c.xml
)
# > allinall.c.xml
# 


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
#         find ../suite -name "*.all.java.xml" | xargs -i tail +3 {} | sed "s|</unit>||">> allinall.java.xml
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
# 
# problem.java.xml : problem.cpp.xml
#         ${XSLT_SETLANGUAGE_PROBLEM_JAVA} $< | ${XSLT_REMOVECPP_PROBLEM} - > $@
# 
# problem.aj.xml : problem.java.xml
#         ${XSLT_SETLANGUAGE_PROBLEM_ASPECTJ} $< > $@
# 
# problem.c.xml : problem.cpp.xml
#         ${XSLT_SETLANGUAGE_PROBLEM_C} $< > $@
# 
# problem.cs.xml : problem.cpp.xml
#         ${XSLT_SETLANGUAGE_PROBLEM_CS} $< > $@
# 
# problem.all.cpp.xml : problem.cpp.xml
#         ${XSLT_MERGEUNIT_PROBLEM} $< > $@
# problem.all.c.xml : problem.c.xml
#         ${XSLT_MERGEUNIT_PROBLEM} $< > $@
# problem.all.cs.xml : problem.cs.xml
#         ${XSLT_MERGEUNIT_PROBLEM} $< > $@
# problem.all.java.xml : problem.java.xml
#         ${XSLT_MERGEUNIT_PROBLEM} $< > $@
# problem.all.aj.xml : problem.aj.xml
#         ${XSLT_MERGEUNIT_PROBLEM} $< > $@
# 
# problem.block.cpp.xml : problem.cpp.xml
#         ${XSLT_BLOCKUNIT_PROBLEM} $< > $@
# problem.block.c.xml : problem.c.xml
#         ${XSLT_BLOCKUNIT_PROBLEM} $< > $@
# problem.block.cs.xml : problem.cs.xml
#         ${XSLT_BLOCKUNIT_PROBLEM} $< > $@
# problem.block.java.xml : problem.java.xml
#         ${XSLT_BLOCKUNIT_PROBLEM_JAVA} $< > $@
# problem.block.aj.xml : problem.aj.xml
#         ${XSLT_BLOCKUNIT_PROBLEM_JAVA} $< > $@
# 
# problem.comment.cpp.xml : problem.cpp.xml
#         ${XSLT_INSERTCOMMENT_PROBLEM} $< > $@
# problem.comment.c.xml : problem.c.xml
#         ${XSLT_INSERTCOMMENT_PROBLEM} $< > $@
# problem.comment.cs.xml : problem.cs.xml
#         ${XSLT_INSERTCOMMENT_PROBLEM} $< > $@
# problem.comment.java.xml : problem.java.xml
#         ${XSLT_INSERTCOMMENT_PROBLEM} $< > $@
# problem.comment.aj.xml : problem.aj.xml
#         ${XSLT_INSERTCOMMENT_PROBLEM} $< > $@
# 
# problem.preproc.cpp.xml : problem.cpp.xml
#         ${XSLT_INSERTPREPROC_PROBLEM} $< > $@
# problem.preproc.c.xml : problem.c.xml
#         ${XSLT_INSERTPREPROC_PROBLEM} $< > $@
# problem.preproc.cs.xml : problem.cs.xml
#         ${XSLT_INSERTPREPROC_PROBLEM} $< > $@
# problem.preproc.java.xml : problem.java.xml
#         ${XSLT_INSERTPREPROC_PROBLEM} $< > $@
# problem.preproc.aj.xml : problem.aj.xml
#         ${XSLT_INSERTPREPROC_PROBLEM} $< > $@
# 
# problem.ifthenelse.cpp.xml : problem.cpp.xml
#         ${XSLT_INSERTIFTHENELSE_PROBLEM} $< > $@
# problem.ifthenelse.c.xml : problem.c.xml
#         ${XSLT_INSERTIFTHENELSE_PROBLEM} $< > $@
# problem.ifthenelse.cs.xml : problem.cs.xml
#         ${XSLT_INSERTIFTHENELSE_PROBLEM} $< > $@
# problem.ifthenelse.java.xml : problem.java.xml
#         ${XSLT_INSERTIFTHENELSE_PROBLEM_JAVA} $< > $@
# problem.ifthenelse.aj.xml : problem.aj.xml
#         ${XSLT_INSERTIFTHENELSE_PROBLEM_JAVA} $< > $@
# 
# problem.formfeed.cpp.xml : problem.cpp.xml
#         ${XSLT_INSERTFORMFEED_PROBLEM} $< > $@
# problem.formfeed.c.xml : problem.c.xml
#         ${XSLT_INSERTFORMFEED_PROBLEM} $< > $@
# problem.formfeed.cs.xml : problem.cs.xml
#         ${XSLT_INSERTFORMFEED_PROBLEM} $< > $@
# problem.formfeed.java.xml : problem.java.xml
#         ${XSLT_INSERTFORMFEED_PROBLEM} $< > $@
# problem.formfeed.aj.xml : problem.aj.xml
#         ${XSLT_INSERTFORMFEED_PROBLEM} $< > $@
# 
# %.cpp : %.cpp.xml
#         ${XSLT_IDENTITY} $< > $@
# 
# %.all.cpp.xml : %.cpp.xml
#         ${XSLT_MERGEUNIT} $< > $@
# 
# %.all.c.xml : %.c.xml
#         ${XSLT_MERGEUNIT} $< > $@
# 
# %.all.cs.xml : %.cs.xml
#         ${XSLT_MERGEUNIT} $< > $@
# 
# %.all.java.xml : %.java.xml
#         ${XSLT_MERGEUNIT} $< > $@
# 
# %.all.aj.xml : %.aj.xml
#         ${XSLT_MERGEUNIT} $< > $@
# 
# %.block.cpp.xml : %.cpp.xml
#         ${XSLT_BLOCKUNIT} $< > $@
# 
# %.struct.cpp.xml : %.cpp.xml
#         ${XSLT_STRUCTUNIT} $< > $@
# 
# %.block.c.xml : %.c.xml
#         ${XSLT_BLOCKUNIT} $< > $@
# 
# %.block.cs.xml : %.cs.xml
#         ${XSLT_BLOCKUNIT} $< > $@
# 
# %.block.java.xml : %.java.xml
#         ${XSLT_BLOCKUNIT_JAVA} $< > $@
# 
# %.block.aj.xml : %.aj.xml
#         ${XSLT_BLOCKUNIT_JAVA} $< > $@
# 
# %.comment.cpp.xml : %.cpp.xml
#         ${XSLT_INSERTCOMMENT} $< > $@
# 
# %.comment.c.xml : %.c.xml
#         ${XSLT_INSERTCOMMENT} $< > $@
# 
# %.comment.cs.xml : %.cs.xml
#         ${XSLT_INSERTCOMMENT} $< > $@
# 
# %.comment.java.xml : %.java.xml
#         ${XSLT_INSERTCOMMENT} $< > $@
# 
# %.comment.aj.xml : %.aj.xml
#         ${XSLT_INSERTCOMMENT} $< > $@
# 
# %.preproc.cpp.xml : %.cpp.xml
#         ${XSLT_INSERTPREPROC} $< > $@
# 
# %.preproc.c.xml : %.c.xml
#         ${XSLT_INSERTPREPROC} $< > $@
# 
# %.preproc.cs.xml : %.cs.xml
#         ${XSLT_INSERTPREPROC} $< > $@
# 
# %.preproc.java.xml : %.java.xml
#         ${XSLT_INSERTPREPROC} $< > $@
# 
# %.preproc.aj.xml : %.aj.xml
#         ${XSLT_INSERTPREPROC} $< > $@
# 
# %.ifthenelse.cpp.xml : %.cpp.xml
#         ${XSLT_INSERTIFTHENELSE} $< > $@
# 
# %.ifthenelse.c.xml : %.c.xml
#         ${XSLT_INSERTIFTHENELSE} $< > $@
# 
# %.ifthenelse.cs.xml : %.cs.xml
#         ${XSLT_INSERTIFTHENELSE} $< > $@
# 
# %.ifthenelse.java.xml : %.java.xml
#         ${XSLT_INSERTIFTHENELSE_JAVA} $< > $@
# 
# %.ifthenelse.aj.xml : %.aj.xml
#         ${XSLT_INSERTIFTHENELSE_JAVA} $< > $@
# 
# %.formfeed.cpp.xml : %.cpp.xml
#         ${XSLT_INSERTFORMFEED} $< > $@
# 
# %.formfeed.c.xml : %.c.xml
#         ${XSLT_INSERTFORMFEED} $< > $@
# 
# %.formfeed.cs.xml : %.cs.xml
#         ${XSLT_INSERTFORMFEED} $< > $@
# 
# %.formfeed.java.xml : %.java.xml
#         ${XSLT_INSERTFORMFEED} $< > $@
# 
# %.formfeed.aj.xml : %.aj.xml
#         ${XSLT_INSERTFORMFEED} $< > $@
# 
# %.java.xml : %.cpp.xml
#         ( test $@ != "literal.java.xml" -a $@ != "operator.java.xml" && ${XSLT_SETLANGUAGE_JAVA} $< | ${XSLT_REMOVECPP} - > $@ ) || echo -n ""
#         ( test $@ = "literal.java.xml" && ${XSLT_SETLANGUAGE_JAVA} $< | ${XSLT_REMOVECPP_LITERAL} - > $@ ) || echo -n ""
#         ( test $@ = "operator_base.java.xml" && ${XSLT_SETLANGUAGE_JAVA} $< | ${XSLT_REMOVECPP_OPERATOR} - > $@ ) || echo -n ""
# 
# 
# %.aj.xml : %.java.xml
#         ${XSLT_SETLANGUAGE_ASPECTJ} $< > $@
# 
# %.c.xml : %.cpp.xml
#         ${XSLT_SETLANGUAGE_C} $< > $@
# 
# %.cs.xml : %.cpp.xml
#         ${XSLT_SETLANGUAGE_CS} $< > $@
# 
# %.cpp11.xml : %.cpp.xml
#         ${XSLT_SETLANGUAGE_CXX_11} $< > $@
# 
# GEN_FILES= $(GEN_JAVA) $(GEN_C) $(GEN_CS) $(GEN_CXX_11) $(GEN_ASPECTJ) ${GEN_PART} ${GEN_ALL} ${GEN_BLOCK} ${GEN_COMMENT} ${GEN_PREPROC} ${GEN_IFTHENELSE} ${GEN_FORMFEED} ${GEN_STRUCT}
# 
# GEN_CPP_BASE=${GEN_SEP} ${foreach nm, $(SEP_PLUS), $(nm).all} ${foreach nm, $(SEP_PLUS), $(nm).block} ${foreach nm, $(SEP_PLUS), $(nm).comment} ${foreach nm, $(SEP_PLUS), $(nm).preproc} ${foreach nm, $(SEP_PLUS), $(nm).struct}
# 
# GEN_CPP= ${foreach nm, $(GEN_CPP_BASE), $(nm).cpp}
# 
# gencpp : ${GEN_CPP}
# 
# gen : ${GEN_FILES}
# 
# #  Test cases generated from other test cases
# 
# # function declaration from function
# function_decl.cpp.xml : function.cpp.xml ${XSLT_DEFN2DECL_XSL}
#         ${XSLT_DEFN2DECL} $< > $@
# 
# function_asterisk_decl.cpp.xml : function_asterisk.cpp.xml ${XSLT_DEFN2DECL_XSL}
#         ${XSLT_DEFN2DECL} $< > $@
# 
# # const function from function
# function_const.cpp.xml : function.cpp.xml ${XSLT_DEFN2DECL_XSL}
#         ${XSLT_ADDCONST} $< > $@
# 
# # while from if
# while.cpp.xml : if.cpp.xml ${XSLT_IF2WHILE_XSL}
#         ${XSLT_IF2WHILE} $< > $@
# 
# # if with a nested conditional from if
# ifnestcond.cpp.xml : if.cpp.xml ${XSLT_NESTCOND_XSL}
#         ${XSLT_NESTCOND} $< > $@
# 
# # return from return base
# return.cpp.xml : return_base.cpp.xml ${XSLT_INSRT_XSL}
#         ${XSLT_INSERTEXPR} $< > $@
# 
# # constructor from constructor base
# constructor.cpp.xml : constructor_base.cpp.xml ${XSLT_DEFN2DECL_XSL}
#         cp $< $@
# 
# # explicit constructor from constructor
# constructor_explicit.cpp.xml : constructor.cpp.xml ${XSLT_INSRT_XSL}
#         ${XSLT_ADDEXPLICIT} $< > $@
# 
# # virtual destructor from destructor
# destructor_virtual.cpp.xml : destructor.cpp.xml ${XSLT_INSRT_XSL}
#         ${XSLT_ADDVIRTUAL} $< > $@
# 
# # throw from throw base
# throw.cpp.xml : throw_base.cpp.xml ${XSLT_INSRT_XSL}
#         ${XSLT_INSERTEXPR} $< > $@
# 
# # struct from class
# struct.cpp.xml : class.cpp.xml ${XSLT_CLASS2STRUCT_XSL}
#         ${XSLT_CLASS2STRUCT} $< > $@
# 
# 
# # union from struct
# union.cpp.xml : struct.cpp.xml ${XSLT_STRUCT2UNION_XSL}
#         ${XSLT_STRUCT2UNION} $< > $@
# 
# # struct from class
# struct_cpp.cpp.xml : class_cpp.cpp.xml ${XSLT_CLASS2STRUCT_XSL}
#         ${XSLT_CLASS2STRUCT} $< > $@
# 
# # union from struct
# union_cpp.cpp.xml : struct_cpp.cpp.xml ${XSLT_STRUCT2UNION_XSL}
#         ${XSLT_STRUCT2UNION} $< > $@
# 
# # struct from class
# struct_cs.cs.xml : class_cs.cs.xml ${XSLT_CLASS2STRUCT_XSL}
#         ${XSLT_CLASS2STRUCT} $< > $@
# 
# # union from struct
# union_cs.cs.xml : struct_cs.cs.xml ${XSLT_STRUCT2UNION_XSL}
#         ${XSLT_STRUCT2UNION} $< > $@
# 
# # interface from class
# interface_java.java.xml : class_java.java.xml ${XSLT_CLASS2INTERFACE_XSL}
#         ${XSLT_CLASS2INTERFACE} $< > $@
# 
# # if with blocks from if
# ifblock.cpp.xml : if.cpp.xml ${XSLT_IF2IFBLOCK_XSL}
#         ${XSLT_IF2IFBLOCK} $< > $@
# 
# clean :
#         rm -f ${GEN_FILES}
