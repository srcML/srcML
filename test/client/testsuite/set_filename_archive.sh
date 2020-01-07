#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# filename is not allowed to be set on the archive, only on the individual
# srcML units

# filename flag on archive of single unit
createfile sub/a.cpp "a;"

srcml sub/a.cpp --archive -f "compressed.tar.gz"
check_exit 2 "srcml: --filename cannot be set for a srcML archive. Use --url instead
"

srcml sub/a.cpp --archive --filename "compressed.tar.gz"
check_exit 2 "srcml: --filename cannot be set for a srcML archive. Use --url instead
"

srcml sub/a.cpp --archive --filename="compressed.tar.gz"
check_exit 2 "srcml: --filename cannot be set for a srcML archive. Use --url instead
"

srcml -l C++ --archive -f 'compressed.tar.gz' -o sub/a.cpp.xml sub/a.cpp
check_exit 2 "srcml: --filename cannot be set for a srcML archive. Use --url instead
"

srcml --archive  -f 'compressed.tar.gz' sub/a.cpp -o sub/a.cpp.xml
check_exit 2 "srcml: --filename cannot be set for a srcML archive. Use --url instead
"

# standard input
echo -n "a;" | srcml -l C++ --archive -f compressed.tar.gz
check_exit 2 "srcml: --filename cannot be set for a srcML archive. Use --url instead
"

echo -n "a;" | srcml -l C++ --archive --filename compressed.tar.gz
check_exit 2 "srcml: --filename cannot be set for a srcML archive. Use --url instead
"

echo -n "a;" | srcml -l C++ --archive --filename=compressed.tar.gz
check_exit 2 "srcml: --filename cannot be set for a srcML archive. Use --url instead
"

# filename flag on archive of multiple units
createfile sub/b.cpp "b;"

srcml sub/a.cpp sub/b.cpp -f "compressed.tar.gz"
check_exit 2 "srcml: --filename cannot be set for a srcML archive. Use --url instead
"

srcml sub/a.cpp sub/b.cpp -f=compressed.tar.gz
check_exit 2 "srcml: --filename cannot be set for a srcML archive. Use --url instead
"

srcml -f "compressed.tar.gz" sub/a.cpp sub/b.cpp
check_exit 2 "srcml: --filename cannot be set for a srcML archive. Use --url instead
"
