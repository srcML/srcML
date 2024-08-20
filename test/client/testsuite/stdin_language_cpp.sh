#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file stdin_language_cpp.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test language_attribute_cpp
define output <<- 'OUTPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++"/>
	OUTPUT

xmlcheck "$output"

echo -n "" | srcml -l "C++"
check "$output"

echo -n "" | srcml --language "C++"
check "$output"

echo -n "" | srcml --language="C++"
check "$output"
