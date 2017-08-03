##
# Makefile for srcML translator, src2srcml

EXE_DIR = ../bin/
OBJ_DIR = ../obj/

WD=${shell pwd}

#SRC2SRCML=${WD}/../bin/src2srcml
#SRCML2SRC=${WD}/../bin/srcml2src

SRCENCODING=
#SRCENCODING=--src-encoding=ISO-8859-1

define language
${shell grep '$1 ' suite.txt | grep -v '^#' | cut -d' ' -f2- | tr -s '\n' ' '}
endef

UNAME=$(firstword $(wildcard /usr/bin/uname /bin/uname))

CAT=cat
RM=rm
ifeq (${UNAME},"")
CAT=type
RM=del
endif


##
# General lists of test cases

# original base of cases for all languages
BASE_COMMON=${call language,'LANGUAGE_ALL_BASE'}

# base for all languages generated from original cases
GEN_BASE=${call language,'LANGUAGE_ALL_GEN'}

# base for C++ generated from original cases
GEN_BASE_CXX=${call language,'LANGUAGE_CXX_FAMILY'}

# base for C only
#BASE_C=functionkr # typedef_cstruct

# base for C family
BASE_C_FAMILY=${call language,'LANGUAGE_C_FAMILY'}

# base for C family
BASE_C=${call language,'LANGUAGE_C_ONLY'}

# base for object-oriented languages
BASE_OO=${call language,'LANGUAGE_OO'}

# base for C++ only
BASE_CXX=${call language,'LANGUAGE_CXX'}

# base for C++ only
BASE_CXX_GEN=${call language,'LANGUAGE_CXX_GEN'}

# base for C# only
BASE_CS=${call language,'LANGUAGE_CSHARP'}

# base for C# only
BASE_CS_GEN=${call language,'LANGUAGE_CSHARP_GEN'}

# base Objective-C
BASE_OBJECTIVE_C=${call language,'LANGUAGE_OBJECTIVE_C'}

# base Objective-C
BASE_OBJECTIVE_C_GEN=${call language,'LANGUAGE_OBJECTIVE_C_GEN'}

# base for Java only
BASE_JAVA=${call language,'LANGUAGE_JAVA'}

# base for Java only
BASE_JAVA_GEN=${call language,'LANGUAGE_JAVA_GEN'}

all : gen

testbase : gen

echobase :
	@echo ${BASE_COMMON}

echogen :
	@echo ${GEN_BASE}

XSLT=xsltproc

# merge separate srcML units into one
XSLT_MERGEUNIT_XSL=xsl/mergeunit.xsl
XSLT_MERGEUNIT=${XSLT} ${XSLT_MERGEUNIT_XSL}
XSLT_MERGEUNIT_PROBLEM_XSL=xsl/mergeunitproblem.xsl
XSLT_MERGEUNIT_PROBLEM=${XSLT} ${XSLT_MERGEUNIT_PROBLEM_XSL}

# generate nested conditionals
XSLT_NESTCOND_XSL=xsl/nestcond.xsl
XSLT_NESTCOND=${XSLT} ${XSLT_NESTCOND_XSL}

# if to while conversion
XSLT_IF2WHILE_XSL=xsl/if2while.xsl
XSLT_IF2WHILE=${XSLT} ${XSLT_IF2WHILE_XSL}

# definition to declaration conversion
XSLT_DEFN2DECL_XSL=xsl/defn2decl.xsl
XSLT_DEFN2DECL=${XSLT} ${XSLT_DEFN2DECL_XSL}

# wrap in a block
XSLT_BLOCKUNIT=${XSLT} xsl/insertblock.xsl

XSLT_BLOCKUNIT_PROBLEM=${XSLT} xsl/insertblockproblem.xsl

XSLT_STRUCTUNIT=${XSLT} xsl/insertstruct.xsl

XSLT_BLOCKUNIT_JAVA=${XSLT} xsl/insertblock.java.xsl
XSLT_BLOCKUNIT_PROBLEM_JAVA=${XSLT} xsl/insertblockproblem.java.xsl

XSLT_ADDEXPLICIT=${XSLT} xsl/addexplicit.xsl

XSLT_ADDVIRTUAL=${XSLT} xsl/addvirtual.xsl

XSLT_ADDCONST=${XSLT} xsl/addconst.xsl

XSLT_IDENTITY=${XSLT} xsl/identity.xsl

XSLT_INSERTEXPR=${XSLT} --param expr_filename "../expression.cpp.xml" xsl/insertexpr.xsl
XSLT_INSERTEXPR_XSL= expression.cpp.xml xsl/insertexpr.xsl

XSLT_INSERTDECL=${XSLT} --param decl_filename "../decl_simple.cpp.xml" xsl/insertparam.xsl
XSLT_INSERTDECL_XSL= decl_simple.cpp.xml xsl/insertdecl.xsl

XSLT_STRUCT2UNION_XSL=xsl/struct2union.xsl
XSLT_STRUCT2UNION=${XSLT} ${XSLT_STRUCT2UNION_XSL}

