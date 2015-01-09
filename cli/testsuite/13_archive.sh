#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

trap "{ cleanup; }" EXIT

# test archive
define fileasrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">
	
	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="sub/a.cpp" hash="1a2c5d67e6f651ae10b7673c53e8c502c97316d6">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

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

createfile sub/a.cpp ""
createfile sub/b.cpp ""
echo -e "\na;" > sub/a.cpp
echo -e "\nb;" > sub/b.cpp

# test that two files will output in an archive by default
src2srcml sub/a.cpp sub/b.cpp

check 3<<< "$nestedfile"

# test explicit archive flag
src2srcml --archive sub/a.cpp

check 3<<< "$fileasrcml"

define filelist <<- 'STDOUT'
	sub/a.cpp
	# fff
	sub/b.cpp
	STDOUT

createfile filelistab "$filelist"

#src2srcml --files-from filelistab

#check 3<< "$nestedfile"

rm -f filelistab

exit 0

