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

# base for C# only
BASE_CS=${call language,'LANGUAGE_CSHARP'}

# base for C# only
BASE_CS_GEN=${call language,'LANGUAGE_CSHARP_GEN'}

# base for Java only
BASE_JAVA=${call language,'LANGUAGE_JAVA'}

all : gen

testbase : gen

echobase :
	@echo ${BASE_COMMON}

echogen :
	@echo ${GEN_BASE}

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

XSLT_BLOCKUNIT_JAVA=${XSLT} xsl/insertblock.java.xsl

XSLT_ADDEXPLICIT=${XSLT} xsl/addexplicit.xsl

XSLT_ADDVIRTUAL=${XSLT} xsl/addvirtual.xsl

XSLT_ADDCONST=${XSLT} xsl/addconst.xsl

XSLT_IDENTITY=${XSLT} xsl/identity.xsl

XSLT_INSERTEXPR=${XSLT} --stringparam expr_filename "expression.cpp.xml" xsl/insertexpr.xsl
XSLT_INSERTEXPR_XSL= expression.cpp.xml xsl/insertexpr.xsl

XSLT_INSERTDECL=${XSLT} --stringparam decl_filename "decl_simple.cpp.xml" xsl/insertparam.xsl
XSLT_INSERTDECL_XSL= decl_simple.cpp.xml xsl/insertdecl.xsl

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

# nest if
XSLT_INSERTIFTHENELSE=${XSLT} xsl/nestif.xsl

# insert form feed
XSLT_INSERTFORMFEED=${XSLT} xsl/insertformfeed.xsl

##
# Test cases for one language are generated from test cases for another language.

XSLT_SETLANGUAGE_XSL=xsl/setlanguage.xsl

XSLT_REMOVECPP_XSL=xsl/removecpp.xsl

XSLT_SETLANGUAGE=${XSLT} ${XSLT_SETLANGUAGE_XSL}

XSLT_SETLANGUAGE_C=${XSLT} --stringparam language "C" ${XSLT_SETLANGUAGE_XSL}

XSLT_SETLANGUAGE_CS=${XSLT} --stringparam language "C\#" ${XSLT_SETLANGUAGE_XSL}

XSLT_SETLANGUAGE_CXX=${XSLT} --stringparam language "CXX" ${XSLT_SETLANGUAGE_XSL}

XSLT_SETLANGUAGE_CXX_11=${XSLT} --stringparam language "C++11" ${XSLT_SETLANGUAGE_XSL}

XSLT_SETLANGUAGE_JAVA=${XSLT} --stringparam language "Java" ${XSLT_SETLANGUAGE_XSL}

XSLT_REMOVECPP=${XSLT} ${XSLT_REMOVECPP_XSL}

SEP_BASE=$(BASE_COMMON) $(BASE_C_FAMILY) $(BASE_CXX) $(BASE_OO)

# basic generated C++ files
GEN_SEP= $(GEN_BASE) $(GEN_BASE_CXX)

# generated cases for C++ from base
GEN_PART=${foreach nm, $(GEN_SEP), $(nm).cpp.xml}

SEP_FILES=$(SEP_BASE) preprocessor

# base for all of Java
BASE_JAVA_ALL_GEN = $(BASE_COMMON) $(GEN_BASE) $(BASE_OO)

BASE_JAVA_ALL = $(BASE_COMMON) $(GEN_BASE) $(BASE_OO) $(BASE_JAVA)

GEN_JAVA= ${foreach nm, $(BASE_JAVA_ALL_GEN), $(nm).java.xml}

BASE_CXX_11_ALL_GEN = $(BASE_COMMON) $(BASE_CXX) $(BASE_OO)

BASE_CXX_11_ALL = $(BASE_COMMON) $(BASE_CXX) $(BASE_OO)

GEN_CXX_11= ${foreach nm, $(BASE_CXX_11_ALL_GEN), $(nm).cpp11.xml}

BASE_C_ALL_GEN = $(BASE_COMMON) $(BASE_C_FAMILY)
BASE_C_ALL = $(BASE_COMMON) $(BASE_C_FAMILY) $(BASE_C)
GEN_C= ${foreach nm, $(BASE_C_ALL_GEN), $(nm).c.xml}

BASE_CS_ALL_GEN = $(BASE_COMMON) $(GEN_BASE) $(BASE_CS_GEN)
BASE_CS_ALL = $(BASE_COMMON) $(BASE_CS) $(BASE_CS_GEN)
GEN_CS= ${foreach nm, $(BASE_CS_ALL_GEN), $(nm).cs.xml}

SEP_PLUS= ${SEP_FILES} ${GEN_SEP}
SEP_PLUSNOPREPROC= ${SEP_BASE} ${GEN_SEP}

