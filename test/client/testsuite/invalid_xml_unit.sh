#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file invalid_xml_unit.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# xml error on invalid input (single file) test
define info_single <<- 'STDOUT'
	xmlns="http://www.srcML.org/srcML/src"
	xmlns:cpp="http://www.srcML.org/srcML/cpp"
	encoding="UTF-8"
	language="C++"
	STDOUT

define illformed <<- 'INPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" language="C++">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit
	INPUT

createfile xml_error/illformed.xml "$illformed"

define xml_error <<- 'STDERR'
	Error Parsing: expected '>'

	STDERR

# @TODO Why isn't the exit status non-0?
srcml xml_error/illformed.xml
check "" "$xml_error"
