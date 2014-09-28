#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test compression tool

define sfile <<< "a;"

define sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	STDOUT

define xmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename=sub/a.cpp>
	</unit>
	STDOUT

# # src2srcml
# if platform.system() != "Windows" :

#	 f = open(sub/a.cpp 'w')
#	 f.write(sfile)
#	 f.close()
#	 src2srcml -n sub/a.cpp -o 'sub/a.cpp.xml.gz' ""
#	 'gunzip' '-c' 'sub/a.cpp.xml.gz' "" fxmlfile)
#	 src2srcml --archive sub/a.cpp -o 'sub/a.cpp.xml.gz' ""
#	 'gunzip' '-c' 'sub/a.cpp.xml.gz' "" fxmlfil)
#	 if sys.platform != 'cygwin' :
#	 src2srcml -l C++ -n -o 'sub/a.cpp.xml.gz' sfile
#	 'gunzip' '-c' 'sub/a.cpp.xml.gz' "" sxmlfile)


# # srcml2src

# if platform.system() != "Windows" :

#	 f = open(sub/a.cpp.xml 'w')
#	 f.write(fxmlfile)
#	 f.close()

#	 srcml2src -n sub/a.cpp.xml -o 'sub/a.cpp.gz' ""
#	 'gunzip' '-c' 'sub/a.cpp.gz' <<< "a")
#	 srcml2src --archive sub/a.cpp.xml -o 'sub/a.cpp.gz' ""

#	 'gunzip' '-c' 'sub/a.cpp.gz' <<< "a")
#	 if sys.platform != 'cygwin' :

#	 srcml2src -n -o 'sub/a.cpp.gz' fxmlfile
#	 'gunzip' '-c' 'sub/a.cpp.gz' <<< "a")

