from ctypes import *
libsrcml = cdll.LoadLibrary('../bin/libsrcml.so')

libsrcml.srcml_new.restype = c_void_p
libsrcml.srcml_new.argtypes = [c_int, c_ulonglong]

libsrcml.srcml_set_input.restype = c_void_p
libsrcml.srcml_set_input.argtypes = [c_void_p, c_char_p]

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

    def setInput(self, path):

        return libsrcml.srcml_set_input(self.translator, path)

    def close(self):
        libsrcml.srcml_close(self.translator)

    def translate(self, path, unit_directory, unit_filename, unit_version, language):
        libsrcml.srcml_translate(self.translator, path, unit_directory, unit_filename, unit_version, language)

    def getsrcML(self) :
        return libsrcml.srcml_get_srcml(self.translator)

    def delete(self) :
        libsrcml.srcml_delete(self.translator)

# test
translator = srcMLTranslator(2, 0)
translator.setInput("a.cpp")
translator.translate("", "", "", "", 2)
print translator.getsrcML()
translator.close()
translator.delete()
