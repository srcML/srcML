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
	getclipboard() { pbpaste; }
elif [[ -n "$DISPLAY" ]] && command -v xclip &> /dev/null; then
	# xclip daemonizes to hold the selection; close inherited fds 3/4 (the test
	# framework's saved stdout/stderr) so the daemon does not keep the ctest
	# output pipe open and hang the run
	setclipboard() { xclip -selection clipboard >/dev/null 2>&1 3>&- 4>&-; }
	getclipboard() { xclip -selection clipboard -o; }
elif [[ -n "$DISPLAY" ]] && command -v xsel &> /dev/null; then
	setclipboard() { xsel --clipboard --input >/dev/null 2>&1 3>&- 4>&-; }
	getclipboard() { xsel --clipboard --output; }
elif [[ -n "$WAYLAND_DISPLAY" ]] && command -v wl-copy &> /dev/null; then
	setclipboard() { wl-copy >/dev/null 2>&1 3>&- 4>&-; }
	getclipboard() { wl-paste --no-newline; }
else
	echo "Test Skipped: no system clipboard available"
	exit 0
fi

# test framework
source $(dirname "$0")/framework_test.sh

# checkclipboard <expected> : like check, but compares the system clipboard
# contents to <expected>. It reads the clipboard itself, so the traced command
# is the preceding srcml. Keep checkclipboard/getclipboard out of the history so
# the trace shows the srcml command, not this check.
HISTIGNORE="check:checkclipboard:getclipboard:#"
checkclipboard() {

    local exit_status=$?

    set -e

    line=$(caller | cut -d' ' -f1)
    TEMPFILE=$PWD'/.test.'$line

    uncapture_output

    firsthistoryentry

    tmpfile1=$TEMPFILE.1
    echo -en "$1" > $tmpfile1

    tmpfileclip=$TEMPFILE.clip
    getclipboard > $tmpfileclip

    $diff $tmpfile1 $tmpfileclip

    [ ! -s $STDERR ]

    set +e

    if [ $exit_status -ne 0 ]; then
        exit 1
    fi

    capture_output

    true
}

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

##
# srcML in the clipboard is detected and read as srcML, just like stdin

# the srcML form of "a;"
srcml_unit='<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>'
srcml_unit=${srcml_unit//REVISION/${REVISION}}

# srcML clipboard, output as source
printf '%s' "$srcml_unit" | setclipboard
srcml --from-clipboard
check "a;"

printf '%s' "$srcml_unit" | setclipboard
srcml -p
check "a;"

# metadata from a srcML clipboard
printf '%s' "$srcml_unit" | setclipboard
srcml -p --show-language
check "C++\n"

##
# a YAML header on source clipboard input sets metadata, no --language needed

defineXML header_srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="foo.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
STDOUT

printf -- '---\nfilename: foo.cpp\nlanguage: C++\nxmlns: http://www.srcML.org/srcML/src\n---\na;\n' | setclipboard
srcml --from-clipboard
check "$header_srcml"

printf -- '---\nfilename: foo.cpp\nlanguage: C++\nxmlns: http://www.srcML.org/srcML/src\n---\na;\n' | setclipboard
srcml -p
check "$header_srcml"

##
# an explicit clipboard:// input mixes with files, keeping command-line order

defineXML mixed <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" filename="sub/x.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit revision="REVISION" language="C++" hash="e8622977d7b817a78d262b7a3d222bee631740f8"><expr_stmt><expr><name>c</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="C++" filename="sub/y.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
STDOUT

createfile sub/x.cpp "a;\n"
createfile sub/y.cpp "b;\n"
printf 'c;' | setclipboard
srcml sub/x.cpp clipboard:// sub/y.cpp -l C++
check "$mixed"

##
# --to-clipboard / -c : write (copy) output to the system clipboard

# source input (--text) -> srcML written to the clipboard
srcml --text "a;" -l C++ --to-clipboard
checkclipboard "$asrcml"

srcml -t "a;" -l C++ -c
checkclipboard "$asrcml"

# srcML input (file) -> source written to the clipboard
createfile sub/a.xml "$srcml_unit"

srcml --to-clipboard sub/a.xml
checkclipboard "a;"

srcml -c sub/a.xml
checkclipboard "a;"

##
# clipboard:// as an explicit --output / -o destination (same as --to-clipboard)

# source input (--text) -> srcML written to the clipboard
srcml --text "a;" -l C++ --output clipboard://
checkclipboard "$asrcml"

srcml -t "a;" -l C++ -o clipboard://
checkclipboard "$asrcml"

# srcML input (file) -> source written to the clipboard
srcml --output clipboard:// sub/a.xml
checkclipboard "a;"

srcml -o clipboard:// sub/a.xml
checkclipboard "a;"

##
# --to-clipboard combined with --from-clipboard (clipboard in and out)

# source (from the clipboard) -> srcML written to the clipboard
printf 'a;' | setclipboard
srcml -p -c -l C++
checkclipboard "$asrcml"

printf 'a;' | setclipboard
srcml --from-clipboard --to-clipboard -l C++
checkclipboard "$asrcml"

# srcML (from the clipboard) -> source written to the clipboard
printf '%s' "$srcml_unit" | setclipboard
srcml -p -c
checkclipboard "a;"

##
# query (XPath) results written to the clipboard

# XPath element results
defineXML xpath_names <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit revision="REVISION" language="C++" item="1"><name>a</name></unit>

	</unit>
STDOUT

srcml --text "a;" -l C++ --xpath '//src:name' --to-clipboard
checkclipboard "$xpath_names"

srcml --text "a;" -l C++ --xpath '//src:name' -c
checkclipboard "$xpath_names"

# XPath scalar (count) result
srcml --text "a;" -l C++ --xpath 'count(//src:name)' -c
checkclipboard "1\n"
