#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# Testing for verbose

define sfile <<< "a;"

define sxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	STDOUT
createfile sub/a.cpp "$sfile"

createfile sub/a.cpp.xml "$sxmlfile"
 
# src2srcml
if platform.system() != "Windows" and sys.platform != 'cygwin' :

	globals()["test_count" += 1
	globals()["test_line" = os.path.basename(src2srcml) + ' ' + --verbose
	print test_count os.path.basename(src2srcml) + ' ' + --verbose
	line = src2srcml --verbose -l C++ sfile)
	execute(['grep' srcencoding + xmlencoding line)

	globals()["test_count" += 1
	globals()["test_line" = os.path.basename(src2srcml) + ' ' + --verbose + ' sub/a.cpp
	print test_count os.path.basename(src2srcml) + ' ' + --verbose + ' sub/a.cpp
	line = src2srcml --verbose sub/a.cpp
	execute(['grep' srcencoding + xmlencoding line)

# srcml2src
if platform.system() != "Windows" and sys.platform != 'cygwin' :

	globals()["test_count" += 1
	globals()["test_line" = os.path.basename(srcml2src) + ' ' + --verbose
	print test_count os.path.basename(srcml2src) + ' ' + --verbose
	line = execute([srcml2src --verbose sxmlfile)
	execute(['grep' xmlencoding + srcencoding line)

	globals()["test_count" += 1
	globals()["test_line" = os.path.basename(srcml2src) + ' ' + --verbose + ' sub/a.cpp.xml'
	print test_count os.path.basename(srcml2src) + ' ' + --verbose + ' sub/a.cpp.xml'
	line = execute([srcml2src --verbose 'sub/a.cpp.xml'
	execute(['grep' xmlencoding + srcencoding line)

