#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

set +e
# filename is not allowed to be set on the archive, only on the individual
# srcML units

# filename flag on archive of single unit
createfile sub/a.cpp "a;"

srcml sub/a.cpp --archive -f "compressed.tar.gz"
check "" "Attribute filename cannot be set for a srcML archive"

srcml sub/a.cpp --archive --filename "compressed.tar.gz"
check "" "Attribute filename cannot be set for a srcML archive"

srcml sub/a.cpp --archive --filename="compressed.tar.gz"
check "" "Attribute filename cannot be set for a srcML archive"

srcml -l C++ --archive -f 'compressed.tar.gz' -o sub/a.cpp.xml sub/a.cpp
check "" "Attribute filename cannot be set for a srcML archive"

srcml --archive  -f 'compressed.tar.gz' sub/a.cpp -o sub/a.cpp.xml
check "" "Attribute filename cannot be set for a srcML archive"


# standard input
echo -n "a;" | srcml -l C++ --archive -f compressed.tar.gz
check "" "Attribute filename cannot be set for a srcML archive"

echo -n "a;" | srcml -l C++ --archive --filename compressed.tar.gz
check "" "Attribute filename cannot be set for a srcML archive"

echo -n "a;" | srcml -l C++ --archive --filename=compressed.tar.gz
check "" "Attribute filename cannot be set for a srcML archive"


# filename flag on archive of multiple units
createfile sub/b.cpp "b;"

srcml sub/a.cpp sub/b.cpp --in-order -f "compressed.tar.gz"
check "" "Attribute filename cannot be set for a srcML archive"

srcml sub/a.cpp sub/b.cpp --in-order -f=compressed.tar.gz
check "" "Attribute filename cannot be set for a srcML archive"

srcml --in-order -f "compressed.tar.gz" sub/a.cpp sub/b.cpp
check "" "Attribute filename cannot be set for a srcML archive"
