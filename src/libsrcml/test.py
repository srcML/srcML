import libsrcml

# test api
archive = libsrcml.srcml_archive()
archive.write_open_memory()

unit = libsrcml.srcml_unit(archive)
unit.parse_filename("a.cpp")
archive.write_unit(unit)

unit = libsrcml.srcml_unit(archive)
unit.parse_memory("b;")
archive.write_unit(unit)

archive.close()

print archive.srcML()
