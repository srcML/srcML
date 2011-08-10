##
# Makefile for building srcML distributions
#

# tools
ECHO=${firstword ${wildcard /opt/local/bin/gecho /bin/echo}}
SED=${firstword ${wilcard /opt/local/bin/gsed /bin/sed}}
GREP=${firstword ${wildcard /bin/grep /usr/bin/grep}}
CUT=/usr/bin/cut
TR=/usr/bin/tr
CP=${firstword ${wildcard /opt/local/bin/gcp /bin/cp}}

# svn repo
SVN_URL=http://calder.sdml.cs.kent.edu/svn/srcML/trunk

# file name base
BASE=srcML
WIN_BASE=${BASE}win

# generated compressed files
SRC_TAR=${BASE}-src.tar
SRC_TAR_GZ=${SRC_TAR}.gz
SRC_ZIP=${BASE}-src.zip

UNIX_TAR_GZ=${BASE}.tar.gz
WIN_ZIP=${BASE}win.zip

# directories
SRC_DIR_BASE=${BASE}-src
SRC_DIR=${SRC_DIR_BASE}/src
OBJ_DIR=${SRC_DIR_BASE}/obj
BIN_DIR=${SRC_DIR_BASE}/bin
DOC_DIR=${SRC_DIR_BASE}/doc
WINBIN_DIR=${BASE}-windows
DLL_DIR=../../dlls

# executables
SRC2SRCML_BIN=src2srcml
SRCML2SRC_BIN=srcml2src
SRC2SRCML_EXE=src2srcml.exe
SRCML2SRC_EXE=srcml2src.exe

SRC2SRCML_PATH=${BIN_DIR}/${SRC2SRCML_BIN}
SRCML2SRC_PATH=${BIN_DIR}/${SRCML2SRC_BIN}

WIN_SRC2SRCML_PATH=${BIN_DIR}/${SRC2SRCML_EXE}
WIN_SRCML2SRC_PATH=${BIN_DIR}/${SRCML2SRC_EXE}
LIBSRCML_DLL=${BIN_DIR}/libsrcml.dll

DOC_FILES=${DOC_DIR}/src2srcml.1.gz ${DOC_DIR}/srcml2src.1.gz ${DOC_DIR}/src2srcml.html ${DOC_DIR}/srcml2src.html
WIN_DOC_FILES=${DOC_DIR}/src2srcml.html ${DOC_DIR}/srcml2src.html
DIST_FILES=${DOC_FILES} ${SRC2SRCML_PATH} ${SRCML2SRC_PATH}
WIN_DIST_FILES=${WIN_DOC_FILES} ${WIN_SRC2SRCML_PATH} ${WIN_SRCML2SRC_PATH}

# generate files
all : src bin

REVISION=$(strip $(shell svn info ${SVN_URL}/src | ${GREP} 'Revision:' | ${CUT} -d: -f2-))

# export the directories
exp :
	@echo ${REVISION}
	make cleansrc
	mkdir ${SRC_DIR_BASE}
	svn export -r${REVISION} ${SVN_URL}/src/ ${SRC_DIR_BASE}/src
	${ECHO} -n "Trunk ${REVISION}" > ${SRC_DIR_BASE}/src/VERSION
	svn info -r${REVISION} ${SVN_URL}/src | ${GREP} 'Last Changed Date:' | ${CUT} -d: -f2- | ${TR} -d '\n' >> ${SRC_DIR_BASE}/src/VERSION
	svn export -r${REVISION} --depth='empty' ${SVN_URL}/obj/ ${SRC_DIR_BASE}/obj --native-eol LF
	svn export -r${REVISION} --depth='empty' ${SVN_URL}/bin/ ${SRC_DIR_BASE}/bin --native-eol LF
	svn export -r${REVISION} ${SVN_URL}/doc/ ${SRC_DIR_BASE}/doc --native-eol LF

clean :
	make cleansrc
	rm -fR  ${SRC_TAR_GZ} ${UNIX_TAR_GZ} ${SRC_ZIP} ${WIN_ZIP}

cleansrc :
	rm -fR ${SRC_DIR_BASE} ${BASE} ${WIN_BASE}

##
# src

src : ${SRC_TAR_GZ} ${SRC_ZIP}

${SRC_TAR_GZ} : exp
	tar czf $@ ${SRC_DIR} ${OBJ_DIR} ${BIN_DIR} ${DOC_DIR}
	gunzip --test $@

${SRC_ZIP} : exp
	zip -rql $@ ${SRC_DIR} ${OBJ_DIR} ${BIN_DIR} ${DOC_DIR}
	unzip -qt $@

##
# bin
bin : ${SRC_TAR_GZ}
	make cleansrc
	tar xzf ${SRC_TAR_GZ}
	cd ${SRC_DIR} && make
	cd ${DOC_DIR} && make
	strip ${SRC2SRCML_PATH} ${SRCML2SRC_PATH}
	mkdir ${BASE} && ${CP} -t ${BASE} ${DIST_FILES}
	tar czf ${UNIX_TAR_GZ} ${BASE}

windows : ${SRC_ZIP}
	make cleansrc
	unzip -aq ${SRC_ZIP}
	cd ${SRC_DIR} && make mingw32
	cd ${SRC_DIR} && make cleanbuild && make
	cd ${DOC_DIR} && make
	i686-pc-mingw32-strip ${WIN_SRC2SRCML_PATH} ${WIN_SRCML2SRC_PATH}
	mkdir ${WIN_BASE} && ${CP} -t ${WIN_BASE} ${WIN_DIST_FILES}
	unix2dos ${WIN_BASE}/*.html
	zip -ql ${WIN_ZIP} ${WIN_BASE}/src2srcml.exe ${WIN_BASE}/srcml2src.exe ${WIN_BASE}/*.dll ${WIN_BASE}/src2srcml.html ${WIN_BASE}/srcml2src.html

windowsdyn : ${SRC_ZIP}
	make cleansrc
	unzip -aq ${SRC_ZIP}
	cd ${SRC_DIR} && make mingw32 DYNAMIC=1 DLL_DIR=${DLL_DIR}
	cd ${SRC_DIR} && make cleanbuild && make
	cd ${DOC_DIR} && make clean && make
	i686-pc-mingw32-strip ${WIN_SRC2SRCML_PATH} ${WIN_SRCML2SRC_PATH} ${LIBSRCML_DLL}
	mkdir ${WIN_BASE} && ${CP} -t ${WIN_BASE} ${WIN_DIST_FILES}
	cp ../dlls/*.dll ${WIN_BASE}
	cp ${BIN_DIR}/libsrcml.dll ${WIN_BASE}
	unix2dos ${WIN_BASE}/*.html
	zip -ql ${WIN_ZIP} ${WIN_BASE}/src2srcml.exe ${WIN_BASE}/srcml2src.exe ${WIN_BASE}/*.dll ${WIN_BASE}/src2srcml.html ${WIN_BASE}/srcml2src.html

dll :
	echo ${DLL_DIR}
