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

libsrcml.srcml_new.restype = c_void_p
libsrcml.srcml_new.argtypes = [c_int, c_ulonglong]

libsrcml.srcml_new_long.restype = c_void_p
libsrcml.srcml_new_long.argtypes = [c_int, c_char_p, c_char_p, c_ulonglong, c_char_p, c_char_p, c_char_p * 7, c_int]

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

# test
#translator = srcMLTranslator(2, 0)
#translator.setInputString("a;")
#translator.translate("a", "b", "c", "d", 2)
#translator.close()
#print translator.getsrcML()
#translator.delete()
