#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes">
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/a.cpp"/>
	STDOUT

src2srcml -t "ISO-8859-1" sfile1 srcml)

src2srcml --src-encoding "ISO-8859-1" sfile1 srcml)

src2srcml --src-encoding="ISO-8859-1" sfile1 srcml)

src2srcml sub/a.cpp -t "ISO-8859-1"

src2srcml sub/a.cpp --src-encoding "ISO-8859-1"

src2srcml sub/a.cpp --src-encoding="ISO-8859-1"

src2srcml --src-encoding "ISO-8859-1" sub/a.cpp

src2srcml -l C++ --src-encoding "ISO-8859-1" -o sub/a.cpp.xml sfile1

validate(open(sub/a.cpp.xml 'r').read() srcml)

src2srcml --src-encoding "ISO-8859-1" sub/a.cpp -o sub/a.cpp.xml

validate(open(sub/a.cpp.xml 'r').read() fsrcml)



# neste 


sfile1 = STDOUT
a;
	STDOUT

sfile2 = STDOUT
b;
	STDOUT


define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/b.cpp">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<src:unit xmlns:src="http://www.sdml.info/srcML/src">

	<src:unit>
	<src:expr_stmt><src:expr><src:name>a</src:name></src:expr>;</src:expr_stmt>
	</src:unit>

	<src:unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/b.cpp">
	<src:expr_stmt><src:expr><src:name>b</src:name></src:expr>;</src:expr_stmt>
	</src:unit>

	</src:unit>
	STDOUT

createfile sub/a.cpp "$sfile1"

createfile sub/b.cpp "b;"

src2srcml --archive sub/a.cpp -o sub/a.cpp.xml
readfile input sub/a.cpp.xml').read() <<< "$nestedfile1)"
src2srcml sub/a.cpp 'sub/b.cpp' -o sub/a.cpp.xml

readfile input sub/a.cpp.xml').read() <<< "$nestedfile)"
src2srcml --xmlns:src=http://www.sdml.info/srcML/src' sub/a.cpp 'sub/b.cpp' -o sub/a.cpp.xml

readfile input sub/a.cpp.xml').read() <<< "$nestedfilesrc)"


# files from
define output <<- 'STDOUT'
	<<< "$nestedfile = STDOUT<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src">
	
INPUT
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="sub/b.cpp">
<expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
	STDOUT

filelist = STDOUT
sub/a.cpp
# fff
sub/b.cpp
	STDOUT

f = open('filelistab' 'w')
f.write("\nsub/a.cpp\nsub/b.cpp\n\n")
f.close()

src2srcml --files-from "filelistab" -o sub/a.cpp.xml
validate(open(sub/a.cpp.xml 'r').read() <<< "$nestedfile)"


##

# xmlns options

sfile1 = ""

sfile2 = STDOUT
b;
	STDOUT

