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


# int srcml_parse_unit_filename(struct srcml_unit* unit, const char* src_filename);
libsrcml.srcml_parse_unit_filename.restype = c_int
libsrcml.srcml_parse_unit_filename.argtypes = [c_void_p, c_char_p]

# int srcml_parse_unit_memory  (struct srcml_unit*, const char* src_buffer, size_t buffer_size);
libsrcml.srcml_parse_unit_memory.restype = c_int
libsrcml.srcml_parse_unit_memory.argtypes = [c_void_p, c_char_p, c_int]

# int srcml_unparse_unit_filename(struct srcml_unit*, const char* src_filename);
libsrcml.srcml_unparse_unit_filename.restype = c_int
libsrcml.srcml_unparse_unit_filename.argtypes = [c_void_p, c_char_p]

# int srcml_unparse_unit_memory  (struct srcml_unit*, char** src_buffer, int * src_size);
libsrcml.srcml_unparse_unit_memory.restype = c_int
libsrcml.srcml_unparse_unit_memory.argtypes = [c_void_p, c_void_p, c_void_p]

# int srcml_write_unit(struct srcml_archive*, const struct srcml_unit*);
libsrcml.srcml_write_unit.restype = c_int
libsrcml.srcml_write_unit.argtypes = [c_void_p, c_void_p]

# struct srcml_unit* srcml_read_unit(struct srcml_archive*);
libsrcml.srcml_read_unit.restype = c_void_p
libsrcml.srcml_read_unit.argtypes = [c_void_p]

# struct srcml_unit* srcml_create_unit(struct srcml_archive* archive);
libsrcml.srcml_create_unit.restype = c_void_p
libsrcml.srcml_create_unit.argtypes = [c_void_p]

# int srcml_free_unit(struct srcml_unit*);
libsrcml.srcml_free_unit.restype = None
libsrcml.srcml_free_unit.argtypes = [c_void_p]

#int srcml_unit_set_language (struct srcml_unit*, const char* language);
libsrcml.srcml_unit_set_language.restype = c_int
libsrcml.srcml_unit_set_language.argtypes = [c_void_p, c_char_p]

#int srcml_unit_set_filename (struct srcml_unit*, const char* filename);
libsrcml.srcml_unit_set_filename.restype = c_int
libsrcml.srcml_unit_set_filename.argtypes = [c_void_p, c_char_p]

#int srcml_unit_set_directory(struct srcml_unit*, const char* directory);
libsrcml.srcml_unit_set_directory.restype = c_int
libsrcml.srcml_unit_set_directory.argtypes = [c_void_p, c_char_p]

#int srcml_unit_set_version  (struct srcml_unit*, const char* version);
libsrcml.srcml_unit_set_version.restype = c_int
libsrcml.srcml_unit_set_version.argtypes = [c_void_p, c_char_p]

#const char* srcml_unit_get_language (const struct srcml_unit*);
libsrcml.srcml_unit_get_language.restype = c_char_p
libsrcml.srcml_unit_get_language.argtypes = [c_void_p]

#const char* srcml_unit_get_filename (const struct srcml_unit*);
libsrcml.srcml_unit_get_filename.restype = c_char_p
libsrcml.srcml_unit_get_filename.argtypes = [c_void_p]

#const char* srcml_unit_get_directory(const struct srcml_unit*);
libsrcml.srcml_unit_get_directory.restype = c_char_p
libsrcml.srcml_unit_get_directory.argtypes = [c_void_p]

#const char* srcml_unit_get_version  (const struct srcml_unit*);
libsrcml.srcml_unit_get_version.restype = c_char_p
libsrcml.srcml_unit_get_version.argtypes = [c_void_p]

#const char* srcml_unit_get_xml      (const struct srcml_unit*);
libsrcml.srcml_unit_get_xml.restype = c_char_p
libsrcml.srcml_unit_get_xml.argtypes = [c_void_p]

import srcml_archive
import srcml_unit

srcml_archive = srcml_archive.srcml_archive
srcml_unit = srcml_unit.srcml_unit
