import libsrcml

test_count = 0
error_count = 0

def verify_test(input, output) :

    globals()['test_count'] += 1

    if input != output :
        print "input|" + input + "|"
        print "output|" + output + "|"
        globals()['error_count'] += 1

# test language
verify_test("['C', 'C++', 'C#', 'Java']", str(libsrcml.language_list()))

verify_test("10000", str(libsrcml.version_number()))
verify_test("0.9", libsrcml.version_string())

archive = libsrcml.srcml_archive()
archive.set_filename("project")
archive.set_language("C++")
archive.set_directory("dir")
archive.set_version("1.0")
archive.write_open_memory()

unit = libsrcml.srcml_unit(archive)
try :
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

print archive.get_filename()
print archive.get_language()
print archive.get_directory()
print archive.get_version()

archive = libsrcml.srcml_archive()
archive.read_open_memory(srcml)
clone = archive.clone()

unit = archive.read_unit()

unit.unparse_memory()
archive.close()
clone.close()

print unit.src()
print unit.get_xml()
