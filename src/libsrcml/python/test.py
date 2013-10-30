import libsrcml

# test api

print str(libsrcml.version_number())
print libsrcml.version_string()

archive = libsrcml.srcml_archive()
archive.set_filename("project")
archive.set_language("C++")
archive.set_directory("dir")
archive.set_version("1.0")
archive.write_open_memory()

unit = libsrcml.srcml_unit(archive)
unit.parse_filename("a.cpp")
archive.write_unit(unit)

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

unit = archive.read_unit()

unit.unparse_memory()

print unit.src()
print unit.get_xml()
