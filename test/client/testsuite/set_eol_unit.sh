#!/bin/bash

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
