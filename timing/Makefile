##
# Makefile for srcML translator, src2srcml

EXE_DIR = ../bin/
OBJ_DIR = ../obj/

SRC2SRCML_BIN=../bin/src2srcml
SRCML2SRC_BIN=../bin/srcml2src

# detect OS                                                                     
OSUPPER = $(shell uname -s 2>/dev/null | tr [:lower:] [:upper:])
OSLOWER = $(shell uname -s 2>/dev/null | tr [:upper:] [:lower:])

# 'linux' is output for Linux system, 'darwin' for OS X                         
DARWIN = $(strip $(findstring DARWIN, $(OSUPPER)))

ifneq ($(DARWIN),)
TIME=/opt/local/bin/gtime --output=/dev/stdout --format="%U"
else
TIME=time 
#TIME=time --output=/dev/stdout --format="%U"
endif

TIMING_MED=timing_med.h
TIMING_JAVA=timing_java.java

PRINTF_FORMAT="\%1.0f lines/second\\n"

ifneq ($(DARWIN),)
SED=/opt/local/bin/gsed
ECHO=/opt/local/bin/gecho
FLAGS=-ne
else
SED=sed
ECHO=echo
FLAGS=-n
endif

PHONY : time
time : ../bin/src2srcml
	@../bin/src2srcml --version
	@../bin/srcml2src --version
	@${ECHO}
	@${ECHO} ${FLAGS} "${SRC2SRCML_BIN} ${TIMING_MED} -o ${TIMING_MED}.xml:"
	@cd .; ${TIME} ${SRC2SRCML_BIN} ${TIMING_MED} -o ${TIMING_MED}.xml | ${SED} 's/[0-9\.]*/ 10872 \/ &/' | bc -l |  xargs printf "%7.0f lines/second\n"
	@${ECHO} ${FLAGS} "${SRCML2SRC_BIN} ${TIMING_MED}.xml -o ${TIMING_MED}vs:"
	@cd .; ${TIME} ${SRCML2SRC_BIN} ${TIMING_MED}.xml -o ${TIMING_MED}v2 | ${SED} 's/[0-9\.]*/10872 \/ &/' | bc -l  |  xargs printf "%7.0f lines/second\n"

PHONY : javatime
javatime :
	@${ECHO} ${FLAGS} "${SRC2SRCML_BIN} ${TIMING_JAVA} -o ${TIMING_JAVA}.xml: "
	@cd .; ${TIME} ${SRC2SRCML_BIN} --language Java ${TIMING_JAVA} -o ${TIMING_JAVA}.xml | ${SED} 's/[0-9\.]*/32628 \/ &/' | bc -l | xargs printf "%7.0f lines/second\n"

	@${ECHO} ${FLAGS} "${SRCML2SRC_BIN} ${TIMING_JAVA}.xml -o ${TIMING_JAVA}v2: "
	@cd .; ${TIME} ${SRCML2SRC_BIN} --language Java ${TIMING_JAVA}.xml -o ${TIMING_JAVA}v2 | ${SED} 's/[0-9\.]*/32628 \/ &/' | bc -l | xargs printf "%7.0f lines/second\n"

clean:
	rm *.*v2 *.xml
