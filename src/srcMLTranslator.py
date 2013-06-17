from ctypes import cdll
libsrcml = cdll.LoadLibrary('../bin/libsrcml.a')

#  srcMLTranslator * srcml_new(int language, const char* srcml_filename, OPTION_TYPE& op);
#  void* srcml_set_input(srcMLTranslator * translator, const char* path);
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
