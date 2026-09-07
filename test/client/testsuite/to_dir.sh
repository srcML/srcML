#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file to_dir.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test
defineXML srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
STDOUT

defineXML nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6" revision="REVISION" language="C++" filename="sub/a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb" revision="REVISION" language="C++" filename="sub/b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
STDOUT

define output <<- 'STDERR'
	1
	sub/a.cpp
	2
	sub/b.cpp

	Source Files: 0\tOther Files: 0\tErrors: 0\tTotal Files: 0
STDERR

createfile a.cpp.xml "$srcml"
rmfile sub/a.cpp

srcml --verbose --to-dir=. a.cpp.xml
check sub/a.cpp "a;\n" "1\nsub/a.cpp\n\nSource Files: 0\tOther Files: 0\tErrors: 0\tTotal Files: 0\n"

createfile a.cpp.xml "$nestedfile"

rmfile sub/a.cpp
rmfile sub/b.cpp

srcml --verbose --to-dir=. a.cpp.xml
check sub/a.cpp "a;\n" "$output"
check sub/b.cpp "b;\n"

rmfile sub/a.cpp
rmfile sub/b.cpp

srcml --verbose --to-dir '.' a.cpp.xml

check sub/a.cpp "a;\n" "$output"
check sub/b.cpp "b;\n"

# an empty value after the equals is dropped, so the following input filename
# becomes the option value
echo "a;" | srcml -l C++ --to-dir= sub/a.cpp
check_exit 1
