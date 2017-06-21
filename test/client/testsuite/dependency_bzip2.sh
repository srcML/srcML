#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# --version is an error on make (?)
bzip2 --help
