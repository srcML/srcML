#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="raw.githubusercontent.com/hmm34/massive-lana/master/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="0.9.5">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="0.9.5" language="C++" filename="raw.githubusercontent.com/hmm34/massive-lana/master/a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

xmlcheck "$output"


# input from a valid remote source file
srcml https://raw.githubusercontent.com/hmm34/massive-lana/master/a.cpp
check 3<<< "$output"

srcml https://raw.githubusercontent.com/hmm34/massive-lana/master/a.cpp -o remote.xml
check remote.xml 3<<< "$output"

srcml https://raw.githubusercontent.com/hmm34/massive-lana/master/a.cpp --archive
check 3<<< "$archive_output"

srcml https://raw.githubusercontent.com/hmm34/massive-lana/master/a.cpp	--archive -o remote_archive.xml
check remote_archive.xml 3<<< "$archive_output"


# files from contains a remote source file
createfile list.txt "https://raw.githubusercontent.com/hmm34/massive-lana/master/a.cpp"

srcml --files-from list.txt
check 3<<< "$archive_output"

srcml --files-from list.txt -o remote_from_list.xml
check remote_from_list.xml 3<<< "$archive_output"


# direct input from a remote source, with various compressions
define empty_srcml_with_url <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" url="test" filename="test"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
	STDOUT

# compression only
srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.bz2?raw=true --url="test" --filename="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.gz?raw=true --url="test" --filename="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.bz2.gz?raw=true --url="test" --filename="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.gz.bz2?raw=true --url="test" --filename="test"
check 3<<< "$empty_srcml_with_url"

# archive/compression mix
srcml https://github.com/hmm34/massive-lana/raw/master/a.cpp.cpio --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.cpio.bz2?raw=true --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.cpio.gz?raw=true --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.tar.bz2?raw=true --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.tar.gz?raw=true --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/hmm34/massive-lana/raw/master/a.cpp.tar --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.tbz2?raw=true --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.tgz?raw=true --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.zip?raw=true --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.zip.bz2?raw=true --url="test"
check 3<<< "$empty_srcml_with_url"

srcml https://github.com/hmm34/massive-lana/blob/master/a.cpp.zip.gz?raw=true --url="test"
check 3<<< "$empty_srcml_with_url"
