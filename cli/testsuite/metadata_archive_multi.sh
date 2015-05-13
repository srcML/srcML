#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test metadata options with xml and unit
rm -f sub/a.cpp sub/b.cpp

define nestedfileextra <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="Java" filename="b.cpp" hash="aecf18b52d520ab280119febd8ff6c803135ddfc"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDIN

define file1 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

srcml2src -X --unit "1" - <<< "$nestedfileextra"
check 3<<< "$file1"

srcml2src -X --unit "1" <<< "$nestedfileextra"
check 3<<< "$file1"

srcml -X --unit "1" --show-filename <<< "$nestedfileextra"
check 3<<< "a.cpp"

srcml -X --unit "2" --show-filename <<< "$nestedfileextra"
check 3<<< "b.cpp"

srcml -X --unit "1" --show-language <<< "$nestedfileextra"
check 3<<< "C"
	
srcml -X --unit "2" --show-language <<< "$nestedfileextra"
check 3<<< "Java"


define input <<- 'INPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:lc="http://www.srcML.org/srcML/linecol" revision="REVISION" language="C++" url="sub">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C" filename="a.cpp" mytag="foo"><expr_stmt lc:line="1"><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="Java" filename="b.cpp" mytag="foo"><expr_stmt lc:line="1"><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="c.cpp" mytag="foo"><expr_stmt lc:line="1"><expr><name>c</name></expr>;</expr_stmt>
	</unit>

	</unit>
	INPUT

define file1 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:lc="http://www.srcML.org/srcML/linecol" revision="REVISION" language="C" directory="sub" filename="a.cpp" mytag="foo"><expr_stmt lc:line="1"><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define file2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:lc="http://www.srcML.org/srcML/linecol" revision="REVISION" language="Java" directory="sub" filename="b.cpp" mytag="foo"><expr_stmt lc:line="1"><expr><name>b</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define file3 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" xmlns:lc="http://www.srcML.org/srcML/linecol" revision="REVISION" language="C++" directory="sub" filename="c.cpp" mytag="foo"><expr_stmt lc:line="1"><expr><name>c</name></expr>;</expr_stmt>
	</unit>
	STDOUT

srcml2src -X --show-url - <<< "$input"
check 3<<< "sub"

srcml2src -X --unit "1" - <<< "$input"
check 3<<< "$file1"

srcml2src -X --unit "1" <<< "$input"
check 3<<< "$file1"

srcml2src -X --unit "2" - <<< "$input"
check 3<<< "$file2"

srcml2src -X --unit "2" <<< "$input"
check 3<<< "$file2"

srcml2src -X --unit "3" - <<< "$input"
check 3<<< "$file3"

srcml2src -X --unit "3" <<< "$input"
check 3<<< "$file3"

srcml -X --unit "1" --show-filename <<< "$input"
check 3<<< "a.cpp"

srcml -X --unit "2" --show-filename <<< "$input"
check 3<<< "b.cpp"

srcml -X --unit "1" --show-language <<< "$input"
check 3<<< "C"

srcml -X --unit "2" --show-language <<< "$input"
check 3<<< "Java"


define nestedfileextra <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="sub">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="Java" filename="empty.java">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile sub/a.cpp.xml "$nestedfileextra"

srcml -X --unit "1" --show-language sub/a.cpp.xml
check 3<<< "C++"

srcml -X --unit "1" --show-filename sub/a.cpp.xml
check 3<<< "a.cpp"

srcml -X --unit "2" --show-language sub/a.cpp.xml
check 3<<< "Java"

srcml -X --unit "2" --show-filename sub/a.cpp.xml
check 3<<< "empty.java"

srcml -X --show-url sub/a.cpp.xml
check 3<<< "sub"
