#!/usr/bin/python
#
# update.py
#
# Michael L. Collard

import sys
import os.path
import stat
import string

def filesize(filename):
	return os.stat(filename)[stat.ST_SIZE]

diffname = sys.argv[1]

filename_base = string.replace(diffname, ".diff.xml", "")
filename_base = string.replace(filename_base, ".diff.prob", "")

filename_base_xml = filename_base + ".xml"

filename_other_base = "/home/collard/plat/HippoDraw-1.5.1/" + filename_base
filename_other_base_xml = filename_other_base + ".xml"

#filename_diff = filename_base + ".diff.xml"
filename_diff = diffname

print filename_base, filesize(filename_base), filesize(filename_base_xml), filesize(filename_other_base), filesize(filename_other_base_xml), filesize(filename_diff)

exit
