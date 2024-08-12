#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file help.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

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
