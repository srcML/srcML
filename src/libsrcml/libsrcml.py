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
libsrcml.srcml_new.restype = c_void_p
libsrcml.srcml_new.argtypes = []

# int srcml_write_open_filename(struct srcml_archive*, const char* srcml_filename);
libsrcml.srcml_new.restype = c_int
libsrcml.srcml_new.argtypes = [c_void_p, c_char_p]

