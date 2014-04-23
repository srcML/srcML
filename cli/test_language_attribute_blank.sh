#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++">
	</unit>
	STDOUT

# test case
srcml -l C++ <<< ""

check 3<<< "$output"

# test case
srcml --language C++ <<< ""

exit

check 3<<< "$output"

# test case
srcml --language=C++ <<< ""

check 3<<< "$output"

# test case
srcml --language="C++" <<< ""

check 3<<< "$output"

# test case
srcml --language='C++' <<< ""

check 3<<< "$output"

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C">
	</unit>
	STDOUT

# test case
srcml --language='C' <<< ""

check 3<<< "$output"

# test case
srcml --language='C' <<< ""

check 3<<< "$output"

# test case
srcml --language='C' <<< ""

check 3<<< "$output"

# test case

srcml -l Java <<< ""

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" language="Java">
	</unit>
	STDOUT

srcml -l C# <<< ""

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C#">
	</unit>
	STDOUT
