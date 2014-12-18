from ctypes import cdll, c_int, c_char_p, POINTER, c_ulonglong
import os, glob, shutil

def srcml(binaryLocation, inputFile, outputFile, fileExt, lang, windows_dep_binary_location=None):
    """
    This does minimal binding to libsrcml, just enough to get by.

    This function maps directly to the libsrcml function of the same name,
    accept the first parameter is the location of the binary file
    that's going to be loaded and accessed with the ctypes library.
    """
    print binaryLocation
    extension = os.path.splitext(binaryLocation)[1]
    # print extension
    if extension.lower() == ".dll":
        from ctypes import windll
        dependencies = glob.glob(os.path.join(windows_dep_binary_location, "*.dll"))

        dir, filename = os.path.split(binaryLocation)
        release_path = os.path.join(dir, "Release", filename)
        release_dir = os.path.join(dir, "Release")
        dbg_path = os.path.join(dir, "Debug", filename)
        dbg_dir = os.path.join(dir, "Debug")
        release_exists = os.path.exists(release_path)
        dbg_exists = os.path.exists(dbg_path)
        
        if release_exists and not(dbg_exists):
            for dep in dependencies:
                shutil.copyfile(dep, os.path.join(release_dir, os.path.split(dep)[1]))
            libsrcml = cdll.LoadLibrary(release_path)
        elif dbg_exists and not(relase_exists):
            for dep in dependencies:
                shutil.copyfile(dep, os.path.join(dbg_dir, os.path.split(dep)[1]))
            libsrcml = cdll.LoadLibrary(dbg_path)
        elif dbg_exists and relase_exists:
            if os.path.getmtime(release_path) < os.path.getmtime(dbg_path):
                for dep in dependencies:
                    shutil.copyfile(dep, os.path.join(release_dir, os.path.split(dep)[1]))
                libsrcml = cdll.LoadLibrary(release_path)
            else:
                for dep in dependencies:
                    shutil.copyfile(dep, os.path.join(dbg_dir, os.path.split(dep)[1]))
                libsrcml = cdll.LoadLibrary(dbg_path)
        elif os.path.exists(binaryLocation):
            for dep in dependencies:
                shutil.copyfile(dep, os.path.join(dir, os.path.split(dep)[1]))
            libsrcml = cdll.LoadLibrary(binaryLocation)
        else:
            raise Exception("Error: cannot locate binary file in expected directories")
        libsrcml.srcml_register_file_extension.restype = c_int
        libsrcml.srcml_register_file_extension.argtypes = [c_char_p, c_char_p]
        libsrcml.srcml.restype = c_int
        libsrcml.srcml.argtypes = [c_char_p, c_char_p]
        libsrcml.srcml_register_file_extension(fileExt, lang)
        ret = libsrcml.srcml(inputFile, outputFile)
        if ret != 0:
            raise Exception("Failed to run srcml file. Error Code: {1}. Input File: {0}. ".format(inputFile, ret))
    
    else:
        libsrcml = cdll.LoadLibrary(binaryLocation)
        libsrcml.srcml_register_file_extension.restype = c_int
        libsrcml.srcml_register_file_extension.argtypes = [c_char_p, c_char_p]
        libsrcml.srcml.restype = c_int
        libsrcml.srcml.argtypes = [c_char_p, c_char_p]
        libsrcml.srcml_register_file_extension(fileExt, lang)
        ret = libsrcml.srcml(inputFile, outputFile)
        if ret != 0:
            raise Exception("Failed to run srcml file. Error Code: {1}. Input File: {0}. ".format(inputFile, ret))
