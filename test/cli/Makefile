OPTION_SRC= ../../src/src2srcml.cpp ../../src/srcml2src.cpp ../../src/srcmlapps.h
STATUS_SRC= ../../src/srcmlapps.h

all : option.py status.py

option.py : ${OPTION_SRC}
	src2srcml ${OPTION_SRC} - | xsltproc extract_options.xsl - > option.py

status.py : ${OPTION_SRC}
	src2srcml --extended ${STATUS_SRC} | xsltproc extract_return_status.xsl - > status.py

clean :
	rm -f option.py
	rm -f status.py
