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

libsrcml.srcml_utility_new.restype = c_void_p
libsrcml.srcml_utility_new.argtypes = [c_char_p, c_char_p, c_ulonglong, c_char_p]

libsrcml.srcml_extract_text.restype = None
libsrcml.srcml_extract_text.argtypes = [c_void_p, c_char_p, c_char_p, c_int]

libsrcml.srcml_delete.restype = None
libsrcml.srcml_delete.argtypes = [c_ulonglong]

class srcMLUtility(object):

    def __init__(self, filename, src_encoding, options, diff_version) :
        self.utility = c_void_p(libsrcml.srcml_utility_new(filename, src_encoding, options, diff_version))

    def extract_text(self, to_dir, ofilename, unit) :
        libsrcml.srcml_extract_text(self.utility, to_dir, ofilename, unit) 

    def delete(self) :
        libsrcml.srcml_utility_delete(self.utility)

# test
utility = srcMLUtility("a.cpp.xml", "UTF-8", 0, "")
utility.extract_text(None, "/dev/stdout", 1)
utility.delete()
