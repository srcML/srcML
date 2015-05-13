#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test get language

# C++ 
define srcmlcpp <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" directory="bar" filename="foo" version="1.2"/>
	STDOUT

createfile sub/a.cpp.xml "$srcmlcpp"

srcml --show-language sub/a.cpp.xml
check 3<<< "C++"

srcml --show-language < sub/a.cpp.xml
check 3<<< "C++"

# Java
define srcmljava <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Java">
	</unit>
	STDOUT

createfile sub/a.java.xml "$srcmljava"

srcml --show-language sub/a.java.xml
check 3<<< "Java"

srcml --show-language < sub/a.java.xml
check 3<<< "Java"

# C
define srcmlc <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C">
	</unit>
	STDOUT

createfile sub/a.c.xml "$srcmlc"

srcml --show-language sub/a.c.xml
check 3<<< "C"

srcml --show-language < sub/a.c.xml
check 3<<< "C"

# Objective-C
define srcmlobjc <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Objective-C">
	</unit>
	STDOUT

createfile sub/a.m.xml "$srcmlobjc"

srcml --show-language sub/a.m.xml
check 3<<< "Objective-C"

srcml --show-language < sub/a.m.xml
check 3<<< "Objective-C"

# Aspect J
define srcmlaj <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="AspectJ">
	</unit>
	STDOUT

createfile sub/a.aj.xml "$srcmlaj"

srcml --show-language sub/a.aj.xml
check 3<<< "AspectJ"

srcml --show-language < sub/a.aj.xml
check 3<<< "AspectJ"

# Empty
define empty <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="" directory="" filename="" version=""/>
	STDIN

createfile sub/a.cpp.xml "$empty"

srcml --show-language sub/a.cpp.xml
check 3<<< ""

srcml --show-language < sub/a.cpp.xml
check 3<<< ""