XSLT_CLASS2STRUCT_XSL=xsl/class2struct.xsl
XSLT_CLASS2STRUCT=${XSLT} ${XSLT_CLASS2STRUCT_XSL}

XSLT_CLASS2INTERFACE_XSL=xsl/class2interface.xsl
XSLT_CLASS2INTERFACE=${XSLT} ${XSLT_CLASS2INTERFACE_XSL}

XSLT_IF2IFBLOCK_XSL=xsl/if2ifblock.xsl
XSLT_IF2IFBLOCK=${XSLT} ${XSLT_IF2IFBLOCK_XSL}

XSLT_RENAME_XSL=xsl/rename.xsl

XSLT_INTERFACE2IMPLEMENTATION_XSL=xsl/interface2implementation.xsl
XSLT_INTERFACE2IMPLEMENTATION=${XSLT} ${XSLT_INTERFACE2IMPLEMENTATION_XSL}

##
# Comments and preprocessor statements are handled out of the normal grammar rules
# in src2srcml.  In order to test the handling of these tokens, special cases are
# generated from the non-comment and preprocessor cases.

# insert comments
XSLT_INSERTCOMMENT=${XSLT} xsl/insertcomments.xsl

XSLT_INSERTCOMMENT_PROBLEM=${XSLT} xsl/insertcommentsproblem.xsl

# insert preprocessor
XSLT_INSERTPREPROC=${XSLT} xsl/insertpreproc.xsl

XSLT_INSERTPREPROC_PROBLEM=${XSLT} xsl/insertpreprocproblem.xsl

# nest if
XSLT_INSERTIFTHENELSE=${XSLT} xsl/nestif.xsl
XSLT_INSERTIFTHENELSE_JAVA=${XSLT} xsl/nestif.java.xsl
XSLT_INSERTIFTHENELSE_PROBLEM=${XSLT} xsl/nestifproblem.xsl
XSLT_INSERTIFTHENELSE_PROBLEM_JAVA=${XSLT} xsl/nestifproblem.java.xsl

# insert form feed
XSLT_INSERTFORMFEED=${XSLT} xsl/insertformfeed.xsl
XSLT_INSERTFORMFEED_PROBLEM=${XSLT} xsl/insertformfeedproblem.xsl

##
# Test cases for one language are generated from test cases for another language.

XSLT_SETLANGUAGE_XSL=xsl/setlanguage.xsl
XSLT_SETLANGUAGE_PROBLEM_XSL=xsl/setlanguageproblem.xsl

XSLT_REMOVECPP_XSL=xsl/removecpp.xsl
XSLT_REMOVECPP_PROBLEM_XSL=xsl/removecppproblem.xsl

XSLT_SETLANGUAGE=${XSLT} ${XSLT_SETLANGUAGE_XSL}

XSLT_SETLANGUAGE_C=${XSLT} --stringparam language "C" ${XSLT_SETLANGUAGE_XSL}

XSLT_SETLANGUAGE_CS=${XSLT} --stringparam language "C\#" ${XSLT_SETLANGUAGE_XSL}

XSLT_SETLANGUAGE_OBJECTIVE_C=${XSLT} --stringparam language "Objective-C" ${XSLT_SETLANGUAGE_XSL}

XSLT_SETLANGUAGE_CXX=${XSLT} --stringparam language "CXX" ${XSLT_SETLANGUAGE_XSL}

XSLT_SETLANGUAGE_CXX_11=${XSLT} --stringparam language "C++11" ${XSLT_SETLANGUAGE_XSL}

XSLT_SETLANGUAGE_JAVA=${XSLT} --stringparam language "Java" ${XSLT_SETLANGUAGE_XSL}
XSLT_SETLANGUAGE_ASPECTJ=${XSLT} --stringparam language "AspectJ" ${XSLT_SETLANGUAGE_XSL}

XSLT_SETLANGUAGE_PROBLEM=${XSLT} ${XSLT_SETLANGUAGE_PROBLEM_XSL}

XSLT_SETLANGUAGE_PROBLEM_C=${XSLT} --stringparam language "C" ${XSLT_SETLANGUAGE_PROBLEM_XSL}

XSLT_SETLANGUAGE_PROBLEM_CS=${XSLT} --stringparam language "C\#" ${XSLT_SETLANGUAGE_PROBLEM_XSL}

XSLT_SETLANGUAGE_PROBLEM_CXX=${XSLT} --stringparam language "CXX" ${XSLT_SETLANGUAGE_PROBLEM_XSL}

XSLT_SETLANGUAGE_PROBLEM_CXX_11=${XSLT} --stringparam language "C++11" ${XSLT_SETLANGUAGE_PROBLEM_XSL}

XSLT_SETLANGUAGE_PROBLEM_JAVA=${XSLT} --stringparam language "Java" ${XSLT_SETLANGUAGE_PROBLEM_XSL}
XSLT_SETLANGUAGE_PROBLEM_ASPECTJ=${XSLT} --stringparam language "AspectJ" ${XSLT_SETLANGUAGE_PROBLEM_XSL}

XSLT_REMOVECPP=${XSLT} ${XSLT_REMOVECPP_XSL}
XSLT_REMOVECPP_PROBLEM=${XSLT} ${XSLT_REMOVECPP_PROBLEM_XSL}

