#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# check missingfile

src2srcml foo.c

check "" "WARNING srcml: Unable to open file foo.c

"


src2srcml abc.c

check "" "WARNING srcml: Unable to open file abc.c

"

src2srcml ../src/foo.c

check "" "WARNING srcml: Unable to open file ../src/foo.c

"
