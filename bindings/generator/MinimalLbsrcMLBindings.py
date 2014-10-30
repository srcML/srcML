from ctypes import cdll, c_int, c_char_p, POINTER, c_ulonglong

def srcml(binaryLocation, inputFile, outputFile, fileExt, lang):
    """
    This does minimal binding to libsrcml, just enough to get by.

    This function maps directly to the libsrcml function of the same name,
    accept the first parameter is the location of the binary file
    that's going to be loaded and accessed with the ctypes library.
    """
    libsrcml = cdll.LoadLibrary(binaryLocation)
    libsrcml.srcml_register_file_extension.restype = c_int
    libsrcml.srcml_register_file_extension.argtypes = [c_char_p, c_char_p]
    libsrcml.srcml.restype = c_int
    libsrcml.srcml.argtypes = [c_char_p, c_char_p]
    libsrcml.srcml_register_file_extension(fileExt, lang)
    ret = libsrcml.srcml(inputFile, outputFile)
    if ret != 0:
        raise Exception("Failed to run srcml file. Error Code: {1}. Input File: {0}. ".format(inputFile, ret))
