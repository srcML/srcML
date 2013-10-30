import os
from ctypes import cdll, c_int, c_char_p

LIBSRCML_PATH=""
if os.path.exists('../../../bin/libsrcml.dylib') :
    LIBSRCML_PATH="../../../bin/libsrcml.dylib"
elif os.path.exists('../../../bin/libsrcml.so') :
    LIBSRCML_PATH="../../../bin/libsrcml.so"
elif os.path.exists('../../../bin/libsrcml.dll') :
    LIBSRCML_PATH="../../../bin/libsrcml.dll"

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

def cleanup_globals() :
    libsrcml.srcml_cleanup_globals()

def version_number() :
    return libsrcml.srcml_version_number()

def version_string() :
    return libsrcml.srcml_version_string()
