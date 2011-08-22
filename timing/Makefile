##
# Makefile for srcML translator, src2srcml

EXE_DIR = ../bin/
OBJ_DIR = ../obj/

SRC2SRCML_BIN=../bin/src2srcml
SRCML2SRC_BIN=../bin/srcml2src

TIME=${firstword ${wildcard /opt/local/bin/gtime time}} --format="%U"

TIMING_MED=timing_med.h
TIMING_JAVA=timing_java.java

PRINTF_FORMAT="\%1.0f lines/second\\n"

SED=${firstword ${wildcard /opt/local/bin/gsed sed}}
ECHO=${firstword ${wildcard /opt/local/bin/gecho echo}}
FLAGS=-ne

PHONY : time
time : ../bin/src2srcml
	@${SRC2SRCML_BIN} --version
	@${SRC2SRCML_BIN} --version
	@${ECHO}

	echo $(TIME)
	@${ECHO} ${FLAGS} "${SRC2SRCML_BIN} ${TIMING_MED} -o ${TIMING_MED}.xml:"
	@cd .; ${TIME} ${SRC2SRCML_BIN} ${TIMING_MED} -o ${TIMING_MED}.xml 2>&1 | ${SED} 's/[0-9\.]*/ 10872 \/ &/' | bc -l |  xargs printf "%7.0f lines/second\n"
	@${ECHO} ${FLAGS} "${SRCML2SRC_BIN} ${TIMING_MED}.xml -o ${TIMING_MED}vs:"
	@cd .; ${TIME} ${SRCML2SRC_BIN} ${TIMING_MED}.xml -o ${TIMING_MED}v2 2>&1 | ${SED} 's/[0-9\.]*/10872 \/ &/' | bc -l  |  xargs printf "%7.0f lines/second\n"

PHONY : javatime
javatime :
	@${ECHO} ${FLAGS} "${SRC2SRCML_BIN} ${TIMING_JAVA} -o ${TIMING_JAVA}.xml: "
	@cd .; ${TIME} ${SRC2SRCML_BIN} --language Java ${TIMING_JAVA} -o ${TIMING_JAVA}.xml 2>&1 | ${SED} 's/[0-9\.]*/32628 \/ &/' | bc -l | xargs printf "%7.0f lines/second\n"

	@${ECHO} ${FLAGS} "${SRCML2SRC_BIN} ${TIMING_JAVA}.xml -o ${TIMING_JAVA}v2: "
	@cd .; ${TIME} ${SRCML2SRC_BIN} ${TIMING_JAVA}.xml -o ${TIMING_JAVA}v2 2>&1 | ${SED} 's/[0-9\.]*/32628 \/ \(&\ + 0.001)/' | bc -l | xargs printf "%7.0f lines/second\n"

clean:
	rm *.*v2 *.xml
