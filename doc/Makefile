all : man

man : src2srcml.1

src2srcml.1 : src2srcml.xml
	xmlto --skip-validation man src2srcml.xml

view : man
	man -l src2srcml.1

install : man
	gzip src2srcml.1
	cp src2srcml.1.gz /usr/share/man/man1/.

clean :
	rm -f foo.1
