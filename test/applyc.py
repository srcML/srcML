#!/usr/bin/python
#
# update.py
#
# Michael L. Collard

import sys
import os.path

filename = sys.argv[1]

command = "/home/collard/bin/src2srcml -l C " + filename + " " + filename + ".xml"

result = os.system(command)
if result != 0:
	print "Error:  " + filename
	command = "cp " + filename + " " + filename + ".prob"
	os.system(command)
else:
	print "Good:  " + filename
exit
