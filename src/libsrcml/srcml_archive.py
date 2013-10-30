from libsrcml import libsrcml
from ctypes import *

from srcml_unit import srcml_unit

# srcml_archive wrapper
class srcml_archive :

    def __init__(self) :
        self.archive = libsrcml.srcml_create_archive()

    def write_open_filename(self, srcml_filename) :
        libsrcml.srcml_write_open_filename(self.archive, srcml_filename)

    def write_open_memory(self) :
        self.buffer = c_char_p()
        self.size = c_int()
        libsrcml.srcml_write_open_memory(self.archive, pointer(self.buffer), pointer(self.size))

    def read_open_filename(self, srcml_filename) :
        libsrcml.srcml_read_open_filename(self.archive, srcml_filename)

    def read_open_memory(self, buffer) :
        libsrcml.srcml_read_open_memory(self.archive, buffer, len(buffer))

    def set_encoding(self, encoding) :
        libsrcml.srcml_archive_set_encoding(self.archive, encoding)

    def set_language(self, language) :
        libsrcml.srcml_archive_set_language(self.archive, language)

    def set_filename(self, filename) :
        libsrcml.srcml_archive_set_filename(self.archive, filename)

    def set_directory(self, directory) :
        libsrcml.srcml_archive_set_directory(self.archive, directory)

    def set_version(self, version) :
        libsrcml.srcml_archive_set_version(self.archive, version)

    def set_all_options(self, options) :
        libsrcml.srcml_archive_set_all_options(self.archive, options)

    def set_option(self, option) :
        libsrcml.srcml_archive_set_option(self.archive, option)

    def clear_option(self, option) :
        libsrcml.srcml_archive_clear_option(self.archive, option)

    def set_tabstop(self, tabstop) :
        libsrcml.srcml_archive_set_tabstop(self.archive, tabstop)

    def register_file_extension(self, extension, language) :
        libsrcml.srcml_archive_set_tabstop(self.archive, extension, language)

    def register_namespace(self, prefix, ns) :
        libsrcml.srcml_archive_set_tabstop(self.archive, prefix, ns)

    def get_encoding(self) :
        return libsrcml.srcml_archive_get_encoding(self.archive)

    def get_language(self) :
        return libsrcml.srcml_archive_get_language(self.archive)

    def get_filename(self) :
        return libsrcml.srcml_archive_get_filename(self.archive)

    def get_directory(self) :
        return libsrcml.srcml_archive_get_directory(self.archive)

    def get_version(self) :
        return libsrcml.srcml_archive_get_version(self.archive)

    def get_options(self) :
        return libsrcml.srcml_archive_get_options(self.archive)

    def get_tabstop(self) :
        return libsrcml.srcml_archive_get_tabstop(self.archive)

    def check_extension(self, filename) :
        return libsrcml.srcml_archive_check_extension(self.archive, filename)

    def srcML(self) :
        return self.buffer.value

    def write_unit(self, unit) :
        libsrcml.srcml_write_unit(self.archive, unit.unit)

    def read_unit(self) :
        return srcml_unit(0, libsrcml.srcml_read_unit(self.archive))

    def close(self) :
        libsrcml.srcml_close_archive(self.archive)

    def __del__(self) :
        libsrcml.srcml_free_archive(self.archive)
