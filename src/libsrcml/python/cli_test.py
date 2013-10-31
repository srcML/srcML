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

verify_test("10000", str(libsrcml.version_number()))
verify_test("libsrcml 0.9", libsrcml.version_string())

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
srcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
"""

# filename
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
unit.parse_memory("a;\n")
archive.write_unit(unit)
archive.close()

verify_test(srcml, archive.srcML())

archive = libsrcml.srcml_archive()
archive.write_open_memory()
try :
    unit = libsrcml.srcml_unit(archive)
    unit.parse_filename("a.cpp")
    archive.write_unit(unit)
except libsrcml.srcMLException as e :
    print str(e) + " - File does not exist"

unit = libsrcml.srcml_unit(archive)
unit.set_filename("b.cpp")
unit.set_language("C")
unit.set_directory("directory")
unit.set_version("1.1")
unit.parse_memory("b;")
print unit.get_filename()
print unit.get_language()
print unit.get_directory()
print unit.get_version()
archive.write_unit(unit)

archive.close()

print archive.srcML()
srcml = archive.srcML()

archive = libsrcml.srcml_archive()
archive.read_open_memory(srcml)
clone = archive.clone()

unit = archive.read_unit()

unit.unparse_memory()
archive.close()
clone.close()

print unit.src()
print unit.get_xml()
