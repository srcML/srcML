from ctypes import *
import os

LIBSRCML_PATH=""
if os.path.exists('../../bin/libsrcml.dylib') :
    LIBSRCML_PATH="../../bin/libsrcml.dylib"
elif os.path.exists('../../bin/libsrcml.so') :
    LIBSRCML_PATH="../../bin/libsrcml.so"
elif os.path.exists('../../bin/libsrcml.dll') :
    LIBSRCML_PATH="../../bin/libsrcml.dll"

libsrcml = cdll.LoadLibrary(LIBSRCML_PATH)

# struct srcml_archive* srcml_create_archive();
libsrcml.srcml_create_archive.restype = c_void_p
libsrcml.srcml_create_archive.argtypes = []

# int srcml_write_open_filename(struct srcml_archive*, const char* srcml_filename);
libsrcml.srcml_write_open_filename.restype = c_int
libsrcml.srcml_write_open_filename.argtypes = [c_void_p, c_char_p]

# int srcml_write_open_memory  (struct srcml_archive*, char** buffer, int * size);
libsrcml.srcml_write_open_memory.restype = c_int
libsrcml.srcml_write_open_memory.argtypes = [c_void_p, c_void_p, c_void_p]

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
# const char* srcml_archive_get_language (const struct srcml_archive*);
# const char* srcml_archive_get_filename (const struct srcml_archive*);
# const char* srcml_archive_get_directory(const struct srcml_archive*);
# const char* srcml_archive_get_version  (const struct srcml_archive*);
# int         srcml_archive_get_options  (const struct srcml_archive*);
# int         srcml_archive_get_tabstop  (const struct srcml_archive*);



# int srcml_parse_unit_filename(struct srcml_unit* unit, const char* src_filename);
libsrcml.srcml_parse_unit_filename.restype = c_int
libsrcml.srcml_parse_unit_filename.argtypes = [c_void_p, c_char_p]

# int srcml_parse_unit_memory  (struct srcml_unit*, const char* src_buffer, size_t buffer_size);
libsrcml.srcml_parse_unit_memory.restype = c_int
libsrcml.srcml_parse_unit_memory.argtypes = [c_void_p, c_char_p, c_int]

# int srcml_write_unit(struct srcml_archive*, const struct srcml_unit*);
libsrcml.srcml_write_unit.restype = c_int
libsrcml.srcml_write_unit.argtypes = [c_void_p, c_void_p]

# struct srcml_unit* srcml_create_unit(struct srcml_archive* archive);
libsrcml.srcml_create_unit.restype = c_void_p
libsrcml.srcml_create_unit.argtypes = [c_void_p]

# int srcml_free_unit(struct srcml_unit*);
libsrcml.srcml_free_unit.restype = None
libsrcml.srcml_free_unit.argtypes = [c_void_p]

# srcml_archive wrapper
class srcml_archive :

    def __init__(self) :
        self.archive = libsrcml.srcml_create_archive()

    def write_open_filename(self, srcml_filename) :
        libsrcml.srcml_write_open_filename(self.archive, srcml_filename)

    def write_open_memory(self) :
        self.size = c_int()
        self.buffer = c_char_p()
        libsrcml.srcml_write_open_memory(self.archive, pointer(self.buffer), pointer(self.size))

    def srcML(self) :
        return self.buffer.value

    def write_unit(self, unit) :
        libsrcml.srcml_write_unit(self.archive, unit.unit)

    def close(self) :
        libsrcml.srcml_close_archive(self.archive)

    def __del__(self) :
        libsrcml.srcml_free_archive(self.archive)

# srcml_unit wrapper
class srcml_unit :

    def __init__(self, archive) :
        self.unit = libsrcml.srcml_create_unit(archive.archive)

    def parse_filename(self, src_filename) :
        libsrcml.srcml_parse_unit_filename(self.unit, src_filename)

    def parse_memory(self, src_buffer) :
        libsrcml.srcml_parse_unit_memory(self.unit, src_buffer, len(src_buffer))

    def __del__(self) :
        libsrcml.srcml_free_unit(self.unit)

