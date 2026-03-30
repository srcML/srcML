#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file version.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	srcml: 1.1.0
	libsrcml: 1.1.0
	srcql: 1.0.0
	markup:
	  Python: 1.0.0
	  C: 1.0.0
	  C++: 1.0.0
	  C#: 1.0.0
	  Java: 1.0.0
	  JavaScript: 1.0.0
	  TypeScript: 1.0.0
	  Objective-C: 1.0.0
STDOUT

srcml -V
check "$output"

srcml --version
check "$output"
