#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
srcml += srcmlend

if platform.system() == "Windows" or sys.platform == 'cygwin' :
	srcml = string.replace(srcml "dir/" "dir\\")

echo -n "" | src2srcml dir
src2srcml dir' -o 'dir/dir.xml' ""
readfile input dir/dir.xml' 'r').read() srcml)

if platform.system() != "Windows" and sys.platform != 'cygwin' :
	execute(['tar' 'czf' 'dir/foo.tar' 'dir/file.c'

echo -n "" |	 src2srcml dir
	src2srcml dir' -o 'dir/dir.xml' ""
	readfile input dir/dir.xml' 'r').read() srcml)

	execute(['rm' 'dir/foo.tar'

#
# nested files

src = STDOUT
a;
	STDOUT

