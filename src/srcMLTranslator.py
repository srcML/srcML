from ctypes import *
libsrcml = cdll.LoadLibrary('../bin/libsrcml.so')

libsrcml.srcml_new.restype = c_void_p
libsrcml.srcml_new.argtypes = [c_int, c_char_p, c_ulonglong]

libsrcml.srcml_set_input.restype = c_void_p
libsrcml.srcml_set_input.argtypes = [c_void_p, c_char_p]

libsrcml.srcml_close.restype = None
libsrcml.srcml_close.argtypes = [c_void_p]

libsrcml.srcml_translate.restype = None
libsrcml.srcml_translate.argtypes = [c_void_p, c_char_p, c_char_p, c_char_p, c_char_p, c_int]

libsrcml.srcml_delete.restype = None
libsrcml.srcml_delete.argtypes = [c_void_p]

#  void srcml_close(srcMLTranslator * translator);
#  void srcml_translate(srcMLTranslator * translator, const char* path, const char* unit_directory, const char* unit_filename, const char* unit_version, int language);
#  void srcml_delete(srcMLTranslator * translator);

class srcMLTranslator(object):

    def __init__(self, language, srcml_filename, op):
        self.translator = libsrcml.srcml_new(language, srcml_filename, op)

    def setInput(self, path):
        libsrcml.srcml_set_input(self.translator, path)

    def close(self):
        libsrcml.srcml_close(self.translator)

    def translate(self, path, unit_directory, unit_filename, unit_version, language):
        libsrcml.srcml_translate(self.translator, path, unit_directory, unit_filename, unit_version, language)

# test

translator = srcMLTranslator(2, "a.cpp.xml", 0)
#translator.setInput("a.cpp")
#translator.translate("", "", "", "", 2)
