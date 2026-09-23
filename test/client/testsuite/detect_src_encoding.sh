#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file detect_src_encoding.sh
#
# @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
#
# With no --src-encoding, the source encoding is detected from the data itself.
# The detection itself is tested in libsrcml, so these are the messages, the
# --src-encoding option, and --output-src of what was read.

# test framework
source $(dirname "$0")/framework_test.sh

defineXML foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"><comment type="line">// café naïve</comment>
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
STDOUT

# ISO-8859-1, which is not valid UTF-8, so detected as ISO-8859-1 and converted
createfile sub/latin1.cpp '// caf\xe9 na\xefve\na;\n'

srcml sub/latin1.cpp --filename "sub/a.cpp"
check "$foutput"

# a specified encoding is used as is, with no detection
srcml sub/latin1.cpp --src-encoding "ISO-8859-1" --filename "sub/a.cpp"
check "$foutput"

# a specified encoding is not a guess, so data that is not valid in it is an error,
# rather than the fallback that a detected encoding gets
defineXML finvalid <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"/>
STDOUT

srcml sub/latin1.cpp --src-encoding "UTF-8" --filename "sub/a.cpp"
check "$finvalid" "srcml: Input is not valid 'UTF-8'\n\n"

# the same for an encoding whose conversion to UTF-8 is not a pass through, so it is
# the conversion itself that rejects the data
srcml sub/latin1.cpp --src-encoding "ASCII" --filename "sub/a.cpp"
check "$finvalid" "srcml: Input is not valid 'ASCII'\n\n"

# a specified encoding spelled as an alias of the one a BOM indicates is that same
# encoding, and not a mismatch to warn about
createfile sub/bom.cpp '\xef\xbb\xbf// caf\xc3\xa9 na\xc3\xafve\na;\n'

srcml sub/bom.cpp --src-encoding "utf8" --filename "sub/a.cpp"
check "$foutput"

# while any other specified encoding is a mismatch to warn about, and is still followed
defineXML fmismatch <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"><comment type="line">// cafÃ© naÃ¯ve</comment>
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
STDOUT

srcml sub/bom.cpp --src-encoding "ISO-8859-1" --filename "sub/a.cpp"
check "$fmismatch" "Warning: the encoding ISO-8859-1 was specified, but the source code has a UTF-8 BOM\n"

# a multibyte character split across the end of the first read of the input, so that
# the data read after it starts partway through that character
# "// " and 1020 more characters put the first byte of the é at 1023, and the second
# at 1024, which is where the next read of the input starts
padding=$(printf 'x%.0s' {1..1020})

createfile sub/split.cpp "// $padding\xc3\xa9\xc3\xa9 tail\na;\n"

srcml sub/split.cpp -o sub/split.xml
srcml sub/split.xml --output-src --src-encoding "UTF-8" -o sub/split_src.cpp
check_file sub/split_src.cpp sub/split.cpp

# the same for a conversion that is not a pass through, where a surrogate pair takes
# four bytes and so can be split even though every other character is two bytes
padding=$(printf 'x%.0s' {1..508})

createfile sub/splitwide.cpp "// $padding\xf0\x9f\x98\x80 tail\na;\n"
iconv -f UTF-8 -t UTF-16LE sub/splitwide.cpp > sub/splitwide16.cpp

srcml sub/splitwide16.cpp -o sub/splitwide16.xml
srcml sub/splitwide16.xml --output-src --src-encoding "UTF-16LE" -o sub/splitwide16_src.cpp
check_file sub/splitwide16_src.cpp sub/splitwide16.cpp

# the encoding is detected from the first read of the input, which is longer than
# this, so that the data only stops being valid UTF-8 well after where it is detected
padding=$(printf 'x%.0s' {1..2000})

# corrected to ISO-8859-1 as the rest of the file is read
createfile sub/late.cpp "// $padding\n// caf\xe9 na\xefve\na;\n"

srcml sub/late.cpp -o sub/late.xml
srcml sub/late.xml --output-src --src-encoding "ISO-8859-1" -o sub/late_src.cpp
check_file sub/late_src.cpp sub/late.cpp

# corrected to ISO-8859-1 over the 0x80 - 0x9F range as the rest of the file is read
createfile sub/latecontrol.cpp "// $padding\n// \x93quoted\x94 \x97 dash\na;\n"

srcml sub/latecontrol.cpp -o sub/latecontrol.xml
srcml sub/latecontrol.xml --output-src --src-encoding "ISO-8859-1" -o sub/latecontrol_src.cpp
check_file sub/latecontrol_src.cpp sub/latecontrol.cpp