# try/catch/finally/throw keywords to @try/@catch/@finally/throw keywords
XSLT_KEYWORD2MKEYWORD_XSL=xsl/keyword2mkeyword.xsl
XSLT_KEYWORD2MKEYWORD=${XSLT} ${XSLT_KEYWORD2MKEYWORD_XSL}

# convert @synthesize to @dynamic
XSLT_SYNTHESIZE2DYNAMIC_XSL=xsl/synthesize2dynamic.xsl
XSLT_SYNTHESIZE2DYNAMIC=${XSLT} ${XSLT_SYNTHESIZE2DYNAMIC_XSL}

SEP_BASE=$(BASE_COMMON) $(BASE_C_FAMILY) $(BASE_CXX) $(BASE_OO)

# basic generated C++ files
GEN_SEP= $(GEN_BASE) $(GEN_BASE_CXX)

# generated cases for C++ from base
GEN_PART=${foreach nm, $(GEN_SEP), $(nm).cpp.xml}

SEP_FILES=$(SEP_BASE) preprocessor

BASE_CXX_ALL_GEN = $(BASE_CXX_GEN)
GEN_CXX= ${foreach nm, $(BASE_CXX_ALL_GEN), $(nm).cpp.xml}

BASE_C_ALL_GEN = $(BASE_COMMON) $(GEN_BASE) $(BASE_C_FAMILY)
BASE_C_ALL = $(BASE_COMMON) $(GEN_BASE) $(BASE_C_FAMILY) $(BASE_C)
GEN_C= ${foreach nm, $(BASE_C_ALL_GEN), $(nm).c.xml}

BASE_CS_ALL_GEN = $(BASE_COMMON) $(GEN_BASE) $(BASE_CS_GEN) $(BASE_OO)
BASE_CS_ALL = $(BASE_COMMON) $(GEN_BASE) $(BASE_CS) $(BASE_CS_GEN)
GEN_CS= ${foreach nm, $(BASE_CS_ALL_GEN), $(nm).cs.xml}

BASE_OBJECTIVE_C_ALL_GEN = $(BASE_COMMON) $(GEN_BASE) $(BASE_C_FAMILY) $(BASE_C) $(BASE_OBJECTIVE_C_GEN)
BASE_OBJECTIVE_C_ALL = $(BASE_COMMON) $(GEN_BASE) $(BASE_C_FAMILY) $(BASE_C) $(BASE_OBJECTIVE_C) $(BASE_OBJECTIVE_C_GEN)
GEN_OBJECTIVE_C = ${foreach nm, $(BASE_OBJECTIVE_C_ALL_GEN), $(nm).m.xml}

# base for all of Java
BASE_JAVA_ALL_GEN = $(BASE_COMMON) $(GEN_BASE) $(BASE_OO) $(BASE_JAVA_GEN)
BASE_JAVA_ALL = $(BASE_COMMON) $(GEN_BASE) $(BASE_OO) $(BASE_JAVA)
GEN_JAVA= ${foreach nm, $(BASE_JAVA_ALL_GEN), $(nm).java.xml}

#BASE_ASPECTJ_ALL_GEN = $(BASE_COMMON) $(GEN_BASE) $(BASE_OO) $(BASE_JAVA)

#GEN_ASPECTJ= ${foreach nm, $(BASE_ASPECTJ_ALL_GEN), $(nm).aj.xml}

SEP_PLUS= ${SEP_FILES} ${GEN_SEP}
SEP_PLUSNOPREPROC= ${SEP_BASE} ${GEN_SEP}

SEP_STRUCT = function function_const function_cpp function_decl function_ns function_operator function_pointer function_throw 

##
# generate cases for all
GEN_ALL= ${foreach nm, $(SEP_PLUS), $(nm).all.cpp.xml} extra_cpp.all.cpp.xml \
	       	${foreach nm, $(BASE_C_ALL), $(nm).all.c.xml} \
	       	${foreach nm, $(BASE_CS_ALL), $(nm).all.cs.xml} \
	       	${foreach nm, $(BASE_OBJECTIVE_C_ALL), $(nm).all.m.xml} \
	       	${foreach nm, $(BASE_JAVA_ALL), $(nm).all.java.xml} \
	    	${foreach nm, $(BASE_ASPECTJ_ALL), $(nm).all.aj.xml} \
			extra_c.all.c.xml extra_cpp.all.cpp.xml

# generated cases for blocks
GEN_BLOCK= ${foreach nm, $(SEP_PLUS),          $(nm).block.cpp.xml} \
           	${foreach nm, $(BASE_C_ALL),    $(nm).block.c.xml} \
           	${foreach nm, $(BASE_CS_ALL),    $(nm).block.cs.xml} \
           	${foreach nm, $(BASE_OBJECTIVE_C_ALL),    $(nm).block.m.xml} \
	       	${foreach nm, $(BASE_CXX_11_ALL), $(nm).block.cpp11.xml} \
           	${foreach nm, $(BASE_JAVA_ALL), $(nm).block.java.xml} \
	       	${foreach nm, $(BASE_ASPECTJ_ALL), $(nm).block.aj.xml} \
		 	extra_c.block.c.xml extra_cpp.block.cpp.xml

