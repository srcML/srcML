#!/bin/bash

# test framework
source $(dirname $BASH_SOURCE)/framework_test.sh

# test directory input
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="dir">

	<unit revision="REVISION" language="Java" filename="dir/file.aj" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="C" filename="dir/file.c" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="C++" filename="dir/file.cpp" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="Java" filename="dir/file.java" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

xmlcheck "$output"

define output2 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="dir2">

	<unit revision="REVISION" language="C++" filename="dir2/a.cpp" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="C++" filename="dir2/b.cpp" hash="127b042b36b196e169310240b313dd9fc065ccf2">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="C++" filename="dir2/c.cpp" hash="b3b530fc0b5ee90a1e6ca6bb15d22907cde385cb">
	<expr_stmt><expr><name>c</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="C++" filename="dir2/g.cpp" hash="b32a0d6d08d49cdc2d76c5ab27c57a1f5e988884">
	<expr_stmt><expr><name>g</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="C++" filename="dir2/h.cpp" hash="704111c5fce3e467dd1089d211912d1da40c8439">
	<expr_stmt><expr><name>h</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="C++" filename="dir2/q.cpp" hash="e4493c14f70ceb409026adfdbbbbb6ce580cf5a9">
	<expr_stmt><expr><name>q</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

xmlcheck "$output2"

define output3 <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" url="symtest">

	<unit revision="REVISION" language="C++" filename="symtest/a.cpp" hash="095856ebb2712a53a4eac934fd6e69fef8e06008">
	<expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="C++" filename="symtest/b.cpp" hash="127b042b36b196e169310240b313dd9fc065ccf2">
	<expr_stmt><expr><name>b</name></expr>;</expr_stmt></unit>

	<unit revision="REVISION" language="C++" filename="symtest/c.cpp" hash="b3b530fc0b5ee90a1e6ca6bb15d22907cde385cb">
	<expr_stmt><expr><name>c</name></expr>;</expr_stmt></unit>

	</unit>
	STDOUT

xmlcheck "$output3"

# directory of just source
createfile dir/file.aj "\na;"
createfile dir/file.c  "\na;"
createfile dir/file.cpp "\na;"
createfile dir/file.java "\na;"

# Check typical source directory
srcml dir --quiet -o dir/dir.xml
check dir/dir.xml "$output"

srcml dir --quiet -o dir/dir.xml
check dir/dir.xml "$output"

srcml dir --quiet -o dir/dir.xml
check dir/dir.xml "$output"

srcml dir --quiet
check "$output"

# directory of source, compressed, and compressed archive variations
createfile dir2/a.cpp "\na;"

createfile dir2/b.cpp "\nb;"
bzip2 dir2/b.cpp

createfile dir2/c.cpp "\nc;"
bzip2 dir2/c.cpp
gzip -f dir2/c.cpp.bz2

createfile dir2/d.cpp "\nd;"
find dir2/d.cpp -print | cpio --quiet -o > dir2/d.cpp.cpio
rm dir2/d.cpp

createfile dir2/e.cpp "\ne;"
find dir2/e.cpp -print | cpio --quiet -o > dir2/e.cpp.cpio
rm dir2/e.cpp
bzip2 dir2/e.cpp.cpio

createfile dir2/f.cpp "\nf;"
find dir2/f.cpp -print | cpio --quiet -o > dir2/f.cpp.cpio
rm dir2/f.cpp
gzip -f dir2/f.cpp.cpio

createfile dir2/g.cpp "\ng;"
gzip -f dir2/g.cpp

createfile dir2/h.cpp "\nh;"
gzip -f dir2/h.cpp
bzip2 dir2/h.cpp.gz

createfile dir2/i.cpp "\ni;"
tar -cf dir2/i.cpp.tar dir2/i.cpp
rm dir2/i.cpp

createfile dir2/j.cpp "\nj;"
tar -cjf dir2/j.cpp.tar.bz2 dir2/j.cpp
rm dir2/j.cpp

createfile dir2/k.cpp "\nk;"
tar -czf dir2/k.cpp.tar.gz dir2/k.cpp
rm dir2/k.cpp

createfile dir2/l.cpp "\nl;"
tar -cjf dir2/l.cpp.tbz2 dir2/l.cpp
rm dir2/l.cpp

createfile dir2/m.cpp "\nm;"
tar -czf dir2/m.cpp.tgz dir2/m.cpp
rm dir2/m.cpp

createfile dir2/n.cpp "\nn;"
zip dir2/n.zip dir2/n.cpp -q
rm dir2/n.cpp

createfile dir2/o.cpp "\no;"
zip dir2/o.zip dir2/o.cpp -q
rm dir2/o.cpp
bzip2 dir2/o.zip

createfile dir2/p.cpp "\np;"
zip dir2/p.zip dir2/p.cpp -q
rm dir2/p.cpp
gzip -f dir2/p.zip

createfile dir2/q.cpp "\nq;"

# Check a directory with a combination of source, compressed, and archived files.
srcml dir2 --quiet -o dir2/dir2.xml
check dir2/dir2.xml "$output2"

srcml dir2 --quiet -o dir2/dir2.xml
check dir2/dir2.xml "$output2"

srcml dir2 --quiet -o dir2/dir2.xml
check dir2/dir2.xml "$output2"

srcml dir2 --quiet
check "$output2"

# #Ensure proper behavior with symbolic links (ignore them)
# createfile symtest/a.cpp "\na;"
# createfile symtest/b.cpp "\nb;"
# createfile symtest/c.cpp "\nc;"

# ln -s $(pwd)/symtest symtest/slink
# ln -s $(pwd)/symtest/b.cpp symtest/sim_b.cpp

# srcml symtest --quiet -o symtest/symtest.xml
# check symtest/symtest.xml "$output3"

# #Cleanup
# rm -rf symtest
# rm -rf dir
# rm -rf dir2

