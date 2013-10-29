import libsrcml

# test api
archive = libsrcml.srcml_archive()
archive.set_filename("project")
archive.set_language("a.cpp")
archive.set_directory("dir")
archive.set_version("1.0")
archive.write_open_memory()

unit = libsrcml.srcml_unit(archive)
unit.parse_filename("a.cpp")
archive.write_unit(unit)

unit = libsrcml.srcml_unit(archive)
unit.parse_memory("b;")
archive.write_unit(unit)

archive.close()

print archive.get_filename()
print archive.get_language()
print archive.get_directory()
print archive.get_version()

print archive.srcML()
