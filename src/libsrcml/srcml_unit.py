from libsrcml import libsrcml
from ctypes import *

# srcml_unit wrapper
class srcml_unit :

    def __init__(self, archive, unit = 0) :
        self.unit = unit
        if self.unit == 0 :
            self.unit = libsrcml.srcml_create_unit(archive.archive)

    def parse_filename(self, src_filename) :
        libsrcml.srcml_parse_unit_filename(self.unit, src_filename)

    def parse_memory(self, src_buffer) :
        libsrcml.srcml_parse_unit_memory(self.unit, src_buffer, len(src_buffer))

    def unparse_filename(self, src_filename) :
        libsrcml.srcml_parse_unit_filename(self.unit, src_filename)

    def unparse_memory(self) :
        self.src_size = c_int()
        self.src_buffer = c_char_p()
        libsrcml.srcml_unparse_unit_memory(self.unit, pointer(self.src_buffer), pointer(self.src_size))

    def set_language(self, language) :
        libsrcml.srcml_unit_set_language(self.unit, language)

    def set_filename(self, filename) :
        libsrcml.srcml_unit_set_filename(self.unit, filename)

    def set_directory(self, directory) :
        libsrcml.srcml_unit_set_directory(self.unit, directory)

    def set_version(self, version) :
        libsrcml.srcml_unit_set_version(self.unit, version)

    def get_language(self) :
        return libsrcml.srcml_unit_get_language(self.unit)

    def get_filename(self) :
        return libsrcml.srcml_unit_get_filename(self.unit)

    def get_directory(self) :
        return libsrcml.srcml_unit_get_directory(self.unit)

    def get_version(self) :
        return libsrcml.srcml_unit_get_version(self.unit)

    def get_xml(self) :
        return libsrcml.srcml_unit_get_xml(self.unit)

    def src(self) :
        return self.src_buffer.value

    def __del__(self) :
        libsrcml.srcml_free_unit(self.unit)

