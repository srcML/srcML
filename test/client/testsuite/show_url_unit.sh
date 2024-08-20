#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file show_url_unit.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test get url on single unit
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" url="bar" filename="foo" version="1.2"/>
  STDOUT

# test on archive
define archive <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="bar">

	<unit revision="REVISION" language="C++" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
  STDOUT

xmlcheck "$input"
xmlcheck "$archive"

createfile sub/a.cpp.xml "$input"
createfile sub/archive.cpp.xml "$archive"

srcml --show-url sub/a.cpp.xml
check "bar"

srcml --show-url < sub/a.cpp.xml
check "bar"

srcml --show-url sub/archive.cpp.xml
check "bar"

srcml --show-url < sub/archive.cpp.xml
check "bar"

# empty on the unit
define input <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="" url="" filename="" version=""/>
  STDOUT

# empty on the archive
define empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="">

	<unit revision="REVISION" language="C++" filename="a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
  STDOUT

xmlcheck "$input"
xmlcheck "$empty"

createfile sub/a.cpp.xml "$input"
createfile sub/archive.cpp.xml "$empty"

srcml --show-url sub/a.cpp.xml
check ""

srcml --show-url < sub/a.cpp.xml
check ""

srcml --show-url sub/archive.cpp.xml
check ""

srcml --show-url < sub/archive.cpp.xml
check ""

# none
define none <<- 'STDIN'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION"/>
  STDIN

xmlcheck "$none"
createfile sub/a.cpp.xml "$none"

srcml --show-url sub/a.cpp.xml
check

srcml --show-url < sub/a.cpp.xml
check