##
# generate cases for all
GEN_ALL= ${foreach nm, $(SEP_PLUS), $(nm).all.cpp.xml} extra_cpp.all.cpp.xml \
	       ${foreach nm, $(BASE_JAVA_ALL), $(nm).all.java.xml} \
	       ${foreach nm, $(BASE_CXX_11_ALL), $(nm).all.cpp11.xml} \
	       ${foreach nm, $(BASE_C_ALL), $(nm).all.c.xml} \
	       ${foreach nm, $(BASE_CS_ALL), $(nm).all.cs.xml} \
		extra_c.all.c.xml extra_cpp.all.cpp.xml

# generated cases for blocks
GEN_BLOCK= ${foreach nm, $(SEP_PLUS),          $(nm).block.cpp.xml} \
               ${foreach nm, $(BASE_C_ALL),    $(nm).block.c.xml} \
               ${foreach nm, $(BASE_CS_ALL),    $(nm).block.cs.xml} \
 	       ${foreach nm, $(BASE_CXX_11_ALL), $(nm).block.cpp11.xml} \
               ${foreach nm, $(BASE_JAVA_ALL), $(nm).block.java.xml} \
		 extra_c.block.c.xml extra_cpp.block.cpp.xml

# generated cases for comments
GEN_COMMENT= ${foreach nm, $(SEP_PLUS),          $(nm).comment.cpp.xml} \
	       ${foreach nm, $(BASE_C_ALL),    $(nm).comment.c.xml} \
	       ${foreach nm, $(BASE_CS_ALL),    $(nm).comment.cs.xml} \
 	       ${foreach nm, $(BASE_CXX_11_ALL), $(nm).comment.cpp11.xml} \
	       ${foreach nm, $(BASE_JAVA_ALL), $(nm).comment.java.xml} \
		   extra_c.comment.c.xml extra_cpp.comment.cpp.xml

# generated cases for formfeed
GEN_FORMFEED = ${foreach nm, $(SEP_PLUS),          $(nm).formfeed.cpp.xml} \
		     ${foreach nm, $(BASE_C_ALL),    $(nm).formfeed.c.xml} \
		     ${foreach nm, $(BASE_CS_ALL),    $(nm).formfeed.cs.xml} \
 	       ${foreach nm, $(BASE_CXX_11_ALL), $(nm).formfeed.cpp11.xml} \
		     ${foreach nm, $(BASE_JAVA_ALL), $(nm).formfeed.java.xml} \
		     extra_c.formfeed.c.xml extra_cpp.formfeed.cpp.xml

# generated test cases for preproc
GEN_PREPROC= ${foreach nm, $(SEP_PLUSNOPREPROC), $(nm).preproc.cpp.xml} \
 	       ${foreach nm, $(BASE_CXX_11_ALL), $(nm).preproc.cpp11.xml} \
	           ${foreach nm, $(BASE_C_ALL), $(nm).preproc.c.xml} \
	           ${foreach nm, $(BASE_CS_ALL), $(nm).preproc.cs.xml} \
		   extra_c.preproc.c.xml extra_cpp.preproc.cpp.xml

# generated test cases for preproc
GEN_IFTHENELSE= ${foreach nm, $(SEP_PLUS), $(nm).ifthenelse.cpp.xml} \
 	       ${foreach nm, $(BASE_CXX_11_ALL), $(nm).ifthenelse.cpp11.xml} \
	           ${foreach nm, $(BASE_C_ALL), $(nm).ifthenelse.c.xml} \
	           ${foreach nm, $(BASE_CS_ALL), $(nm).ifthenelse.cs.xml} \
		   extra_c.ifthenelse.c.xml extra_cpp.ifthenelse.cpp.xml

allinall.c.xml :
	@echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > allinall.c.xml
	@echo "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\" dir=\"allinall.c\">" >> allinall.c.xml

	find ../suite -name "*.all.c.xml" | xargs -i tail +3 {} | sed "s|</unit>||">> allinall.c.xml
	@echo "</unit>" >> allinall.c.xml

allinall.java.xml :
	@echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > allinall.java.xml
	@echo "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"Java\" dir=\"allinall.java\">" >> allinall.java.xml

	find ../suite -name "*.all.java.xml" | xargs -i tail +3 {} | sed "s|</unit>||">> allinall.java.xml
	@echo "</unit>" >> allinall.java.xml

allinall.1.cpp.xml :
	@echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > allinall.1.cpp.xml
	@echo "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"allinall.1.cpp\">" >> allinall.1.cpp.xml

	@find ../suite -name "*.all.cpp.xml" | sed "s|cstruct.all.cpp.xml||" | sed "s|functionkr.all.cpp.xml||" | head -30 | xargs -i tail +3 {} | sed "s|</unit>||" >> allinall.1.cpp.xml
	@echo "</unit>" >> allinall.1.cpp.xml

