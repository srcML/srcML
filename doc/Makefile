MAN_PAGES= src2srcml.1 srcml2src.1
MAN_GZIP= src2srcml.1.gz srcml2src.1.gz

all : man gzip

man : ${MAN_PAGES} src2srcml.html srcml2src.html

src2srcml.1 : src2srcml.xml option.dtd option.dtd
	docbook2x-man src2srcml.xml

srcml2src.1 : srcml2src.xml option.dtd option.dtd
	docbook2x-man srcml2src.xml

view : man
	man -l src2srcml.1

src2srcml.1.gz : src2srcml.1
	gzip -f src2srcml.1

srcml2src.1.gz : srcml2src.1
	gzip -f srcml2src.1

src2srcml.html : src2srcml.1
	man2html src2srcml.1 > src2srcml.html

srcml2src.html : srcml2src.1
	man2html srcml2src.1 > srcml2src.html

OPTION_SRC= ../src/src2srcml.cpp ../src/srcml2src.cpp ../src/srcmlapps.h
option.dtd : ${OPTION_SRC}
	echo -n "<!ENTITY DATE \"" > option.dtd
	date >> option.dtd
	echo -n "\" >" >> option.dtd
	src2srcml ${OPTION_SRC} - | xsltproc extract_options.xsl - >> option.dtd
	src2srcml --extended ${OPTION_SRC} - | xsltproc extract_return_status.xsl - >> option.dtd

gzip : src2srcml.1.gz srcml2src.1.gz

install : ${MAN_GZIP}
	#cp src2srcml.1.gz srcml2src.1.gz /usr/share/man/man1/.

clean :
	rm -f ${MAN_PAGES} ${MAN_GZIP} option.dtd status.dtd


