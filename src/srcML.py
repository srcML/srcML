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

# srcMLTranslator functions
libsrcml.srcml_new.restype = c_void_p
libsrcml.srcml_new.argtypes = [c_int, c_ulonglong]

libsrcml.srcml_new_long.restype = c_void_p
libsrcml.srcml_new_long.argtypes = [c_int, c_char_p, c_char_p, c_ulonglong, c_char_p, c_char_p, c_char_p, c_char_p * 8, c_int]

libsrcml.srcml_set_input.restype = c_void_p
libsrcml.srcml_set_input.argtypes = [c_void_p, c_char_p]

libsrcml.srcml_set_input_string.restype = c_void_p
libsrcml.srcml_set_input_string.argtypes = [c_void_p, c_char_p]

libsrcml.srcml_close.restype = None
libsrcml.srcml_close.argtypes = [c_void_p]

libsrcml.srcml_translate.restype = None
libsrcml.srcml_translate.argtypes = [c_void_p, c_char_p, c_char_p, c_char_p, c_char_p, c_int]

libsrcml.srcml_get_srcml.restype = c_char_p
libsrcml.srcml_get_srcml.argtypes = [c_void_p]

libsrcml.srcml_delete.restype = None
libsrcml.srcml_delete.argtypes = [c_void_p]

# srcMLUtility Functions
libsrcml.srcml_utility_file_new.restype = c_void_p
libsrcml.srcml_utility_file_new.argtypes = [c_char_p, c_char_p, c_ulonglong, c_char_p]

libsrcml.srcml_utility_memory_new.restype = c_void_p
libsrcml.srcml_utility_memory_new.argtypes = [c_char_p, c_int, c_char_p, c_ulonglong, c_char_p]

libsrcml.srcml_extract_text_file.restype = None
libsrcml.srcml_extract_text_file.argtypes = [c_void_p, c_char_p, c_char_p, c_int]

libsrcml.srcml_extract_text_buffer.restype = c_char_p
libsrcml.srcml_extract_text_buffer.argtypes = [c_void_p, c_int]

libsrcml.srcml_unit_count.restype = c_int
libsrcml.srcml_unit_count.argtypes = [c_void_p, c_void_p]

libsrcml.srcml_extract_xml_buffer.restype = c_char_p
libsrcml.srcml_extract_xml_buffer.argtypes = [c_void_p, c_int]

libsrcml.srcml_long_info.restype = c_char_p
libsrcml.srcml_long_info.argtypes = [c_void_p]

libsrcml.srcml_delete.restype = None
libsrcml.srcml_delete.argtypes = [c_ulonglong]

# free functions
libsrcml.srcml_version_string.restype = c_char_p
libsrcml.srcml_version_string.argtypes = []

def srcml_version_string() :
    return libsrcml.srcml_version_string()

class srcMLTranslator(object):

    def __init__(self, language, op):
        self.translator = libsrcml.srcml_new(language, op)

    def __init__(self, language, src_encoding, xml_encoding, op, directory, filename, version, uri, tabsize):
        self.translator = libsrcml.srcml_new_long(language, src_encoding, xml_encoding, op, directory, filename, version, uri, tabsize)

    def setInput(self, path):
        return libsrcml.srcml_set_input(self.translator, path)

    def setInputString(self, source):
        return libsrcml.srcml_set_input_string(self.translator, source)

    def close(self):
        libsrcml.srcml_close(self.translator)

    def translate(self, path, unit_directory, unit_filename, unit_version, language):
        libsrcml.srcml_translate(self.translator, path, unit_directory, unit_filename, unit_version, language)

    def getsrcML(self) :
        return libsrcml.srcml_get_srcml(self.translator)

    def delete(self) :
        libsrcml.srcml_delete(self.translator)


class srcMLUtility(object):

    def __init__(self, filename, src_encoding, options, diff_version) :
        self.utility = c_void_p(libsrcml.srcml_utility_file_new(filename, src_encoding, options, diff_version))

    def __init__(self, buffer, size, src_encoding, options, diff_version) :
        self.utility = c_void_p(libsrcml.srcml_utility_memory_new(buffer, size, src_encoding, options, diff_version))

    def extract_text(self, to_dir, ofilename, unit) :
        libsrcml.srcml_extract_text_file(self.utility, to_dir, ofilename, unit) 

    def extract_text(self, unit) :
        return libsrcml.srcml_extract_text_buffer(self.utility, unit) 

    def unit_count(self, file) :
        return libsrcml.srcml_unit_count(self.utility, file) 

    def extract_xml(self, unit) :
        return libsrcml.srcml_extract_xml_buffer(self.utility, unit) 

    def long_info(self) :
        return libsrcml.srcml_long_info(self.utility) 

    def delete(self) :
        libsrcml.srcml_utility_delete(self.utility)

# test
#from srcMLapps import *
#translator = srcMLTranslator(2, "ISO-8859-1", "ISO-8859-1", 0, "directory", "file", "version", URI_PREFIX, 8)
#translator.setInputString("a;")
#translator.translate("a", "b", "c", "d", 2)
#translator.close()
#print translator.getsrcML()
#translator.delete()

#srcml = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
#<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
#</unit>"""

#utility = srcMLUtility(srcml, len(srcml) + 1, "UTF-8", 0, "")
#print utility.long_info()
#print utility.extract_text(1)
#utility.delete()

#print srcml_version()
