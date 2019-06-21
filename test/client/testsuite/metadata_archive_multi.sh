#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test metadata options with xml and unit
rm -f sub/a.cpp sub/b.cpp

define nestedfileextra <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit revision="REVISION" language="Java" filename="b.cpp" hash="aecf18b52d520ab280119febd8ff6c803135ddfc"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDIN

define file1 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

xmlcheck "$nestedfileextra"
xmlcheck "$file1"

srcml -X --unit "1" - <<< "$nestedfileextra"
check "$file1"

srcml -X --unit "1" <<< "$nestedfileextra"
check "$file1"

srcml -X --unit "1" --show-filename <<< "$nestedfileextra"
check "a.cpp\n"

srcml -X --unit "2" --show-filename <<< "$nestedfileextra"
check "b.cpp\n"

srcml -X --unit "1" --show-language <<< "$nestedfileextra"
check "C\n"

srcml -X --unit "2" --show-language <<< "$nestedfileextra"
check "Java\n"

define input <<- 'INPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:lc="http://www.srcML.org/srcML/linecol" revision="REVISION" language="C++" url="sub">

	<unit revision="REVISION" language="C" filename="a.cpp" mytag="foo"><expr_stmt lc:line="1"><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit revision="REVISION" language="Java" filename="b.cpp" mytag="foo"><expr_stmt lc:line="1"><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	<unit revision="REVISION" language="C++" filename="c.cpp" mytag="foo"><expr_stmt lc:line="1"><expr><name>c</name></expr>;</expr_stmt>
	</unit>

	</unit>
	INPUT

define file1 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:lc="http://www.srcML.org/srcML/linecol" revision="REVISION" language="C" filename="a.cpp" mytag="foo"><expr_stmt lc:line="1"><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define file2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:lc="http://www.srcML.org/srcML/linecol" revision="REVISION" language="Java" filename="b.cpp" mytag="foo"><expr_stmt lc:line="1"><expr><name>b</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define file3 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:lc="http://www.srcML.org/srcML/linecol" revision="REVISION" language="C++" filename="c.cpp" mytag="foo"><expr_stmt lc:line="1"><expr><name>c</name></expr>;</expr_stmt>
	</unit>
	STDOUT

mkdir sub
createfile sub/input.xml "$input"

# unit always only gets namespaces for src and cpp by default
srcml --show-url <<< "$input"
check "sub\n"

srcml -X --unit "1" - <<< "$input"
check "$file1"

srcml -X --unit "1" <<< "$input"
check "$file1"

srcml -X --unit "2" - <<< "$input"
check "$file2"

srcml -X --unit "2" <<< "$input"
check "$file2"

srcml -X --unit "3" - <<< "$input"
check "$file3"

srcml -X --unit "3" <<< "$input"
check "$file3"

srcml -X --unit "1" --show-filename <<< "$input"
check "a.cpp\n"

srcml -X --unit "2" --show-filename <<< "$input"
check "b.cpp\n"

srcml -X --unit "1" --show-language <<< "$input"
check "C\n"

srcml -X --unit "2" --show-language <<< "$input"
check "Java\n"

define nestedfileextra <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="sub">

	<unit revision="REVISION" language="C++" filename="a.cpp">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit revision="REVISION" language="Java" filename="empty.java">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/a.cpp.xml "$nestedfileextra"

srcml -X --unit "1" --show-language sub/a.cpp.xml
check "C++\n"

srcml -X --unit "1" --show-filename sub/a.cpp.xml
check "a.cpp\n"

srcml -X --unit "2" --show-language sub/a.cpp.xml
check "Java\n"

srcml -X --unit "2" --show-filename sub/a.cpp.xml
check "empty.java\n"

srcml --show-url sub/a.cpp.xml
check "sub\n"
