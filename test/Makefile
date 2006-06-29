##
# Makefile for srcML translator, src2srcml

EXE_DIR = ../bin/
OBJ_DIR = ../obj/

WD=${shell pwd}

#SRC2SRCML=${WD}/../bin/src2srcml
#SRCML2SRC=${WD}/../bin/srcml2src

SRCENCODING=
#SRCENCODING=--src-encoding=ISO-8859-1

ALLINALLCUTOFF=35

PHONY : testbase
testbase : gen

PHONY : short
short : testbase
	@export SRC2SRCML=${SRC2SRCML}; export SRCML2SRC=${SRCML2SRC}; time ./testsuite.py ".*\.all|macro"

PHONY : shortc
shortc : testbase
	@export SRC2SRCML=${SRC2SRCML}; export SRCML2SRC=${SRCML2SRC}; time ./testsuite.py ".*\.all|macro" C

PHONY : shortcpp
shortcpp : testbase
	@export SRC2SRCML=${SRC2SRCML}; export SRCML2SRC=${SRCML2SRC}; time ./testsuite.py ".*\.all|macro" C++

PHONY : shortjava
shortjava : testbase
	@export SRC2SRCML=${SRC2SRCML}; export SRCML2SRC=${SRCML2SRC}; time ./testsuite.py ".*\.all" Java

PHONY : 
full : testbase
	@export SRC2SRCML=${SRC2SRCML}; export SRCML2SRC=${SRCML2SRC}; time ./testsuite.py

PHONY : fullcpp
fullcpp : testbase
	@export SRC2SRCML=${SRC2SRCML}; export SRCML2SRC=${SRCML2SRC}; time ./testsuite.py ".*" C++

PHONY : fullc
fullc : testbase
	@export SRC2SRCML=${SRC2SRCML}; export SRCML2SRC=${SRCML2SRC}; time ./testsuite.py ".*" C

PHONY : fulljava
fulljava : testbase
	@export SRC2SRCML=${SRC2SRCML}; export SRCML2SRC=${SRCML2SRC}; time ./testsuite.py ".*" Java

XSLT=/usr/bin/xsltproc

# merge separate srcML units into one
XSLT_MERGEUNIT_XSL=xsl/mergeunit.xsl
XSLT_MERGEUNIT=${XSLT} ${XSLT_MERGEUNIT_XSL}

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

XSLT_ADDEXPLICIT=${XSLT} xsl/addexplicit.xsl

XSLT_ADDVIRTUAL=${XSLT} xsl/addvirtual.xsl

XSLT_ADDCONST=${XSLT} xsl/addconst.xsl

XSLT_IDENTITY=${XSLT} xsl/identity.xsl

XSLT_INSERTEXPR=${XSLT} --stringparam expr_filename "../suite/expression.cpp.xml" xsl/insertexpr.xsl
XSLT_INSERTEXPR_XSL= ../suite/expression.cpp.xml xsl/insertexpr.xsl

XSLT_INSERTDECL=${XSLT} --stringparam decl_filename "../suite/decl_simple.cpp.xml" xsl/insertparam.xsl
XSLT_INSERTDECL_XSL= ../suite/decl_simple.cpp.xml xsl/insertdecl.xsl

XSLT_STRUCT2UNION_XSL=xsl/struct2union.xsl
XSLT_STRUCT2UNION=${XSLT} ${XSLT_STRUCT2UNION_XSL}

XSLT_CLASS2STRUCT_XSL=xsl/class2struct.xsl
XSLT_CLASS2STRUCT=${XSLT} ${XSLT_CLASS2STRUCT_XSL}

XSLT_CLASS2INTERFACE_XSL=xsl/class2interface.xsl
XSLT_CLASS2INTERFACE=${XSLT} ${XSLT_CLASS2INTERFACE_XSL}

XSLT_IF2IFBLOCK_XSL=xsl/if2ifblock.xsl
XSLT_IF2IFBLOCK=${XSLT} ${XSLT_IF2IFBLOCK_XSL}

##
# Comments and preprocessor statements are handled out of the normal grammar rules
# in src2srcml.  In order to test the handling of these tokens, special cases are
# generated from the non-comment and preprocessor cases.

# insert comments
XSLT_INSERTCOMMENT=${XSLT} xsl/insertcomments.xsl

# insert preprocessor
XSLT_INSERTPREPROC=${XSLT} xsl/insertpreproc.xsl

# insert form feed
XSLT_INSERTFORMFEED=${XSLT} xsl/insertformfeed.xsl

