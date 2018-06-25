##
# @file python_test_bindings.py
#
# @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
#
# The srcML Toolkit is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# The srcML Toolkit is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with the srcML Toolkit; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


## These are minimal bindings for runtest.py

import os
from ctypes import cdll, c_int, c_void_p, c_char_p, c_ulonglong, c_size_t, pointer

LIBSRCML_PATH=""
if os.path.exists('../bin/libsrcml.dylib') :
    LIBSRCML_PATH = "../bin/libsrcml.dylib"
elif os.path.exists('../bin/libsrcml.so') :
    LIBSRCML_PATH = "../bin/libsrcml.so"
elif os.path.exists('../bin/libsrcml.dll') :
    LIBSRCML_PATH = "../bin/libsrcml.dll"
elif os.path.exists('../../bin/libsrcml.dylib') :
    LIBSRCML_PATH = "../../bin/libsrcml.dylib"
elif os.path.exists('../../bin/libsrcml.so') :
    LIBSRCML_PATH = "../../bin/libsrcml.so"
elif os.path.exists('../../bin/libsrcml.dll') :
    LIBSRCML_PATH = "../../bin/libsrcml.dll"
elif os.path.exists('../bin/Release/libsrcml.dylib') :
    LIBSRCML_PATH = "../bin/Release/libsrcml.dylib"
elif os.path.exists('../bin/Release/libsrcml.so') :
    LIBSRCML_PATH = "../bin/Release/libsrcml.so"
elif os.path.exists('../bin/Release/libsrcml.dll') :
    LIBSRCML_PATH = "../bin/Release/libsrcml.dll"
elif os.path.exists('../bin/Debug/libsrcml.dylib') :
    LIBSRCML_PATH = "../bin/Debug/libsrcml.dylib"
elif os.path.exists('../bin/Debug/libsrcml.so') :
    LIBSRCML_PATH = "../bin/Debug/libsrcml.so"
elif os.path.exists('../bin/Debug/libsrcml.dll') :
    LIBSRCML_PATH = "../bin/Debug/libsrcml.dll"
# Note: Some of the following do not make sense
# to be in /usr/bin or /usr/local/bin, so could be
# weeded out
elif os.path.exists('/usr/local/lib/libsrcml.dylib') :
    LIBSRCML_PATH = "/usr/local/lib/libsrcml.dylib"
elif os.path.exists('/usr/local/lib/libsrcml.so') :
    LIBSRCML_PATH = "/usr/local/lib/libsrcml.so"
elif os.path.exists('/usr/local/lib/libsrcml.dll') :
    LIBSRCML_PATH = "/usr/local/lib/libsrcml.dll"
elif os.path.exists('/usr/lib/libsrcml.dylib') :
    LIBSRCML_PATH = "/usr/lib/libsrcml.dylib"
elif os.path.exists('/usr/lib/libsrcml.so') :
    LIBSRCML_PATH = "/usr/lib/libsrcml.so"
elif os.path.exists('/usr/lib/libsrcml.dll') :
    LIBSRCML_PATH = "/usr/lib/libsrcml.dll"

libsrcml = cdll.LoadLibrary(LIBSRCML_PATH)

# Return status indicating no errors
SRCML_STATUS_OK = 0
def check_return(value) :
    if value == SRCML_STATUS_OK :
        return
    raise srcMLException("Received invalid return status: " + str(value))

class srcMLException(Exception) :

    def __init__(self, message) :
        self.message = message

    def __str__(self) :
        return self.message

# const char* srcml_version_string();
libsrcml.srcml_version_string.restype = c_char_p
libsrcml.srcml_version_string.argtypes = []

def version_string() :
    return libsrcml.srcml_version_string()

# int srcml_unit_parse_memory(struct srcml_unit*, const char* src_buffer, size_t buffer_size);
libsrcml.srcml_unit_parse_memory.restype = c_int
libsrcml.srcml_unit_parse_memory.argtypes = [c_void_p, c_char_p, c_int]

# int srcml_unit_unparse_memory(struct srcml_unit*, char** src_buffer, size_t * src_size);
libsrcml.srcml_unit_unparse_memory.restype = c_int
libsrcml.srcml_unit_unparse_memory.argtypes = [c_void_p, c_void_p, c_void_p]

# struct srcml_unit* srcml_unit_create(struct srcml_archive* archive);
libsrcml.srcml_unit_create.restype = c_void_p
libsrcml.srcml_unit_create.argtypes = [c_void_p]

# void srcml_unit_free(struct srcml_unit*);
libsrcml.srcml_unit_free.restype = None
libsrcml.srcml_unit_free.argtypes = [c_void_p]

# int srcml_unit_set_src_encoding(struct srcml_unit*, const char* encoding);
libsrcml.srcml_unit_set_src_encoding.restype = c_int
libsrcml.srcml_unit_set_src_encoding.argtypes = [c_void_p, c_char_p]

# int srcml_unit_set_language(struct srcml_unit*, const char* language);
libsrcml.srcml_unit_set_language.restype = c_int
libsrcml.srcml_unit_set_language.argtypes = [c_void_p, c_char_p]

# const char* srcml_unit_get_xml_fragment(const struct srcml_unit*);
libsrcml.srcml_unit_get_xml_fragment.restype = c_char_p
libsrcml.srcml_unit_get_xml_fragment.argtypes = [c_void_p]

