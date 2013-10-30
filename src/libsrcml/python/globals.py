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

# int srcml(const char* input_filename, const char* output_filename);

# int srcml_set_encoding  (const char* encoding);
# int srcml_set_language  (const char* language);
# int srcml_set_filename  (const char* filename);
# int srcml_set_directory (const char* directory);
# int srcml_set_version   (const char* version);
# int srcml_set_all_options   (int option);
# int srcml_set_option    (int option);
# int srcml_clear_option  (int option);
# int srcml_set_tabstop   (int tabstop);
# int srcml_register_file_extension(const char* extension, const char* language);
# int srcml_register_namespace(const char* prefix, const char* ns);

# const char* srcml_get_encoding ();
# const char* srcml_get_language ();
# const char* srcml_get_filename ();
# const char* srcml_get_directory();
# const char* srcml_get_version  ();
# int         srcml_get_options  ();
# int         srcml_get_tabstop  ();
# int         srcml_get_namespace_size();
# const char* srcml_get_prefix(int pos);
# const char* srcml_get_prefix_uri(const char* namespace_uri);
# const char* srcml_get_namespace(int pos);
# const char* srcml_get_namespace_prefix(const char* prefix);
