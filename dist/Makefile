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
CMAKE_DIR=${SRC_DIR_BASE}/CMake
CMAKE_LIST=${SRC_DIR_BASE}/CMakeLists.txt
WINBIN_DIR=${BASE}-windows
DLL_DIR=../../../dlls

# executables
SRCML_BIN=srcml
SRCML_EXE=srcml.exe

SRCML_PATH=${BIN_DIR}/${SRCML_BIN}

WIN_SRCML_PATH=${BIN_DIR}/${SRCML_EXE}
LIBSRCML_BASE=${BIN_DIR}/libsrcml
LIBSRCML_SO=${LIBSRCML_BASE}.so
LIBSRCML_DYLIB=${LIBSRCML_BASE}.dylib
LIBSRCML_DLL=${LIBSRCML_BASE}.dll
LIBSRCML_UNIX=$(shell [[ $(shell uname) = 'Linux' ]] && echo ${LIBSRCML_SO}) $(shell [[ $(shell uname) = 'Darwin' ]] && echo ${LIBSRCML_DYLIB})


DOC_FILES=${DOC_DIR}/src2srcml.1.gz ${DOC_DIR}/srcml2src.1.gz ${DOC_DIR}/src2srcml.html ${DOC_DIR}/srcml2src.html
WIN_DOC_FILES=${DOC_DIR}/src2srcml.html ${DOC_DIR}/srcml2src.html
DIST_FILES= ${SRCML_PATH} ${LIBSRCML_UNIX} #${DOC_FILES}
WIN_DIST_FILES=${WIN_SRCML_PATH} ${LIBSRCML_DLL} #${WIN_DOC_FILES}

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
	tar czf $@ ${SRC_DIR} ${BIN_DIR} ${DOC_DIR} ${CMAKE_DIR} ${CMAKE_LIST}
	gunzip --test $@

${SRC_ZIP} : exp
	zip -orql $@ ${SRC_DIR} ${BIN_DIR} ${DOC_DIR} ${CMAKE_DIR} ${CMAKE_LIST}
	unzip -uq $@

##
# bin
bin : ${SRC_TAR_GZ}
	tar xzf ${SRC_TAR_GZ}
	cd ${SRC_DIR_BASE} && cmake . && make
	cd ${DOC_DIR} && make
	strip -x ${SRCML_PATH} ${LIBSRCML_UNIX}
	mkdir ${BASE} && ${CP} ${DIST_FILES} ${BASE}
	tar czf ${UNIX_TAR_GZ} ${BASE}

windows : ${SRC_ZIP}
	unzip -uq ${SRC_ZIP}
	cd ${SRC_DIR_BASE} && mingw32-cmake . && make
	cd ${SRC_DIR_BASE} && rm -f CMakeCache.txt && cmake . && make
	cd ${DOC_DIR} && make
	${STRIP} ${WIN_SRCML_PATH} ${LIBSRCML_DLL}
	mkdir ${WIN_BASE} && ${CP} ${WIN_DIST_FILES} ${WIN_BASE}
	unix2dos ${WIN_BASE}/*.html
	zip -oql ${WIN_ZIP} ${WIN_DIST_FILES}

windowsdyn : ${SRC_ZIP}
	unzip -uq  ${SRC_ZIP}
	cd ${SRC_DIR_BASE} && mingw32-cmake . && make
	cd ${SRC_DIR_BASE} && rm -rf CMakeCache.txt CMakeFiles cmake_install.cmake && cmake . && make
	cd ${DOC_DIR} && make clean && make
	${STRIP} ${WIN_SRCML_PATH} ${LIBSRCML_DLL}
	mkdir ${WIN_BASE} && ${CP} ${WIN_DIST_FILES} ${WIN_BASE}
	${CP} ../dlls/*.dll ${WIN_BASE}
	unix2dos ${WIN_BASE}/*.html
	zip -oql ${WIN_ZIP} ${WIN_DIST_FILES}

dll :
	echo ${DLL_DIR}
