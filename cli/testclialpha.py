#!/usr/bin/python
#
# testcli.py

import sys
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

limit = 0

def check(command, input, output):

        if sys.platform == 'cygwin' and input != "" :
                return

        globals()["test_count"] += 1
        if globals()["limit"] != 0 and globals()["test_count"] > globals()["limit"]:
                exit(0)

        globals()["test_line"] = os.path.basename(command[0]) + ' ' + ' '.join(command[1:])
        print test_count, os.path.basename(command[0]), ' '.join(command[1:])
        
        line = execute(command, input)

        return validateWithInput(input, line, output)
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

        return validateWithInput(input, line, error)
        
def validateWithInput(input, gen, expected):
        gen = str(gen)
        expected = str(expected)

        if (platform.system() == "Windows" or platform.system() == "CYGWIN_NT-6.1" or sys.platform == 'cygwin') and string.find(gen, "\r\n") != -1:
                expected = string.replace(expected, "\n", "\r\n")

        if (platform.system() != "Windows" and platform.system() != "CYGWIN_NT-6.1" and sys.platform != 'cygwin') and string.find(gen, "\r\n") != -1:
                globals()["eol_error_count"] += 1
                globals()["eol_error_list"].append(globals()["test_count"])

        if gen != expected:
                globals()["error_count"] = globals()["error_count"] + 1
                globals()["error_list"].append(globals()["test_count"])
                globals()["error_lines"].append(globals()["test_line"])
                print "ERROR"
                print "input|" + input + "|"
                print "expected|" + expected + "|"
                print "gen|" + gen + "|"
        return

def validate(gen, expected):
        gen = str(gen)
        expected = str(expected)

        if (platform.system() == "Windows" or platform.system() == "CYGWIN_NT-6.1" or sys.platform == 'cygwin') and string.find(gen, "\r\n") != -1:
                expected = string.replace(expected, "\n", "\r\n")

        if (platform.system() != "Windows" and platform.system() != "CYGWIN_NT-6.1" and sys.platform != 'cygwin') and string.find(gen, "\r\n") != -1:
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
                globals()["error_list"].append(globals()["test_count"])
                globals()["error_lines"].append(globals()["test_line"])
                print "Status error:  ", p.returncode, command

        return last_line

def executeNoOutput(command, input) :
        p = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=None, stderr=subprocess.PIPE)
        p.communicate(input)[0]

        if p.returncode != 0:
                globals()["error_count"] = globals()["error_count"] + 1
                globals()["error_list"].append(globals()["test_count"])
                globals()["error_lines"].append(globals()["test_line"])
                print "Status error:  ", p.returncode, command
        
def executeWithError(command, input):
        p = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        last_line = p.communicate(input)[0]

        if p.returncode == 0:
                globals()["error_count"] = globals()["error_count"] + 1
                globals()["error_list"].append(globals()["test_count"])
                globals()["error_lines"].append(globals()["test_line"])
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
        if base == src2srcml and not(shortflag == option.LANGUAGE_FLAG_SHORT or longflag == option.LANGUAGE_FLAG_FULL) :
                if optionvalue != "":
                        if shortflag != "":
                                check([base, option.LANGUAGE_FLAG_SHORT, 'C++', shortflag, optionvalue], progin, progout)
                        check([base, option.LANGUAGE_FLAG_SHORT, 'C++', longflag, optionvalue], progin, progout)
                        check([base, option.LANGUAGE_FLAG_SHORT, 'C++', longflag + "=" + optionvalue], progin, progout)
                else:
                        if shortflag != "":
                                check([base, option.LANGUAGE_FLAG_SHORT, 'C++', shortflag], progin, progout)
                        check([base, option.LANGUAGE_FLAG_SHORT, 'C++', longflag], progin, progout)

        else :
                if optionvalue != "":
                        if shortflag != "":
                                check([base, shortflag, optionvalue], progin, progout)
                        check([base, longflag, optionvalue], progin, progout)
                        check([base, longflag + "=" + optionvalue], progin, progout)
                else:
                        if shortflag != "":
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
        
# version of src2srcml
def src2srcmlversion():

        last_line = subprocess.Popen([src2srcml, "-V"],
                                     stdout=subprocess.PIPE).communicate()[0]

        return last_line.strip()

# version of srcml2src
def srcml2srcversion():

        last_line = subprocess.Popen([srcml2src, "-V"],
                                     stdout=subprocess.PIPE).communicate()[0]

        return last_line.strip()

src2srcml = os.environ.get("SRC2SRCML")
if src2srcml == "" or src2srcml == None:
        if sys.platform == 'cygwin' :
                src2srcml = "../bin/srcml.exe"
        elif os.path.exists('../bin/srcml') :
                src2srcml = "../bin/srcml"
        elif os.path.exists('../bin/srcml.exe') :
                src2srcml = "../bin/srcml.exe"
        else :
                print "src2srcml Not found"
                exit
        
srcml2src = os.environ.get("SRCML2SRC")
if srcml2src == "" or srcml2src == None:
        if sys.platform == 'cygwin' :
                srcml2src = "../bin/srcml.exe"
        elif os.path.exists('../bin/srcml') :
                srcml2src = "../bin/srcml"
        elif os.path.exists('../bin/srcml.exe') :
                srcml2src = "../bin/srcml.exe"
        else :
                print "srcml2src Not found"
                exit

