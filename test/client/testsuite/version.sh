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
	srcml REVISION
	libsrcml REVISION
	STDOUT

srcml -V | head -n 2
check "$output"

srcml --version | head -n 2
check "$output"
