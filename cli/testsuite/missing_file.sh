#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# check missingfile

src2srcml foo.c

check 4<<- 'STDERR'
	srcml: Unable to open file foo.c
	STDERR

src2srcml abc.c

check 4<<- 'STDERR'
	srcml: Unable to open file abc.c
	STDERR

src2srcml ../src/foo.c

check 4<<- 'STDERR'
	srcml: Unable to open file ../src/foo.c
	STDERR