# generated cases for structs
GEN_STRUCT= ${foreach nm, $(SEP_STRUCT),          $(nm).struct.cpp.xml}

# generated cases for comments
GEN_COMMENT= ${foreach nm, $(SEP_PLUS),          $(nm).comment.cpp.xml} \
	       ${foreach nm, $(BASE_C_ALL),    $(nm).comment.c.xml} \
	       ${foreach nm, $(BASE_CS_ALL),    $(nm).comment.cs.xml} \
	       ${foreach nm, $(BASE_OBJECTIVE_C_ALL),    $(nm).comment.m.xml} \
	       ${foreach nm, $(BASE_CXX_11_ALL), $(nm).comment.cpp11.xml} \
	       ${foreach nm, $(BASE_JAVA_ALL), $(nm).comment.java.xml} \
	       ${foreach nm, $(BASE_ASPECTJ_ALL), $(nm).comment.aj.xml} \
		   extra_c.comment.c.xml extra_cpp.comment.cpp.xml

# generated cases for formfeed
GEN_FORMFEED = ${foreach nm, $(SEP_PLUS),          $(nm).formfeed.cpp.xml} \
		     ${foreach nm, $(BASE_C_ALL),    $(nm).formfeed.c.xml} \
		     ${foreach nm, $(BASE_CS_ALL),    $(nm).formfeed.cs.xml} \
		     ${foreach nm, $(BASE_OBJECTIVE_C_ALL),    $(nm).formfeed.m.xml} \
	             ${foreach nm, $(BASE_CXX_11_ALL), $(nm).formfeed.cpp11.xml} \
		     ${foreach nm, $(BASE_JAVA_ALL), $(nm).formfeed.java.xml} \
	             ${foreach nm, $(BASE_ASPECTJ_ALL), $(nm).formfeed.aj.xml} \
		     extra_c.formfeed.c.xml extra_cpp.formfeed.cpp.xml

# generated test cases for preproc
GEN_PREPROC= ${foreach nm, $(SEP_PLUSNOPREPROC), $(nm).preproc.cpp.xml} \
	           	${foreach nm, $(BASE_C_ALL), $(nm).preproc.c.xml} \
	           	${foreach nm, $(BASE_CS_ALL), $(nm).preproc.cs.xml} \
	           	${foreach nm, $(BASE_OBJECTIVE_C_ALL), $(nm).preproc.m.xml} \
               	${foreach nm, $(BASE_CXX_11_ALL), $(nm).preproc.cpp11.xml} \
				extra_c.preproc.c.xml extra_cpp.preproc.cpp.xml

# generated test cases for preproc
GEN_IFTHENELSE= ${foreach nm, $(SEP_PLUS), $(nm).ifthenelse.cpp.xml} \
	           ${foreach nm, $(BASE_C_ALL), $(nm).ifthenelse.c.xml} \
	           ${foreach nm, $(BASE_CS_ALL), $(nm).ifthenelse.cs.xml} \
	           ${foreach nm, $(BASE_OBJECTIVE_C_ALL), $(nm).ifthenelse.m.xml} \
	           ${foreach nm, $(BASE_CXX_11_ALL), $(nm).ifthenelse.cpp11.xml} \
	           ${foreach nm, $(BASE_JAVA_ALL), $(nm).ifthenelse.java.xml} \
	           ${foreach nm, $(BASE_ASPECTJ_ALL), $(nm).ifthenelse.aj.xml} \
		   extra_c.ifthenelse.c.xml extra_cpp.ifthenelse.cpp.xml

allinall.c.xml :
	@echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > allinall.c.xml
	@echo "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C\" dir=\"allinall.c\">" >> allinall.c.xml

	find ../suite -name "*.all.c.xml" | xargs -i tail +3 {} | sed "s|</unit>||">> allinall.c.xml
	@echo "</unit>" >> allinall.c.xml

allinall.java.xml :
	@echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > allinall.java.xml
	@echo "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"Java\" dir=\"allinall.java\">" >> allinall.java.xml

	find ../suite -name "*.all.java.xml" | xargs -i tail +3 {} | sed "s|</unit>||">> allinall.java.xml
	@echo "</unit>" >> allinall.java.xml

allinall.1.cpp.xml :
	@echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > allinall.1.cpp.xml
	@echo "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" dir=\"allinall.1.cpp\">" >> allinall.1.cpp.xml

	@find ../suite -name "*.all.cpp.xml" | sed "s|struct_c.all.c.xml||" | sed "s|functionkr.all.cpp.xml||" | head -30 | xargs -i tail +3 {} | sed "s|</unit>||" >> allinall.1.cpp.xml
	@echo "</unit>" >> allinall.1.cpp.xml

allinall.2.cpp.xml :
	@echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > allinall.2.cpp.xml
	@echo "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" dir=\"allinall.2.cpp\">" >> allinall.2.cpp.xml

	@find ../suite -name "*.all.cpp.xml" | sed "s|struct_c.all.c.xml||" | sed "s|functionkr.all.cpp.xml||" | tail -18 | xargs -i tail +3 {} | sed "s|</unit>||" >> allinall.2.cpp.xml
	@echo "</unit>" >> allinall.2.cpp.xml

