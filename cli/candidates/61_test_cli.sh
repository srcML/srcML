#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test extract all command
define sfile1 <<< "a;"

define sfile2 <<< "b;"