##
# Test cases for one language are generated from test cases for another language.

XSLT_SETLANGUAGE_XSL=xsl/setlanguage.xsl

XSLT_SETLANGUAGE=${XSLT} ${XSLT_SETLANGUAGE_XSL}

XSLT_SETLANGUAGE_C=${XSLT} --stringparam language "C" ${XSLT_SETLANGUAGE_XSL}

XSLT_SETLANGUAGE_CXX=${XSLT} --stringparam language "CXX" ${XSLT_SETLANGUAGE_XSL}

XSLT_SETLANGUAGE_JAVA=${XSLT} --stringparam language "Java" ${XSLT_SETLANGUAGE_XSL}

##
# General lists of test cases

# original base of cases for all languages
BASE_COMMON=comment string variable_names expression decl_simple \
	          if for do switch call function enum return_base problem

# base for all languages generated from original cases
GEN_BASE=while ifnestcond return function_decl ifblock

# base for C++ generated from original cases
GEN_BASE_CXX=throw constructor_explicit destructor_virtual function_const struct union

# basic generated C++ files
GEN_SEP= $(GEN_BASE) $(GEN_BASE_CXX)

# generated cases for C++ from base
GEN_PART=${foreach nm, $(GEN_SEP), ../suite/$(nm).cpp.xml}

# base for C only
BASE_C=functionkr 

# base for C family
BASE_C_FAMILY=asm macro cstruct typedef extern main function_pointer decl_struct

# base for object-oriented languages
BASE_OO=function_ns decl_ns variable_names_ns

# base for C++ only
BASE_CXX=friend try namespace template typedef_template constructor destructor throw_base function_throw catch_form class function_operator

# base for Java only
BASE_JAVA=class_java keyword_java interface_java package

SEP_BASE=$(BASE_COMMON) $(BASE_C) $(BASE_C_FAMILY) $(BASE_CXX)

SEP_FILES=$(SEP_BASE) preprocessor

# base for all of Java
BASE_JAVA_ALL_GEN = $(BASE_COMMON) $(GEN_BASE) $(BASE_00)

BASE_JAVA_ALL = $(BASE_COMMON) $(GEN_BASE) $(BASE_00) $(BASE_JAVA)

GEN_JAVA= ${foreach nm, $(BASE_JAVA_ALL_GEN), ../suite/$(nm).java.xml}

BASE_C_ALL = $(BASE_COMMON) $(BASE_C_FAMILY)
GEN_C= ${foreach nm, $(BASE_C_ALL), ../suite/$(nm).c.xml}

SEP_PLUS= ${SEP_FILES} ${GEN_SEP}
SEP_PLUSNOPREPROC= ${SEP_BASE} ${GEN_SEP}

##
# generate cases for all
GEN_ALL= ${foreach nm, $(SEP_PLUS), ../suite/$(nm).all.cpp.xml} ../suite/extra.all.cpp.xml \
	       ${foreach nm, $(BASE_JAVA_ALL), ../suite/$(nm).all.java.xml} \
	       ${foreach nm, $(BASE_C_ALL), ../suite/$(nm).all.c.xml} ../suite/extra.all.c.xml

# generated cases for blocks
GEN_BLOCK= ${foreach nm, $(SEP_PLUS),          ../suite/$(nm).block.cpp.xml} \
                 ${foreach nm, $(BASE_C_ALL),    ../suite/$(nm).block.c.xml} \
                 ${foreach nm, $(BASE_JAVA_ALL), ../suite/$(nm).block.java.xml} \
		 ../suite/extra.block.c.xml ../suite/extra.block.cpp.xml

# generated cases for comments
GEN_COMMENT= ${foreach nm, $(SEP_PLUS),          ../suite/$(nm).comment.cpp.xml} \
		   ${foreach nm, $(BASE_C_ALL),    ../suite/$(nm).comment.c.xml} \
		   ${foreach nm, $(BASE_JAVA_ALL), ../suite/$(nm).comment.java.xml} \
		   ../suite/extra.comment.c.xml ../suite/extra.comment.cpp.xml

# generated cases for formfeed
GEN_FORMFEED = ${foreach nm, $(SEP_PLUS),          ../suite/$(nm).formfeed.cpp.xml} \
		     ${foreach nm, $(BASE_C_ALL),    ../suite/$(nm).formfeed.c.xml} \
		     ${foreach nm, $(BASE_JAVA_ALL), ../suite/$(nm).formfeed.java.xml} \
		     ../suite/extra.formfeed.c.xml ../suite/extra.formfeed.cpp.xml

