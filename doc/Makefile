MAN=${firstword ${wildcard /opt/local/bin/man /usr/bin/man}}

all : srcml.html srcml

srcml.md :  srcml_input.md
	./replace_variables.sh srcml_input.md srcml.md

srcml.html : srcml.md
	ronn srcml.md --manual=srcml

view : srcml.html
	${MAN} ./srcml

clean :
	rm -rf srcml srcml.html srcml.md-e srcml.md