# srcml_unit wrapper
class srcml_unit :

    def __init__(self, archive, unit = 0) :
        self.unit = unit
        if self.unit == 0 :
            self.unit = libsrcml.srcml_unit_create(archive.archive)

    def parse_memory(self, src_buffer) :
        check_return(libsrcml.srcml_unit_parse_memory(self.unit, src_buffer, len(src_buffer)))

    def unparse_memory(self) :
        self.src_size = c_size_t()
        self.src_buffer = c_char_p()
        check_return(libsrcml.srcml_unit_unparse_memory(self.unit, pointer(self.src_buffer), pointer(self.src_size)))

    def set_src_encoding(self, encoding) :
        check_return(libsrcml.srcml_unit_set_src_encoding(self.unit, encoding))

    def set_language(self, language) :
        check_return(libsrcml.srcml_unit_set_language(self.unit, language))

    def get_xml_fragment(self) :
        return libsrcml.srcml_unit_get_xml_fragment(self.unit)

    def src(self) :
        return self.src_buffer.value

    def __del__(self) :
        libsrcml.srcml_unit_free(self.unit)

# struct srcml_archive* srcml_archive_create();
libsrcml.srcml_archive_create.restype = c_void_p
libsrcml.srcml_archive_create.argtypes = []

# struct srcml_archive* srcml_archive_clone(const struct srcml_archive*);
libsrcml.srcml_archive_clone.restype = c_void_p
libsrcml.srcml_archive_clone.argtypes = [c_void_p]

# int srcml_archive_write_open_memory(struct srcml_archive*, char** buffer, size_t * size);
libsrcml.srcml_archive_write_open_memory.restype = c_int
libsrcml.srcml_archive_write_open_memory.argtypes = [c_void_p, c_void_p, c_void_p]

# int srcml_archive_read_open_memory(struct srcml_archive*, const char* buffer, size_t buffer_size);
libsrcml.srcml_archive_read_open_memory.restype = c_int
libsrcml.srcml_archive_read_open_memory.argtypes = [c_void_p, c_char_p, c_size_t]

# void srcml_archive_free(struct srcml_archive* archive);
libsrcml.srcml_archive_free.restype = None
libsrcml.srcml_archive_free.argtypes = [c_void_p]

# void srcml_archive_close(struct srcml_archive*);
libsrcml.srcml_archive_close.restype = None
libsrcml.srcml_archive_close.argtypes = [c_void_p]

# int srcml_archive_set_src_encoding(struct srcml_archive*, const char* src_encoding);
libsrcml.srcml_archive_set_src_encoding.restype = c_int
libsrcml.srcml_archive_set_src_encoding.argtypes = [c_void_p, c_char_p]

# int srcml_archive_set_xml_encoding(struct srcml_archive*, const char* encoding);
libsrcml.srcml_archive_set_xml_encoding.restype = c_int
libsrcml.srcml_archive_set_xml_encoding.argtypes = [c_void_p, c_char_p]

# int srcml_archive_set_language(struct srcml_archive*, const char* language);
libsrcml.srcml_archive_set_language.restype = c_int
libsrcml.srcml_archive_set_language.argtypes = [c_void_p, c_char_p]

# const char* srcml_archive_get_url(const struct srcml_archive*);
libsrcml.srcml_archive_get_url.restype = c_char_p
libsrcml.srcml_archive_get_url.argtypes = [c_void_p]

# int srcml_archive_get_options(const struct srcml_archive*);
libsrcml.srcml_archive_get_options.restype = c_ulonglong
libsrcml.srcml_archive_get_options.argtypes = [c_void_p]

# struct srcml_unit* srcml_archive_read_unit(struct srcml_archive*);
libsrcml.srcml_archive_read_unit.restype = c_void_p
libsrcml.srcml_archive_read_unit.argtypes = [c_void_p]

# srcml_archive wrapper
class srcml_archive :

    def __init__(self, archive = 0) :
        self.archive = archive
        if self.archive == 0 :
            self.archive = libsrcml.srcml_archive_create()

    def clone(self) :
        return srcml_archive(libsrcml.srcml_archive_clone(self.archive))

    def write_open_memory(self) :
        self.buffer = c_char_p()
        self.size = c_size_t()
        check_return(libsrcml.srcml_archive_write_open_memory(self.archive, pointer(self.buffer), pointer(self.size)))

    def read_open_memory(self, buffer) :
        check_return(libsrcml.srcml_archive_read_open_memory(self.archive, buffer, len(buffer)))

    def set_src_encoding(self, src_encoding) :
        check_return(libsrcml.srcml_archive_set_src_encoding(self.archive, src_encoding))

    def set_xml_encoding(self, encoding) :
        check_return(libsrcml.srcml_archive_set_xml_encoding(self.archive, encoding))

    def set_language(self, language) :
        check_return(libsrcml.srcml_archive_set_language(self.archive, language))

    def get_url(self) :
        return libsrcml.srcml_archive_get_url(self.archive)

    def get_options(self) :
        return libsrcml.srcml_archive_get_options(self.archive)

    def read_unit(self) :
        unit = libsrcml.srcml_archive_read_unit(self.archive)

        if unit != None :
            return srcml_unit(0, unit)
        return None

    def close(self) :
        libsrcml.srcml_archive_close(self.archive)

    def __del__(self) :
        libsrcml.srcml_archive_free(self.archive)
