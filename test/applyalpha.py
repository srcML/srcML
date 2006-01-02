#!/usr/bin/python
#
# update.py
#
# Michael L. Collard

import sys
import os.path

filename = sys.argv[1]

command = "/home/collard/bin/src2srcml.alpha < " + filename + " > " + filename + ".xml.alpha"

result = os.system(command)
if result != 0:
	print "Error:  " + filename
	command = "cp " + filename + " " + filename + ".prob.alpha"
	os.system(command)
else:
	print "Good:  " + filename
exit
