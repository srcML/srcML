#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

srcml2src -V
srcml2src --version

