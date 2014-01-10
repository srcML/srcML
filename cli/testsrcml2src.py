#!/usr/bin/python
#
# clitest.py
#
# Michael L. Collard
# collard@cs.kent.edu

import os.path
import subprocess
import platform
import string

import option
import status

test_count = 0
error_count = 0
eol_error_count = 0
exception_count = 0
error_list = []
eol_error_list = []
test_line = ""
error_lines = []

def check(command, input, output):

        globals()["test_count"] += 1
        globals()["test_line"] = os.path.basename(command[0]) + ' ' + ' '.join(command[1:])
        print test_count, os.path.basename(command[0]), ' '.join(command[1:])
        
        line = execute(command, input)

        return validate(line, output)
        #return validate(output.strip(), line.strip())

def checkNoOutput(command, input):

        globals()["test_count"] += 1
        globals()["test_line"] = os.path.basename(command[0]) + ' ' + ' '.join(command[1:])
        print test_count, os.path.basename(command[0]), ' '.join(command[1:])
        
        executeNoOutput(command, input)


def checkError(command, input, error) :

        globals()["test_count"] += 1
        globals()["test_line"] = os.path.basename(command[0]) + ' ' + ' '.join(command[1:])
        print test_count, os.path.basename(command[0]), ' '.join(command[1:])

        line = executeWithError(command, input)

        return validate(line, error)
        
def validate(gen, expected):
        gen = str(gen)
        expected = str(expected)

        if (platform.system() == "Windows" or platform.system() == "CYGWIN_NT-6.1") and string.find(gen, "\r\n") != -1:
                expected = string.replace(expected, "\n", "\r\n")

        if string.find(gen, "\r\n") != -1:
                globals()["eol_error_count"] += 1
                globals()["eol_error_list"].append(globals()["test_count"])

        if gen != expected:
                globals()["error_count"] = globals()["error_count"] + 1
                globals()["error_list"].append(globals()["test_count"])
                globals()["error_lines"].append(globals()["test_line"])
                print "ERROR"
                print "expected|" + expected + "|"
                print "gen|" + gen + "|"
        return

def execute(command, input):
        p = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        last_line = p.communicate(input)[0]

        if p.returncode != 0:
                globals()["error_count"] = globals()["error_count"] + 1
                print "Status error:  ", p.returncode, command

        return last_line

def executeNoOutput(command, input) :
        p = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=None, stderr=subprocess.PIPE)
        p.communicate(input)[0]

        if p.returncode != 0:
                globals()["error_count"] = globals()["error_count"] + 1
                print "Status error:  ", p.returncode, command
        
def executeWithError(command, input):
        p = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        last_line = p.communicate(input)[0]

        if p.returncode == 0:
                globals()["error_count"] = globals()["error_count"] + 1
                print "Status error:  ", p.returncode, command

        return last_line

def getreturn(command, input):
        global exception_count
        p = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        try:
                p.communicate(input)
        except OSError, (errornum, strerror):
                try:
                        p.communicate(input)
                except OSError, (errornum, strerror):
                        exception_count += 1
                        

        globals()["test_count"] += 1
        globals()["test_line"] = os.path.basename(command[0]) + ' ' + ' '.join(command[1:])
        print test_count, "Status: ", p.returncode, "\t", os.path.basename(command[0]), ' '.join(command[1:])
        return p.returncode



def checkallforms(base, shortflag, longflag, optionvalue, progin, progout):
        if base == src2srcml and (shortflag != option.LANGUAGE_FLAG_SHORT or longflag != option.LANGUAGE_FLAG_SHORT) :
                if optionvalue != "":
                        check([base, option.LANGUAGE_FLAG_SHORT, 'C++', shortflag, optionvalue], progin, progout)
                        check([base, option.LANGUAGE_FLAG_SHORT, 'C++', longflag, optionvalue], progin, progout)
                        check([base, option.LANGUAGE_FLAG_SHORT, 'C++', longflag + "=" + optionvalue], progin, progout)
                else:
                        check([base, option.LANGUAGE_FLAG_SHORT, 'C++', shortflag], progin, progout)
                        check([base, option.LANGUAGE_FLAG_SHORT, 'C++', longflag], progin, progout)

        else :
                if optionvalue != "":
                        check([base, shortflag, optionvalue], progin, progout)
                        check([base, longflag, optionvalue], progin, progout)
                        check([base, longflag + "=" + optionvalue], progin, progout)
                else:
                        check([base, shortflag], progin, progout)
                        check([base, longflag], progin, progout)

        return

def checkallformsfile(base, inputfile, shortflag, longflag, optionvalue, progin, progout):
        if optionvalue != "":
                check([base, inputfile, shortflag, optionvalue], progin, progout)
                check([base, inputfile, longflag, optionvalue], progin, progout)
                check([base, inputfile, longflag + "=" + optionvalue], progin, progout)
        else:
                check([base, inputfile, shortflag], progin, progout)
                check([base, inputfile, longflag], progin, progout)

        return
        
def checkallformsext(base, shortflag, longflag, optionvalue, other, progin, progout):
        if optionvalue != "":
                check([base, shortflag, optionvalue, other], progin, progout)
                check([base, longflag, optionvalue, other], progin, progout)
                check([base, longflag + "=" + optionvalue, other], progin, progout)
        else:
                check([base, shortflag, other], progin, progout)
                check([base, longflag, other], progin, progout)

        return
        
# version of srcml2src
def srcml2srcversion():

        last_line = subprocess.Popen([srcml2src, "-V"],
                                     stdout=subprocess.PIPE).communicate()[0]

        return last_line.strip()

srcml2src = os.environ.get("SRCML2SRC")
if srcml2src == "" or srcml2src == None:
        if platform.system() != "Windows":
                srcml2src = "../../bin/srcml2src"
        else :
                srcml2src = "srcml2src.exe"

default_srcml2src_encoding = "UTF-8"

xml_declaration= '<?xml version="1.0" encoding="' + default_srcml2src_encoding + '" standalone="yes"?>'

print xml_declaration

# xpath

# xpath numeric

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

    <unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C" filename="emptysrc/empty.c"/>

    <unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="emptysrc/empty.c++"/>

    <unit language="Java" filename="emptysrc/empty.java"/>

</unit>
"""

xpath = option.XPATH_FLAG + '=count(/src:unit)'

check([srcml2src, xpath], srcml, "3\n")

# footer
print
print "Error count:\t\t", error_count, "\t", error_list
print "EOL Error count:\t", eol_error_count, "\t", eol_error_list
print "Exception count:\t", exception_count
print

for i in range(len(error_list)) :
        print str(error_list[i]) + "\t" + error_lines[i]
print
print srcml2srcversion()

exit
