#!/usr/bin/python
#
# update.py
#
# Michael L. Collard

import sys
import os.path

filename = sys.argv[1]
query = sys.argv[2]

command = "xpath -e \"" + query + "\" " + filename

result = os.system(command)

