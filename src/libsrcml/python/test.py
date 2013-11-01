import libsrcml
import difflib
import os

test_count = 0
error_count = 0

def verify_test(correct, output) :

    globals()['test_count'] += 1

    if correct != output :
        for line in difflib.unified_diff(str(correct).split("\n"), str(output).split("\n")) :
            print line
        globals()['error_count'] += 1

# test language
verify_test("['C', 'C++', 'C#', 'Java']", str(libsrcml.language_list()))

# test versions
verify_test(libsrcml.SRCML_VERSION_NUMBER, libsrcml.version_number())
verify_test(libsrcml.SRCML_VERSION_STRING, libsrcml.version_string())

# test set/get archive
archive = libsrcml.srcml_archive()
archive.set_filename("project")
archive.set_language("C++")
archive.set_directory("dir")
archive.set_version("1.0")
verify_test("project", archive.get_filename())
verify_test("C++", archive.get_language())
verify_test("dir", archive.get_directory())
verify_test("1.0", archive.get_version())

archive.set_option(1)
archive.set_option(2)
verify_test(3, archive.get_options())
archive.clear_option(2)
verify_test(1, archive.get_options())
archive.set_all_options(2)
verify_test(2, archive.get_options())

archive.set_tabstop(4)
verify_test(4, archive.get_tabstop())

archive.close()

file = open("a.foo", "w")
gen = file.write("")
file.close()
archive = libsrcml.srcml_archive()
archive.register_file_extension("foo", "C++")
archive.register_namespace("s", "http://www.sdml.info/srcML/src")
archive.write_open_memory()
unit = libsrcml.srcml_unit(archive)
unit.parse_filename("a.foo")
archive.write_unit(unit)
archive.close()
os.remove("a.foo")
verify_test("""<s:unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"/>""", unit.get_xml())

# write/parse tests
src = "a;\n"
srcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
"""

# filename
file = open("a.cpp", "w")
gen = file.write(src)
file.close()
archive = libsrcml.srcml_archive()
archive.write_open_filename("project.xml")
unit = libsrcml.srcml_unit(archive)
unit.parse_filename("a.cpp")
archive.write_unit(unit)
archive.close()

file = open("project.xml", "r")
gen = file.read()
file.close()
verify_test(srcml, gen)
os.remove("a.cpp")
os.remove("project.xml")

# memory
archive = libsrcml.srcml_archive()
archive.write_open_memory()
unit = libsrcml.srcml_unit(archive)
unit.set_language("C++")
unit.parse_memory(src)
archive.write_unit(unit)
archive.close()

verify_test(srcml, archive.srcML())

# fd
file = open("a.cpp", "w")
gen = file.write(src)
file.close()
archive = libsrcml.srcml_archive()
fd = os.open("project.xml", os.O_WRONLY | os.O_CREAT)
archive.write_open_fd(fd)
src_fd = os.open("a.cpp", os.O_RDONLY)
unit = libsrcml.srcml_unit(archive)
unit.set_language("C++")
unit.parse_fd(src_fd)
archive.write_unit(unit)
archive.close()
os.close(src_fd)
os.close(fd)

file = open("project.xml", "r")
gen = file.read()
file.close()
verify_test(srcml, gen)
os.remove("a.cpp")
os.remove("project.xml")

# FILE
#file = open("a.cpp", "w")
#gen = file.write(src)
#file.close()
#archive = libsrcml.srcml_archive()
#file = open("project.xml", "w")
#archive.write_open_FILE(file)
#src_file = open("a.cpp", "r")
#unit = libsrcml.srcml_unit(archive)
#unit.set_language("C++")
#unit.parse_FILE(src_file)
#archive.write_unit(unit)
#archive.close()
#close(src_file)
#close(file)

#file = open("project.xml", "r")
#gen = file.read()
#file.close()
#verify_test(srcml, gen)
#os.remove("a.cpp")
#os.remove("project.xml")

# read/unparse

# filename
file = open("project.xml", "w")
gen = file.write(srcml)
file.close()
archive = libsrcml.srcml_archive()
archive.read_open_filename("project.xml")
unit = archive.read_unit()
unit.unparse_filename("a.cpp")
archive.close()

file = open("a.cpp", "r")
gen = file.read()
file.close()
verify_test(src, gen)
os.remove("a.cpp")
os.remove("project.xml")

# memory
archive = libsrcml.srcml_archive()
archive.read_open_memory(srcml)
unit = archive.read_unit()
unit.unparse_memory()
archive.close()
verify_test(src, unit.src())

# fd
file = open("project.xml", "w")
gen = file.write(srcml)
file.close()
archive = libsrcml.srcml_archive()
fd = os.open("project.xml", os.O_RDONLY)
archive.read_open_fd(fd)
unit = archive.read_unit()
src_fd = os.open("a.cpp", os.O_WRONLY | os.O_CREAT)
unit.unparse_fd(src_fd)
archive.close()
os.close(src_fd)
os.close(fd)

file = open("a.cpp", "r")
gen = file.read()
file.close()
verify_test(src, gen)
os.remove("a.cpp")
os.remove("project.xml")

# FILE
#file = open("project.xml", "w")
#gen = file.write(srcml)
#file.close()
#archive = libsrcml.srcml_archive()
#file = open("project.xml", "r")
#archive.read_open_FILE(file)
#unit = archive.read_unit()
#src_file = os.open("a.cpp", "w")
#unit.unparse_FILE(src_fd)
#archive.close()
#os.close(src_fd)
#os.close(fd)

#file = open("a.cpp", "r")
#gen = file.read()
#file.close()
#verify_test(src, gen)
#os.remove("a.cpp")
#os.remove("project.xml")

# unit set/get
archive = libsrcml.srcml_archive()
unit = libsrcml.srcml_unit(archive)
unit.set_filename("b.cpp")
unit.set_language("C")
unit.set_directory("directory")
unit.set_version("1.1")
verify_test("b.cpp", unit.get_filename())
verify_test("C", unit.get_language())
verify_test("directory", unit.get_directory())
verify_test("1.1", unit.get_version())
archive.close()

# exception
archive = libsrcml.srcml_archive()
test = ""
try :
    archive.write_unit(unit)
    
except libsrcml.srcMLException as e :
    test = "Exception"
archive.close()
verify_test("Exception", test)
