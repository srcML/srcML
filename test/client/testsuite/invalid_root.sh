#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file invalid_root.sh
#
# @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# well-formed XML where the root is not a unit is not srcML

createfile sub/a0.xml '<a/>'

srcml sub/a0.xml
check_exit 1

srcml -S sub/a0.xml
check_exit 1

createfile sub/a1.xml '<project/>'

srcml sub/a1.xml
check_exit 1

srcml -S sub/a1.xml
check_exit 1

createfile sub/a2.xml '<a><b/></a>'

srcml sub/a2.xml
check_exit 1

srcml -S sub/a2.xml
check_exit 1

createfile sub/a3.xml '<a>text</a>'

srcml sub/a3.xml
check_exit 1

srcml -S sub/a3.xml
check_exit 1

createfile sub/a4.xml '<a><unit xmlns="http://www.srcML.org/srcML/src"/></a>'

srcml sub/a4.xml
check_exit 1

srcml -S sub/a4.xml
check_exit 1

createfile sub/a5.xml '<?xml version="1.0"?><!-- comment --><a/>'

srcml sub/a5.xml
check_exit 1

srcml -S sub/a5.xml
check_exit 1

# in a list of files
createfile sub/list.txt "sub/a0.xml"

srcml --files-from sub/list.txt
check_exit 1

# a unit root, with or without a namespace, is still srcML
createfile sub/unit.xml '<unit/>'

srcml -S sub/unit.xml
check

createfile sub/unitns.xml '<unit xmlns="http://www.srcML.org/srcML/src"/>'

srcml -S sub/unitns.xml
check
