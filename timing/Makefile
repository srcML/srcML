##
# Makefile for srcML translator, src2srcml

EXE_DIR = ../bin/
OBJ_DIR = ../obj/

SRC2SRCML_BIN=../bin/src2srcml
SRCML2SRC_BIN=../bin/srcml2src

TIME=/usr/bin/time --output=/dev/stdout --format="%U"
TIMING_MED=timing_med.h
TIMING_LONG=timing_long.h
TIMING_JAVA=timing_java.java

PRINTF_FORMAT="\%1.0f lines/second\\n"

all : time javatime

PHONY : time
time : ${SRC2SRCML_BIN}
	@${SRC2SRCML_BIN} --version
	@${SRCML2SRC_BIN} --version
	@echo
	@echo -n "src2srcml:\t"
	@cd .; ${TIME} ${SRC2SRCML_BIN} ${TIMING_MED} ${TIMING_MED}.xml | sed 's/[0-9\.]*/ 10872 \/ &/' | bc -l |  xargs printf "%'7.0f lines/second\n"
	@echo -n "srcml2src:\t"
	@cd .; ${TIME} ${SRCML2SRC_BIN} ${TIMING_MED}.xml ${TIMING_MED}v2 | sed 's/[0-9\.]*/10872 \/ &/' | bc -l  |  xargs printf "%'7.0f lines/second\n"

PHONY : long
long : ${SRC2SRCML_BIN}
	@${SRC2SRCML_BIN} --version
	@${SRCML2SRC_BIN} --version
	@echo
	@echo -n "src2srcml long:\t"
	@cd .; ${TIME} ${SRC2SRCML_BIN} ${TIMING_LONG} ${TIMING_LONG}.xml | sed 's/[0-9\.]*/ 86976 \/ &/' | bc -l |  xargs printf "%'7.0f lines/second\n"

PHONY : javatime
javatime :
	@echo -n "src2srcml Java:\t"
	@cd .; ${TIME} ${SRC2SRCML_BIN} --language Java ${TIMING_JAVA} ${TIMING_JAVA}.xml | sed 's/[0-9\.]*/32628 \/ &/' | bc -l | xargs printf "%'7.0f lines/second\n"
