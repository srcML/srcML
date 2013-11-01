import libsrcml
import difflib

test_count = 0
error_count = 0

def verify_test(correct, output) :

    globals()['test_count'] += 1

    if correct != output :
        for line in difflib.unified_diff(correct.split("\n"), output.split("\n")) :
            print line
        globals()['error_count'] += 1

# test language
verify_test("['C', 'C++', 'C#', 'Java']", str(libsrcml.language_list()))

# test versions
verify_test("10000", str(libsrcml.version_number()))
verify_test("libsrcml 0.9", libsrcml.version_string())

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
archive.close()

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

# memory
archive = libsrcml.srcml_archive()
archive.write_open_memory()
unit = libsrcml.srcml_unit(archive)
unit.set_language("C++")
unit.parse_memory(src)
archive.write_unit(unit)
archive.close()

verify_test(srcml, archive.srcML())

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

# memory
archive = libsrcml.srcml_archive()
archive.read_open_memory(srcml)
unit = archive.read_unit()
unit.unparse_memory()
archive.close()
verify_test(src, unit.src())

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
