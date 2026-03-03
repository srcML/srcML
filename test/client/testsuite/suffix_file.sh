#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file suffix_file.sh
#
# @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test position
defineXML src <<- 'STDOUT'
	n1;
	n2;
	n3;
STDOUT

createfile n.cpp "$src"

cat n.cpp

srcml n.cpp:1 --output-src
check "n1;\n"