allinone : allinonecpp allinonec allinonejava
	#@make cleanall

allinonec : allinall.c.xml
	-${SRCML2SRC} ${SRCENCODING} allinall.c.xml | ${SRC2SRCML} -l C ${SRCENCODING} --directory="allinall.c" | diff -w  - allinall.c.xml

allinonecpp : allinall.1.cpp.xml allinall.2.cpp.xml
	-${SRCML2SRC} ${SRCENCODING} allinall.1.cpp.xml | ${SRC2SRCML} -l C++ ${SRCENCODING} --directory="allinall.1.cpp" | diff -w  - allinall.1.cpp.xml
	-${SRCML2SRC} ${SRCENCODING} allinall.2.cpp.xml | ${SRC2SRCML} -l C++ ${SRCENCODING} --directory="allinall.2.cpp" | diff -w  - allinall.2.cpp.xml

allinonejava : allinall.java.xml
	-${SRCML2SRC} ${SRCENCODING} allinall.java.xml | ${SRC2SRCML} -l Java ${SRCENCODING} --directory="allinall.java" | diff -w  - allinall.java.xml

cleanall :
	rm -f allinall.*

problem.java.xml : problem.cpp.xml
	${XSLT_SETLANGUAGE_PROBLEM_JAVA} $< | ${XSLT_REMOVECPP_PROBLEM} - > $@

problem.aj.xml : problem.java.xml
	${XSLT_SETLANGUAGE_PROBLEM_ASPECTJ} $< > $@

problem.c.xml : problem.cpp.xml
	${XSLT_SETLANGUAGE_PROBLEM_C} $< > $@

problem.cs.xml : problem.cpp.xml
	${XSLT_SETLANGUAGE_PROBLEM_CS} $< > $@

problem.all.cpp.xml : problem.cpp.xml
	${XSLT_MERGEUNIT_PROBLEM} $< > $@
problem.all.c.xml : problem.c.xml
	${XSLT_MERGEUNIT_PROBLEM} $< > $@
problem.all.cs.xml : problem.cs.xml
	${XSLT_MERGEUNIT_PROBLEM} $< > $@
problem.all.m.xml : problem.m.xml
	${XSLT_MERGEUNIT_PROBLEM} $< > $@
problem.all.java.xml : problem.java.xml
	${XSLT_MERGEUNIT_PROBLEM} $< > $@
problem.all.aj.xml : problem.aj.xml
	${XSLT_MERGEUNIT_PROBLEM} $< > $@

problem.block.cpp.xml : problem.cpp.xml
	${XSLT_BLOCKUNIT_PROBLEM} $< > $@
problem.block.c.xml : problem.c.xml
	${XSLT_BLOCKUNIT_PROBLEM} $< > $@
problem.block.cs.xml : problem.cs.xml
	${XSLT_BLOCKUNIT_PROBLEM} $< > $@
problem.block.m.xml : problem.m.xml
	${XSLT_BLOCKUNIT_PROBLEM} $< > $@
problem.block.java.xml : problem.java.xml
	${XSLT_BLOCKUNIT_PROBLEM_JAVA} $< > $@
problem.block.aj.xml : problem.aj.xml
	${XSLT_BLOCKUNIT_PROBLEM_JAVA} $< > $@

problem.comment.cpp.xml : problem.cpp.xml
	${XSLT_INSERTCOMMENT_PROBLEM} $< > $@
problem.comment.c.xml : problem.c.xml
	${XSLT_INSERTCOMMENT_PROBLEM} $< > $@
problem.comment.cs.xml : problem.cs.xml
	${XSLT_INSERTCOMMENT_PROBLEM} $< > $@
problem.comment.m.xml : problem.m.xml
	${XSLT_INSERTCOMMENT_PROBLEM} $< > $@
problem.comment.java.xml : problem.java.xml
	${XSLT_INSERTCOMMENT_PROBLEM} $< > $@
problem.comment.aj.xml : problem.aj.xml
	${XSLT_INSERTCOMMENT_PROBLEM} $< > $@

problem.preproc.cpp.xml : problem.cpp.xml
	${XSLT_INSERTPREPROC_PROBLEM} $< > $@
problem.preproc.c.xml : problem.c.xml
	${XSLT_INSERTPREPROC_PROBLEM} $< > $@
problem.preproc.cs.xml : problem.cs.xml
	${XSLT_INSERTPREPROC_PROBLEM} $< > $@
problem.preproc.m.xml : problem.m.xml
	${XSLT_INSERTPREPROC_PROBLEM} $< > $@
problem.preproc.java.xml : problem.java.xml
	${XSLT_INSERTPREPROC_PROBLEM} $< > $@
problem.preproc.aj.xml : problem.aj.xml
	${XSLT_INSERTPREPROC_PROBLEM} $< > $@

problem.ifthenelse.cpp.xml : problem.cpp.xml
	${XSLT_INSERTIFTHENELSE_PROBLEM} $< > $@
