##
# Makefile for srcML translator, src2srcml

EXE_DIR = ../bin/
OBJ_DIR = ../obj/

SRC2SRCML_BIN=../bin/src2srcml
SRCML2SRC_BIN=../bin/srcml2src

TIME=/opt/local/bin/gtime --output=/dev/stdout --format="%U"
TIMING_MED=timing_med.h
TIMING_JAVA=timing_java.java

PRINTF_FORMAT="\%1.0f lines/second\\n"

SED=/opt/local/bin/gsed
ECHO=/opt/local/bin/gecho

PHONY : time
time : ../bin/src2srcml
	@../bin/src2srcml --version
	@../bin/srcml2src --version
	@${ECHO}
	@${ECHO} -ne "src2srcml:\t"
	@cd .; ${TIME} ../bin/src2srcml ${TIMING_MED} -o ${TIMING_MED}.xml | ${SED} 's/[0-9\.]*/ 10872 \/ &/' | bc -l |  xargs printf "%7.0f lines/second\n"
	@${ECHO} -ne "srcml2src:\t"
	@cd .; ${TIME} ../bin/srcml2src ${TIMING_MED}.xml -o ${TIMING_MED}v2 | ${SED} 's/[0-9\.]*/10872 \/ &/' | bc -l  |  xargs printf "%7.0f lines/second\n"

PHONY : javatime
javatime :
	@${ECHO} -ne "srcml2src Java:\t"
	@cd .; ${TIME} ../bin/src2srcml --language Java ${TIMING_JAVA} -o ${TIMING_JAVA}.xml | ${SED} 's/[0-9\.]*/32628 \/ &/' | bc -l | xargs printf "%7.0f lines/second\n"
