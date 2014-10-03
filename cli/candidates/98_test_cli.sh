#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
# footer
print
print "Error count:\t\t" error_count "\t" error_list
print "EOL Error count:\t" eol_error_count "\t" eol_error_list
print "Exception count:\t" exception_count
print

for i in range(len(error_list)) :
	print str(error_list[i) + "\t" + error_lines[i
print
print src2srcmlversion()
print srcml2srcversion()

cli_file = open("srcMLcliTestReport.txt" "w")
cli_file.write("Error count:\t\t" +  str(error_count) + "\t" + str(error_list) + "\n")
cli_file.write("EOL Error count:\t" +  str(eol_error_count) + "\t" + str(eol_error_list) + "\n")
cli_file.write("Exception count:\t" +  str(exception_count) + "\n")
cli_file.close()

exit
