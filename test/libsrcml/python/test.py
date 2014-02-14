import sys
sys.path.append("../../../src/libsrcml/python")
import srcml
import difflib
import os
import ctypes


test_count = 0
error_count = 0
LIBC_PATH = ""
if sys.platform == "darwin" :
    LIBC_PATH = "libc.dylib"
elif sys.platform == "linux2" :
    LIBC_PATH = "libc.so.6"
else :
    LIBC_PATH = "msvcrt.dll"

libc = ctypes.cdll.LoadLibrary(LIBC_PATH)


if sys.platform == "win32" or sys.platform == "cygwin" :
    os.open = libc._open
    os.close = libc._close
    os.O_WRONLY = 1
    os.O_CREAT = 256
    os.O_RDONLY = 0

libc.fopen.restype = ctypes.c_void_p 
libc.fopen.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
libc.fclose.restype = ctypes.c_int
libc.fclose.argtypes = [ctypes.c_void_p]

def verify_test(correct, output) :

    globals()['test_count'] += 1

    if sys.platform == "win32" or sys.platform == "cygwin" :
        correct = str(correct).replace("\r", "")
        output  = str(output).replace("\r", "")

    if correct != output :
        print str(globals()['test_count']) + "\t"
        for line in difflib.unified_diff(str(correct).split("\n"), str(output).split("\n")) :
            print line
        globals()['error_count'] += 1

# test versions
verify_test(srcml.SRCML_VERSION_NUMBER, srcml.version_number())
verify_test(srcml.SRCML_VERSION_STRING, srcml.version_string())

# test set/get archive
archive = srcml.srcml_archive()
archive.set_filename("project")
archive.set_language("C++")
archive.set_directory("dir")
archive.set_version("1.0")
verify_test("project", archive.get_filename())
verify_test("C++", archive.get_language())
verify_test("dir", archive.get_directory())
verify_test("1.0", archive.get_version())

archive.set_options(0)
archive.enable_option(1)
archive.enable_option(2)
verify_test(3, archive.get_options())
archive.disable_option(2)
verify_test(1, archive.get_options())
archive.set_options(2)
verify_test(2, archive.get_options())

archive.set_tabstop(4)
verify_test(4, archive.get_tabstop())

verify_test(7, archive.get_namespace_size());
verify_test("cpp", archive.get_prefix(1))
verify_test("cpp", archive.get_prefix_uri("http://www.sdml.info/srcML/cpp"))
verify_test("http://www.sdml.info/srcML/cpp", archive.get_namespace(1))
verify_test("http://www.sdml.info/srcML/cpp", archive.get_namespace_prefix("cpp"))

archive.close()

file = open("a.foo", "w")
gen = file.write("")
file.close()
archive = srcml.srcml_archive()
archive.disable_option(srcml.SRCML_OPTION_ARCHIVE)
archive.register_file_extension("foo", "C++")
archive.register_namespace("s", "http://www.sdml.info/srcML/src")
archive.register_macro("MACRO", "src:macro")
archive.write_open_memory()
unit = srcml.srcml_unit(archive)
unit.parse_filename("a.foo")
archive.write_unit(unit)
archive.close()
os.remove("a.foo")
verify_test("""<s:unit xmlns:s="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><macro-list token="MACRO" type="src:macro"/></s:unit>""", unit.get_xml())

# write/parse tests
src = "a;\n"
asrcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
"""

# filename
file = open("a.cpp", "w")
gen = file.write(src)
file.close()
archive = srcml.srcml_archive()
archive.write_open_filename("project.xml")
unit = srcml.srcml_unit(archive)
unit.parse_filename("a.cpp")
archive.write_unit(unit)
archive.close()

file = open("project.xml", "r")
gen = file.read()
file.close()
verify_test(asrcml, gen)
os.remove("a.cpp")
os.remove("project.xml")

# memory
archive = srcml.srcml_archive()
archive.write_open_memory()
unit = srcml.srcml_unit(archive)
unit.set_language("C++")
unit.parse_memory(src)
archive.write_unit(unit)
archive.close()

verify_test(asrcml, archive.srcML())

# fd
file = open("a.cpp", "w")
gen = file.write(src)
file.close()
archive = srcml.srcml_archive()
fd = os.open("project.xml", os.O_WRONLY | os.O_CREAT)
archive.write_open_fd(fd)
src_fd = os.open("a.cpp", os.O_RDONLY)
unit = srcml.srcml_unit(archive)
unit.set_language("C++")
unit.parse_fd(src_fd)
archive.write_unit(unit)
archive.close()
os.close(src_fd)
os.close(fd)

file = open("project.xml", "r")
gen = file.read()
file.close()
verify_test(asrcml, gen)
os.remove("a.cpp")
os.remove("project.xml")

# FILE
file = open("a.cpp", "w")
gen = file.write(src)
file.close()
archive = srcml.srcml_archive()
file = libc.fopen("project.xml", "w")
archive.write_open_FILE(file)
src_file = libc.fopen("a.cpp", "r")
unit = srcml.srcml_unit(archive)
unit.set_language("C++")
unit.parse_FILE(src_file)
archive.write_unit(unit)
archive.close()
libc.fclose(src_file)
libc.fclose(file)

file = open("project.xml", "r")
gen = file.read()
file.close()
verify_test(asrcml, gen)
os.remove("a.cpp")
os.remove("project.xml")

# read/unparse

# filename
file = open("project.xml", "w")
gen = file.write(asrcml)
file.close()
archive = srcml.srcml_archive()
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
archive = srcml.srcml_archive()
archive.read_open_memory(asrcml)
unit = archive.read_unit()
unit.unparse_memory()
archive.close()
verify_test(src, unit.src())

# fd
file = open("project.xml", "w")
gen = file.write(asrcml)
file.close()
archive = srcml.srcml_archive()
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
gen = file.write(asrcml)
file.close()
archive = srcml.srcml_archive()
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
asrcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
</unit>

</unit>
"""

