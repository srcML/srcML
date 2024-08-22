#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file show_language_unit.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

define NEWLINE <<- 'STDOUT'
	STDOUT

# test get language

# C++
define srcmlcpp <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" directory="bar" filename="foo" version="1.2"/>
	STDOUT

xmlcheck "$srcmlcpp"
createfile sub/a.cpp.xml "$srcmlcpp"

srcml --show-language sub/a.cpp.xml
check "C++\n"

srcml --show-language < sub/a.cpp.xml
check "C++\n"

# Java
define srcmljava <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Java">
	</unit>
	STDOUT

xmlcheck "$srcmljava"
createfile sub/a.java.xml "$srcmljava"

srcml --show-language sub/a.java.xml
check "Java\n"

srcml --show-language < sub/a.java.xml
check "Java\n"

# C
define srcmlc <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C">
	</unit>
	STDOUT

xmlcheck "$srcmlc"
createfile sub/a.c.xml "$srcmlc"

srcml --show-language sub/a.c.xml
check "C\n"

srcml --show-language < sub/a.c.xml
check "C\n"

# Objective-C
define srcmlobjc <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Objective-C">
	</unit>
	STDOUT

xmlcheck "$srcmlobjc"
createfile sub/a.m.xml "$srcmlobjc"

srcml --show-language sub/a.m.xml
check "Objective-C\n"

srcml --show-language < sub/a.m.xml
check "Objective-C\n"

# Aspect J
define srcmlaj <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Java">
	</unit>
	STDOUT

xmlcheck "$srcmlaj"
createfile sub/a.aj.xml "$srcmlaj"

srcml --show-language sub/a.aj.xml
check "Java\n"

srcml --show-language < sub/a.aj.xml
check "Java\n"

# Empty
define empty <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="" directory="" filename="" version=""/>
	STDIN

xmlcheck "$empty"
createfile sub/a.cpp.xml "$empty"

srcml --show-language sub/a.cpp.xml
check "\n"

srcml --show-language < sub/a.cpp.xml
check "\n"
