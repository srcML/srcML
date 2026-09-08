#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file detect_src_encoding.sh
#
# @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
#
# With no --src-encoding, the source encoding is detected from the data itself.
# Only UTF-8 is detectable, as every byte sequence is valid ISO-8859-1, so what is
# not UTF-8 falls back to ISO-8859-1.

# test framework
source $(dirname "$0")/framework_test.sh

# fcomment is set to the srcML of the single line comment $1, which is all that
# the cases below differ in
comment() {
	defineXML fcomment <<- STDOUT
		<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"><comment type="line">$1</comment>
		<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
		</unit>
	STDOUT
}

defineXML foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"><comment type="line">// café naïve</comment>
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
STDOUT

# valid UTF-8, so detected as UTF-8 and passed through unchanged
createfile sub/utf8.cpp '// caf\xc3\xa9 na\xc3\xafve\na;\n'

srcml sub/utf8.cpp --filename "sub/a.cpp"
check "$foutput"

# the same characters in ISO-8859-1, which is not valid UTF-8, so they are converted
createfile sub/latin1.cpp '// caf\xe9 na\xefve\na;\n'

srcml sub/latin1.cpp --filename "sub/a.cpp"
check "$foutput"

# data that is not valid UTF-8 for reasons the lead byte alone does not show,
# each of which is the fallback to ISO-8859-1, where every one of these bytes is
# a character of its own
# the cases are separate files, as the check stops at the first one it finds

# an overlong encoding, here of '/', which UTF-8 requires to be a single byte
createfile sub/overlong.cpp '// \xc0\xaf\na;\n'

comment '// \xc3\x80\xc2\xaf'

srcml sub/overlong.cpp --filename "sub/a.cpp"
check "$fcomment"

# a UTF-16 surrogate, which is not a character that UTF-8 encodes
createfile sub/surrogate.cpp '// \xed\xa0\x80\na;\n'

comment '// \xc3\xad\xc2\xa0\xc2\x80'

srcml sub/surrogate.cpp --filename "sub/a.cpp"
check "$fcomment"

# past U+10FFFF, which is the largest character there is
createfile sub/toolarge.cpp '// \xf5\x80\x80\x80\na;\n'

comment '// \xc3\xb5\xc2\x80\xc2\x80\xc2\x80'

srcml sub/toolarge.cpp --filename "sub/a.cpp"
check "$fcomment"

# a continuation byte with no lead byte before it
createfile sub/orphan.cpp '// \x80\na;\n'

comment '// \xc2\x80'

srcml sub/orphan.cpp --filename "sub/a.cpp"
check "$fcomment"

# a lead byte with its continuation bytes cut off by the end of the file, which is
# not an error while there is more data to read, and is one once there is not
createfile sub/truncated.cpp '// caf\xc3\na;\n'

comment '// caf\xc3\x83'

srcml sub/truncated.cpp --filename "sub/a.cpp"
check "$fcomment"

# not UTF-8, so ISO-8859-1, including the 0x80 - 0x9F range, which holds the C1
# control characters there and so passes these bytes through as those characters
defineXML fcontrol <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"><comment type="line">// \xc2\x93quoted\xc2\x94 \xc2\x97 dash</comment>
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
STDOUT

createfile sub/control.cpp '// \x93quoted\x94 \x97 dash\na;\n'

srcml sub/control.cpp --filename "sub/a.cpp"
check "$fcontrol"

# a specified encoding is used as is, with no detection, which for this data
# is what detecting it arrives at anyway
srcml sub/control.cpp --src-encoding "ISO-8859-1" --filename "sub/a.cpp"
check "$fcontrol"

# a specified encoding is not a guess, so data that is not valid in it is an error,
# rather than the fallback that a detected encoding gets
# the conversion from UTF-8 to UTF-8 is a pass through that does not itself reject
# anything, so without this check these bytes would go straight into the srcML
# and leave it not valid XML
createfile sub/invalid.cpp '// caf\xe9\na;\n'

defineXML finvalid <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"/>
STDOUT

srcml sub/invalid.cpp --src-encoding "UTF-8" --filename "sub/a.cpp"
check "$finvalid" "srcml: Input is not valid 'UTF-8'\n\n"

# the same data with no encoding specified is detected as ISO-8859-1 instead
defineXML fdetected <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"><comment type="line">// caf\xc3\xa9</comment>
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
STDOUT

srcml sub/invalid.cpp --filename "sub/a.cpp"
check "$fdetected"

# a UTF-8 BOM is used as is, with no detection
createfile sub/bom.cpp '\xef\xbb\xbf// caf\xc3\xa9 na\xc3\xafve\na;\n'

srcml sub/bom.cpp --filename "sub/a.cpp"
check "$foutput"

# UTF-16 and UTF-32 with no BOM, where the NUL bytes of the ASCII characters
# give both the width of a character and the byte order
# the bytes are given directly, as the point of these is where the NUL bytes fall
defineXML fwide <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"><comment type="line">// é</comment>
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
STDOUT

createfile sub/utf16le.cpp '\x2f\x00\x2f\x00\x20\x00\xe9\x00\x0a\x00\x61\x00\x3b\x00\x0a\x00'

srcml sub/utf16le.cpp --filename "sub/a.cpp"
check "$fwide"

createfile sub/utf16be.cpp '\x00\x2f\x00\x2f\x00\x20\x00\xe9\x00\x0a\x00\x61\x00\x3b\x00\x0a'

srcml sub/utf16be.cpp --filename "sub/a.cpp"
check "$fwide"

createfile sub/utf32le.cpp '\x2f\x00\x00\x00\x2f\x00\x00\x00\x20\x00\x00\x00\xe9\x00\x00\x00\x0a\x00\x00\x00\x61\x00\x00\x00\x3b\x00\x00\x00\x0a\x00\x00\x00'

srcml sub/utf32le.cpp --filename "sub/a.cpp"
check "$fwide"

createfile sub/utf32be.cpp '\x00\x00\x00\x2f\x00\x00\x00\x2f\x00\x00\x00\x20\x00\x00\x00\xe9\x00\x00\x00\x0a\x00\x00\x00\x61\x00\x00\x00\x3b\x00\x00\x00\x0a'

srcml sub/utf32be.cpp --filename "sub/a.cpp"
check "$fwide"

# a stray NUL byte in data that is otherwise ASCII is not enough of them to be
# a wide encoding, which would take one for every character
createfile sub/nul.cpp '// a\x00b\na;\n'

comment '// a<escape char="0x00"/>b'

srcml sub/nul.cpp --filename "sub/a.cpp"
check "$fcomment"

# a character outside the BMP is a surrogate pair, whose low-order byte is NUL and
# so falls where the data bytes of the other characters are, which is why nearly all
# of a position being NUL, rather than all of it, is what makes a wide encoding
# the bytes are given directly, as the point of this is where that NUL byte falls
defineXML fwidepair <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="sub/a.cpp"><comment type="line">// 😀 x</comment>
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>
STDOUT

createfile sub/utf16pair.cpp '\x2f\x00\x2f\x00\x20\x00\x3d\xd8\x00\xde\x20\x00\x78\x00\x0a\x00\x61\x00\x3b\x00\x0a\x00'

srcml sub/utf16pair.cpp --filename "sub/a.cpp"
check "$fwidepair"

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
