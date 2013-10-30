from globals import libsrcml
from ctypes import *

from srcml_unit import srcml_unit

# void srcml_cleanup_globals();
libsrcml.srcml_cleanup_globals.restype = None
libsrcml.srcml_cleanup_globals.argtypes = []

# int srcml_version_number();
libsrcml.srcml_version_number.restype = c_int
libsrcml.srcml_version_number.argtypes = []

# const char* srcml_version_string();
libsrcml.srcml_version_string.restype = c_char_p
libsrcml.srcml_version_string.argtypes = []

# struct srcml_archive* srcml_create_archive();
libsrcml.srcml_create_archive.restype = c_void_p
libsrcml.srcml_create_archive.argtypes = []

# int srcml_write_open_filename(struct srcml_archive*, const char* srcml_filename);
libsrcml.srcml_write_open_filename.restype = c_int
libsrcml.srcml_write_open_filename.argtypes = [c_void_p, c_char_p]

# int srcml_write_open_memory  (struct srcml_archive*, char** buffer, int * size);
libsrcml.srcml_write_open_memory.restype = c_int
libsrcml.srcml_write_open_memory.argtypes = [c_void_p, c_void_p, c_void_p]

# int srcml_read_open_filename(struct srcml_archive*, const char* srcml_filename);
libsrcml.srcml_read_open_filename.restype = c_int
libsrcml.srcml_read_open_filename.argtypes = [c_void_p, c_char_p]

# int srcml_read_open_memory  (struct srcml_archive*, const char* buffer, size_t buffer_size);
libsrcml.srcml_read_open_memory.restype = c_int
libsrcml.srcml_read_open_memory.argtypes = [c_void_p, c_char_p, c_int]

# void srcml_free_archive(struct srcml_archive* archive);
libsrcml.srcml_free_archive.restype = None
libsrcml.srcml_free_archive.argtypes = [c_void_p]

# void srcml_close_archive(struct srcml_archive*);
libsrcml.srcml_close_archive.restype = None
libsrcml.srcml_close_archive.argtypes = [c_void_p]


# int srcml_archive_set_encoding  (struct srcml_archive*, const char* encoding);
libsrcml.srcml_archive_set_encoding.restype = c_int
libsrcml.srcml_archive_set_encoding.argtypes = [c_void_p, c_char_p]

# int srcml_archive_set_language  (struct srcml_archive*, const char* language);
libsrcml.srcml_archive_set_language.restype = c_int
libsrcml.srcml_archive_set_language.argtypes = [c_void_p, c_char_p]

# int srcml_archive_set_filename  (struct srcml_archive*, const char* filename);
libsrcml.srcml_archive_set_filename.restype = c_int
libsrcml.srcml_archive_set_filename.argtypes = [c_void_p, c_char_p]

# int srcml_archive_set_directory (struct srcml_archive*, const char* directory)
libsrcml.srcml_archive_set_directory.restype = c_int
libsrcml.srcml_archive_set_directory.argtypes = [c_void_p, c_char_p]

# int srcml_archive_set_version   (struct srcml_archive*, const char* version)
libsrcml.srcml_archive_set_version.restype = c_int
libsrcml.srcml_archive_set_version.argtypes = [c_void_p, c_char_p]

# int srcml_archive_set_all_options   (struct srcml_archive*, int option);
libsrcml.srcml_archive_set_all_options.restype = c_int
libsrcml.srcml_archive_set_all_options.argtypes = [c_void_p, c_int]

# int srcml_archive_set_option    (struct srcml_archive*, int option);
libsrcml.srcml_archive_set_option.restype = c_int
libsrcml.srcml_archive_set_option.argtypes = [c_void_p, c_int]

# int srcml_archive_clear_option  (struct srcml_archive*, int option);
libsrcml.srcml_archive_clear_option.restype = c_int
libsrcml.srcml_archive_clear_option.argtypes = [c_void_p, c_int]

# int srcml_archive_set_tabstop   (struct srcml_archive*, int tabstop);
libsrcml.srcml_archive_set_tabstop.restype = c_int
libsrcml.srcml_archive_set_tabstop.argtypes = [c_void_p, c_int]

# int srcml_archive_register_file_extension(struct srcml_archive*, const char* extension, const char* language);
libsrcml.srcml_archive_register_file_extension.restype = c_int
libsrcml.srcml_archive_register_file_extension.argtypes = [c_void_p, c_char_p, c_char_p]

# int srcml_archive_register_namespace(struct srcml_archive*, const char* prefix, const char* ns);
libsrcml.srcml_archive_register_namespace.restype = c_int
libsrcml.srcml_archive_register_namespace.argtypes = [c_void_p, c_char_p, c_char_p]

# const char* srcml_archive_get_encoding (const struct srcml_archive*);
libsrcml.srcml_archive_get_encoding.restype = c_char_p
libsrcml.srcml_archive_get_encoding.argtypes = [c_void_p]

# const char* srcml_archive_get_language (const struct srcml_archive*);
libsrcml.srcml_archive_get_language.restype = c_char_p
libsrcml.srcml_archive_get_language.argtypes = [c_void_p]

# const char* srcml_archive_get_filename (const struct srcml_archive*);
libsrcml.srcml_archive_get_filename.restype = c_char_p
libsrcml.srcml_archive_get_filename.argtypes = [c_void_p]

# const char* srcml_archive_get_directory(const struct srcml_archive*);
libsrcml.srcml_archive_get_directory.restype = c_char_p
libsrcml.srcml_archive_get_directory.argtypes = [c_void_p]

# const char* srcml_archive_get_version  (const struct srcml_archive*);
libsrcml.srcml_archive_get_version.restype = c_char_p
libsrcml.srcml_archive_get_version.argtypes = [c_void_p]

# int         srcml_archive_get_options  (const struct srcml_archive*);
libsrcml.srcml_archive_get_options.restype = c_int
libsrcml.srcml_archive_get_options.argtypes = [c_void_p]

# int         srcml_archive_get_tabstop  (const struct srcml_archive*);
libsrcml.srcml_archive_get_tabstop.restype = c_int
libsrcml.srcml_archive_get_tabstop.argtypes = [c_void_p]

# const char* srcml_archive_check_extension(struct srcml_archive* archive, const char* filename);
libsrcml.srcml_archive_check_extension.restype = c_char_p
libsrcml.srcml_archive_check_extension.argtypes = [c_void_p, c_char_p]

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
