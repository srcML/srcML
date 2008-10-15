##
# Makefile for srcML translator, src2srcml

EXE_DIR = ../bin/
OBJ_DIR = ../obj/

SRC2SRCML_BIN=../bin/src2srcml
SRCML2SRC_BIN=../bin/srcml2src

TIME=/usr/bin/time --output=/dev/stdout --format="%U"
TIMING_MED=timing_med.h
TIMING_JAVA=timing_java.java

PHONY : time
time : ../bin/src2srcml
	@echo "src2srcml:  "
	@cd .; ${TIME} ../bin/src2srcml ${TIMING_MED} ${TIMING_MED}.xml | sed 's/[0-9\.]*/ 10872 \/ &/' | bc -l
	@echo "srcml2src:  "
	@cd .; ${TIME} ../bin/srcml2src ${TIMING_MED}.xml ${TIMING_MED}v2 | sed 's/[0-9\.]*/10872 \/ &/' | bc -l

PHONY : javatime
javatime :
	@echo "src2srcml --language=Java:  "
	@cd .; ${TIME} ../bin/src2srcml --language Java ${TIMING_JAVA} ${TIMING_JAVA}.xml | sed 's/[0-9\.]*/32628 \/ &/' | bc -l
