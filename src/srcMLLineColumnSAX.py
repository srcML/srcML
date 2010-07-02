#!/usr/bin/python

import sys
from xml.sax import parse
from xml.sax.handler import ContentHandler

##
# SAX Hander to add a line and column attribute
#
# Michael Decker
# June 2, 2010
#
class srcMLLineColumnSAXHandler (ContentHandler) :

    # Init method to create variables
    def __init__(self, outfile) :
        self.column = 1
        self.line = 1
        self.outfile = open(outfile, 'w')

    # Empty Start Document
    def startDocument(self) :
        self.outfile.write('<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n')

    # Empty End Document
    def endDocument(self) :
        pass

    # Parse to obtain versions, and container names
    def startElement(self, name, attrs) :
            self.outfile.write('<' + name)
            for attribute in attrs.getNames() :
                self.outfile.write(' ' + attribute + '="' + attrs.getValue(attribute) + '"')
            if name != 'unit' :
                self.outfile.write(' ' + 'linecolumn="' + str(self.line) + ':' + str(self.column) + '"')
            self.outfile.write('>')

    def endElement(self, name) :
        self.outfile.write('</' + name + '>')

    def characters(self, content) :
        for character in content :
            if character == '\n' :
                self.column = 1
                self.line += 1
            else :
                self.column += 1
        self.outfile.write(content)

# create first handler
handler = srcMLLineColumnSAXHandler(sys.argv[2])

# parse first file
parse(sys.argv[1], handler)
