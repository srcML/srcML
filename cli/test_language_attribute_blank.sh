#!/bin/bash
#
# empty_language_flag_short

# test framework
source /Users/collard/srcML/cli/framework_test.sh

srcml -l C++ << 'STDIN'
 
STDIN

check_stdout 3<< 'STDOUT' 4<< 'STDERR'
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"> 
</unit>
STDOUT
STDERR

srcml -l C++ -o foo.xml << 'STDIN'
 
STDIN

check_file foo.xml 3<< 'FILECONTENTS' 4<< 'STDERR'
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"> 
</unit>
FILECONTENTS
STDERR
