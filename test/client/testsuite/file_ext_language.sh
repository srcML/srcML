#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# check correct language based on file extension

createfile emptysrc/empty.c ""
src2srcml emptysrc/empty.c

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C" filename="emptysrc/empty.c"/>
'

createfile emptysrc/empty.h ""
src2srcml emptysrc/empty.h

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C" filename="emptysrc/empty.h"/>
'

createfile emptysrc/empty.c.gz ""
src2srcml emptysrc/empty.c.gz

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C" filename="emptysrc/empty.c.gz"/>
'


createfile emptysrc/empty.h.gz ""
src2srcml emptysrc/empty.h.gz

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C" filename="emptysrc/empty.h.gz"/>
'

#check c++ file extensions

createfile emptysrc/empty.cpp ""
src2srcml emptysrc/empty.cpp

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.cpp"/>
'

createfile emptysrc/empty.cc ""
src2srcml emptysrc/empty.cc

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.cc"/>
'

createfile emptysrc/empty.cxx ""
src2srcml emptysrc/empty.cxx

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.cxx"/>
'

createfile emptysrc/empty.c++ ""
src2srcml emptysrc/empty.c++

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.c++"/>
'

createfile emptysrc/empty.hpp ""
src2srcml emptysrc/empty.hpp

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.hpp"/>
'

createfile emptysrc/empty.hh ""
src2srcml emptysrc/empty.hh

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.hh"/>
'

createfile emptysrc/empty.hxx ""
src2srcml emptysrc/empty.hxx

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.hxx"/>
'

createfile emptysrc/empty.h++ ""
src2srcml emptysrc/empty.h++

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.h++"/>
'

createfile emptysrc/empty.tcc ""
src2srcml emptysrc/empty.tcc

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.tcc"/>
'

createfile emptysrc/empty.cpp.gz ""
src2srcml emptysrc/empty.cpp.gz

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.cpp.gz"/>
'

createfile emptysrc/empty.cc.gz ""
src2srcml emptysrc/empty.cc.gz

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.cc.gz"/>
'

createfile emptysrc/empty.cxx.gz ""
src2srcml emptysrc/empty.cxx.gz

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.cxx.gz"/>
'

createfile emptysrc/empty.c++.gz ""
src2srcml emptysrc/empty.c++.gz

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.c++.gz"/>
'

createfile emptysrc/empty.hpp.gz ""
src2srcml emptysrc/empty.hpp.gz

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.hpp.gz"/>
'

createfile emptysrc/empty.hh.gz ""
src2srcml emptysrc/empty.hh.gz

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.hh.gz"/>
'

createfile emptysrc/empty.hxx.gz ""
src2srcml emptysrc/empty.hxx.gz

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.hxx.gz"/>
'

createfile emptysrc/empty.h++.gz ""
src2srcml emptysrc/empty.h++.gz

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.h++.gz"/>
'

createfile emptysrc/empty.tcc.gz ""
src2srcml emptysrc/empty.tcc.gz

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="emptysrc/empty.tcc.gz"/>
'


# check java file extension
createfile emptysrc/empty.java ""
src2srcml emptysrc/empty.java

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Java" filename="emptysrc/empty.java"/>
'

createfile emptysrc/empty.java.gz ""
src2srcml emptysrc/empty.java.gz

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Java" filename="emptysrc/empty.java.gz"/>
'

createfile emptysrc/empty.aj.gz ""
src2srcml emptysrc/empty.aj.gz

check '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Java" filename="emptysrc/empty.aj.gz"/>
'
