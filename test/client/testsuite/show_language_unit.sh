#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test get language

# C++
define srcmlcpp <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" directory="bar" filename="foo" version="1.2"/>
  STDOUT

xmlcheck "$srcmlcpp"
createfile sub/a.cpp.xml "$srcmlcpp"

srcml --show-language sub/a.cpp.xml
check "C++"

srcml --show-language < sub/a.cpp.xml
check "C++"

# Java
define srcmljava <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Java">
	</unit>
  STDOUT

xmlcheck "$srcmljava"
createfile sub/a.java.xml "$srcmljava"

srcml --show-language sub/a.java.xml
check "Java"

srcml --show-language < sub/a.java.xml
check "Java"

# C
define srcmlc <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C">
	</unit>
  STDOUT

xmlcheck "$srcmlc"
createfile sub/a.c.xml "$srcmlc"

srcml --show-language sub/a.c.xml
check "C"

srcml --show-language < sub/a.c.xml
check "C"

# Objective-C
define srcmlobjc <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Objective-C">
	</unit>
  STDOUT

xmlcheck "$srcmlobjc"
createfile sub/a.m.xml "$srcmlobjc"

srcml --show-language sub/a.m.xml
check "Objective-C"

srcml --show-language < sub/a.m.xml
check "Objective-C"

# Aspect J
define srcmlaj <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Java">
	</unit>
  STDOUT

xmlcheck "$srcmlaj"
createfile sub/a.aj.xml "$srcmlaj"

srcml --show-language sub/a.aj.xml
check "Java"

srcml --show-language < sub/a.aj.xml
check "Java"

# Empty
define empty <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="" directory="" filename="" version=""/>
  STDIN

xmlcheck "$empty"
createfile sub/a.cpp.xml "$empty"

srcml --show-language sub/a.cpp.xml
check ""

srcml --show-language < sub/a.cpp.xml
check ""