# generated test cases for preproc
GEN_PREPROC= ${foreach nm, $(SEP_PLUSNOPREPROC), ../suite/$(nm).preproc.cpp.xml} \
	           ${foreach nm, $(BASE_C_ALL), ../suite/$(nm).preproc.c.xml} \
		   ../suite/extra.preproc.c.xml ../suite/extra.preproc.cpp.xml

../suite/allinall.c.xml :
	@echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > ../suite/allinall.c.xml
	@echo "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\" dir=\"allinall.c\">" >> ../suite/allinall.c.xml

	find ../suite -name "*.all.c.xml" | xargs -i tail +3 {} | sed "s|</unit>||">> ../suite/allinall.c.xml
	@echo "</unit>" >> ../suite/allinall.c.xml

../suite/allinall.java.xml :
	@echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > ../suite/allinall.java.xml
	@echo "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"Java\" dir=\"allinall.java\">" >> ../suite/allinall.java.xml

	find ../suite -name "*.all.java.xml" | xargs -i tail +3 {} | sed "s|</unit>||">> ../suite/allinall.java.xml
	@echo "</unit>" >> ../suite/allinall.java.xml

../suite/allinall.1.cpp.xml :
	@echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > ../suite/allinall.1.cpp.xml
	@echo "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"allinall.1.cpp\">" >> ../suite/allinall.1.cpp.xml

	@find ../suite -name "*.all.cpp.xml" | sed "s|../suite/cstruct.all.cpp.xml||" | sed "s|../suite/functionkr.all.cpp.xml||" | head -30 | xargs -i tail +3 {} | sed "s|</unit>||" >> ../suite/allinall.1.cpp.xml
	@echo "</unit>" >> ../suite/allinall.1.cpp.xml

../suite/allinall.2.cpp.xml :
	@echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > ../suite/allinall.2.cpp.xml
	@echo "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"allinall.2.cpp\">" >> ../suite/allinall.2.cpp.xml

	@find ../suite -name "*.all.cpp.xml" | sed "s|../suite/cstruct.all.cpp.xml||" | sed "s|../suite/functionkr.all.cpp.xml||" | tail -18 | xargs -i tail +3 {} | sed "s|</unit>||" >> ../suite/allinall.2.cpp.xml
	@echo "</unit>" >> ../suite/allinall.2.cpp.xml

allinone : allinonecpp allinonec allinonejava
	#@make cleanall

allinonec : ../suite/allinall.c.xml
	-${SRCML2SRC} ${SRCENCODING} ../suite/allinall.c.xml | ${SRC2SRCML} -l C ${SRCENCODING} --directory="allinall.c" | diff -w  - ../suite/allinall.c.xml

allinonecpp : ../suite/allinall.1.cpp.xml ../suite/allinall.2.cpp.xml
	-${SRCML2SRC} ${SRCENCODING} ../suite/allinall.1.cpp.xml | ${SRC2SRCML} -l C++ ${SRCENCODING} --directory="allinall.1.cpp" | diff -w  - ../suite/allinall.1.cpp.xml
	-${SRCML2SRC} ${SRCENCODING} ../suite/allinall.2.cpp.xml | ${SRC2SRCML} -l C++ ${SRCENCODING} --directory="allinall.2.cpp" | diff -w  - ../suite/allinall.2.cpp.xml

allinonejava : ../suite/allinall.java.xml
	-${SRCML2SRC} ${SRCENCODING} ../suite/allinall.java.xml | ${SRC2SRCML} -l Java ${SRCENCODING} --directory="allinall.java" | diff -w  - ../suite/allinall.java.xml

cleanall :
	rm -f ../suite/allinall.*

%.cpp : %.cpp.xml
	${XSLT_IDENTITY} $< > $@

%.all.cpp.xml : %.cpp.xml
	${XSLT_MERGEUNIT} $< > $@

%.all.c.xml : %.c.xml
	${XSLT_MERGEUNIT} $< > $@

%.all.java.xml : %.java.xml
	${XSLT_MERGEUNIT} $< > $@

%.block.cpp.xml : %.cpp.xml
	${XSLT_BLOCKUNIT} $< > $@

%.block.c.xml : %.c.xml
	${XSLT_BLOCKUNIT} $< > $@

%.block.java.xml : %.java.xml
	${XSLT_BLOCKUNIT} $< > $@

%.comment.cpp.xml : %.cpp.xml
	${XSLT_INSERTCOMMENT} $< > $@

%.comment.c.xml : %.c.xml
	${XSLT_INSERTCOMMENT} $< > $@

