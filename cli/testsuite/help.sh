#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

src2srcml -h
src2srcml --help

src2srcml -h src2srcml
src2srcml --help src2srcml
src2srcml --help="src2srcml"

src2srcml -h srcml2src
src2srcml --help srcml2src
src2srcml --help="srcml2src"
