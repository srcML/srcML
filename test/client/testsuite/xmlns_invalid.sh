#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# can't overwrite default namespace without redefining src
createfile sub/a.cpp "a;"
createfile sub/b.cpp "b;"

srcml --xmlns=http://www.foo.com sub/a.cpp sub/b.cpp --in-order
check_exit 1

srcml sub/a.cpp sub/b.cpp --xmlns=http://www.foo.com --in-order
check_exit 1

srcml sub/a.cpp sub/b.cpp --in-order --xmlns=http://www.foo.com
check_exit 1

srcml --in-order --xmlns=http://www.foo.com sub/a.cpp sub/b.cpp
check_exit 1

srcml --in-order sub/a.cpp sub/b.cpp --xmlns=http://www.foo.com
check_exit 1

srcml --xmlns=http://www.foo.com sub/a.cpp sub/b.cpp --in-order -o sub/a.xml
check_exit 1

srcml --xmlns=http://www.foo.com sub/a.cpp sub/b.cpp -o sub/a.xml --in-order
check_exit 1

srcml --xmlns=http://www.foo.com -o sub/a.xml sub/a.cpp sub/b.cpp --in-order
check_exit 1

srcml -o sub/a.xml --xmlns=http://www.foo.com sub/a.cpp sub/b.cpp --in-order
check_exit 1

srcml sub/a.cpp sub/b.cpp --xmlns=http://www.foo.com --in-order -o sub/a.xml
check_exit 1

srcml sub/a.cpp sub/b.cpp --in-order -o sub/a.xml --xmlns=http://www.foo.com
check_exit 1

srcml sub/a.cpp sub/b.cpp -o sub/a.xml --in-order --xmlns=http://www.foo.com
check_exit 1

srcml -o sub/a.xml sub/a.cpp sub/b.cpp --in-order --xmlns=http://www.foo.com
check_exit 1

srcml --in-order --xmlns=http://www.foo.com sub/a.cpp sub/b.cpp -o sub/a.xml
check_exit 1

srcml --in-order --xmlns=http://www.foo.com -o sub/a.xml sub/a.cpp sub/b.cpp
check_exit 1

srcml --in-order -o sub/a.xml --xmlns=http://www.foo.com sub/a.cpp sub/b.cpp
check_exit 1

srcml -o sub/a.xml --in-order --xmlns=http://www.foo.com sub/a.cpp sub/b.cpp
check_exit 1

srcml --in-order sub/a.cpp sub/b.cpp --xmlns=http://www.foo.com -o sub/a.xml
check_exit 1

srcml --in-order sub/a.cpp sub/b.cpp -o sub/a.xml --xmlns=http://www.foo.com
check_exit 1

srcml --in-order -o sub/a.xml sub/a.cpp sub/b.cpp --xmlns=http://www.foo.com
check_exit 1

srcml -o sub/a.xml --in-order sub/a.cpp sub/b.cpp --xmlns=http://www.foo.com
check_exit 1