%.comment.java.xml : %.java.xml
	${XSLT_INSERTCOMMENT} $< > $@

%.preproc.cpp.xml : %.cpp.xml
	${XSLT_INSERTPREPROC} $< > $@

%.preproc.c.xml : %.c.xml
	${XSLT_INSERTPREPROC} $< > $@

%.preproc.java.xml : %.java.xml
	${XSLT_INSERTPREPROC} $< > $@

%.formfeed.cpp.xml : %.cpp.xml
	${XSLT_INSERTFORMFEED} $< > $@

%.formfeed.c.xml : %.c.xml
	${XSLT_INSERTFORMFEED} $< > $@

%.formfeed.java.xml : %.java.xml
	${XSLT_INSERTFORMFEED} $< > $@

%.java.xml : %.cpp.xml
	${XSLT_SETLANGUAGE_JAVA} $< > $@

%.c.xml : %.cpp.xml
	${XSLT_SETLANGUAGE_C} $< > $@

GEN_FILES=${GEN_PART} ${GEN_ALL} ${GEN_BLOCK} ${GEN_COMMENT} ${GEN_PREPROC} ${GEN_FORMFEED} ../suite/constructor.cpp.xml $(GEN_JAVA) $(GEN_C)

GEN_CPP_BASE=${GEN_SEP} ${foreach nm, $(SEP_PLUS), $(nm).all} ${foreach nm, $(SEP_PLUS), $(nm).block} ${foreach nm, $(SEP_PLUS), $(nm).comment} ${foreach nm, $(SEP_PLUS), $(nm).preproc}

GEN_CPP= ${foreach nm, $(GEN_CPP_BASE), ../suite/$(nm).cpp}

all : ${GEN_FILES} ${GEN_CPP}
	./testsuite.py

gencpp : ${GEN_CPP}

gen : ${GEN_FILES}

#  Test cases generated from other test cases

# function declaration from function
../suite/function_decl.cpp.xml : ../suite/function.cpp.xml ${XSLT_DEFN2DECL_XSL}
	${XSLT_DEFN2DECL} $< > $@

# const function from function
../suite/function_const.cpp.xml : ../suite/function.cpp.xml ${XSLT_DEFN2DECL_XSL}
	${XSLT_ADDCONST} $< > $@

# while from if
../suite/while.cpp.xml : ../suite/if.cpp.xml ${XSLT_IF2WHILE_XSL}
	${XSLT_IF2WHILE} $< > $@

# if with a nested conditional from if
../suite/ifnestcond.cpp.xml : ../suite/if.cpp.xml ${XSLT_NESTCOND_XSL}
	${XSLT_NESTCOND} $< > $@

# return from return base
../suite/return.cpp.xml : ../suite/return_base.cpp.xml ${XSLT_INSRT_XSL}
	${XSLT_INSERTEXPR} $< > $@

# constructor from constructor base
../suite/constructor.cpp.xml : ../suite/constructor_base.cpp.xml ${XSLT_DEFN2DECL_XSL}
	cp $< $@

# explicit constructor from constructor
../suite/constructor_explicit.cpp.xml : ../suite/constructor.cpp.xml ${XSLT_INSRT_XSL}
	${XSLT_ADDEXPLICIT} $< > $@

# virtual destructor from destructor
../suite/destructor_virtual.cpp.xml : ../suite/destructor.cpp.xml ${XSLT_INSRT_XSL}
	${XSLT_ADDVIRTUAL} $< > $@

# throw from throw base
../suite/throw.cpp.xml : ../suite/throw_base.cpp.xml ${XSLT_INSRT_XSL}
	${XSLT_INSERTEXPR} $< > $@

# struct from class
../suite/struct.cpp.xml : ../suite/class.cpp.xml ${XSLT_CLASS2STRUCT_XSL}
	${XSLT_CLASS2STRUCT} $< > $@

# union from struct
../suite/union.cpp.xml : ../suite/struct.cpp.xml ${XSLT_STRUCT2UNION_XSL}
	${XSLT_STRUCT2UNION} $< > $@

# interface from class
../suite/interface_java.java.xml : ../suite/class_java.java.xml ${XSLT_CLASS2INTERFACE_XSL}
	${XSLT_CLASS2INTERFACE} $< > $@

# if with blocks from if
../suite/ifblock.cpp.xml : ../suite/if.cpp.xml ${XSLT_IF2IFBLOCK_XSL}
	${XSLT_IF2IFBLOCK} $< > $@

clean :
	rm -f ${GEN_FILES}
