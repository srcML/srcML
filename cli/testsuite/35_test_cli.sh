#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# Test src2srcml options with files

sfile1 = ""

sfile2 = STDOUT
b;
	STDOUT

f = open(sub/a.cpp 'w')
f.write(sfile1)
f.close()

