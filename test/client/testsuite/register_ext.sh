#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file register_ext.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test register language
defineXML fxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Java" filename="sub/a.cpp"/>
STDOUT

createfile sub/a.cpp ""

# src to srcml
srcml --register-ext cpp=Java sub/a.cpp
check "$fxmlfile"

srcml --register-ext="cpp=Java" sub/a.cpp
check "$fxmlfile"

srcml --register-ext cpp=Java sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$fxmlfile"

srcml --register-ext cpp=Java sub/a.cpp --register-ext xml=Java -o sub/a.cpp.xml
check sub/a.cpp.xml "$fxmlfile"

defineXML fsxmlfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="a.cpp"/>
STDOUT

createfile sub/a.xml "$fsxmlfile"

srcml --register-ext xml=Java sub/a.xml
check

srcml --register-ext="xml=Java" sub/a.xml
check

# an empty value after the equals is dropped, so the following input filename
# becomes the option value
echo "a;" | srcml -l C++ --register-ext= sub/a.cpp
check_exit 2
