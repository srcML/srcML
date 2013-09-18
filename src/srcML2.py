from ctypes import *
from srcMLapps import *
import os
LIBSRCML_PATH=""
if os.path.exists('../bin/libsrcml.dylib') :
    LIBSRCML_PATH="../bin/libsrcml.dylib"
elif os.path.exists('../bin/libsrcml.so') :
    LIBSRCML_PATH="../bin/libsrcml.so"
elif os.path.exists('../bin/libsrcml.dll') :
    LIBSRCML_PATH="../bin/libsrcml.dll"

libsrcml = cdll.LoadLibrary(LIBSRCML_PATH)

# srcml_archive
libsrcml.srcml_create_archive.restype = c_void_p
libsrcml.srcml_create_archive.argtypes = []

libsrcml.srcml_free_archive.restype = None
libsrcml.srcml_free_archive.argtypes = [c_void_p]

libsrcml.srcml_close_archive.restype = None
libsrcml.srcml_close_archive.argtypes = [c_void_p]

libsrcml.srcml_write_open_memory.restype = c_int
libsrcml.srcml_write_open_memory.argtypes = [c_void_p, c_void_p]

libsrcml.srcml_write_unit.restype = c_int
libsrcml.srcml_write_unit.argtypes = [c_void_p, c_void_p]

# srcml_unit
libsrcml.srcml_create_unit.restype = c_void_p
libsrcml.srcml_create_unit.argtypes = [c_void_p]

libsrcml.srcml_free_unit.restype = None
libsrcml.srcml_free_unit.argtypes = [c_void_p]

libsrcml.srcml_parse_unit_memory.restype = c_int
libsrcml.srcml_parse_unit_memory.argtypes = [c_void_p, c_char_p, c_int]


# free functions
libsrcml.srcml_version_string.restype = c_char_p
libsrcml.srcml_version_string.argtypes = []

def srcml_version_string() :
    return libsrcml.srcml_version_string()

class srcMLTranslator(object):

    def __init__(self):
        self.archive = libsrcml.srcml_create_archive()
        self.buffer = c_char_p()
        libsrcml.srcml_write_open_memory(self.archive, pointer(self.buffer))

    def translate(self, src):
        self.unit = libsrcml.srcml_create_unit(self.archive)
        libsrcml.srcml_parse_unit_memory(self.unit, src, len(src))
        libsrcml.srcml_write_unit(self.archive, self.unit)
        libsrcml.srcml_free_unit(self.unit)

    def close(self) :
        libsrcml.srcml_close_archive(self.archive)

    def getsrcML(self) :
        return self.buffer.value

    def delete(self) :
        libsrcml.srcml_free_archive(self.archive)

# test
#from srcMLapps import *
translator = srcMLTranslator()
translator.translate("a;")
translator.close()
print translator.getsrcML()
translator.delete()

#srcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
#<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
#</unit>"""

#utility = srcMLUtility(srcml, len(srcml) + 1, "UTF-8", 0, "")
#print utility.long_info()
#print utility.extract_text(1)
#utility.delete()

#print srcml_version()
