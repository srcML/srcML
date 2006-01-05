MAN_PAGES= src2srcml.1 srcml2src.1
MAN_GZIP= src2srcml.1.gz srcml2src.1.gz

all : man

man : ${MAN_PAGES}

src2srcml.1 : src2srcml.xml
	xmlto --skip-validation man src2srcml.xml

srcml2src.1 : srcml2src.xml
	xmlto --skip-validation man srcml2src.xml

view : man
	man -l src2srcml.1

src2srcml.1.gz : src2srcml.1
	gzip src2srcml.1

srcml2src.1.gz : srcml2src.1
	gzip srcml2src.1

gzip : man
	gzip src2srcml.1
	gzip srcml2src.1

install : ${MAN_GZIP}
	cp src2srcml.1.gz srcml2src.1.gz /usr/share/man/man1/.

clean :
	rm -f ${MAN_PAGES} ${MAN_GZIP}

