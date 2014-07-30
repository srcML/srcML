#!/usr/bin/python

import os, sys, xml.sax, subprocess, xml.dom.minidom as minidom, traceback
from xml.dom.minidom import parse
from os.path import join, getsize
import xml.etree.ElementTree as ET

strm = open("CPP-ctor-2.cpp", "r")
code = "".join(strm.readlines())

print code

srcMLProc = subprocess.Popen(["srcml", "--language", "C++", "--literal", "--modifier", "--operator"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
srcMLProc.stdin.write(code)
srcMLProc.stdin.flush()
srcMLProc.stdin.close()
# linesTemp = ""
# srcMLProc.wait()
errOutput = srcMLProc.stderr.readlines()
# print dir(srcMLProc)
print "OutputFrom srd Error:", "".join(errOutput)
print "Parsing input"
linesTemp = "".join(srcMLProc.stdout.readlines())
print "\"{0}\"".format(linesTemp)
exampleDOM = minidom.parseString(linesTemp)