problem.ifthenelse.c.xml : problem.c.xml
	${XSLT_INSERTIFTHENELSE_PROBLEM} $< > $@
problem.ifthenelse.cs.xml : problem.cs.xml
	${XSLT_INSERTIFTHENELSE_PROBLEM} $< > $@
problem.ifthenelse.m.xml : problem.m.xml
	${XSLT_INSERTIFTHENELSE_PROBLEM} $< > $@
problem.ifthenelse.java.xml : problem.java.xml
	${XSLT_INSERTIFTHENELSE_PROBLEM_JAVA} $< > $@
problem.ifthenelse.aj.xml : problem.aj.xml
	${XSLT_INSERTIFTHENELSE_PROBLEM_JAVA} $< > $@

problem.formfeed.cpp.xml : problem.cpp.xml
	${XSLT_INSERTFORMFEED_PROBLEM} $< > $@
problem.formfeed.c.xml : problem.c.xml
	${XSLT_INSERTFORMFEED_PROBLEM} $< > $@
problem.formfeed.cs.xml : problem.cs.xml
	${XSLT_INSERTFORMFEED_PROBLEM} $< > $@
problem.formfeed.m.xml : problem.m.xml
	${XSLT_INSERTFORMFEED_PROBLEM} $< > $@
problem.formfeed.java.xml : problem.java.xml
	${XSLT_INSERTFORMFEED_PROBLEM} $< > $@
problem.formfeed.aj.xml : problem.aj.xml
	${XSLT_INSERTFORMFEED_PROBLEM} $< > $@

%.cpp : %.cpp.xml
	${XSLT_IDENTITY} $< > $@

%.all.cpp.xml : %.cpp.xml
	${XSLT_MERGEUNIT} $< > $@
	echo "$<"

%.all.c.xml : %.c.xml
	${XSLT_MERGEUNIT} $< > $@

%.all.cs.xml : %.cs.xml
	${XSLT_MERGEUNIT} $< > $@

%.all.m.xml : %.m.xml
	${XSLT_MERGEUNIT} $< > $@

%.all.java.xml : %.java.xml
	${XSLT_MERGEUNIT} $< > $@

%.all.aj.xml : %.aj.xml
	${XSLT_MERGEUNIT} $< > $@

%.block.cpp.xml : %.cpp.xml
	${XSLT_BLOCKUNIT} $< > $@

%.struct.cpp.xml : %.cpp.xml
	${XSLT_STRUCTUNIT} $< > $@

%.block.c.xml : %.c.xml
	${XSLT_BLOCKUNIT} $< > $@

%.block.cs.xml : %.cs.xml
	${XSLT_BLOCKUNIT} $< > $@

%.block.m.xml : %.m.xml
	${XSLT_BLOCKUNIT} $< > $@

%.block.java.xml : %.java.xml
	${XSLT_BLOCKUNIT_JAVA} $< > $@

%.block.aj.xml : %.aj.xml
	${XSLT_BLOCKUNIT_JAVA} $< > $@

%.comment.cpp.xml : %.cpp.xml
	${XSLT_INSERTCOMMENT} $< > $@

%.comment.c.xml : %.c.xml
	${XSLT_INSERTCOMMENT} $< > $@

%.comment.cs.xml : %.cs.xml
	${XSLT_INSERTCOMMENT} $< > $@

%.comment.m.xml : %.m.xml
	${XSLT_INSERTCOMMENT} $< > $@

%.comment.java.xml : %.java.xml
	${XSLT_INSERTCOMMENT} $< > $@

%.comment.aj.xml : %.aj.xml
	${XSLT_INSERTCOMMENT} $< > $@

%.preproc.cpp.xml : %.cpp.xml
	${XSLT_INSERTPREPROC} $< > $@

%.preproc.c.xml : %.c.xml
	${XSLT_INSERTPREPROC} $< > $@

%.preproc.cs.xml : %.cs.xml
	${XSLT_INSERTPREPROC} $< > $@

%.preproc.m.xml : %.m.xml
	${XSLT_INSERTPREPROC} $< > $@

%.preproc.java.xml : %.java.xml
	${XSLT_INSERTPREPROC} $< > $@

%.preproc.aj.xml : %.aj.xml
	${XSLT_INSERTPREPROC} $< > $@

%.ifthenelse.cpp.xml : %.cpp.xml
	${XSLT_INSERTIFTHENELSE} $< > $@

%.ifthenelse.c.xml : %.c.xml
	${XSLT_INSERTIFTHENELSE} $< > $@

%.ifthenelse.cs.xml : %.cs.xml
	${XSLT_INSERTIFTHENELSE} $< > $@

%.ifthenelse.m.xml : %.m.xml
	${XSLT_INSERTIFTHENELSE} $< > $@

%.ifthenelse.java.xml : %.java.xml
	${XSLT_INSERTIFTHENELSE_JAVA} $< > $@

%.ifthenelse.aj.xml : %.aj.xml
	${XSLT_INSERTIFTHENELSE_JAVA} $< > $@

%.formfeed.cpp.xml : %.cpp.xml
	${XSLT_INSERTFORMFEED} $< > $@

