#!/usr/bin/python
#
# libarchivetest.py
#
# Michael J. Decker
# mjd52@uakron.edu

import os.path
import subprocess

import option
import status

test_count = 0
error_count = 0

def check(command, input, output):

	print os.path.basename(command[0]), ' '.join(command[1:])
	
	line = execute(command, input)

	return validate(line, output)
	
def validate(gen, expected):
	if gen != expected:
		globals()['error_count'] = globals()['error_count'] + 1
		print 'ERROR'
		print 'expected|' + str(expected) + '|'
		print 'gen|' + str(gen) + '|'
	return

def execute(command, input):
	p = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	last_line = p.communicate(input)[0]

	if p.returncode != 0:
		globals()['error_count'] = globals()['error_count'] + 1
		print 'Status error:  ', p.returncode, command

	return last_line

def getreturn(command, input):
	p = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	p.communicate(input)
	print 'Status: ', p.returncode, '\t', os.path.basename(command[0]), ' '.join(command[1:])
	return p.returncode

def checkallforms(base, shortflag, longflag, optionvalue, progin, progout):
	if base == src2srcml and (shortflag != option.LANGUAGE_FLAG_SHORT or longflag != option.LANGUAGE_FLAG_SHORT) :
		if optionvalue != '':
			check([base, option.LANGUAGE_FLAG_SHORT, 'C++', shortflag, optionvalue], progin, progout)
			check([base, option.LANGUAGE_FLAG_SHORT, 'C++', longflag, optionvalue], progin, progout)
			check([base, option.LANGUAGE_FLAG_SHORT, 'C++', longflag + '=' + optionvalue], progin, progout)
		else:
			check([base, option.LANGUAGE_FLAG_SHORT, 'C++', shortflag], progin, progout)
			check([base, option.LANGUAGE_FLAG_SHORT, 'C++', longflag], progin, progout)

	else :
		if optionvalue != '':
		       	check([base, shortflag, optionvalue], progin, progout)
		       	check([base, longflag, optionvalue], progin, progout)
		       	check([base, longflag + '=' + optionvalue], progin, progout)
		else:
			check([base, shortflag], progin, progout)
			check([base, longflag], progin, progout)

	return

def checkallformsfile(base, inputfile, shortflag, longflag, optionvalue, progin, progout):
	if optionvalue != '':
		check([base, inputfile, shortflag, optionvalue], progin, progout)
		check([base, inputfile, longflag, optionvalue], progin, progout)
		check([base, inputfile, longflag + '=' + optionvalue], progin, progout)
	else:
		check([base, inputfile, shortflag], progin, progout)
		check([base, inputfile, longflag], progin, progout)

	return
	
# version of src2srcml
def src2srcmlversion():

	last_line = subprocess.Popen([src2srcml, '-V'],
				     stdout=subprocess.PIPE).communicate()[0]

	return last_line.strip()

# version of srcml2src
def srcml2srcversion():

	last_line = subprocess.Popen([srcml2src, '-V'],
				     stdout=subprocess.PIPE).communicate()[0]

	return last_line.strip()

src2srcml = os.environ.get('SRC2SRCML')
if src2srcml == '' or src2srcml == None:
	src2srcml = '../../bin/src2srcml'
	
srcml2src = os.environ.get('SRCML2SRC')
if srcml2src == '' or srcml2src == None:
	srcml2src = '../../bin/srcml2src'

default_src2srcml_encoding = 'UTF-8'
default_srcml2src_encoding = 'UTF-8'

xml_declaration= '<?xml version="1.0" encoding="' + default_src2srcml_encoding + '" standalone="yes"?>'

print xml_declaration

os.system('rm a.cpp*')

src ="""
a;
"""

f = open('archive/a.cpp', 'w')
f.write(src);
f.close()

os.system('./archive/archive.sh archive/a.cpp archive/a.cpp')

##
# test gz

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="archive/a.cpp.gz">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

check([src2srcml, 'archive/a.cpp.gz'], '', srcml)
check([src2srcml, 'archive/a.cpp.gz', '-o', 'archive/a.cpp.xml'], '', '')
validate(open('archive/a.cpp.xml', 'r').read(), srcml)

