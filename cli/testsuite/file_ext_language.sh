#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# check correct language based on file extension

createfile emptysrc/empty.c ""
src2srcml emptysrc/empty.c

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C" filename="emptysrc/empty.c"/>
	STDOUT

createfile emptysrc/empty.h ""
src2srcml emptysrc/empty.h

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C" filename="emptysrc/empty.h"/>
	STDOUT

createfile emptysrc/empty.c.gz ""
src2srcml emptysrc/empty.c.gz

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C" filename="emptysrc/empty.c.gz"/>
	STDOUT


createfile emptysrc/empty.h.gz ""
src2srcml emptysrc/empty.h.gz

check 3<<- STDOUT
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C" filename="emptysrc/empty.h.gz"/>
	STDOUT

# check c++ file extensions

createfile emptysrc/empty.cpp ""
src2srcml emptysrc/empty.cpp

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.cpp"/>
	STDOUT

createfile emptysrc/empty.cc ""
src2srcml emptysrc/empty.cc

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.cc"/>
	STDOUT

createfile emptysrc/empty.cxx ""
src2srcml emptysrc/empty.cxx

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.cxx"/>
	STDOUT

createfile emptysrc/empty.c++ ""
src2srcml emptysrc/empty.c++

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.c++"/>
	STDOUT

createfile emptysrc/empty.hpp ""
src2srcml emptysrc/empty.hpp

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.hpp"/>
	STDOUT

createfile emptysrc/empty.hh ""
src2srcml emptysrc/empty.hh

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.hh"/>
	STDOUT

createfile emptysrc/empty.hxx ""
src2srcml emptysrc/empty.hxx

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.hxx"/>
	STDOUT

createfile emptysrc/empty.h++ ""
src2srcml emptysrc/empty.h++

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.h++"/>
	STDOUT

createfile emptysrc/empty.tcc ""
src2srcml emptysrc/empty.tcc

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.tcc"/>
	STDOUT

createfile emptysrc/empty.cpp.gz ""
src2srcml emptysrc/empty.cpp.gz

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.cpp.gz"/>
	STDOUT

createfile emptysrc/empty.cc.gz ""
src2srcml emptysrc/empty.cc.gz

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.cc.gz"/>
	STDOUT

createfile emptysrc/empty.cxx.gz ""
src2srcml emptysrc/empty.cxx.gz

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.cxx.gz"/>
	STDOUT

createfile emptysrc/empty.c++.gz ""
src2srcml emptysrc/empty.c++.gz

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.c++.gz"/>
	STDOUT

createfile emptysrc/empty.hpp.gz ""
src2srcml emptysrc/empty.hpp.gz

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.hpp.gz"/>
	STDOUT

createfile emptysrc/empty.hh.gz ""
src2srcml emptysrc/empty.hh.gz

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.hh.gz"/>
	STDOUT

createfile emptysrc/empty.hxx.gz ""
src2srcml emptysrc/empty.hxx.gz

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.hxx.gz"/>
	STDOUT

createfile emptysrc/empty.h++.gz ""
src2srcml emptysrc/empty.h++.gz

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.h++.gz"/>
	STDOUT

createfile emptysrc/empty.tcc.gz ""
src2srcml emptysrc/empty.tcc.gz

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.tcc.gz"/>
	STDOUT


# check java file extension
createfile emptysrc/empty.java ""
src2srcml emptysrc/empty.java

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Java" filename="emptysrc/empty.java"/>
	STDOUT

createfile emptysrc/empty.java.gz ""
src2srcml emptysrc/empty.java.gz

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Java" filename="emptysrc/empty.java.gz"/>
	STDOUT

createfile emptysrc/empty.aj.gz ""
src2srcml emptysrc/empty.aj.gz

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="AspectJ" filename="emptysrc/empty.aj.gz"/>
	STDOUT
