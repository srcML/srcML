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

all : testbase
	./runtest.py

gen : testbase

PHONY : testbase
testbase :
	cd testsuite && make -f Makefile.gen
	cd ..

PHONY : short
short : testbase
	@export SRC2SRCML=${SRC2SRCML}; export SRCML2SRC=${SRCML2SRC}; time ./runtest.py ".*\.all|macro"

PHONY : shortc
shortc : testbase
	@export SRC2SRCML=${SRC2SRCML}; export SRCML2SRC=${SRCML2SRC}; time ./runtest.py ".*\.all|macro" C

PHONY : shortcpp
shortcpp : testbase
	@export SRC2SRCML=${SRC2SRCML}; export SRCML2SRC=${SRCML2SRC}; time ./runtest.py ".*\.all|macro" C++

PHONY : shortjava
shortjava : testbase
	@export SRC2SRCML=${SRC2SRCML}; export SRCML2SRC=${SRCML2SRC}; time ./runtest.py ".*\.all" Java

clean :
	cd testsuite && make clean
	cd ..
	rm -f srcMLTestReport*