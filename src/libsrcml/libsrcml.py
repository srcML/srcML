from ctypes import *
import os

LIBSRCML_PATH=""
if os.path.exists('../../bin/libsrcml.dylib') :
    LIBSRCML_PATH="../../bin/libsrcml.dylib"
elif os.path.exists('../../bin/libsrcml.so') :
    LIBSRCML_PATH="../../bin/libsrcml.so"
elif os.path.exists('../../bin/libsrcml.dll') :
    LIBSRCML_PATH="../../bin/libsrcml.dll"

libsrcml = cdll.LoadLibrary(LIBSRCML_PATH)
