#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
srcml2src --units <<< "$nestedfile"

check 4<<< "2"

srcml2src --units <<< "$nestedfilesrc"

check 4<<< "2"

srcml2src -U "1" <<< "$nestedfile"

check 3<<< "$sfile1"

srcml2src --unit "1" <<< "$nestedfile"

check 3<<< "$sfile1"

srcml2src --unit="1" <<< "$nestedfile"

check 3<<< "$sfile1"

srcml2src -U "2" <<< "$nestedfile"

check 3<<< "b;"

srcml2src --unit "2" <<< "$nestedfile"

check 3<<< "b;"

srcml2src --unit="2" <<< "$nestedfile"

check 3<<< "b;"

