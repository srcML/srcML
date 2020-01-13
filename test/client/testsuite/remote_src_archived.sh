#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# direct input from a remote source, with various compressions
define srcml_with_url <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="test">

	<unit revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

# archive/compression mix
srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp.cpio --url="test"
check "$srcml_with_url"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp.cpio.bz2 --url="test"
check "$srcml_with_url"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp.cpio.gz --url="test"
check "$srcml_with_url"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp.tar.bz2 --url="test"
check "$srcml_with_url"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp.tar.gz --url="test"
check "$srcml_with_url"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp.tar --url="test"
check "$srcml_with_url"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp.tbz2 --url="test"
check "$srcml_with_url"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp.tgz --url="test"
check "$srcml_with_url"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp.zip --url="test"
check "$srcml_with_url"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp.zip.bz2 --url="test"
check "$srcml_with_url"

srcml https://raw.githubusercontent.com/srcML/test-data/1.0.0/source/a.cpp.zip.gz --url="test"
check "$srcml_with_url"

# empty direct remote source input with various compressions
define empty_srcml_with_url <<- 'STDOUT'
       <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
       <unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" url="test" filename="test"/>
       STDOUT

# archive/compressed mix
srcml https://github.com/srcML/test-data/raw/master/empty/empty.cpp.cpio --url="test"
check "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/empty/empty.cpp.cpio.bz2?raw=true --url="test"
check "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/empty/empty.cpp.cpio.gz?raw=true --url="test"
check "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/empty/empty.cpp.tar.bz2?raw=true --url="test"
check "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/empty/empty.cpp.tar.gz?raw=true --url="test"
check "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/raw/master/empty/empty.cpp.tar --url="test"
check "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/empty/empty.cpp.tbz2?raw=true --url="test"
check "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/empty/empty.cpp.tgz?raw=true --url="test"
check "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/empty/empty.cpp.zip?raw=true --url="test"
check "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/empty/empty.cpp.zip.bz2?raw=true --url="test"
check "$empty_srcml_with_url"

srcml https://github.com/srcML/test-data/blob/master/empty/empty.cpp.zip.gz?raw=true --url="test"
check "$empty_srcml_with_url"
