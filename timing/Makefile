##
# Makefile for srcML translator, src2srcml

EXE_DIR = ../bin/
OBJ_DIR = ../obj/

SRC2SRCML_BIN=../bin/src2srcml
SRCML2SRC_BIN=../bin/srcml2src

TIMING_MED=timing_med.h
TIMING_JAVA=timing_java.java

PHONY : time
time : ../bin/src2srcml
	cd .; time ../bin/src2srcml ${TIMING_MED} ${TIMING_MED}.xml
	cd .; time ../bin/srcml2src ${TIMING_MED}.xml ${TIMING_MED}v2

PHONY : javatime
javatime :
	cd .; time ../bin/src2srcml --language Java ${TIMING_JAVA} ${TIMING_JAVA}.xml
