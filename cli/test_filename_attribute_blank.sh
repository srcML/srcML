#!/bin/bash
#
# empty_language_flag_short

# test framework
source $(dirname "$0")/framework_test.sh

srcml -l C++ << 'STDIN'
 
STDIN

check 3<< 'STDOUT' 4<< 'STDERR'
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"> 
</unit>
STDOUT
STDERR

srcml -l C++ -o foo.xml << 'STDIN'
 
STDIN

check foo.xml 3<< 'FILECONTENTS' 4<< 'STDERR'
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"> 
</unit>
FILECONTENTS
STDERR

rm -f a.cpp
echo "" > a.cpp
srcml -l C++ --filename="foo.cpp" a.cpp

check 3<< 'STDOUT' 4<< 'STDERR'
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="foo.cpp">
</unit>
STDOUT
STDERR

rm -f a.cpp
echo "" > a.cpp
srcml -l C++ a.cpp

check 3<< 'STDOUT' 4<< 'STDERR'
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="a.cpp">
</unit>
STDOUT
STDERR