# skip_unit
archive = srcml.srcml_archive()
archive.read_open_memory(asrcml)
archive.skip_unit()
unit = archive.read_unit()
unit.unparse_memory()
archive.close()
verify_test(src, unit.src())

# read_unit_position
archive = srcml.srcml_archive()
archive.read_open_memory(asrcml)
unit = archive.read_unit_position(2)
unit.unparse_memory()
archive.close()
verify_test(src, unit.src())

# clone
archive = srcml.srcml_archive()
archive.set_language("C++")
clone_archive = archive.clone()
verify_test("C++", clone_archive.get_language())
archive.close()

# transforms

# xpath
archive = srcml.srcml_archive()
archive.read_open_memory(asrcml)
archive.append_transform_xpath("//src:unit")
oarchive = archive.clone()
oarchive.write_open_memory()
archive.apply_transforms(oarchive)
oarchive.close()
archive.close()

verify_test(asrcml, oarchive.srcML())

# xslt
archive = srcml.srcml_archive()
archive.read_open_memory(asrcml)
archive.append_transform_xslt("copy.xsl")
oarchive = archive.clone()
oarchive.write_open_memory()
archive.apply_transforms(oarchive)
oarchive.close()
archive.close()

verify_test(asrcml, oarchive.srcML())

# relaxng
archive = srcml.srcml_archive()
archive.read_open_memory(asrcml)
archive.append_transform_relaxng("schema.rng")
oarchive = archive.clone()
oarchive.write_open_memory()
archive.apply_transforms(oarchive)
oarchive.close()
archive.close()

verify_test(asrcml, oarchive.srcML())

# clear transforms
archive = srcml.srcml_archive()
archive.read_open_memory(asrcml)
archive.append_transform_xpath("//src:unit")
archive.append_transform_xslt("copy.xsl")
archive.append_transform_relaxng("schema.rng")
archive.clear_transforms()
oarchive = archive.clone()
oarchive.write_open_memory()
archive.apply_transforms(oarchive)
oarchive.close()
archive.close()

verify_test(None, oarchive.srcML())

# unit set/get
archive = srcml.srcml_archive()
unit = srcml.srcml_unit(archive)
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
archive = srcml.srcml_archive()
test = ""
try :
    archive.write_unit(unit)
    
except srcml.srcMLException as e :
    test = "Exception"
archive.close()
verify_test("Exception", test)

# cleanup_globals
srcml.cleanup_globals()

# test language
verify_test("['C', 'C++', 'C#', 'Java']", str(srcml.language_list()))

file = open("a.cpp", "w")
file.write("a;\n")
file.close()

srcml.srcml("a.cpp", "project.xml")

os.remove("a.cpp")
file = open("project.xml", "r")
xml = file.read()
file.close()

asrcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
"""
verify_test(asrcml, xml)

srcml.set_language("C++")
srcml.set_filename("a.cpp")
srcml.set_directory("directory")
srcml.set_version("version")

verify_test("C++", srcml.get_language())
verify_test("a.cpp", srcml.get_filename())
verify_test("directory", srcml.get_directory())
verify_test("version", srcml.get_version())

srcml.set_options(0)
srcml.enable_option(1)
verify_test(1, srcml.get_options())

srcml.set_options(2)
verify_test(2, srcml.get_options())

srcml.set_options(1 | 2)
srcml.disable_option(2)
verify_test(1, srcml.get_options())

srcml.set_tabstop(4)
verify_test(4, srcml.get_tabstop())

os.remove("project.xml")

file = open("a.foo", "w")
file.write("a;\n")
file.close()

srcml.set_language(None)
srcml.set_filename(None)
srcml.set_directory(None)
srcml.set_version(None)
srcml.set_options(srcml.SRCML_OPTION_XML_DECL | srcml.SRCML_OPTION_NAMESPACE_DECL)
srcml.set_tabstop(8)

srcml.register_file_extension("foo", "C++")
srcml.register_namespace("s", "http://www.sdml.info/srcML/src")
srcml.srcml("a.foo", "project.xml")

file = open("project.xml", "r")
xml = file.read()
file.close()
os.remove("a.foo")

asrcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<s:unit xmlns:s="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="a.foo"><s:expr_stmt><s:expr><s:name>a</s:name></s:expr>;</s:expr_stmt>
</s:unit>
"""

verify_test(asrcml, xml)

verify_test(2, srcml.check_language("C++"))
verify_test("C++", srcml.check_extension("a.cpp"))
verify_test(1, srcml.check_format("a.cpp.tar"))
verify_test(1, srcml.check_encoding("UTF-8"))
verify_test(1, srcml.check_xslt())
verify_test(1, srcml.check_exslt())
srcml.srcml("", "")
verify_test("No language provided.", srcml.error_string())

asrcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="a.cpp"/>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="b.cpp"/>

</unit>
"""

file = open("project.xml", "w")
file.write(asrcml)
file.close()

verify_test(['a.cpp', 'b.cpp'], srcml.filename_list("project.xml"))

os.remove("project.xml")
