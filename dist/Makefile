##
# Makefile for building srcML distributions
#
# Michael L. Collard
# collard@cs.kent.edu

# specific project data
VERSION_NUM=11.09.2006
VERSION=beta-Nov-09-2006
VERSION_DIR=commercial

DEST_DIR=/var/www/sdml/projects/srcml/commercial/tirawireless${VERSION_NUM}/.
DEST_URL=collard@gehry.sdml.cs.kent.edu:${DEST_DIR}

SVN_URL=http://www.sdml.info/svn/srcML/tags/${VERSION}

# base of file name
BASE=srcml-${VERSION_NUM}

# generated files
SRC_TAR=${BASE}-src.tar
SRC_TAR_GZ=${SRC_TAR}.gz
SRC_ZIP=${BASE}-src.zip

DOC_TAR_GZ=${BASE}-doc.tar.gz
DOC_ZIP=${BASE}-doc.zip

EXE_TAR_GZ=${BASE}-linux.tar.gz

EXE_STATIC_TAR_GZ=${BASE}-linux-static.tar.gz

WIN_ZIP=${BASE}-windows.zip

# directories
SRC_DIR_BASE=${BASE}-src
SRC_DIR=${SRC_DIR_BASE}/src
BIN_DIR=${BASE}-bin
SRC_DTD_DIR=${SRC_DIR_BASE}/dtd
DTD_DIR=${BASE}-dtd
SRC_DOC_DIR=${SRC_DIR_BASE}/doc
DOC_DIR=${BASE}-doc
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
DTD_FILES= ${DTD_DIR}/srcml.dtd ${DTD_DIR}/srcmlj.dtd

DOC_FILES= ${DOC_DIR}/src2srcml.1 ${DOC_DIR}/srcml2src.1 ${DOC_DIR}/src2srcml.html ${DOC_DIR}/srcml2src.html

SRC_FILES= ${SRC_DIR} ${SRC_DTD_DIR}/srcml.dtd ${SRC_DTD_DIR}/srcmlj.dtd ${SRC_DOC_DIR}/src2srcml.1 ${SRC_DOC_DIR}/srcml2src.1 ${SRC_DOC_DIR}/src2srcml.html ${SRC_DOC_DIR}/srcml2src.html

# some doc files are copied from src
EXP_DOC_FILES= ${DOC_FILES} ${DOC_DIR}/README ${DOC_DIR}/NEWS ${DOC_DIR}/CHANGES

# all generated files
GEN_FILES=${SRC_TAR_GZ} ${DOC_TAR_GZ} ${EXE_TAR_GZ} ${EXE_STATIC_TAR_GZ} ${SRC_ZIP} ${DOC_ZIP}

# generate files
all : ${GEN_FILES}

# file extraction
PHONY : export
export :
	make clean
	mkdir ${BASE}-src
	svn export ${SVN_URL}/src ${BASE}-src/src
	svn export ${SVN_URL}/dtd ${BASE}-src/dtd
	svn export ${SVN_URL}/doc ${BASE}-src/doc
	svn export ${SVN_URL}/dtd ${BASE}-dtd
	svn export ${SVN_URL}/doc ${BASE}-doc
	mkdir ${BASE}-src/obj ${BASE}-src/bin

##
# src

# src tar gz
src : srctar srczip

srctar : ${SRC_TAR_GZ}

${SRC_TAR_GZ} : export
	rm -f $@
	tar czf $@ ${SRC_FILES}
	gunzip --test $@

# src zip file for Windows with Windows line ending
srczip : ${SRC_ZIP}

#zip -rql $@ ${SRC_FILES} -x ${SRC_DIR}/LICENSE.pdf
#zip -q $@ ${SRC_DIR}/LICENSE.pdf
${SRC_ZIP} : export
	@echo BUILDING:  $@
	rm -f $@
	zip -rql $@ ${SRC_FILES}
	unzip -qt $@

##
# doc

# doc tar gz
doc : doctar doczip

doctar : ${DOC_TAR_GZ}

${DOC_TAR_GZ} : export doc_files
	rm -f $@
	tar czf $@ ${EXP_DOC_FILES}
	gunzip --test $@

# doc zip file for Windows with Windows line ending
doczip : ${DOC_ZIP}

#zip -rql $@ ${EXP_DOC_FILES} -x ${DOC_DIR}/LICENSE.pdf
#zip -q $@ ${DOC_DIR}/LICENSE.pdf
${DOC_ZIP} : export doc_files
	rm -f $@
	zip -rql $@ ${EXP_DOC_FILES}
	unzip -qt $@

# files not directly available in doc dir
#cp ${SRC_DIR}/LICENSE.pdf ${DOC_DIR}/.
doc_files :
	cp ${SRC_DIR}/README ${DOC_DIR}/.
	cp ${SRC_DIR}/NEWS ${DOC_DIR}/.
	cp ${SRC_DIR}/CHANGES ${DOC_DIR}/.

##
# binaries

# linux
bin : ${EXE_TAR_GZ}

${EXE_TAR_GZ} : export
	cd ${SRC_DIR}; make
	mkdir -p ${BIN_DIR}
	cp ${SRC_DIR_BASE}/bin/src2srcml ${BIN_DIR}/.
	cp ${SRC_DIR_BASE}/bin/srcml2src ${BIN_DIR}/.
	${SRC2SRCML_PATH} --version
	${SRCML2SRC_PATH} --version
	rm -f $@
	tar czf $@ ${BIN_DIR}
	gunzip --test $@

${EXE_STATIC_TAR_GZ} : export
	cd ${SRC_DIR}; mkdir ../lib; make STATIC=true
	mkdir -p ${BIN_DIR}
	cp ${SRC_DIR_BASE}/bin/src2srcml ${BIN_DIR}/.
	cp ${SRC_DIR_BASE}/bin/srcml2src ${BIN_DIR}/.
	${SRC2SRCML_PATH} --version
	${SRCML2SRC_PATH} --version
	rm -f $@
	tar czf $@ ${BIN_DIR}
	gunzip --test $@

bintest :
	@rm -fR tmp
	@mkdir tmp
	@cp ${EXE_TAR_GZ} tmp/.
	@cd tmp; tar xzf ${EXE_TAR_GZ}
	${SRC2SRCML_PATH} --version
	${SRCML2SRC_PATH} --version
	@echo "Hello World;" | ${SRC2SRCML_PATH} | ${SRCML2SRC_PATH}

# windows (must be called explicitly)
winbin : ${WIN_ZIP}

${WIN_ZIP} :
	rm -fR ${WINBIN_DIR}
	mkdir -p ${WINBIN_DIR}
	cp ${SRC2SRCML} ${SRCML2SRC} ${WINBIN_DIR}
	zip -rql $@ ${WINBIN_DIR}
	unzip -qt $@

##
# test

test :
	gunzip --test ${SRC_TAR_GZ}
	gunzip --test ${DOC_TAR_GZ}
	gunzip --test ${EXE_TAR_GZ}
	unzip -qt ${SRC_ZIP}
	unzip -qt ${DOC_ZIP}

##
# installation

# install on server
install :
	scp ${GEN_FILES} ${DEST_URL}

installwin :
	scp ${WIN_ZIP} ${DEST_URL}

clean :
	rm -fR ${GEN_FILES} ${WIN_ZIP} ${SRC_DIR_BASE} ${BIN_DIR} ${DTD_DIR} ${DOC_DIR}
