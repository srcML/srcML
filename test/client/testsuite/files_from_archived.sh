#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# files from an archived file list
define error <<- 'STDOUT'
	srcml: filelist requires a non-archived file format
	STDOUT

createfile archive/a.cpp "a;"
createfile list.txt "archive/a.cpp"

# cpio
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.cpio"/>
	STDOUT

echo list.txt | tr " " "\n" | cpio --quiet -o > list.txt.cpio

srcml --files-from list.txt.cpio
check "$archive_output" "$error"

srcml --files-from list.txt.cpio -o archive/list.xml
check archive/list.xml "$archive_output" "$error"

# cpio.bz2
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.cpio.bz2"/>
	STDOUT

bzip2 -c list.txt.cpio > list.txt.cpio.bz2

srcml --files-from list.txt.cpio.bz2
check "$archive_output" "$error"

srcml --files-from list.txt.cpio.bz2 -o archive/list.xml
check archive/list.xml "$archive_output" "$error"

# cpio.gz
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.cpio.gz"/>
	STDOUT

gzip -c list.txt.cpio > list.txt.cpio.gz

srcml --files-from list.txt.cpio.gz
check "$archive_output" "$error"

srcml --files-from list.txt.cpio.gz -o archive/list.xml
check archive/list.xml "$archive_output" "$error"

# tar
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.tar"/>
	STDOUT

tar -cf list.txt.tar list.txt

srcml --files-from list.txt.tar
check "$archive_output" "$error"

srcml --files-from list.txt.tar -o archive/list.xml
check archive/list.xml "$archive_output" "$error"

# tar.bz2
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.tar.bz2"/>
	STDOUT

tar -cjf list.txt.tar.bz2 list.txt

srcml --files-from list.txt.tar.bz2
check "$archive_output" "$error"

srcml --files-from list.txt.tar.bz2 -o archive/list.xml
check archive/list.xml "$archive_output" "$error"

# tar.gz
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.tar.gz"/>
	STDOUT

tar -czf list.txt.tar.gz list.txt

srcml --files-from list.txt.tar.gz
check "$archive_output" "$error"

srcml --files-from list.txt.tar.gz -o archive/list.xml
check archive/list.xml "$archive_output" "$error"

# tbz2
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.tbz2"/>
	STDOUT

tar -cjf list.txt.tbz2 list.txt

srcml --files-from list.txt.tbz2
check "$archive_output" "$error"

srcml --files-from list.txt.tbz2 -o archive/list.xml
check archive/list.xml "$archive_output" "$error"

# tgz
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.tgz"/>
	STDOUT

tar -czf list.txt.tgz list.txt

srcml --files-from list.txt.tgz
check "$archive_output" "$error"

srcml --files-from list.txt.tgz -o archive/list.xml
check archive/list.xml "$archive_output" "$error"

# zip
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.zip"/>
	STDOUT

zip --quiet list.txt.zip list.txt

srcml --files-from list.txt.zip
check "$archive_output" "$error"

srcml --files-from list.txt.zip -o archive/list.xml
check archive/list.xml "$archive_output" "$error"

# zip.bz2
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.zip.bz2"/>
	STDOUT

bzip2 -c list.txt.zip > list.txt.zip.bz2

srcml --files-from list.txt.zip.bz2
check "$archive_output" "$error"

srcml --files-from list.txt.zip.bz2 -o archive/list.xml
check archive/list.xml "$archive_output" "$error"

# zip.gz
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.zip.gz"/>
	STDOUT

gzip -c list.txt.zip > list.txt.zip.gz

srcml --files-from list.txt.zip.gz
check "$archive_output" "$error"

srcml --files-from list.txt.zip.gz -o archive/list.xml
check archive/list.xml "$archive_output" "$error"
