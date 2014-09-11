#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# srcml2src error return

# invalid input filename
srcml2src foobar

check 4<<< "2"

# unknown option
srcml2src --strip foobar

check 4<<< "3"

# unknown encoding
srcml2src --src-encoding="ISO"

check 4<<< "4"

srcml2src --src-encoding

check 4<<< "1"
	
# source encoding not given

# unit option selected but no value
srcml2src --unit

check 4<<< "3"

# unit value too large

srcml2src --unit 3 <<< "$nestedfile"

check 4<<< "4"

srcml2src --unit 3 --xml <<< "$nestedfile"

check 4<<< "4"

srcml2src --unit 3 --filename <<< "$nestedfile"

check 4<<< "4"

srcml2src --unit 3 --directory <<< "$nestedfile"

check 4<<< "4"

srcml2src --unit 3 --src-version <<< "$nestedfile"

check 4<<< "4"

# invalid combinations
srcml2src --xml --src-encoding "UTF-8" "foobar"

check 4<<< "5"
