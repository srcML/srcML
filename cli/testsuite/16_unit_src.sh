#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh


define nestedfile <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/b.cpp" hash="aecf18b52d520ab280119febd8ff6c803135ddfc">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define nestedfilesrc <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<src:unit xmlns:src="http://www.sdml.info/srcML/src">

	<src:unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<src:expr_stmt><src:expr><src:name>a</src:name></src:expr>;</src:expr_stmt>
	</src:unit>

	<src:unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/b.cpp" hash="aecf18b52d520ab280119febd8ff6c803135ddfc">
	<src:expr_stmt><src:expr><src:name>b</src:name></src:expr>;</src:expr_stmt>
	</src:unit>

	</src:unit>
	STDOUT


# test
srcml2src --units <<< "$nestedfile"

check 3<<< "2"

srcml2src --units <<< "$nestedfilesrc"

check 3<<< "2"

srcml2src -U "1" <<< "$nestedfile"

check 3<<< $'\na;\n'

srcml2src --unit "1" <<< "$nestedfile"

check 3<<< $'\na;\n'

srcml2src --unit="1" <<< "$nestedfile"

check 3<<< $'\na;\n'

srcml2src -U "2" <<< "$nestedfile"

check 3<<< $'\nb;\n'

srcml2src --unit "2" <<< "$nestedfile"

check 3<<< $'\nb;\n'

srcml2src --unit="2" <<< "$nestedfile"

check 3<<< $'\nb;\n'

