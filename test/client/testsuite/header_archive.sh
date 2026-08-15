#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file header_archive.sh
#
# @copyright Copyright (C) 2025 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

write_header_archive() {
    local output_file=$1
    shift

    : > "$output_file"

    local first_unit=true
    local unit
    for unit in "$@"; do
        if [ "$first_unit" = true ]; then
            first_unit=false
        else
            printf '\0' >> "$output_file"
        fi

        printf '%b' "${unit//$'\n'/$EOL}" >> "$output_file"
    done
}

normalize_windows_hashes() {
    local var_name=$1
    local value=${!var_name}

    value=${value//2b22284231f33eb19e66388951726a07ccbec135/f4344df8b5c0b54554a29c64f631984f0d11fd36}
    value=${value//4e505f4b4ab0455bc5357bfe34ddd8430a71d66a/b01f85862956f59fe53b0bca9472d976ec1dddf6}

    printf -v "$var_name" '%s' "$value"
}

defineXML headerXML <<- 'OUTPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0">

	<unit revision="1.0.0" language="C++" filename="n.cpp" hash="2b22284231f33eb19e66388951726a07ccbec135"><decl_stmt><decl><type><name>int</name></type> <name>n</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>;</decl_stmt>
	</unit>

	<unit revision="1.0.0" language="C++" filename="m.cpp" hash="4e505f4b4ab0455bc5357bfe34ddd8430a71d66a"><decl_stmt><decl><type><name>int</name></type> <name>m</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>;</decl_stmt>
	</unit>

	</unit>
OUTPUT
if [[ "$OSTYPE" == 'msys' || "$OSTYPE" == 'cygwin' ]]; then
    normalize_windows_hashes headerXML
fi

createfile project.xml "$headerXML"

define file1 <<-'EOF'
	---
	xmlns: "http://www.srcML.org/srcML/src"
	language: "C++"
	filename: "n.cpp"
	---
	int n = 0;
EOF

define file2 <<-'EOF'
	---
	xmlns: "http://www.srcML.org/srcML/src"
	language: "C++"
	filename: "m.cpp"
	---
	int m = 0;
EOF

write_header_archive project.txt "$file1" "$file2"

srcml project.xml --header
check project.txt

srcml --header project.txt
check project.xml

cat project.txt | srcml
check project.xml

cat project.txt | srcml | srcml --header
check project.txt

defineXML headerCustomXML <<- 'OUTPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0">

	<unit xmlns:metrics="http://www.srcML.org/srcML/metrics" revision="1.0.0" language="C++" filename="n.cpp" hash="2b22284231f33eb19e66388951726a07ccbec135"><decl_stmt><decl><type><name>int</name></type> <name>n</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>;</decl_stmt>
	</unit>

	<unit revision="1.0.0" language="C++" filename="m.cpp" hash="4e505f4b4ab0455bc5357bfe34ddd8430a71d66a"><decl_stmt><decl><type><name>int</name></type> <name>m</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>;</decl_stmt>
	</unit>

	</unit>
OUTPUT
if [[ "$OSTYPE" == 'msys' || "$OSTYPE" == 'cygwin' ]]; then
    normalize_windows_hashes headerCustomXML
fi

createfile projectCustom.xml "$headerCustomXML"

define file1Custom <<-'EOF'
	---
	xmlns: "http://www.srcML.org/srcML/src"
	"xmlns:metrics": "http://www.srcML.org/srcML/metrics"
	language: "C++"
	filename: "n.cpp"
	---
	int n = 0;
EOF

define file2Custom <<-'EOF'
	---
	xmlns: "http://www.srcML.org/srcML/src"
	language: "C++"
	filename: "m.cpp"
	---
	int m = 0;
EOF

write_header_archive projectCustom.txt "$file1Custom" "$file2Custom"

srcml projectCustom.xml --header
check projectCustom.txt

srcml --header projectCustom.txt
check projectCustom.xml

cat projectCustom.txt | srcml
check projectCustom.xml

cat projectCustom.txt | srcml | srcml --header
check projectCustom.txt

defineXML headerCustomAttributeXML <<- 'OUTPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0">

	<unit xmlns:metrics="http://www.srcML.org/srcML/metrics" revision="1.0.0" language="C++" filename="n.cpp" hash="2b22284231f33eb19e66388951726a07ccbec135" metrics:complexity="n"><decl_stmt><decl><type><name>int</name></type> <name>n</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>;</decl_stmt>
	</unit>

	<unit revision="1.0.0" language="C++" filename="m.cpp" hash="4e505f4b4ab0455bc5357bfe34ddd8430a71d66a"><decl_stmt><decl><type><name>int</name></type> <name>m</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>;</decl_stmt>
	</unit>

	</unit>
OUTPUT
if [[ "$OSTYPE" == 'msys' || "$OSTYPE" == 'cygwin' ]]; then
    normalize_windows_hashes headerCustomAttributeXML
fi

createfile projectCustomAttribute.xml "$headerCustomAttributeXML"

define file1CustomAttribute <<-'EOF'
	---
	xmlns: "http://www.srcML.org/srcML/src"
	"xmlns:metrics": "http://www.srcML.org/srcML/metrics"
	language: "C++"
	filename: "n.cpp"
	"metrics:complexity": "n"
	---
	int n = 0;
EOF

define file2CustomAttribute <<-'EOF'
	---
	xmlns: "http://www.srcML.org/srcML/src"
	language: "C++"
	filename: "m.cpp"
	---
	int m = 0;
EOF

write_header_archive projectCustomAttribute.txt "$file1CustomAttribute" "$file2CustomAttribute"

srcml projectCustomAttribute.xml --header
check projectCustomAttribute.txt

srcml --header projectCustomAttribute.txt
check projectCustomAttribute.xml

cat projectCustomAttribute.txt | srcml
check projectCustomAttribute.xml

cat projectCustomAttribute.txt | srcml | srcml --header
check projectCustomAttribute.txt

defineXML headerCustomAttributeAttributeXML <<- 'OUTPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0">

	<unit xmlns:metrics="http://www.srcML.org/srcML/metrics" revision="1.0.0" language="C++" filename="n.cpp" hash="2b22284231f33eb19e66388951726a07ccbec135" metrics:complexity="n"><decl_stmt><decl><type><name>int</name></type> <name>n</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>;</decl_stmt>
	</unit>

	<unit xmlns:slicing="http://www.srcML.org/srcML/slicing" revision="1.0.0" language="C++" filename="m.cpp" hash="4e505f4b4ab0455bc5357bfe34ddd8430a71d66a" slicing:slice="ABCDEGH"><decl_stmt><decl><type><name>int</name></type> <name>m</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>;</decl_stmt>
	</unit>

	</unit>
OUTPUT
if [[ "$OSTYPE" == 'msys' || "$OSTYPE" == 'cygwin' ]]; then
    normalize_windows_hashes headerCustomAttributeAttributeXML
fi

createfile projectCustomAttributeAttribute.xml "$headerCustomAttributeAttributeXML"

define file1CustomAttributeAttribute <<-'EOF'
	---
	xmlns: "http://www.srcML.org/srcML/src"
	"xmlns:metrics": "http://www.srcML.org/srcML/metrics"
	language: "C++"
	filename: "n.cpp"
	"metrics:complexity": "n"
	---
	int n = 0;
EOF

define file2CustomAttributeAttribute <<-'EOF'
	---
	xmlns: "http://www.srcML.org/srcML/src"
	"xmlns:slicing": "http://www.srcML.org/srcML/slicing"
	language: "C++"
	filename: "m.cpp"
	"slicing:slice": "ABCDEGH"
	---
	int m = 0;
EOF

write_header_archive projectCustomAttributeAttribute.txt "$file1CustomAttributeAttribute" "$file2CustomAttributeAttribute"

srcml projectCustomAttributeAttribute.xml --header
check projectCustomAttributeAttribute.txt

srcml --header projectCustomAttributeAttribute.txt
check projectCustomAttributeAttribute.xml

cat projectCustomAttributeAttribute.txt | srcml
check projectCustomAttributeAttribute.xml

cat projectCustomAttributeAttribute.txt | srcml | srcml --header
check projectCustomAttributeAttribute.txt

defineXML headerURLXML <<- 'OUTPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0" url="http://github.com">

	<unit revision="1.0.0" language="C++" filename="n.cpp" hash="2b22284231f33eb19e66388951726a07ccbec135"><decl_stmt><decl><type><name>int</name></type> <name>n</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>;</decl_stmt>
	</unit>

	<unit revision="1.0.0" language="C++" filename="m.cpp" hash="4e505f4b4ab0455bc5357bfe34ddd8430a71d66a"><decl_stmt><decl><type><name>int</name></type> <name>m</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>;</decl_stmt>
	</unit>

	</unit>
OUTPUT
if [[ "$OSTYPE" == 'msys' || "$OSTYPE" == 'cygwin' ]]; then
    normalize_windows_hashes headerURLXML
fi

createfile projectURL.xml "$headerURLXML"

define file1URL <<-'EOF'
	---
	xmlns: "http://www.srcML.org/srcML/src"
	url: "http://github.com"
	language: "C++"
	filename: "n.cpp"
	---
	int n = 0;
EOF

define file2URL <<-'EOF'
	---
	xmlns: "http://www.srcML.org/srcML/src"
	language: "C++"
	filename: "m.cpp"
	---
	int m = 0;
EOF

write_header_archive projectURL.txt "$file1URL" "$file2URL"

srcml projectURL.xml --header
check projectURL.txt

srcml --header projectURL.txt
check projectURL.xml

cat projectURL.txt | srcml
check projectURL.xml

cat projectURL.txt | srcml | srcml --header
check projectURL.txt

defineXML headerRootXML <<- 'OUTPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:metrics="http://www.srcML.org/srcML/metrics" revision="1.0.0" url="http://github.com" metrics:complexity="1">

	<unit revision="1.0.0" language="C++" filename="n.cpp" hash="2b22284231f33eb19e66388951726a07ccbec135"><decl_stmt><decl><type><name>int</name></type> <name>n</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>;</decl_stmt>
	</unit>

	<unit revision="1.0.0" language="C++" filename="m.cpp" hash="4e505f4b4ab0455bc5357bfe34ddd8430a71d66a"><decl_stmt><decl><type><name>int</name></type> <name>m</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>;</decl_stmt>
	</unit>

	</unit>
OUTPUT
createfile projectRoot.xml "$headerRootXML"

defineXML headerRootOutputXML <<- 'OUTPUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0" url="http://github.com">

	<unit xmlns:metrics="http://www.srcML.org/srcML/metrics" revision="1.0.0" language="C++" filename="n.cpp" hash="2b22284231f33eb19e66388951726a07ccbec135" metrics:complexity="1"><decl_stmt><decl><type><name>int</name></type> <name>n</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>;</decl_stmt>
	</unit>

	<unit xmlns:metrics="http://www.srcML.org/srcML/metrics" revision="1.0.0" language="C++" filename="m.cpp" hash="4e505f4b4ab0455bc5357bfe34ddd8430a71d66a" metrics:complexity="1"><decl_stmt><decl><type><name>int</name></type> <name>m</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>;</decl_stmt>
	</unit>

	</unit>
OUTPUT
if [[ "$OSTYPE" == 'msys' || "$OSTYPE" == 'cygwin' ]]; then
    normalize_windows_hashes headerRootOutputXML
fi

createfile projectRootOutput.xml "$headerRootOutputXML"

define file1Root <<-'EOF'
	---
	xmlns: "http://www.srcML.org/srcML/src"
	"xmlns:metrics": "http://www.srcML.org/srcML/metrics"
	"metrics:complexity": "1"
	url: "http://github.com"
	language: "C++"
	filename: "n.cpp"
	---
	int n = 0;
EOF

define file2Root <<-'EOF'
	---
	xmlns: "http://www.srcML.org/srcML/src"
	"xmlns:metrics": "http://www.srcML.org/srcML/metrics"
	"metrics:complexity": "1"
	language: "C++"
	filename: "m.cpp"
	---
	int m = 0;
EOF

write_header_archive projectRoot.txt "$file1Root" "$file2Root"

srcml projectRoot.xml --header
check projectRoot.txt

srcml --header projectRoot.txt
check projectRootOutput.xml

cat projectRoot.txt | srcml
check projectRootOutput.xml

defineXML notHeaderOutputXML <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
STDOUT
createfile notHeaderOutput.xml "$notHeaderOutputXML"

define file1NotHeader <<-'EOF'
	-----BEGIN PRIVATE KEY-----
	ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/+=
	-----END PRIVATE KEY-----
	-----BEGIN CERTIFICATE-----
	ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/+=
	-----END CERTIFICATE-----
EOF

define file2NotHeader <<-'EOF'
	------------------------------------------------------------------------
	-- example.decTest -- example                                         --
	-- Copyright (c) ...                                                  --
	------------------------------------------------------------------------
	-- Example1                                                           --
	--                                                                    --
	-- "Example2"                                                         --
	--                                                                    --
	-- 'Example3'                                                         --
	------------------------------------------------------------------------
	version: XX.YY.ZZ

	-- Example4

	a: 0
	b: example_five
	c: 10
	d: -10

	'1' -> '1'
EOF

echo -n "$file1NotHeader" > file1NotHeader.pem
srcml file1NotHeader.pem -o file1NotHeader.pem.xml
diff notHeaderOutput.xml file1NotHeader.pem.xml

echo -n "$file2NotHeader" > file2NotHeader.decTest
srcml file2NotHeader.decTest -o file2NotHeader.decTest.xml
diff notHeaderOutput.xml file2NotHeader.decTest.xml
