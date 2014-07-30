#!/bin/sh

cat CPP-ctor-2.cpp | srcml --language=C++ --operator --modifier --literal 

echo $?