%.formfeed.c.xml : %.c.xml
	${XSLT_INSERTFORMFEED} $< > $@

%.formfeed.cs.xml : %.cs.xml
	${XSLT_INSERTFORMFEED} $< > $@

%.formfeed.m.xml : %.m.xml
	${XSLT_INSERTFORMFEED} $< > $@

%.formfeed.java.xml : %.java.xml
	${XSLT_INSERTFORMFEED} $< > $@

%.formfeed.aj.xml : %.aj.xml
	${XSLT_INSERTFORMFEED} $< > $@

%.java.xml : %.cpp.xml
	${XSLT_SETLANGUAGE_JAVA} $< | ${XSLT_REMOVECPP} - > $@

%.aj.xml : %.java.xml
	${XSLT_SETLANGUAGE_ASPECTJ} $< > $@

%.c.xml : %.cpp.xml
	${XSLT_SETLANGUAGE_C} $< > $@

%.cs.xml : %.cpp.xml
	${XSLT_SETLANGUAGE_CS} $< > $@

%.m.xml : %.cpp.xml
	${XSLT_SETLANGUAGE_OBJECTIVE_C} $< > $@

%.m.xml : %.c.xml
	${XSLT_SETLANGUAGE_OBJECTIVE_C} $< > $@

%.cpp11.xml : %.cpp.xml
	${XSLT_SETLANGUAGE_CXX_11} $< > $@

GEN_FILES= $(GEN_JAVA) $(GEN_C) $(GEN_CS) $(GEN_OBJECTIVE_C) $(GEN_CXX) $(GEN_ASPECTJ) ${GEN_PART} ${GEN_ALL} ${GEN_BLOCK} ${GEN_COMMENT} ${GEN_PREPROC} ${GEN_IFTHENELSE} ${GEN_FORMFEED} ${GEN_STRUCT}

GEN_CPP_BASE=${GEN_SEP} ${foreach nm, $(SEP_PLUS), $(nm).all} ${foreach nm, $(SEP_PLUS), $(nm).block} ${foreach nm, $(SEP_PLUS), $(nm).comment} ${foreach nm, $(SEP_PLUS), $(nm).preproc} ${foreach nm, $(SEP_PLUS), $(nm).struct}

GEN_CPP= ${foreach nm, $(GEN_CPP_BASE), $(nm).cpp}

gencpp : ${GEN_CPP}

gen : ${GEN_FILES}

#  Test cases generated from other test cases

# function declaration from function
function_decl.cpp.xml : function.cpp.xml ${XSLT_DEFN2DECL_XSL}
	${XSLT_DEFN2DECL} $< > $@

function_asterisk_decl.cpp.xml : function_asterisk.cpp.xml ${XSLT_DEFN2DECL_XSL}
	${XSLT_DEFN2DECL} $< > $@

# const function from function
function_const.cpp.xml : function.cpp.xml ${XSLT_DEFN2DECL_XSL}
	${XSLT_ADDCONST} $< > $@

# while from if
while.cpp.xml : if.cpp.xml ${XSLT_IF2WHILE_XSL}
	${XSLT_IF2WHILE} $< > $@

# if with a nested conditional from if
ifnestcond.cpp.xml : if.cpp.xml ${XSLT_NESTCOND_XSL}
	${XSLT_NESTCOND} $< > $@

# return from return base
return.cpp.xml : return_base.cpp.xml ${XSLT_INSRT_XSL}
	${XSLT_INSERTEXPR} $< > $@

# constructor from constructor base
constructor.cpp.xml : constructor_base.cpp.xml ${XSLT_DEFN2DECL_XSL}
	cp $< $@

# explicit constructor from constructor
constructor_explicit.cpp.xml : constructor.cpp.xml ${XSLT_INSRT_XSL}
	${XSLT_ADDEXPLICIT} $< > $@

# virtual destructor from destructor
destructor_virtual.cpp.xml : destructor.cpp.xml ${XSLT_INSRT_XSL}
	${XSLT_ADDVIRTUAL} $< > $@

# throw from throw base
throw.cpp.xml : throw_base.cpp.xml ${XSLT_INSRT_XSL}
	${XSLT_INSERTEXPR} $< > $@

# struct from class
struct.cpp.xml : class.cpp.xml ${XSLT_CLASS2STRUCT_XSL}
	${XSLT_CLASS2STRUCT} $< > $@


# union from struct
union.cpp.xml : struct.cpp.xml ${XSLT_STRUCT2UNION_XSL}
	${XSLT_STRUCT2UNION} $< > $@

# struct from class
struct_cpp.cpp.xml : class_cpp.cpp.xml ${XSLT_CLASS2STRUCT_XSL}
	${XSLT_CLASS2STRUCT} $< > $@

# union from struct
union_cpp.cpp.xml : struct_cpp.cpp.xml ${XSLT_STRUCT2UNION_XSL}
	${XSLT_STRUCT2UNION} $< > $@

# struct from class
struct_cs.cs.xml : class_cs.cs.xml ${XSLT_CLASS2STRUCT_XSL}
	${XSLT_CLASS2STRUCT} $< > $@

