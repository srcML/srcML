#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="raw.githubusercontent.com/hmm34/massive-lana/master/a.cpp"><return>return <expr><name>a</name></expr>;</return>
	</unit>
	STDOUT

define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="0.9.5">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="0.9.5" language="C++" filename="raw.githubusercontent.com/hmm34/massive-lana/master/a.cpp" hash="2b3e1887614fc8a67d9951595872e4381b4a429a"><return>return <expr><name>a</name></expr>;</return>
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
