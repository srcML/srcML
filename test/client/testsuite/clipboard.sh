#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file clipboard.sh
#
# @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)

# Determine a command to place text on the system clipboard. clip (used by
# srcml) reads the same clipboard, so the tests round-trip through it.
# Skip when no clipboard is available, e.g., a headless CI machine.
if command -v pbcopy &> /dev/null; then
	setclipboard() { pbcopy; }
elif [[ -n "$DISPLAY" ]] && command -v xclip &> /dev/null; then
	# xclip daemonizes to hold the selection; close inherited fds 3/4 (the test
	# framework's saved stdout/stderr) so the daemon does not keep the ctest
	# output pipe open and hang the run
	setclipboard() { xclip -selection clipboard >/dev/null 2>&1 3>&- 4>&-; }
elif [[ -n "$DISPLAY" ]] && command -v xsel &> /dev/null; then
	setclipboard() { xsel --clipboard --input >/dev/null 2>&1 3>&- 4>&-; }
elif [[ -n "$WAYLAND_DISPLAY" ]] && command -v wl-copy &> /dev/null; then
	setclipboard() { wl-copy >/dev/null 2>&1 3>&- 4>&-; }
else
	echo "Test Skipped: no system clipboard available"
	exit 0
fi

# test framework
source $(dirname "$0")/framework_test.sh

##
# --from-clipboard / -p : read (paste) source code from the system clipboard

# empty clipboard
defineXML empty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++"/>
STDOUT

printf '' | setclipboard
srcml --from-clipboard -l C++
check "$empty"

printf '' | setclipboard
srcml -p -l C++
check "$empty"

# simple input
defineXML asrcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>
STDOUT

printf 'a;' | setclipboard
srcml --from-clipboard -l C++
check "$asrcml"

printf 'a;' | setclipboard
srcml -p -l C++
check "$asrcml"

# output to a file
mkdir sub

printf 'a;' | setclipboard
srcml --from-clipboard -l C++ -o sub/a.cpp.xml
check sub/a.cpp.xml "$asrcml"

printf 'a;' | setclipboard
srcml -l C++ -p -o sub/a.cpp.xml
check sub/a.cpp.xml "$asrcml"
