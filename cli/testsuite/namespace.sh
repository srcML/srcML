#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++"/>
	STDOUT

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/a.cpp"/>
	STDOUT

createfile sub/a.cpp ""

# separate
src2srcml --xmlns="http://www.sdml.info/srcML/src" sub/a.cpp

check 3<<< "$foutput"

echo -n "" | src2srcml -l C++ --xmlns="http://www.sdml.info/srcML/src" -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$output"

src2srcml --xmlns="http://www.sdml.info/srcML/src" sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$foutput"

src2srcml --xmlns:cpp="http://www.sdml.info/srcML/cpp" sub/a.cpp

check 3<<< "$foutput"

echo -n "" | src2srcml -l C++ --xmlns:cpp="http://www.sdml.info/srcML/cpp" -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$output"

src2srcml --xmlns:cpp="http://www.sdml.info/srcML/cpp" sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$foutput"

# multiple
echo -n "" | src2srcml --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" sub/a.cpp

check 3<<< "$foutput"

echo -n "" | src2srcml -l C++ --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$output"

src2srcml --xmlns="http://www.sdml.info/srcML/src" --xmlns:cpp="http://www.sdml.info/srcML/cpp" sub/a.cpp -o sub/a.cpp.xml

check sub/a.cpp.xml 3<<< "$foutput"

