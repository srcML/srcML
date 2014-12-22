#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# check correct language based on file extension

src2srcml emptysrc/empty.c

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.c"/>
	STDOUT

src2srcml emptysrc/empty.h

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.h"/>
	STDOUT

src2srcml emptysrc/empty.c.gz

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.c"/>
	STDOUT


src2srcml emptysrc/empty.h.gz

check 3<<- STDOUT
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.h"/>
	STDOUT

# check c++ file extensions

src2srcml emptysrc/empty.cpp

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.c"/>
	STDOUT

src2srcml emptysrc/empty.cc

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.c"/>
	STDOUT

src2srcml emptysrc/empty.cxx

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.c"
	STDOUT

src2srcml emptysrc/empty.c++

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.c"
	STDOUT

src2srcml emptysrc/empty.hpp

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.h"
	STDOUT

src2srcml emptysrc/empty.hh

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.h"/>
	STDOUT

src2srcml emptysrc/empty.hxx

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.h"
	STDOUT

src2srcml emptysrc/empty.h++

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.h"
	STDOUT

src2srcml emptysrc/empty.tcc

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.t"/>
	STDOUT

src2srcml emptysrc/empty.cpp.gz

check 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.c"/>
	STDOUT


src2srcml emptysrc/empty.cc.gz

check 3<<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.c"/>
	STDOUT

src2srcml emptysrc/empty.cxx.gz

check 3<<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.c"/>
	STDOUT

src2srcml emptysrc/empty.c++.gz

check 3<<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.c"/>
	STDOUT

src2srcml emptysrc/empty.hpp.gz

check 3<<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.h"/>
	STDOUT

src2srcml emptysrc/empty.hh.gz

check 3<<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.h"/>
	STDOUT

define emptyh 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.h"/>
	STDOUT

src2srcml emptysrc/empty.hxx.gz

check 3<<<- "$emptyh"

src2srcml emptysrc/empty.h++.gz

check 3<<<- "$emptyh"

src2srcml emptysrc/empty.tcc.gz

check 3<<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C" filename="emptysrc/empty.t"/>
	STDOUT

define emptyjava 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0" language="C" filename="emptysrc/empty.java"/>
	STDOUT

# check java file extension
src2srcml emptysrc/empty.java

check 3<<< "$emptyjava"

src2srcml emptysrc/empty.java.gz

check 3<<< "$emptyjava"

define emptyaj 3<<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0" language="C" filename="emptysrc/empty.aj"/>
	STDOUT

src2srcml emptysrc/empty.aj.gz

check 3<<< "$emptyaj"