##
# test gz.bz2

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="archive/a.cpp.gz.bz2">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

check([src2srcml, 'archive/a.cpp.gz.bz2'], '', srcml)
check([src2srcml, 'archive/a.cpp.gz.bz2', '-o', 'archive/a.cpp.xml'], '', '')
validate(open('archive/a.cpp.xml', 'r').read(), srcml)

##
# test bz2

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="archive/a.cpp.bz2">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

check([src2srcml, 'archive/a.cpp.bz2'], '', srcml)
check([src2srcml, 'archive/a.cpp.bz2', '-o', 'archive/a.cpp.xml'], '', '')
validate(open('archive/a.cpp.xml', 'r').read(), srcml)

##
# test bz2.gz

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="archive/a.cpp.bz2.gz">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

check([src2srcml, 'archive/a.cpp.bz2.gz'], '', srcml)
check([src2srcml, 'archive/a.cpp.bz2.gz', '-o', 'archive/a.cpp.xml'], '', '')
validate(open('archive/a.cpp.xml', 'r').read(), srcml)

##
# test tar

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="archive/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
"""

check([src2srcml, 'archive/a.cpp.tar'], '', srcml)
check([src2srcml, 'archive/a.cpp.tar', '-o', 'archive/a.cpp.xml'], '', '')
validate(open('archive/a.cpp.xml', 'r').read(), srcml)

##
# test tgz

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="archive/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
"""

check([src2srcml, 'archive/a.cpp.tgz'], '', srcml)
check([src2srcml, 'archive/a.cpp.tgz', '-o', 'archive/a.cpp.xml'], '', '')
validate(open('archive/a.cpp.xml', 'r').read(), srcml)

##
# test tar.gz

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="archive/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
"""

check([src2srcml, 'archive/a.cpp.tar.gz'], '', srcml)
check([src2srcml, 'archive/a.cpp.tar.gz', '-o', 'archive/a.cpp.xml'], '', '')
validate(open('archive/a.cpp.xml', 'r').read(), srcml)

##
# test tar.bz2

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="archive/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
"""

check([src2srcml, 'archive/a.cpp.tar.bz2'], '', srcml)
check([src2srcml, 'archive/a.cpp.tar.bz2', '-o', 'archive/a.cpp.xml'], '', '')
validate(open('archive/a.cpp.xml', 'r').read(), srcml)

##
# test cpio

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="archive/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
"""

check([src2srcml, 'archive/a.cpp.cpio'], '', srcml)
check([src2srcml, 'archive/a.cpp.cpio', '-o', 'archive/a.cpp.xml'], '', '')
validate(open('archive/a.cpp.xml', 'r').read(), srcml)

##
# test cpio.gz

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="archive/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
"""

check([src2srcml, 'archive/a.cpp.cpio.gz'], '', srcml)
check([src2srcml, 'archive/a.cpp.cpio.gz', '-o', 'archive/a.cpp.xml'], '', '')
validate(open('archive/a.cpp.xml', 'r').read(), srcml)

##
# test cpio.bz2

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="archive/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
"""

check([src2srcml, 'archive/a.cpp.cpio.bz2'], '', srcml)
check([src2srcml, 'archive/a.cpp.cpio.bz2', '-o', 'archive/a.cpp.xml'], '', '')
validate(open('archive/a.cpp.xml', 'r').read(), srcml)

##
# test zip

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="archive/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
"""

check([src2srcml, 'archive/a.cpp.zip'], '', srcml)
check([src2srcml, 'archive/a.cpp.zip', '-o', 'archive/a.cpp.xml'], '', '')
validate(open('archive/a.cpp.xml', 'r').read(), srcml)

##
# test zip.gz

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="archive/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
"""

check([src2srcml, 'archive/a.cpp.zip.gz'], '', srcml)
check([src2srcml, 'archive/a.cpp.zip.gz', '-o', 'archive/a.cpp.xml'], '', '')
validate(open('archive/a.cpp.xml', 'r').read(), srcml)

##
# test zip.bz2

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="archive/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
"""

