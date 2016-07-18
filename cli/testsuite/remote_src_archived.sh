#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# direct input from a remote source, with various compressions
define srcml_with_url <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" url="test" filename="test"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT


# archive/compression mix
srcml https://github.com/srcML/test-data/raw/master/a.cpp.cpio --url="test"
check 3<<< "$srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/a.cpp.cpio.bz2?raw=true --url="test"
check 3<<< "$srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/a.cpp.cpio.gz?raw=true --url="test"
check 3<<< "$srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/a.cpp.tar.bz2?raw=true --url="test"
check 3<<< "$srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/a.cpp.tar.gz?raw=true --url="test"
check 3<<< "$srcml_with_url"

srcml https://github.com/srcML/test-data/raw/master/a.cpp.tar --url="test"
check 3<<< "$srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/a.cpp.tbz2?raw=true --url="test"
check 3<<< "$srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/a.cpp.tgz?raw=true --url="test"
check 3<<< "$srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/a.cpp.zip?raw=true --url="test"
check 3<<< "$srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/a.cpp.zip.bz2?raw=true --url="test"
check 3<<< "$srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/a.cpp.zip.gz?raw=true --url="test"
check 3<<< "$srcml_with_url"


# empty direct remote source input with various compressions
define empty_srcml_with_url <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" url="test" filename="test"/>
	STDOUT

# archive/compressed mix
srcml https://github.com/srcML/test-data/raw/master/a.cpp.cpio --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/a.cpp.cpio.bz2?raw=true --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/a.cpp.cpio.gz?raw=true --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/a.cpp.tar.bz2?raw=true --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/a.cpp.tar.gz?raw=true --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/raw/master/a.cpp.tar --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/a.cpp.tbz2?raw=true --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/a.cpp.tgz?raw=true --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/a.cpp.zip?raw=true --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/a.cpp.zip.bz2?raw=true --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/a.cpp.zip.gz?raw=true --url="test"
check 3<<< "$empty_srcml_with_url"
