#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh
set +e

# test
##
# srcml2src error return

# invalid input filename
srcml2src foobar

check_exit 0

# unknown option
srcml2src --strip foobar

check_exit 3

# unknown encoding
srcml2src --src-encoding="ISO"

check_exit 4

# source encoding not given
srcml2src --src-encoding

check_exit 7

# unit option selected but no value
srcml2src --unit

check_exit 7

# unit value too large

srcml2src --unit 3 <<< "$nestedfile"

check_exit 1

srcml2src --unit 3 -X <<< "$nestedfile"

check_exit 1

srcml2src --unit 3 --filename <<< "$nestedfile"

check_exit 7

srcml2src --unit 3 --directory <<< "$nestedfile"

check_exit 7

srcml2src --unit 3 --src-version <<< "$nestedfile"

check_exit 7

# invalid combinations
srcml2src -X --src-encoding "UTF-8" "foobar"

check_exit 0
