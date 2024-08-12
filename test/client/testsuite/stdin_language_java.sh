#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file stdin_language_java.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test language_attribute_java
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Java"/>
	STDOUT

xmlcheck "$output"

echo -n "" | srcml -l Java
check "$output"

echo -n "" | srcml --language Java
check "$output"

echo -n "" | srcml --language=Java
check "$output"
