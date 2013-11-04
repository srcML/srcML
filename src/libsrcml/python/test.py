import libsrcml
import difflib
import os
import ctypes
import sys

test_count = 0
error_count = 0
LIBC_PATH = ""
if sys.platform == "darwin" :
    LIBC_PATH = "libc.dylib"
elif sys.platform == "linux2" :
    LIBC_PATH = "libc.so"
else :
    LIBC_PATH = "libc.dll"

libc = ctypes.cdll.LoadLibrary(LIBC_PATH)
libc.fopen.restype = ctypes.c_void_p 
libc.fopen.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
libc.fclose.restype = ctypes.c_int
libc.fclose.argtypes = [ctypes.c_void_p]

def verify_test(correct, output) :

    globals()['test_count'] += 1

    if correct != output :
        for line in difflib.unified_diff(str(correct).split("\n"), str(output).split("\n")) :
            print line
        globals()['error_count'] += 1

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
file = open("a.cpp", "w")
gen = file.write(src)
file.close()
archive = libsrcml.srcml_archive()
file = libc.fopen("project.xml", "w")
archive.write_open_FILE(file)
src_file = libc.fopen("a.cpp", "r")
unit = libsrcml.srcml_unit(archive)
unit.set_language("C++")
unit.parse_FILE(src_file)
archive.write_unit(unit)
archive.close()
libc.fclose(src_file)
libc.fclose(file)

file = open("project.xml", "r")
gen = file.read()
file.close()
verify_test(srcml, gen)
os.remove("a.cpp")
os.remove("project.xml")

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
file = open("project.xml", "w")
gen = file.write(srcml)
file.close()
archive = libsrcml.srcml_archive()
file = libc.fopen("project.xml", "r")
archive.read_open_FILE(file)
unit = archive.read_unit()
src_file = libc.fopen("a.cpp", "w")
unit.unparse_FILE(src_file)
archive.close()
libc.fclose(src_file)
libc.fclose(file)

file = open("a.cpp", "r")
gen = file.read()
file.close()
verify_test(src, gen)
os.remove("a.cpp")
os.remove("project.xml")

src = "b;\n"
srcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
"""

# skip_unit
archive = libsrcml.srcml_archive()
archive.read_open_memory(srcml)
archive.skip_unit()
unit = archive.read_unit()
unit.unparse_memory()
archive.close()
verify_test(src, unit.src())

# read_unit_position
archive = libsrcml.srcml_archive()
archive.read_open_memory(srcml)
unit = archive.read_unit_position(2)
unit.unparse_memory()
archive.close()
verify_test(src, unit.src())

# clone
archive = libsrcml.srcml_archive()
archive.set_language("C++")
clone_archive = archive.clone()
verify_test("C++", clone_archive.get_language())
archive.close()

# transforms

# xpath
archive = libsrcml.srcml_archive()
archive.read_open_memory(srcml)
archive.append_transform_xpath("//src:unit")
oarchive = libsrcml.srcml_archive()
oarchive.write_open_memory()
archive.apply_transforms(oarchive)
oarchive.close()
archive.close()

verify_test(srcml, oarchive.srcML())

# xslt
archive = libsrcml.srcml_archive()
archive.read_open_memory(srcml)
archive.append_transform_xslt("copy.xsl")
oarchive = libsrcml.srcml_archive()
oarchive.write_open_memory()
archive.apply_transforms(oarchive)
oarchive.close()
archive.close()

verify_test(srcml, oarchive.srcML())

# relaxng
archive = libsrcml.srcml_archive()
archive.read_open_memory(srcml)
archive.append_transform_relaxng("schema.rng")
oarchive = libsrcml.srcml_archive()
oarchive.write_open_memory()
archive.apply_transforms(oarchive)
oarchive.close()
archive.close()

verify_test(srcml, oarchive.srcML())

#
archive = libsrcml.srcml_archive()
archive.read_open_memory(srcml)
archive.append_transform_xpath("//src:unit")
archive.append_transform_xslt("copy.xsl")
archive.append_transform_relaxng("schema.rng")
archive.clear_transforms()
oarchive = libsrcml.srcml_archive()
oarchive.write_open_memory()
archive.apply_transforms(oarchive)
oarchive.close()
archive.close()

srcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src">

</unit>
"""
verify_test(srcml, oarchive.srcML())

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

# cleanup_globals
libsrcml.cleanup_globals()

# test language
verify_test("['C', 'C++', 'C#', 'Java']", str(libsrcml.language_list()))

file = open("a.cpp", "w")
file.write("a;\n")
file.close()

libsrcml.srcml("a.cpp", "project.xml")

os.remove("a.cpp")
file = open("project.xml", "r")
xml = file.read()
file.close()

srcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="project.xml"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""
verify_test(srcml, xml)

libsrcml.set_language("C++")
libsrcml.set_filename("a.cpp")
libsrcml.set_directory("directory")
libsrcml.set_version("version")

verify_test("C++", libsrcml.get_language())
verify_test("a.cpp", libsrcml.get_filename())
verify_test("directory", libsrcml.get_directory())
verify_test("version", libsrcml.get_version())

libsrcml.set_option(1)
verify_test(1, libsrcml.get_options())

libsrcml.set_all_options(2)
verify_test(2, libsrcml.get_options())

libsrcml.set_all_options(1 | 2)
libsrcml.clear_option(2)
verify_test(1, libsrcml.get_options())

libsrcml.set_tabstop(4)
verify_test(4, libsrcml.get_tabstop())

os.remove("project.xml")

file = open("a.foo", "w")
file.write("a;\n")
file.close()

libsrcml.set_language(None)
libsrcml.set_filename(None)
libsrcml.set_directory(None)
libsrcml.set_version(None)
libsrcml.set_all_options(0)
libsrcml.set_tabstop(8)

libsrcml.register_file_extension("foo", "C++")
libsrcml.register_namespace("s", "http://www.sdml.info/srcML/src")
libsrcml.srcml("a.foo", "project.xml")

file = open("project.xml", "r")
xml = file.read()
file.close()
os.remove("a.foo")

srcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<s:unit xmlns:s="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="project.xml"><s:expr_stmt><s:expr><s:name>a</s:name></s:expr>;</s:expr_stmt>
</s:unit>
"""
verify_test(srcml, xml)

verify_test(2, libsrcml.check_language("C++"))
verify_test("C++", libsrcml.check_extension("a.cpp"))
verify_test(0, libsrcml.check_format("a.cpp.tar"))
verify_test(0, libsrcml.check_encoding("UTF-8"))
verify_test(1, libsrcml.check_xslt())
verify_test(1, libsrcml.check_exslt())
libsrcml.srcml("", "")
verify_test("No language provided.", libsrcml.error_string())

srcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="a.cpp"/>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="b.cpp"/>

</unit>
"""

file = open("project.xml", "w")
file.write(srcml)
file.close()

verify_test("['a.cpp', 'b.cpp']", libsrcml.filename_list("project.xml"))
os.remove("project.xml")
