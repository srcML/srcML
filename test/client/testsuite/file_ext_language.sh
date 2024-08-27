#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
#
# @file file_ext_language.sh
#
# @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)

# test framework
source $(dirname "$0")/framework_test.sh

# test
##
# check correct language based on file extension

# check C file extensions
define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C" filename="emptysrc/empty.c"/>
	STDOUT

createfile emptysrc/empty.c ""

srcml emptysrc/empty.c
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C" filename="emptysrc/empty.c"/>
	STDOUT

createfile emptysrc/empty.c.gz ""

srcml emptysrc/empty.c.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C" filename="emptysrc/empty.h"/>
	STDOUT

createfile emptysrc/empty.h ""

srcml emptysrc/empty.h
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C" filename="emptysrc/empty.h"/>
	STDOUT

createfile emptysrc/empty.h.gz ""

srcml emptysrc/empty.h.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C" filename="emptysrc/empty.i"/>
	STDOUT

createfile emptysrc/empty.i ""

srcml emptysrc/empty.i
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C" filename="emptysrc/empty.i"/>
	STDOUT

createfile emptysrc/empty.i.gz ""

srcml emptysrc/empty.i.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C" filename="emptysrc/empty.cu"/>
	STDOUT

createfile emptysrc/empty.cu ""

srcml emptysrc/empty.cu
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C" filename="emptysrc/empty.cu"/>
	STDOUT

createfile emptysrc/empty.cu.gz ""

srcml emptysrc/empty.cu.gz
check "$checkvar"

# check C++ file extensions
define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.cpp"/>
	STDOUT

createfile emptysrc/empty.cpp ""

srcml emptysrc/empty.cpp
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.cpp"/>
	STDOUT

createfile emptysrc/empty.cpp.gz ""

srcml emptysrc/empty.cpp.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.CPP"/>
	STDOUT

createfile emptysrc/empty.CPP ""

srcml emptysrc/empty.CPP
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.CPP"/>
	STDOUT

createfile emptysrc/empty.CPP.gz ""

srcml emptysrc/empty.CPP.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.cppm"/>
	STDOUT

createfile emptysrc/empty.cppm ""

srcml emptysrc/empty.cppm
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.cppm"/>
	STDOUT

createfile emptysrc/empty.cppm.gz ""

srcml emptysrc/empty.cppm.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.cp"/>
	STDOUT

createfile emptysrc/empty.cp ""

srcml emptysrc/empty.cp
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.cp"/>
	STDOUT

createfile emptysrc/empty.cp.gz ""

srcml emptysrc/empty.cp.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.hpp"/>
	STDOUT

createfile emptysrc/empty.hpp ""

srcml emptysrc/empty.hpp
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.hpp"/>
	STDOUT

createfile emptysrc/empty.hpp.gz ""

srcml emptysrc/empty.hpp.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.mpp"/>
	STDOUT

createfile emptysrc/empty.mpp ""

srcml emptysrc/empty.mpp
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.mpp"/>
	STDOUT

createfile emptysrc/empty.mpp.gz ""

srcml emptysrc/empty.mpp.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.cxx"/>
	STDOUT

createfile emptysrc/empty.cxx ""

srcml emptysrc/empty.cxx
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.cxx"/>
	STDOUT

createfile emptysrc/empty.cxx.gz ""

srcml emptysrc/empty.cxx.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.hxx"/>
	STDOUT

createfile emptysrc/empty.hxx ""

srcml emptysrc/empty.hxx
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.hxx"/>
	STDOUT

createfile emptysrc/empty.hxx.gz ""

srcml emptysrc/empty.hxx.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.ixx"/>
	STDOUT

createfile emptysrc/empty.ixx ""

srcml emptysrc/empty.ixx
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.ixx"/>
	STDOUT

createfile emptysrc/empty.ixx.gz ""

srcml emptysrc/empty.ixx.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.cc"/>
	STDOUT

createfile emptysrc/empty.cc ""

srcml emptysrc/empty.cc
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.cc"/>
	STDOUT

