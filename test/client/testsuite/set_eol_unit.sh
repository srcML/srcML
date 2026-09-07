#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file set_eol_unit.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

srcml --text="\na;\n" -l C++ | srcml --unit=1
check "\na;\n"

srcml --text="\na;\n" -l C++ | srcml --unit=1 --eol lf
check "\na;\n"

srcml --text="\na;\n" -l C++ | srcml --unit=1 --eol=lf
check "\na;\n"

srcml --text="\na;\n" -l C++ | srcml --unit=1 --eol UNIX
check "\na;\n"

srcml --text="\na;\n" -l C++ | srcml --unit=1 --eol=UNIX
check "\na;\n"

srcml --text="\na;\n" -l C++ | srcml --unit=1 --eol cr
check "\ra;\r"

srcml --text="\na;\n" -l C++ | srcml --unit=1 --eol=cr
check "\ra;\r"

srcml --text="\na;\n" -l C++ | srcml --unit=1 --eol crlf
check "\r\na;\r\n"

srcml --text="\na;\n" -l C++ | srcml --unit=1 --eol=crlf
check "\r\na;\r\n"

srcml --text="\na;\n" -l C++ | srcml --unit=1 --eol Windows
check "\r\na;\r\n"

srcml --text="\na;\n" -l C++ | srcml --unit=1 --eol=Windows
check "\r\na;\r\n"

# an empty value after the equals is dropped, so the following input filename
# becomes the option value
echo "a;" | srcml -l C++ --eol= sub/a.cpp --output-src
check_exit 2
