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

# void srcml_free_archive(struct srcml_archive* archive);
libsrcml.srcml_free_archive.restype = None
libsrcml.srcml_free_archive.argtypes = [c_void_p]

# void srcml_close_archive(struct srcml_archive*);
libsrcml.srcml_free_archive.restype = None
libsrcml.srcml_free_archive.argtypes = [c_void_p]

# int srcml_parse_unit_filename(struct srcml_unit* unit, const char* src_filename);
libsrcml.srcml_parse_unit_filename.restype = c_int
libsrcml.srcml_parse_unit_filename.argtypes = [c_void_p, c_char_p]

# int srcml_write_unit(struct srcml_archive*, const struct srcml_unit*);
libsrcml.srcml_write_unit.restype = c_int
libsrcml.srcml_write_unit.argtypes = [c_void_p, c_void_p]

# struct srcml_unit* srcml_create_unit(struct srcml_archive* archive);
libsrcml.srcml_create_unit.restype = c_void_p
libsrcml.srcml_create_unit.argtypes = [c_void_p]

# int srcml_free_unit(struct srcml_unit*);
libsrcml.srcml_free_unit.restype = None
libsrcml.srcml_free_unit.argtypes = [c_void_p]
