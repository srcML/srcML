##
# Makefile for srcML translator, src2srcml

EXE_DIR = ../bin/
OBJ_DIR = ../obj/

WD=${shell pwd}

SRC2SRCML_BIN=${WD}/../bin/src2srcml
SRCML2SRC_BIN=${WD}/../bin/srcml2src

PHONY : testbase
testbase : gen

PHONY : shorttest
shorttest : testbase
	export SRC2SRCML_BIN=${SRC2SRCML_BIN}; export SRCML2SRC_BIN=${SRCML2SRC_BIN}; time ./testsuite.py ".*\.all|macro"

PHONY : longtest
longtest : testbase
	export SRC2SRCML_BIN=${SRC2SRCML_BIN}; export SRCML2SRC_BIN=${SRCML2SRC_BIN}; time ./testsuite.py

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

GEN_FILES_SEP=while ifnestcond return throw function_decl constructor_explicit destructor_virtual function_const struct union 

GEN_FILES_PART=${foreach nm, $(GEN_FILES_SEP), ../suite/$(nm).cpp.xml}

SEP_FILES_BASE_COMMON=comment string variable_names expression decl_simple if for do switch call function enum return_base problem ifblock

SEP_FILES_BASE_C=functionkr 

SEP_FILES_BASE_C_FAMILY=asm macro cstruct typedef extern main function_pointer decl_struct

SEP_FILES_BASE_OO=function_ns decl_ns variable_names_ns

SEP_FILES_BASE_CXX=friend try namespace template typedef_template constructor destructor throw_base function_throw catch_form class

SEP_FILES_BASE_JAVA=class_java keyword_java interface_java package

SEP_FILES_BASE=$(SEP_FILES_BASE_COMMON) $(SEP_FILES_BASE_C) $(SEP_FILES_BASE_C_FAMILY) $(SEP_FILES_BASE_CXX) $(SEP_FILES_BASE_JAVA)

SEP_FILES=$(SEP_FILES_BASE) preprocessor


SEP_FILES_BASE_JAVA_ALL = $(SEP_FILES_BASE_COMMON) $(SEP_FILES_BASE_00)
GEN_FILES_JAVA= ${foreach nm, $(SEP_FILES_BASE_JAVA_ALL), ../suite/$(nm).java.xml}

SEP_FILES_BASE_C_ALL = $(SEP_FILES_BASE_COMMON) $(SEP_FILES_BASE_C_FAMILY)
GEN_FILES_C= ${foreach nm, $(SEP_FILES_BASE_C_ALL), ../suite/$(nm).c.xml}

GEN_FILES_ALL_PART= ${foreach nm, $(SEP_FILES), $(nm).all}

SEP_FILES_PLUS= ${SEP_FILES} ${GEN_FILES_SEP}

GEN_FILES_ALL_JAVA=${foreach nm, $(SEP_FILES_BASE_JAVA_ALL), ../suite/$(nm).all.java.xml}

GEN_FILES_ALL_C=${foreach nm, $(SEP_FILES_BASE_C_ALL), ../suite/$(nm).all.c.xml} ../suite/extra.all.c.xml

GEN_FILES_ALL= ${foreach nm, $(SEP_FILES_PLUS), ../suite/$(nm).all.cpp.xml} ../suite/extra.all.cpp.xml

GEN_FILES_BLOCK= ${foreach nm, $(SEP_FILES_PLUS),          ../suite/$(nm).block.cpp.xml} \
                 ${foreach nm, $(SEP_FILES_BASE_C_ALL),    ../suite/$(nm).block.c.xml} \
                 ${foreach nm, $(SEP_FILES_BASE_JAVA_ALL), ../suite/$(nm).block.java.xml} \
		 ../suite/extra.block.c.xml ../suite/extra.block.cpp.xml

GEN_FILES_COMMENT= ${foreach nm, $(SEP_FILES_PLUS),          ../suite/$(nm).comment.cpp.xml} \
		   ${foreach nm, $(SEP_FILES_BASE_C_ALL),    ../suite/$(nm).comment.c.xml} \
		   ${foreach nm, $(SEP_FILES_BASE_JAVA_ALL), ../suite/$(nm).comment.java.xml} \
		   ../suite/extra.comment.c.xml ../suite/extra.comment.cpp.xml

GEN_FILES_FORMFEED = ${foreach nm, $(SEP_FILES_PLUS),          ../suite/$(nm).formfeed.cpp.xml} \
		     ${foreach nm, $(SEP_FILES_BASE_C_ALL),    ../suite/$(nm).formfeed.c.xml} \
		     ${foreach nm, $(SEP_FILES_BASE_JAVA_ALL), ../suite/$(nm).formfeed.java.xml} \
		     ../suite/extra.formfeed.c.xml ../suite/extra.formfeed.cpp.xml

GEN_FILES_PREPROC= ${foreach nm, $(SEP_FILES_PLUS), ../suite/$(nm).preproc.cpp.xml} \
	           ${foreach nm, $(SEP_FILES_BASE_C_ALL), ../suite/$(nm).preproc.c.xml} \
		   ../suite/extra.preproc.c.xml ../suite/extra.preproc.cpp.xml

get_language=srcml2src -l $1

language2extension=./get_extension.py $1

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

GEN_FILES=${GEN_FILES_PART} ${GEN_FILES_ALL} ${GEN_FILES_BLOCK} ${GEN_FILES_COMMENT} ${GEN_FILES_PREPROC} ${GEN_FILES_FORMFEED} ../suite/constructor.cpp.xml $(GEN_FILES_JAVA) $(GEN_FILES_ALL_JAVA) $(GEN_FILES_C) $(GEN_FILES_ALL_C)

GEN_FILES_CPP_BASE=${GEN_FILES_SEP} ${foreach nm, $(SEP_FILES_PLUS), $(nm).all} ${foreach nm, $(SEP_FILES_PLUS), $(nm).block} ${foreach nm, $(SEP_FILES_PLUS), $(nm).comment} ${foreach nm, $(SEP_FILES_PLUS), $(nm).preproc}

GEN_FILES_CPP= ${foreach nm, $(GEN_FILES_CPP_BASE), ../suite/$(nm).cpp}

all : ${GEN_FILES} ${GEN_FILES_CPP}
	rm -f ../suite/preprocessor.preproc.cpp.xml
	./testsuite.py

gencpp : ${GEN_FILES_CPP}

gen : ${GEN_FILES}
	rm -f ../suite/preprocessor.preproc.cpp.xml
	rm -f ../suite/class_java.preproc.cpp.xml
	rm -f ../suite/keyword_java.preproc.cpp.xml
	rm -f ../suite/interface_java.preproc.cpp.xml
	rm -f ../suite/package.preproc.cpp.xml

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
../suite/interface_java.cpp.xml : ../suite/class_java.cpp.xml ${XSLT_CLASS2INTERFACE_XSL}
	${XSLT_CLASS2INTERFACE} $< > $@

# if with blocks from if
../suite/ifblock.cpp.xml : ../suite/if.cpp.xml ${XSLT_IF2IFBLOCK_XSL}
	${XSLT_IF2IFBLOCK} $< > $@

cleangen :
	rm -f ${GEN_FILES}
