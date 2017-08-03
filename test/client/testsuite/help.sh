#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

srcml -h
srcml --help

srcml -h src2srcml
srcml --help src2srcml
srcml --help="src2srcml"

srcml -h srcml2src
srcml --help srcml2src
srcml --help="srcml2src"