check([src2srcml, 'archive/a.cpp.zip.bz2'], '', srcml)
check([src2srcml, 'archive/a.cpp.zip.bz2', '-o', 'archive/a.cpp.xml'], '', '')
validate(open('archive/a.cpp.xml', 'r').read(), srcml)

##
# srcml2src

##
# input

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="archive/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

file = open('archive/a.cpp.xml', 'w')
file.write(srcml)
file.close()

os.system('./archive/archive.sh archive/a.cpp.xml archive/a.cpp.xml')

##
# gz

src ="""
a;
"""

check([srcml2src, 'archive/a.cpp.xml.gz'], '', src)
check([srcml2src, 'archive/a.cpp.xml.gz', '-o', 'archive/a.cpp'], '', '')
validate(open('archive/a.cpp', 'r').read(), src)

##
# gz and bz2

src ="""
a;
"""

check([srcml2src, 'archive/a.cpp.xml.gz.bz2'], '', src)
check([srcml2src, 'archive/a.cpp.xml.gz.bz2', '-o', 'archive/a.cpp'], '', '')
validate(open('archive/a.cpp', 'r').read(), src)

##
# bz2

src ="""
a;
"""

check([srcml2src, 'archive/a.cpp.xml.bz2'], '', src)
check([srcml2src, 'archive/a.cpp.xml.bz2', '-o', 'archive/a.cpp'], '', '')
validate(open('archive/a.cpp', 'r').read(), src)

##
# bz2 and gz

src ="""
a;
"""

check([srcml2src, 'archive/a.cpp.xml.bz2.gz'], '', src)
check([srcml2src, 'archive/a.cpp.xml.bz2.gz', '-o', 'archive/a.cpp'], '', '')
validate(open('archive/a.cpp', 'r').read(), src)

##
# zip

src ="""
a;
"""

check([srcml2src, 'archive/a.cpp.xml.zip'], '', src)
check([srcml2src, 'archive/a.cpp.xml.zip', '-o', 'archive/a.cpp'], '', '')
validate(open('archive/a.cpp', 'r').read(), src)

##
# zip and gz

src ="""
a;
"""

check([srcml2src, 'archive/a.cpp.xml.zip.gz'], '', src)
check([srcml2src, 'archive/a.cpp.xml.zip.gz', '-o', 'archive/a.cpp'], '', '')
validate(open('archive/a.cpp', 'r').read(), src)

##
# zip and bz2

src ="""
a;
"""

check([srcml2src, 'archive/a.cpp.xml.zip.bz2'], '', src)
check([srcml2src, 'archive/a.cpp.xml.zip.bz2', '-o', 'archive/a.cpp'], '', '')
validate(open('archive/a.cpp', 'r').read(), src)

##
# output

##
# tar

##
# single

src ="""
a;
"""

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="archive/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""

file = open('archive/a.cpp.xml', 'w')
file.write(srcml)
file.close()

check([srcml2src, '-o', 'archive/a.cpp.tar'], srcml, '')
validate(execute(['tar', '-Oxf', 'archive/a.cpp.tar'], ''), src)

check([srcml2src, 'archive/a.cpp.xml', '-o', 'archive/a.cpp.tar'], '', '')
validate(execute(['tar', '-Oxf', 'archive/a.cpp.tar'], ''), src)

##
# archive

src ="""
a;
"""

srcml = xml_declaration + """
<unit xmlns="http://www.sdml.info/srcML/src" language="C++" filename="archive/a.cpp">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="archive/a.cpp">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
"""

file = open('archive/a.cpp.xml', 'w')
file.write(srcml)
file.close()

check([srcml2src, '-o', 'archive/a.cpp.tar'], srcml, '')
validate(execute(['tar', '-Oxf', 'archive/a.cpp.tar'], ''), src)

check([srcml2src, 'archive/a.cpp.xml', '-o', 'archive/a.cpp.tar'], '', '')
validate(execute(['tar', '-Oxf', 'archive/a.cpp.tar'], ""), src)

# footer
print
print 'Error count: ', error_count
print
print src2srcmlversion()
print srcml2srcversion()

exit
