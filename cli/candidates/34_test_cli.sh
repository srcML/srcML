#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
# srcml2src

srcml2src sub/a.cpp.xml --output sub/a.cpp

check sub/a.cpp "$sfile"

srcml2src sub/a.cpp.xml --output=sub/a.cpp

check sub/a.cpp "$sfile"

srcml2src sub/a.cpp.xml -o sub/a.cpp

check sub/a.cpp "$sfile"

srcml2src '-' -o sub/a.cpp <<< "$sxmlfile"

check sub/a.cpp "$sfile"

srcml2src -o sub/a.cpp <<< "$sxmlfile"

check sub/a.cpp "$sfile"


# non-windows
if platform.system() != "Windows" :
	srcml2src '-' '--output' '/dev/stdout' sxmlfile sfile)
	srcml2src '-' '--output=/dev/stdout' sxmlfile sfile)
	srcml2src '-' -o '/dev/stdout' sxmlfile sfile)

