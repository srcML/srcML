#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# verify it works
echo -en "\n"
check_lines 1

echo -en "\r\n" | srcml -l C | srcml
check_lines 1

echo -en "\r" | srcml -l C | srcml
check_lines 1

echo -en "\n\n"
check_lines 2

echo -en "\r\n\r\n" | srcml -l C | srcml
check_lines 2

echo -en "\r\r" | srcml -l C | srcml
check_lines 2

echo -en "\n\r" | srcml -l C | srcml
check_lines 2

echo -en "\n\r\n" | srcml -l C | srcml
check_lines 2

echo -en "\r\r\n" | srcml -l C | srcml
check_lines 2

echo -en "\n\n\n"
check_lines 3

echo -en "\r\n\r\n\r\n" | srcml -l C | srcml
check_lines 3

echo -en "\r\r\r" | srcml -l C | srcml
check_lines 3

echo -en "\r\n\r\n\r" | srcml -l C | srcml
check_lines 3

echo -en "\r\n\n\r" | srcml -l C | srcml
check_lines 3

echo -en "\n\r\n\n" | srcml -l C | srcml
check_lines 3

echo -en "\n\r\n\r" | srcml -l C | srcml
check_lines 3

echo -en "\r\r\n\n" | srcml -l C | srcml
check_lines 3

echo -en "\r\r\n\r" | srcml -l C | srcml
check_lines 3