# union from struct
#union_cs.cs.xml : struct_cs.cs.xml ${XSLT_STRUCT2UNION_XSL}
	#${XSLT_STRUCT2UNION} $< > $@

# union from struct
union_c.c.xml : struct_c.c.xml ${XSLT_STRUCT2UNION_XSL}
	${XSLT_STRUCT2UNION} $< > $@

# interface from class
interface_java.java.xml : class_java.java.xml ${XSLT_CLASS2INTERFACE_XSL}
	${XSLT_CLASS2INTERFACE} $< > $@

# if with blocks from if
ifblock.cpp.xml : if.cpp.xml ${XSLT_IF2IFBLOCK_XSL}
	${XSLT_IF2IFBLOCK} $< > $@

c_cpp_mode.restrict.cpp.xml : xsl/c_cpp_mode_basic_template xsl/c_cpp_mode_enum_template xsl/c_cpp_mode_specifier_template xsl/c_cpp_mode_call_template
	${CAT} xsl/c_cpp_mode_header $^ xsl/c_cpp_mode_footer > $@.temp
	${XSLT} --stringparam new_name restrict ${XSLT_RENAME_XSL} $@.temp > $@
	${RM} $@.temp

c_cpp_mode.mutable.cpp.xml : xsl/c_cpp_mode_basic_template xsl/c_cpp_mode_enum_template xsl/c_cpp_mode_specifier_template xsl/c_cpp_mode_call_template
	${CAT} xsl/c_cpp_mode_header $^ xsl/c_cpp_mode_footer > $@.temp
	${XSLT} --stringparam new_name mutable ${XSLT_RENAME_XSL} $@.temp > $@
	${RM} $@.temp

c_cpp_mode.try.cpp.xml : xsl/c_cpp_mode_basic_template xsl/c_cpp_mode_simple_decl_template xsl/c_cpp_mode_decl_template xsl/c_cpp_mode_single_template xsl/c_cpp_mode_enum_template xsl/c_cpp_mode_call_template
	${CAT} xsl/c_cpp_mode_header $^ xsl/c_cpp_mode_footer > $@.temp
	${XSLT} --stringparam new_name try ${XSLT_RENAME_XSL} $@.temp > $@
	${RM} $@.temp

c_cpp_mode.catch.cpp.xml : xsl/c_cpp_mode_basic_template xsl/c_cpp_mode_simple_decl_template xsl/c_cpp_mode_decl_template xsl/c_cpp_mode_single_template xsl/c_cpp_mode_enum_template
	${CAT} xsl/c_cpp_mode_header $^ xsl/c_cpp_mode_footer > $@.temp
	${XSLT} --stringparam new_name catch ${XSLT_RENAME_XSL} $@.temp > $@
	${RM} $@.temp


c_cpp_mode.class.cpp.xml : xsl/c_cpp_mode_basic_template xsl/c_cpp_mode_decl_template xsl/c_cpp_mode_single_template xsl/c_cpp_mode_call_template
	${CAT} xsl/c_cpp_mode_header $^ xsl/c_cpp_mode_footer > $@.temp
	${XSLT} --stringparam new_name class ${XSLT_RENAME_XSL} $@.temp > $@
	${RM} $@.temp

# class implementation from class interface
class_implementation_m.m.xml : class_interface_m.m.xml ${XSLT_INTERFACE2IMPLEMENTATION_XSL}
	${XSLT_INTERFACE2IMPLEMENTATION} $< > $@

# Java finallly from C#
finally_java.java.xml : finally_cs.cs.xml ${XSLT_SETLANGUAGE_CS_XSL}
	${XSLT_SETLANGUAGE_JAVA} $< | ${XSLT_REMOVECPP_PROBLEM} - > $@

# Objective-C try from C++
try_m.m.xml : try.cpp.xml ${XSLT_KEYWORD2MKEYWORD_XSL}
	${XSLT_KEYWORD2MKEYWORD} $< > $@

# Objective-C catch from C++
catch_m.m.xml : catch_form.cpp.xml ${XSLT_KEYWORD2MKEYWORD_XSL}
	${XSLT_KEYWORD2MKEYWORD} $< > $@

# Objective-C finally from C++
finally_m.m.xml : finally_cs.cs.xml ${XSLT_KEYWORD2MKEYWORD_XSL}
	${XSLT_KEYWORD2MKEYWORD} $< > $@

# Objective-C throw from C++
throw_m.m.xml : throw.cpp.xml ${XSLT_KEYWORD2MKEYWORD_XSL}
	${XSLT_KEYWORD2MKEYWORD} $< > $@

# Objective-C throw from C++
throw_base_m.m.xml : throw_base.cpp.xml ${XSLT_KEYWORD2MKEYWORD_XSL}
	${XSLT_KEYWORD2MKEYWORD} $< > $@

# Objective-C dynamic from synthesize
dynamic_m.m.xml : synthesize_m.m.xml ${XSLT_SYNTHESIZE2DYNAMIC_XSL}
	${XSLT_SYNTHESIZE2DYNAMIC} $< > $@

clean :
	rm -f ${GEN_FILES}
