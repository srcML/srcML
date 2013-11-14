##
# Makefile for building srcML distributions
#

# tools
ECHO=${firstword ${wildcard /opt/local/bin/gecho /usr/local/bin/gecho /bin/echo}}
SED=${firstword ${wilcard /opt/local/bin/gsed /usr/local/bin/gecho /bin/sed}}
GREP=${firstword ${wildcard /bin/grep /usr/bin/grep}}
CUT=/usr/bin/cut
TR=/usr/bin/tr
CP=${firstword ${wildcard /opt/local/bin/gcp /usr/local/bin/gcp /bin/cp}}
STRIP=${firstword ${wildcard /usr/bin/i686-pc-mingw32-strip /usr/bin/i686-w64-mingw32-strip}}

# git repo
GIT_URL=git@github.com:srcML/srcML.git

# file name base
BASE=srcML
WIN_BASE=${BASE}-Win

# generated compressed files
SRC_TAR=${BASE}-src.tar
SRC_TAR_GZ=${SRC_TAR}.gz
SRC_ZIP=${BASE}-src.zip

UNIX_TAR_GZ=${BASE}.tar.gz
WIN_ZIP=${BASE}-Win.zip

# directories
SRC_DIR_BASE=${BASE}-src
SRC_DIR=${SRC_DIR_BASE}/src
BIN_DIR=${SRC_DIR_BASE}/bin
DOC_DIR=${SRC_DIR_BASE}/doc
WINBIN_DIR=${BASE}-windows
DLL_DIR=../../../dlls

# executables
SRC2SRCML_BIN=src2srcml
SRCML2SRC_BIN=srcml2src
SRC2SRCML_EXE=src2srcml.exe
SRCML2SRC_EXE=srcml2src.exe

SRC2SRCML_PATH=${BIN_DIR}/${SRC2SRCML_BIN}
SRCML2SRC_PATH=${BIN_DIR}/${SRCML2SRC_BIN}

WIN_SRC2SRCML_PATH=${BIN_DIR}/${SRC2SRCML_EXE}
WIN_SRCML2SRC_PATH=${BIN_DIR}/${SRCML2SRC_EXE}
LIBSRCML_BASE=${BIN_DIR}/libsrcml
LIBSRCML_SO=${LIBSRCML_BASE}.so
LIBSRCML_DYLIB=${LIBSRCML_BASE}.dylib
LIBSRCML_DLL=${LIBSRCML_BASE}.dll

DOC_FILES=${DOC_DIR}/src2srcml.1.gz ${DOC_DIR}/srcml2src.1.gz ${DOC_DIR}/src2srcml.html ${DOC_DIR}/srcml2src.html
WIN_DOC_FILES=${DOC_DIR}/src2srcml.html ${DOC_DIR}/srcml2src.html
DIST_FILES=${DOC_FILES} ${SRC2SRCML_PATH} ${SRCML2SRC_PATH}
WIN_DIST_FILES=${WIN_DOC_FILES} ${WIN_SRC2SRCML_PATH} ${WIN_SRCML2SRC_PATH}

# generate files
all : src bin

GITREVISION=$(strip $(shell git log -n 1 --remotes --format="%H"))

# export the directories
exp :
	@echo ${GITREVISION}
	make cleansrc
	mkdir ${SRC_DIR_BASE}
	cd ..; git archive master CMakeLists.txt CMake src doc | tar xv -C dist/${SRC_DIR_BASE}
	git log -n 1 --remotes --format="Trunk ${GITREVISION} %cd" > ${SRC_DIR_BASE}/src/VERSION
	${ECHO} "${GITREVISION}" > ${SRC_DIR_BASE}/src/REVISION
	mkdir ${SRC_DIR_BASE}/bin

clean :
	make cleansrc
	rm -fR  ${SRC_TAR_GZ} ${UNIX_TAR_GZ} ${SRC_ZIP} ${WIN_ZIP}

cleansrc :
	rm -fR ${SRC_DIR_BASE} ${BASE} ${WIN_BASE}

##
# src

src : ${SRC_TAR_GZ} ${SRC_ZIP}

${SRC_TAR_GZ} : exp
	tar czf $@ ${SRC_DIR} ${BIN_DIR} ${DOC_DIR}
	gunzip --test $@

${SRC_ZIP} : exp
	zip -orql $@ ${SRC_DIR} ${BIN_DIR} ${DOC_DIR}
	unzip -uq $@

##
# bin
bin : ${SRC_TAR_GZ}
	tar xzf ${SRC_TAR_GZ}
	cd ${SRC_DIR_BASE} && cmake . && make
	cd ${DOC_DIR} && make
	strip ${SRC2SRCML_PATH} ${SRCML2SRC_PATH}
	mkdir ${BASE} && ${CP} ${DIST_FILES} ${BASE}
#	( test -e ${LIBSRCML_SO} && cp ${LIBSRCML_SO} ${BASE} ) || ( test -e ${LIBSRCML_DYLIB} && cp ${LIBSRCML_DYLIB} ${BASE} )
	tar czf ${UNIX_TAR_GZ} ${BASE}

windows : ${SRC_ZIP}
	unzip -uq ${SRC_ZIP}
	cd ${SRC_DIR} && mingw32-cmake . && make
	cd ${SRC_DIR} && rm -f CMakeCache.txt && cmake . && make
	cd ${DOC_DIR} && make
	i686-pc-mingw32-strip ${WIN_SRC2SRCML_PATH} ${WIN_SRCML2SRC_PATH}
	mkdir ${WIN_BASE} && ${CP} ${WIN_DIST_FILES} ${WIN_BASE}
	unix2dos ${WIN_BASE}/*.html
	zip -oql ${WIN_ZIP} ${WIN_BASE}/src2srcml.exe ${WIN_BASE}/srcml2src.exe ${WIN_BASE}/*.dll ${WIN_BASE}/src2srcml.html ${WIN_BASE}/srcml2src.html

windowsdyn : ${SRC_ZIP}
	unzip -uq  ${SRC_ZIP}
	cd ${SRC_DIR} && mingw32-cmake . && make
	cd ${SRC_DIR} && rm -rf CMakeCache.txt CMakeFiles cmake_install.cmake && cmake . && make
	cd ${DOC_DIR} && make clean && make
	${STRIP} ${WIN_SRC2SRCML_PATH} ${WIN_SRCML2SRC_PATH} ${LIBSRCML_DLL}
	mkdir ${WIN_BASE} && ${CP} ${WIN_DIST_FILES} ${WIN_BASE}
	${CP} ../dlls/*.dll ${WIN_BASE}
	${CP} ${BIN_DIR}/libsrcml.dll ${WIN_BASE}
	unix2dos ${WIN_BASE}/*.html
	zip -oql ${WIN_ZIP} ${WIN_BASE}/src2srcml.exe ${WIN_BASE}/srcml2src.exe ${WIN_BASE}/*.dll ${WIN_BASE}/src2srcml.html ${WIN_BASE}/srcml2src.html

dll :
	echo ${DLL_DIR}
