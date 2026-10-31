#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# Regression for https://github.com/srcML/srcML/issues/2463.
# Use synthetic inputs only. Check both position modes and exact source round trips.

source $(dirname "$0")/framework_test.sh

index=0
while IFS= read -r input; do
    createfile input${index}.cs "$input"
    index=$((index + 1))
done <<'SOURCE'
var value = candidate is List<(int, string?)> ? 1 : 2;
value = Container<(First, Second?)> ? left : right;
value = (Container<(First, Second?)>)candidate ? left : right;
value = Container<(First, Second?)>?.Member;
List<(int, string?)>? Create() => null;
List<(int count, string? text)>? Value => null;
public event EventHandler<(bool done, string? text)>? Changed;
var value = candidate is Container<(First, Nested<(Second, Third?)>)> ? 1 : 2;
List<string?> value;
List<(int, string?)>? value;
List<(int, string?)>? Create() { return null; }
values[condition ? first : second] ? left : right;
SOURCE

for position in "" "--position"; do
    srcml $position input*.cs -o archive.xml
    check

    xmllint --noout archive.xml
    check

    srcml --to-dir "roundtrip$position" archive.xml
    check

    for input in input*.cs; do
        diff "$input" "roundtrip$position/$input"
        check
    done
done

# The inner nullable marker must not terminate the outer ternary condition.
createfile input.cs 'var value = candidate is List<(int, string?)> ? 1 : 2;'

srcml input.cs -o output.xml
check

xmllint --xpath 'count(//*[local-name()="ternary"])' output.xml
check '1\n'

xmllint --xpath 'string(//*[local-name()="ternary"]/*[local-name()="condition"]/*[local-name()="expr"])' output.xml
check 'candidate is List<(int, string?)>\n'

xmllint --xpath 'string(//*[local-name()="ternary"]/*[local-name()="then"]/*[local-name()="expr"])' output.xml
check '1\n'

xmllint --xpath 'string(//*[local-name()="ternary"]/*[local-name()="else"]/*[local-name()="expr"])' output.xml
check '2\n'