allinall.2.cpp.xml :
	@echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" > allinall.2.cpp.xml
	@echo "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"allinall.2.cpp\">" >> allinall.2.cpp.xml

	@find ../suite -name "*.all.cpp.xml" | sed "s|cstruct.all.cpp.xml||" | sed "s|functionkr.all.cpp.xml||" | tail -18 | xargs -i tail +3 {} | sed "s|</unit>||" >> allinall.2.cpp.xml
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

%.cpp : %.cpp.xml
	${XSLT_IDENTITY} $< > $@

%.all.cpp.xml : %.cpp.xml
	${XSLT_MERGEUNIT} $< > $@

%.all.c.xml : %.c.xml
	${XSLT_MERGEUNIT} $< > $@

%.all.cs.xml : %.cs.xml
	${XSLT_MERGEUNIT} $< > $@

%.all.java.xml : %.java.xml
	${XSLT_MERGEUNIT} $< > $@

%.block.cpp.xml : %.cpp.xml
	${XSLT_BLOCKUNIT} $< > $@

%.block.c.xml : %.c.xml
	${XSLT_BLOCKUNIT} $< > $@

%.block.cs.xml : %.cs.xml
	${XSLT_BLOCKUNIT} $< > $@

%.block.java.xml : %.java.xml
	${XSLT_BLOCKUNIT_JAVA} $< > $@

%.comment.cpp.xml : %.cpp.xml
	${XSLT_INSERTCOMMENT} $< > $@

%.comment.c.xml : %.c.xml
	${XSLT_INSERTCOMMENT} $< > $@

%.comment.cs.xml : %.cs.xml
	${XSLT_INSERTCOMMENT} $< > $@

%.comment.java.xml : %.java.xml
	${XSLT_INSERTCOMMENT} $< > $@

%.preproc.cpp.xml : %.cpp.xml
	${XSLT_INSERTPREPROC} $< > $@

%.preproc.c.xml : %.c.xml
	${XSLT_INSERTPREPROC} $< > $@

%.preproc.cs.xml : %.cs.xml
	${XSLT_INSERTPREPROC} $< > $@

%.preproc.java.xml : %.java.xml
	${XSLT_INSERTPREPROC} $< > $@

%.ifthenelse.cpp.xml : %.cpp.xml
	${XSLT_INSERTIFTHENELSE} $< > $@

%.ifthenelse.c.xml : %.c.xml
	${XSLT_INSERTIFTHENELSE} $< > $@

%.ifthenelse.cs.xml : %.cs.xml
	${XSLT_INSERTIFTHENELSE} $< > $@

%.ifthenelse.java.xml : %.java.xml
	${XSLT_INSERTIFTHENELSE} $< > $@

%.formfeed.cpp.xml : %.cpp.xml
	${XSLT_INSERTFORMFEED} $< > $@

%.formfeed.c.xml : %.c.xml
	${XSLT_INSERTFORMFEED} $< > $@

%.formfeed.cs.xml : %.cs.xml
	${XSLT_INSERTFORMFEED} $< > $@

%.formfeed.java.xml : %.java.xml
	${XSLT_INSERTFORMFEED} $< > $@

%.java.xml : %.cpp.xml
	${XSLT_SETLANGUAGE_JAVA} $< | ${XSLT_REMOVECPP} - > $@

%.c.xml : %.cpp.xml
	${XSLT_SETLANGUAGE_C} $< > $@

%.cs.xml : %.cpp.xml
	${XSLT_SETLANGUAGE_CS} $< > $@

%.cpp11.xml : %.cpp.xml
	${XSLT_SETLANGUAGE_CXX_11} $< > $@

GEN_FILES=${GEN_PART} ${GEN_ALL} ${GEN_BLOCK} ${GEN_COMMENT} ${GEN_PREPROC} ${GEN_IFTHENELSE} ${GEN_FORMFEED} $(GEN_JAVA) $(GEN_C) $(GEN_CS) $(GEN_CXX_11)

GEN_CPP_BASE=${GEN_SEP} ${foreach nm, $(SEP_PLUS), $(nm).all} ${foreach nm, $(SEP_PLUS), $(nm).block} ${foreach nm, $(SEP_PLUS), $(nm).comment} ${foreach nm, $(SEP_PLUS), $(nm).preproc}

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
struct_cs.cs.xml : class_cs.cs.xml ${XSLT_CLASS2STRUCT_XSL}
	${XSLT_CLASS2STRUCT} $< > $@

# union from struct
union_cs.cs.xml : struct_cs.cs.xml ${XSLT_STRUCT2UNION_XSL}
	${XSLT_STRUCT2UNION} $< > $@

# interface from class
interface_java.java.xml : class_java.java.xml ${XSLT_CLASS2INTERFACE_XSL}
	${XSLT_CLASS2INTERFACE} $< > $@

# if with blocks from if
ifblock.cpp.xml : if.cpp.xml ${XSLT_IF2IFBLOCK_XSL}
	${XSLT_IF2IFBLOCK} $< > $@

clean :
	rm -f ${GEN_FILES}
