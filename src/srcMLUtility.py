from ctypes import *
import os
LIBSRCML_PATH=""
if os.path.exists('../bin/libsrcml.dylib') :
    LIBSRCML_PATH="../bin/libsrcml.dylib"
elif os.path.exists('../bin/libsrcml.so') :
    LIBSRCML_PATH="../bin/libsrcml.so"
elif os.path.exists('../bin/libsrcml.dll') :
    LIBSRCML_PATH="../bin/libsrcml.dll"

libsrcml = cdll.LoadLibrary(LIBSRCML_PATH)

libsrcml.srcml_utility_file_new.restype = c_void_p
libsrcml.srcml_utility_file_new.argtypes = [c_char_p, c_char_p, c_ulonglong, c_char_p]

libsrcml.srcml_utility_memory_new.restype = c_void_p
libsrcml.srcml_utility_memory_new.argtypes = [c_char_p, c_int, c_char_p, c_ulonglong, c_char_p]

libsrcml.srcml_extract_text_file.restype = None
libsrcml.srcml_extract_text_file.argtypes = [c_void_p, c_char_p, c_char_p, c_int]

libsrcml.srcml_extract_text_buffer.restype = c_char_p
libsrcml.srcml_extract_text_buffer.argtypes = [c_void_p, c_int]

libsrcml.srcml_delete.restype = None
libsrcml.srcml_delete.argtypes = [c_ulonglong]

class srcMLUtility(object):

    def __init__(self, filename, src_encoding, options, diff_version) :
        self.utility = c_void_p(libsrcml.srcml_utility_file_new(filename, src_encoding, options, diff_version))

    def __init__(self, buffer, size, src_encoding, options, diff_version) :
        self.utility = c_void_p(libsrcml.srcml_utility_memory_new(buffer, size, src_encoding, options, diff_version))

    def extract_text(self, to_dir, ofilename, unit) :
        libsrcml.srcml_extract_text_file(self.utility, to_dir, ofilename, unit) 

    def extract_text(self, unit) :
        return libsrcml.srcml_extract_text_buffer(self.utility, unit) 

    def delete(self) :
        libsrcml.srcml_utility_delete(self.utility)

# test
srcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>"""

utility = srcMLUtility(srcml, len(srcml) + 1, "UTF-8", 0, "")
print utility.extract_text(1)
utility.delete()
