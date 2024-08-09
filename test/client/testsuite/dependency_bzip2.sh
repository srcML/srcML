#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file dependency_bzip2.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# --version is an error on make (?)
bzip2 --help
