VERSION=beta-aug-29-2005
VERSION_DIR=tags
SRC=srcML
SRC_TAR=${SRC}.tar
SRC_TAR_GZ=${SRC_TAR}.gz
SRC_ZIP=${SRC}.zip

SRC_DIR=src
DTD_DIR=dtd
DOC_DIR=doc

all :
	rm -fR src obj bin doc dtd
	svn export http://www.sdml.info/svn/srcML/${VERSION_DIR}/${VERSION}/${SRC_DIR}
	svn export http://www.sdml.info/svn/srcML/${VERSION_DIR}/${VERSION}/${DTD_DIR}
	svn export http://www.sdml.info/svn/srcML/${VERSION_DIR}/${VERSION}/${DOC_DIR}
	mkdir obj bin
	cd src; make; cd ..;
	rm -f ${SRC_TAR_GZ}; tar czvf ${SRC_TAR_GZ} ${SRC_DIR} ${DOC_DIR} ${DTD_DIR}
	rm -f ${SRC_ZIP}; zip -r ${SRC_ZIP}  ${SRC_DIR} ${DOC_DIR} ${DTD_DIR}
	gzip bin/src2srcml
	gzip bin/srcml2src