createfile emptysrc/empty.cc.gz ""

srcml emptysrc/empty.cc.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.ccm"/>
	STDOUT

createfile emptysrc/empty.ccm ""

srcml emptysrc/empty.ccm
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.ccm"/>
	STDOUT

createfile emptysrc/empty.ccm.gz ""

srcml emptysrc/empty.ccm.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.hh"/>
	STDOUT

createfile emptysrc/empty.hh ""

srcml emptysrc/empty.hh
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.hh"/>
	STDOUT

createfile emptysrc/empty.hh.gz ""

srcml emptysrc/empty.hh.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.hm"/>
	STDOUT

createfile emptysrc/empty.hm ""

srcml emptysrc/empty.hm
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.hm"/>
	STDOUT

createfile emptysrc/empty.hm.gz ""

srcml emptysrc/empty.hm.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.c++"/>
	STDOUT

createfile emptysrc/empty.c++ ""

srcml emptysrc/empty.c++
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.c++"/>
	STDOUT

createfile emptysrc/empty.c++.gz ""

srcml emptysrc/empty.c++.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.c++m"/>
	STDOUT

createfile emptysrc/empty.c++m ""

srcml emptysrc/empty.c++m
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.c++m"/>
	STDOUT

createfile emptysrc/empty.c++m.gz ""

srcml emptysrc/empty.c++m.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.h++"/>
	STDOUT

createfile emptysrc/empty.h++ ""

srcml emptysrc/empty.h++
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.h++"/>
	STDOUT

createfile emptysrc/empty.h++.gz ""

srcml emptysrc/empty.h++.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.C"/>
	STDOUT

createfile emptysrc/empty.C ""

srcml emptysrc/empty.C
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.C"/>
	STDOUT

createfile emptysrc/empty.C.gz ""

srcml emptysrc/empty.C.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.H"/>
	STDOUT

createfile emptysrc/empty.H ""

srcml emptysrc/empty.H
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.H"/>
	STDOUT

createfile emptysrc/empty.H.gz ""

srcml emptysrc/empty.H.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.tcc"/>
	STDOUT

createfile emptysrc/empty.tcc ""

srcml emptysrc/empty.tcc
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.tcc"/>
	STDOUT

createfile emptysrc/empty.tcc.gz ""

srcml emptysrc/empty.tcc.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.txx"/>
	STDOUT

createfile emptysrc/empty.txx ""

srcml emptysrc/empty.txx
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.txx"/>
	STDOUT

createfile emptysrc/empty.txx.gz ""

srcml emptysrc/empty.txx.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.ii"/>
	STDOUT

createfile emptysrc/empty.ii ""

srcml emptysrc/empty.ii
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C++" filename="emptysrc/empty.ii"/>
	STDOUT

createfile emptysrc/empty.ii.gz ""

srcml emptysrc/empty.ii.gz
check "$checkvar"

# check Java file extensions
define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Java" filename="emptysrc/empty.java"/>
	STDOUT

createfile emptysrc/empty.java ""

srcml emptysrc/empty.java
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Java" filename="emptysrc/empty.java"/>
	STDOUT

createfile emptysrc/empty.java.gz ""

srcml emptysrc/empty.java.gz
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Java" filename="emptysrc/empty.aj"/>
	STDOUT

createfile emptysrc/empty.aj ""

srcml emptysrc/empty.aj
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="Java" filename="emptysrc/empty.aj"/>
	STDOUT

createfile emptysrc/empty.aj.gz ""

srcml emptysrc/empty.aj.gz
check "$checkvar"

# check C# file extensions
define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C#" filename="emptysrc/empty.cs"/>
	STDOUT

createfile emptysrc/empty.cs ""

srcml emptysrc/empty.cs
check "$checkvar"

define checkvar <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" language="C#" filename="emptysrc/empty.cs"/>
	STDOUT

createfile emptysrc/empty.cs.gz ""

srcml emptysrc/empty.cs.gz
check "$checkvar"
