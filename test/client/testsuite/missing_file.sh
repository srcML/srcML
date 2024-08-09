#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file missing_file.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# check missingfile

srcml foo.c
check_exit 1 "srcml: Unable to open file foo.c
"

srcml abc.c
check_exit 1 "srcml: Unable to open file abc.c
"

srcml ../src/foo.c
check_exit 1 "srcml: Unable to open file ../src/foo.c
"
