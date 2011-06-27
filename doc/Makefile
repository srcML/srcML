MAN_PAGES= src2srcml.1 srcml2src.1
MAN_GZIP= src2srcml.1.gz srcml2src.1.gz
SRC2SRCML=../bin/src2srcml
SRCML2SRC=../bin/srcml2src

DOCBOOK2XMAN=${firstword ${wildcard /usr/bin/docbook2x-man /opt/local/bin/docbook2man}}
MAN2HTML=${firstword ${wildcard /usr/bin/man2html /opt/local/bin/man2html}}
MAN=${firstword ${wildcard /opt/local/bin/man /usr/bin/man2html}}

ECHO=/bin/echo

all : man gzip

man : ${MAN_PAGES} src2srcml.html srcml2src.html

src2srcml.1 : src2srcml.xml option.dtd option.dtd
	${DOCBOOK2XMAN} src2srcml.xml

srcml2src.1 : srcml2src.xml option.dtd option.dtd
	${DOCBOOK2XMAN} --string-param quotes-on-literals=1 srcml2src.xml

view : man
	man ./src2srcml.1
	man ./srcml2src.1

src2srcml.1.gz : src2srcml.1
	gzip -f src2srcml.1

srcml2src.1.gz : srcml2src.1
	gzip -f srcml2src.1

src2srcml.html : src2srcml.1
	${MAN2HTML} < $< > $@.tmp
	tail -n +3 $@.tmp > $@

srcml2src.html : srcml2src.1
	${MAN2HTML} < $< > $@.tmp
	tail -n +3 $@.tmp > $@

OPTION_SRC= ../src/src2srcml.cpp ../src/srcml2src.cpp ../src/srcmlapps.hpp ../src/srcmlns.hpp
option.dtd : ${OPTION_SRC}
	${ECHO} -n "<!ENTITY DATE \"" > option.dtd
	date | perl -p -e "s|\n||" >> option.dtd
	${ECHO} "\" >" >> option.dtd
	${SRC2SRCML} ${OPTION_SRC} | ${SRCML2SRC} --xslt=extract_options.xsl >> option.dtd
	${SRC2SRCML} --literal ${OPTION_SRC} | ${SRCML2SRC} --xslt=extract_return_status.xsl - >> option.dtd

gzip : src2srcml.1.gz srcml2src.1.gz

install : ${MAN_GZIP}
	cp src2srcml.1.gz srcml2src.1.gz /usr/share/man/man1/.

clean :
	rm -f ${MAN_PAGES} ${MAN_GZIP} option.dtd status.dtd


