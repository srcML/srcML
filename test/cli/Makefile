OPTION_SRC= ../../src/src2srcml.cpp ../../src/srcml2src.cpp ../../src/srcmlapps.h
option.py : ${OPTION_SRC}
	src2srcml ${OPTION_SRC} - | xsltproc extract_options.xsl - > option.py
clean :
	rm -f option.py

