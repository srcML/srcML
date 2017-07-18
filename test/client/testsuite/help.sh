#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

srcml -h
srcml --help

srcml -h srcml
srcml --help srcml
srcml --help="srcml"

srcml -h srcml
srcml --help srcml
srcml --help="srcml"
