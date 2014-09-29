#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

exit 1

# test
srcml_new = f.read()
f.close()

if False :
	srcml2src --revision '1' srcdiff src_old)
	srcml2src --revision '1' 'diff/diff.cpp.xml' '' src_old)
	srcml2src --revision '2' srcdiff src_new)
	srcml2src --revision '2' 'diff/diff.cpp.xml' '' src_new)
	
	srcml2src --revision '1' -o 'diff/old.cpp' srcdiff '')
	readfile input diff/old.cpp' 'r').read() src_old)
	srcml2src --revision '1' 'diff/diff.cpp.xml' -o 'diff/old.cpp' '' '')
	readfile input diff/old.cpp' 'r').read() src_old)
	srcml2src --revision '2' -o 'diff/new.cpp' srcdiff '')
	readfile input diff/new.cpp' 'r').read() src_new)
	srcml2src --revision '2' 'diff/diff.cpp.xml' -o 'diff/new.cpp' '' '')
	readfile input diff/new.cpp' 'r').read() src_new)

	srcml2src --xml --revision '1' 'diff/diff.cpp.xml' '' srcml_old)
	srcml2src --xml --revision '2' 'diff/diff.cpp.xml' '' srcml_new)

	srcml2src --xml --revision '1' -o 'diff/old.cpp.xml' srcdiff '')
	readfile input diff/old.cpp.xml' 'r').read() srcml_old)
	srcml2src --xml --revision '1' 'diff/diff.cpp.xml' -o 'diff/old.cpp.xml' '' '')
	readfile input diff/old.cpp.xml' 'r').read() srcml_old)
	srcml2src --xml --revision '2' -o 'diff/new.cpp.xml' srcdiff '')
	readfile input diff/new.cpp.xml' 'r').read() srcml_new)
	srcml2src --xml --revision '2' 'diff/diff.cpp.xml' -o 'diff/new.cpp.xml' '' '')
	readfile input diff/new.cpp.xml' 'r').read() srcml_new)

# cpp option

sfile1 = ""


f = open('sub/a.java' 'w')
f.write(sfile1)
f.close()

