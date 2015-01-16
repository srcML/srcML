#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test get language

# C++ 
define srcmlcpp <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" dir="bar" filename="foo" version="1.2"/>
	STDOUT

createfile sub/a.cpp.xml "$srcmlcpp"

srcml --get-language sub/a.cpp.xml
check 3<<< "C++"

srcml --get-language < sub/a.cpp.xml
check 3<<< "C++"

# Java
define srcmljava <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0" language="Java">
	</unit>
	STDOUT

createfile sub/a.java.xml "$srcmljava"

srcml --get-language sub/a.java.xml
check 3<<< "Java"

srcml --get-language < sub/a.java.xml
check 3<<< "Java"

# C
define srcmlc <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0" language="C">
	</unit>
	STDOUT

createfile sub/a.c.xml "$srcmlc"

srcml --get-language sub/a.c.xml
check 3<<< "C"

srcml --get-language < sub/a.c.xml
check 3<<< "C"

# Objective-C
define srcmlobjc <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0" language="Objective-C">
	</unit>
	STDOUT

createfile sub/a.m.xml "$srcmlobjc"

srcml --get-language sub/a.m.xml
check 3<<< "Objective-C"

srcml --get-language < sub/a.m.xml
check 3<<< "Objective-C"

# Aspect J
define srcmlaj <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0" language="AspectJ">
	</unit>
	STDOUT

createfile sub/a.aj.xml "$srcmlaj"

srcml --get-language sub/a.aj.xml
check 3<<< "AspectJ"

srcml --get-language < sub/a.aj.xml
check 3<<< "AspectJ"

# Empty
define empty <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="" dir="" filename="" version=""/>
	STDIN

createfile sub/a.cpp.xml "$empty"

srcml --get-language sub/a.cpp.xml
check 3<<< ""

srcml --get-language < sub/a.cpp.xml
check 3<<< ""

# None
define none <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp"/>
	STDIN

createfile sub/a.cpp.xml "$none"

srcml --get-language sub/a.cpp.xml
check_null

srcml --get-language < sub/a.cpp.xml
check_null

