VERSION_NUM=1.3
VERSION=tirawireless
VERSION_DIR=commercial

DEST_DIR=/var/www/sdml/projects/srcml/commercial/tirawireless${VERSION_NUM}/.
DEST_URL=collard\@gehry.sdml.cs.kent.edu:${DEST_DIR}

SVN_URL=http://www.sdml.info/svn/srcML/branches/${VERSION_DIR}/${VERSION}

BASE=srcml-${VERSION_NUM}
FULL_BASE=/home/collard/svnsrcml/srcML/branches/commercial/tirawireless/dist/${BASE}

SRC=${BASE}-
SRC_TAR=${SRC}src.tar


# generated files
SRC_TAR_GZ=${SRC_TAR}.gz
SRC_ZIP=${SRC}src.zip

DOC_TAR_GZ=${BASE}-doc.tar.gz
DOC_ZIP=${BASE}-doc.zip

EXE_TAR_GZ=${BASE}-linux.tar.gz

# files
SRC_DIR=${BASE}/src
BIN_DIR=${BASE}/bin
DTD_DIR=${BASE}/dtd
DOC_DIR=${BASE}/doc

SRC2SRCML_EXE=src2srcml
SRCML2SRC_EXE=srcml2src

SRC2SRCML_PATH=${BASE}/bin/${SRC2SRCML_EXE}
SRCML2SRC_PATH=${BASE}/bin/${SRCML2SRC_EXE}

DTD_FILES= ${DTD_DIR}/srcml.dtd ${DTD_DIR}/srcmlj.dtd
DOC_FILES= ${DOC_DIR}/src2srcml.1 ${DOC_DIR}/srcml2src.1 ${SRC_DIR}/NEWS ${SRC_DIR}/CHANGES ${DOC_DIR}/src2srcml.html ${DOC_DIR}/srcml2src.html
SRC_FILES= ${SRC_DIR} ${DOC_FILES} ${DTD_FILES}

EXP_DOC_FILES= ${DOC_DIR}/LICENSE.pdf ${DOC_DIR}/README

# result
GEN_FILES=${SRC_TAR_GZ} ${DOC_TAR_GZ} ${EXE_TAR_GZ} ${SRC_ZIP} ${DOC_ZIP}

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
	rm -f ${SRC_TAR_GZ}
	tar czvf ${SRC_TAR_GZ} ${SRC_FILES}

# src zip file for Windows with Windows line ending
${SRC_ZIP} : export
	rm -f ${SRC_ZIP}
	zip -rl ${SRC_ZIP} ${SRC_FILES} -x ${SRC_DIR}/LICENSE.pdf
	zip ${SRC_ZIP} ${SRC_DIR}/LICENSE.pdf

##
# doc

# doc tar gz
${DOC_TAR_GZ} : export doc_files
	rm -f ${DOC_TAR_GZ}
	tar czvf ${DOC_TAR_GZ} ${EXP_DOC_FILES}

# doc zip file for Windows with Windows line ending
${DOC_ZIP} : export doc_files
	rm -f ${DOC_ZIP}
	zip -rl ${DOC_ZIP} ${EXP_DOC_FILES} -x ${DOC_DIR}/LICENSE.pdf
	zip ${DOC_ZIP} ${DOC_DIR}/LICENSE.pdf

# files not directly available in doc dir
doc_files :
	cp ${SRC_DIR}/LICENSE.pdf ${DOC_DIR}/LICENSE.pdf
	cp ${SRC_DIR}/README ${DOC_DIR}/README

##
# linux

# linux binaries
${EXE_TAR_GZ} : export
	cd ${SRC_DIR}; make
	gzip ${SRC2SRCML_PATH}
	gzip ${SRCML2SRC_PATH}
	rm -f ${EXE_TAR_GZ}; tar czvf ${EXE_TAR_GZ} ${SRC2SRCML_PATH}.gz ${SRCML2SRC_PATH}.gz

##
# installation

# install on server
install :
	scp ${GEN_FILES} ${DEST_URL}

clean :
	rm -fR ${GEN_FILES} ${BASE}

