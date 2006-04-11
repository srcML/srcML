##
# Makefile for building srcML distributions
#
# Michael L. Collard
# collard@cs.kent.edu

# specific project data
include Makefile.data

# base of file name
BASE=srcml-${VERSION_NUM}

# generated files
SRC_TAR=${BASE}-src.tar
SRC_TAR_GZ=${SRC_TAR}.gz
SRC_ZIP=${BASE}-src.zip

DOC_TAR_GZ=${BASE}-doc.tar.gz
DOC_ZIP=${BASE}-doc.zip

EXE_TAR_GZ=${BASE}-linux.tar.gz

WIN_ZIP=${BASE}-windows.zip

# directories
SRC_DIR=${BASE}/src
BIN_DIR=${BASE}/bin
DTD_DIR=${BASE}/dtd
DOC_DIR=${BASE}/doc

# executables
SRC2SRCML_EXE=src2srcml
SRCML2SRC_EXE=srcml2src

SRC2SRCML_PATH=${BASE}/bin/${SRC2SRCML_EXE}
SRCML2SRC_PATH=${BASE}/bin/${SRCML2SRC_EXE}

WIN_SRC2SRCML_PATH=
WIN_SRCML2SRC_PATH=

# files
DTD_FILES= ${DTD_DIR}/srcml.dtd ${DTD_DIR}/srcmlj.dtd
DOC_FILES= ${DOC_DIR}/src2srcml.1 ${DOC_DIR}/srcml2src.1 ${SRC_DIR}/NEWS ${SRC_DIR}/CHANGES ${DOC_DIR}/src2srcml.html ${DOC_DIR}/srcml2src.html
SRC_FILES= ${SRC_DIR} ${DOC_FILES} ${DTD_FILES}

# some doc files are copied from src
EXP_DOC_FILES= ${DOC_DIR}/LICENSE.pdf ${DOC_DIR}/README

# all generated files
GEN_FILES=${SRC_TAR_GZ} ${DOC_TAR_GZ} ${EXE_TAR_GZ} ${SRC_ZIP} ${DOC_ZIP}

# generate files
all : ${GEN_FILES}

# file extraction
PHONY : export
export :
	make clean
	mkdir ${BASE}
	svn export ${SVN_URL}/src ${BASE}/src
	svn export ${SVN_URL}/dtd ${BASE}/dtd
	svn export ${SVN_URL}/doc ${BASE}/doc
	mkdir ${BASE}/obj ${BASE}/bin

##
# src

# src tar gz
${SRC_TAR_GZ} : export
	rm -f $@
	tar czf $@ ${SRC_FILES}
	gunzip --test $@

# src zip file for Windows with Windows line ending
${SRC_ZIP} : export
	@echo BUILDING:  $@
	rm -f $@
	zip -rql $@ ${SRC_FILES} -x ${SRC_DIR}/LICENSE.pdf
	zip -q $@ ${SRC_DIR}/LICENSE.pdf
	unzip -t $@

##
# doc

# doc tar gz
${DOC_TAR_GZ} : export doc_files
	rm -f $@
	tar czf $@ ${EXP_DOC_FILES}
	gunzip --test $@

# doc zip file for Windows with Windows line ending
${DOC_ZIP} : export doc_files
	rm -f $@
	zip -rql $@ ${EXP_DOC_FILES} -x ${DOC_DIR}/LICENSE.pdf
	zip -q $@ ${DOC_DIR}/LICENSE.pdf
	unzip -t $@

# files not directly available in doc dir
doc_files :
	cp ${SRC_DIR}/LICENSE.pdf ${DOC_DIR}/LICENSE.pdf
	cp ${SRC_DIR}/README ${DOC_DIR}/README

##
# binaries

# linux
${EXE_TAR_GZ} : export
	cd ${SRC_DIR}; make
	./${SRC2SRCML_PATH} --version
	./${SRCML2SRC_PATH} --version
	gzip ${SRC2SRCML_PATH} ${SRCML2SRC_PATH}
	rm -f $@
	tar czf $@ ${SRC2SRCML_PATH}.gz ${SRCML2SRC_PATH}.gz
	gunzip --test $@

# windows (must be called explicitly)
${WIN_ZIP} :
	zip -rql $@ ${SRC2SRCML_PATH}.gz ${SRCML2SRC_PATH}.gz
	unzip -t $@

##
# test

test :
	gunzip --test ${SRC_TAR_GZ}
	gunzip --test ${DOC_TAR_GZ}
	gunzip --test ${EXE_TAR_GZ}
	unzip -t ${SRC_ZIP}
	unzip -t ${DOC_ZIP}

##
# installation

# install on server
install :
	scp ${GEN_FILES} ${DEST_URL}

installwin :
	scp ${WIN_ZIP} ${DEST_URL}

clean :
	rm -fR ${GEN_FILES} ${WIN_ZIP} ${BASE}

