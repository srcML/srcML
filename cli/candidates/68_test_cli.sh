#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# Help and version

src2srcml -h
src2srcml --help

srcml2src -V
srcml2src --version

