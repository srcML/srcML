#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file empty_stdin.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++"/>
	STDOUT

xmlcheck "$output"

# test simple
echo -n "" | srcml -l C++
check "$output"
