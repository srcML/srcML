#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# direct input from a remote source, with various compressions
define srcml_with_url <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" url="test" filename="test"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

# compression only
srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.bz2?raw=true --url="test" --filename="test"
check 3<<< "$srcml_with_url"

srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.gz?raw=true --url="test" --filename="test"
check 3<<< "$srcml_with_url"

srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.bz2.gz?raw=true --url="test" --filename="test"
check 3<<< "$srcml_with_url"

srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.gz.bz2?raw=true --url="test" --filename="test"
check 3<<< "$srcml_with_url"


# empty direct remote source input with various compressions
define empty_srcml_with_url <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" url="test" filename="test"/>
	STDOUT

# compressed
srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.bz2?raw=true --url="test" --filename="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.gz?raw=true --url="test" --filename="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.bz2.gz?raw=true --url="test" --filename="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.gz.bz2?raw=true --url="test" --filename="test"
check 3<<< "$empty_srcml_with_url"
