#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file expand_tabs.sh
#
# @copyright Copyright (C) 2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test tabs
srcml --text="	" -l C++ --expand-tabs | srcml
check "        ";

srcml --text=" 	" -l C++ --expand-tabs | srcml
check  "        "

srcml --text="  	" -l C++ --expand-tabs | srcml
check  "        "

srcml --text="   	" -l C++ --expand-tabs | srcml
check  "        "

srcml --text="    	" -l C++ --expand-tabs | srcml
check  "        "

srcml --text="     	" -l C++ --expand-tabs | srcml
check  "        "

srcml --text="      	" -l C++ --expand-tabs | srcml
check  "        "

srcml --text="       	" -l C++ --expand-tabs | srcml
check  "        "

srcml --text="//	" -l C++ --expand-tabs | srcml
check  "//      "

srcml --text="// 	" -l C++ --expand-tabs | srcml
check  "//      "

srcml --text="//  	" -l C++ --expand-tabs | srcml
check  "//      "

srcml --text="//   	" -l C++ --expand-tabs | srcml
check  "//      "

srcml --text="//    	" -l C++ --expand-tabs | srcml
check  "//      "

srcml --text="//     	" -l C++ --expand-tabs | srcml
check  "//      "

srcml --text="\"	\"" -l C++ --expand-tabs | srcml
check  "\"       \""

srcml --text="\" 	\"" -l C++ --expand-tabs | srcml
check  "\"       \""

srcml --text="\"  	\"" -l C++ --expand-tabs | srcml
check  "\"       \""

srcml --text="\"   	\"" -l C++ --expand-tabs | srcml
check  "\"       \""

srcml --text="\"    	\"" -l C++ --expand-tabs | srcml
check  "\"       \""

srcml --text="\"     	\"" -l C++ --expand-tabs | srcml
check  "\"       \""
