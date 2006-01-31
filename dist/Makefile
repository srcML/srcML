VERSION=tirawireless
VERSION_DIR=branches/commercial

DEST_DIR=/var/www/sdml/projects/srcml/commercial/tirawireless1.2/.

SRC=srcML
SRC_TAR=${SRC}.tar
SRC_TAR_GZ=${SRC_TAR}.gz
SRC_ZIP=${SRC}.zip

SRC_DIR=src
DTD_DIR=dtd
DOC_DIR=doc

SRC2SRCML_EXE=src2srcml
SRCML2SRC_EXE=srcml2src

SRC2SRCML_PATH=bin/${SRC2SRCML_EXE}
SRCML2SRC_PATH=bin/${SRCML2SRC_EXE}

DTD_FILES= ${DTD_DIR}/srcml.dtd ${DTD_DIR}/srcmlj.dtd
DOC_FILES= ${DOC_DIR}/src2srcml.1 ${DOC_DIR}/srcml2src.1
SRC_FILES= ${SRC_DIR} ${DOC_FILES} ${DTD_FILES}

all :
	rm -fR src obj bin doc dtd
	svn export http://www.sdml.info/svn/srcML/${VERSION_DIR}/${VERSION}/${SRC_DIR}
	svn export http://www.sdml.info/svn/srcML/${VERSION_DIR}/${VERSION}/${DTD_DIR}
	svn export http://www.sdml.info/svn/srcML/${VERSION_DIR}/${VERSION}/${DOC_DIR}
	mkdir obj bin
	rm -f ${SRC_TAR_GZ}; tar czvf ${SRC_TAR_GZ} ${SRC_FILES}
	rm -f ${SRC_ZIP}; zip -r ${SRC_ZIP} ${SRC_FILES}
	cd src; make; cd ..;
	gzip ${SRC2SRCML_PATH}
	gzip ${SRCML2SRC_PATH}


install :
	scp ${SRC_ZIP} ${SRC_TAR_GZ} ${SRC2SRCML_PATH}.gz ${SRCML2SRC_PATH}.gz ${DTD_FILES} ${DOC_FILES} collard\@gehry.sdml.cs.kent.edu:${DEST_DIR}
