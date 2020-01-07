#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

define axml <<- 'STDOUT'
  <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
  <unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
  </unit>
  STDOUT

define bxml <<- 'STDOUT'
  <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
  <unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="b.cpp"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
  </unit>
  STDOUT

createfile "a.xml" "$axml"
createfile "b.xml" "$bxml"

define output <<- 'STDOUT'
  <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
  <unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

  <unit revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
  </unit>

  <unit revision="REVISION" language="C++" filename="b.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
  </unit>

  </unit>
  STDOUT

# files from has a list of srcml units
createfile "list.txt" "a.xml
b.xml"

srcml --files-from list.txt
check "$output"

srcml --files-from list.txt -o output.xml
check output.xml "$output"
