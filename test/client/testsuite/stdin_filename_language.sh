#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file stdin_language_cpp.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test stdin filename attribute as language
define output <<- 'OUTPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="foo.cpp"/>
	OUTPUT

define output_language_precedence <<- 'OUTPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C" filename="foo.cpp"/>
	OUTPUT

xmlcheck "$output"
xmlcheck "$output_language_precedence"

echo -n "" | srcml -f "foo.cpp"
check "$output"

echo -n "" | srcml --filename "foo.cpp"
check "$output"

echo -n "" | srcml --filename="foo.cpp"
check "$output"

echo -n "" | srcml -l C -f "foo.cpp"
check "$output_language_precedence"
