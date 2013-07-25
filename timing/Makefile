##
# Makefile for srcML translator, src2srcml

EXE_DIR = ../bin/
OBJ_DIR = ../obj/

SRC2SRCML_BIN=../bin/src2srcml
SRCML2SRC_BIN=../bin/srcml2src

TIME=${firstword ${wildcard /usr/local/bin/gtime /opt/local/bin/gtime /usr/bin/time}} --format="%U"

TIMING_MED=timing_med.h
TIMING_JAVA=timing_java.java
TIMING_CSHARP=timing_csharp.cs

PRINTF_FORMAT="\%1.0f lines/second\\n"

SED=${firstword ${wildcard /usr/local/bin/gsed /opt/local/bin/gsed /bin/sed /usr/bin/sed}}
ECHO=${firstword ${wildcard /usr/local/bin/gecho /opt/local/bin/gecho /bin/echo}}

PHONY : time
time : ../bin/src2srcml
	@${SRC2SRCML_BIN} --version
	@${ECHO}
	@${SRCML2SRC_BIN} --version
	@${ECHO}
	@${ECHO} -ne "C:\t"
	@cd .; ${TIME} ${SRC2SRCML_BIN} ${TIMING_MED} -o ${TIMING_MED}.xml 2>&1 | ${SED} 's/[0-9\.]*/ 10872 \/ &/' | bc -l |  xargs printf "%'9.0f LOC/sec\n"
	@${ECHO} -ne "C++:\t"
	@cd .; ${TIME} ${SRC2SRCML_BIN} --language C++ ${TIMING_MED} -o ${TIMING_MED}.xml 2>&1 | ${SED} 's/[0-9\.]*/ 10872 \/ &/' | bc -l |  xargs printf "%'9.0f LOC/sec\n"
	@${ECHO} -ne "C#:\t"
	@cd .; ${TIME} ${SRC2SRCML_BIN} ${TIMING_CSHARP} -o ${TIMING_CSHARP}.xml 2>&1 | ${SED} 's/[0-9\.]*/ 24576 \/ &/' | bc -l |  xargs printf "%'9.0f LOC/sec\n"
	@${ECHO} -ne "Java:\t"
	@cd .; ${TIME} ${SRC2SRCML_BIN} --language Java ${TIMING_JAVA} -o ${TIMING_JAVA}.xml 2>&1 | ${SED} 's/[0-9\.]*/32628 \/ &/' | bc -l | xargs printf "%'9.0f LOC/sec\n"
	@${ECHO} -ne "SRC:\t"
	@cd .; ${TIME} ${SRCML2SRC_BIN} ${TIMING_MED}.xml -o ${TIMING_MED}v2 2>&1 | ${SED} 's/[0-9\.]*/10872 \/ &/' | bc -l  |  xargs printf "%'9.0f LOC/sec\n"

clean:
	rm *.*v2 *.xml
