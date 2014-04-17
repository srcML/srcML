#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

srcml -l C++ << 'STDIN'
 
STDIN

check 3<< 'STDOUT'
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"> 
</unit>
STDOUT

srcml --language C++ << 'STDIN'
 
STDIN

check 3<< 'STDOUT'
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"> 
</unit>
STDOUT

srcml --language=C++ << 'STDIN'
 
STDIN

check 3<< 'STDOUT'
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"> 
</unit>
STDOUT

srcml --language="C++" << 'STDIN'
 
STDIN

check 3<< 'STDOUT'
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"> 
</unit>
STDOUT

srcml --language='C++' << 'STDIN'
 
STDIN

check 3<< 'STDOUT'
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"> 
</unit>
STDOUT

srcml --language='C' << 'STDIN'
 
STDIN

check 3<< 'STDOUT'
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C"> 
</unit>
STDOUT

srcml -l Java << 'STDIN'
 
STDIN

check 3<< 'STDOUT'
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" language="Java"> 
</unit>
STDOUT

srcml -l C# << 'STDIN'
 
STDIN

check 3<< 'STDOUT'
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C#"> 
</unit>
STDOUT
