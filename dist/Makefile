##
# Makefile for building srcML distributions
#

# tools
ECHO=${wildcard /opt/local/bin/gecho /bin/echo}
SED=${firstword ${wilcard /opt/local/bin/gsed /bin/sed}}
GREP=${firstword ${wildcard /bin/grep /usr/bin/grep}}
CUT=/usr/bin/cut
TR=/usr/bin/tr

# svn repo
SVN_URL=http://calder.sdml.cs.kent.edu/svn/srcML/trunk

# file name base
BASE=srcML

# generated compressed files
SRC_TAR=${BASE}-src.tar
SRC_TAR_GZ=${SRC_TAR}.gz
SRC_ZIP=${BASE}-src.zip

EXE_TAR_GZ=${BASE}-linux.tar.gz
WIN_ZIP=${BASE}-windows.zip

# directories
SRC_DIR_BASE=${BASE}
SRC_DIR=${SRC_DIR_BASE}/src
OBJ_DIR=${SRC_DIR_BASE}/obj
BIN_DIR=${SRC_DIR_BASE}/bin
DOC_DIR=${SRC_DIR_BASE}/doc
WINBIN_DIR=${BASE}-windows

# executables
SRC2SRCML_BIN=src2srcml
SRCML2SRC_BIN=srcml2src
SRC2SRCML_EXE=src2srcml.exe
SRCML2SRC_EXE=srcml2src.exe

SRC2SRCML_PATH=${BIN_DIR}/${SRC2SRCML_BIN}
SRCML2SRC_PATH=${BIN_DIR}/${SRCML2SRC_BIN}

WIN_SRC2SRCML_PATH=${BIN_DIR}/${SRC2SRCML_EXE}
WIN_SRCML2SRC_PATH=${BIN_DIR}/${SRCML2SRC_EXE}

# files
SRC_FILES= ${SRC_DIR} 

# all generated files
GEN_FILES=srctar srczip

# generate files
all : src

REVISION=$(strip $(shell svn info ${SVN_URL}/src | ${GREP} 'Revision:' | ${CUT} -d: -f2-))

# export the directories
exp :
	echo ${REVISION}
	make clean
	mkdir ${BASE}
	svn export -r${REVISION} ${SVN_URL}/src/ ${BASE}/src
	${ECHO} -n "Trunk${REVISION}" > ${BASE}/src/VERSION
	svn info -r${REVISION} ${SVN_URL}/src | ${GREP} 'Last Changed Date:' | ${CUT} -d: -f2- | ${TR} -d '\n' >> ${BASE}/src/VERSION
	svn export -r${REVISION} --depth='empty' ${SVN_URL}/obj/ ${BASE}/obj
	svn export -r${REVISION} --depth='empty' ${SVN_URL}/bin/ ${BASE}/bin
	svn export -r${REVISION} ${SVN_URL}/doc/ ${BASE}/doc

clean :
	rm -fR  ${SRC_DIR_BASE} ${SRC_TAR_GZ} ${SRC_ZIP}

##
# src

src : ${SRC_TAR_GZ} ${SRC_ZIP}

${SRC_TAR_GZ} : exp
	tar czf $@ ${SRC_FILES} ${OBJ_DIR} ${BIN_DIR}
	gunzip --test $@

${SRC_ZIP} : exp
	zip -rql $@ ${SRC_FILES} ${OBJ_DIR} ${BIN_DIR}
	unzip -qt $@

##
# bin
bin : src
	rm -fR ${SRC_DIR_BASE}
	tar xzf ${SRC_TAR_GZ}
	cd ${SRC_DIR} && make