#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
srcml = srcmlstart

for file in dir :
	if file == 'file.aj' :
	srcml += aj
	if file == 'file.c' :
	srcml += c
	if file == 'file.cpp' :
	srcml += cpp
	if file == 'file.java' :
	srcml += java

