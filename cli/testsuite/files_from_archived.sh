#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# files from an archived file list
define error <<- 'STDOUT'
	ERROR
	STDOUT

createfile archive/a.cpp "a;"
createfile list.txt "archive/a.cpp"


# cpio
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.cpio"/>
	STDOUT

echo list.txt | tr " " "\n" | cpio --quiet -o > list.txt.cpio

src2srcml --files-from list.txt.cpio
check 3<<< "$archive_output" 4<<< "$error"

src2srcml --files-from list.txt.cpio -o archive/list.xml
check archive/list.xml 3<<< "$archive_output" 4<<< "$error"

# cpio.bz2
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.cpio.bz2"/>
	STDOUT

bzip2 -c list.txt.cpio > list.txt.cpio.bz2

src2srcml --files-from list.txt.cpio.bz2
check 3<<< "$archive_output" 4<<< "$error"

src2srcml --files-from list.txt.cpio.bz2 -o archive/list.xml
check archive/list.xml 3<<< "$archive_output" 4<<< "$error"

# cpio.gz
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.cpio.gz"/>
	STDOUT

gzip -c list.txt.cpio > archive/a.cpp.cpio.gz

src2srcml --files-from list.txt.cpio.gz
check 3<<< "$archive_output" 4<<< "$error"

src2srcml --files-from list.txt.cpio.gz -o archive/list.xml
check archive/list.xml 3<<< "$archive_output" 4<<< "$error"

# tar
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.tar"/>
	STDOUT

tar -cf list.txt.tar list.txt

src2srcml --files-from list.txt.tar
check 3<<< "$archive_output" 4<<< "$error"

src2srcml --files-from list.txt.tar -o archive/list.xml
check archive/list.xml 3<<< "$archive_output" 4<<< "$error"

# tar.bz2
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.tar.bz2"/>
	STDOUT

tar -cjf list.txt.tar.bz2 list.txt

src2srcml --files-from list.txt.tar.bz2
check 3<<< "$archive_output" 4<<< "$error"

src2srcml --files-from list.txt.tar.bz2 -o archive/list.xml
check archive/list.xml 3<<< "$archive_output" 4<<< "$error"

# tar.gz
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.tar.gz"/>
	STDOUT

tar -czf list.txt.tar.gz list.txt

src2srcml --files-from list.txt.tar.gz
check 3<<< "$archive_output" 4<<< "$error"

src2srcml --files-from list.txt.tar.gz -o archive/list.xml
check archive/list.xml 3<<< "$archive_output" 4<<< "$error"

# tbz2
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.tbz2"/>
	STDOUT

tar -cjf list.txt.tbz2 list.txt

src2srcml --files-from list.txt.tbz2
check 3<<< "$archive_output" 4<<< "$error"

src2srcml --files-from list.txt.tbz2 -o archive/list.xml
check archive/list.xml 3<<< "$archive_output" 4<<< "$error"

# tgz
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.tgz"/>
	STDOUT

tar -czf list.txt.tgz list.txt

src2srcml --files-from list.txt.tgz
check 3<<< "$archive_output" 4<<< "$error"

src2srcml --files-from list.txt.tgz -o archive/list.xml
check archive/list.xml 3<<< "$archive_output" 4<<< "$error"

# zip
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.zip"/>
	STDOUT

zip --quiet list.txt.zip list.txt

src2srcml --files-from list.txt.zip
check 3<<< "$archive_output" 4<<< "$error"

src2srcml --files-from list.txt.zip -o archive/list.xml
check archive/list.xml 3<<< "$archive_output" 4<<< "$error"

# zip.bz2
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.zip.bz2"/>
	STDOUT

bzip2 -c list.txt.zip > list.txt.zip.bz2

src2srcml --files-from list.txt.zip.bz2
check 3<<< "$archive_output" 4<<< "$error"

src2srcml --files-from list.txt.zip.bz2 -o archive/list.xml
check archive/list.xml 3<<< "$archive_output" 4<<< "$error"

# zip.gz
define archive_output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="list.txt.zip.gz"/>
	STDOUT

gzip -c list.txt.zip > list.txt.zip.gz

src2srcml --files-from list.txt.zip.gz
check 3<<< "$archive_output" 4<<< "$error"

src2srcml --files-from list.txt.zip.gz -o archive/list.xml
check archive/list.xml 3<<< "$archive_output" 4<<< "$error"
