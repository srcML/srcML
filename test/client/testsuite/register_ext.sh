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

# registered extensions apply to directories
defineXML fxmldir <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="dir">

	<unit revision="REVISION" language="C++" filename="dir/a.foo" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
STDOUT

createfile dir/a.foo ""

srcml --register-ext foo=C++ dir
check "$fxmldir"

srcml --register-ext="foo=C++" dir
check "$fxmldir"

# disable an extension with the language NONE
createfile sub/a.py ""

defineXML fxmlcppfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/a.cpp" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
STDOUT

srcml --register-ext py=NONE sub/a.py sub/a.cpp
check "$fxmlcppfile"

srcml --register-ext="py=NONE" sub/a.py sub/a.cpp
check "$fxmlcppfile"

# re-enable an extension after disabling it
defineXML fxmlpyfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Python" filename="sub/a.py"/>
STDOUT

srcml --register-ext py=NONE --register-ext py=Python sub/a.py
check "$fxmlpyfile"

# disabled extensions are skipped in directories
defineXML fxmldir2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="dir2">

	<unit revision="REVISION" language="C++" filename="dir2/a.cpp" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

	</unit>
STDOUT

createfile dir2/a.cpp ""
createfile dir2/a.py ""

srcml --register-ext py=NONE dir2
check "$fxmldir2"
