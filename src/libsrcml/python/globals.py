##
# @file globals.py
#
# @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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

import os
from ctypes import cdll, c_int, c_char_p, POINTER, c_ulonglong
from exception import *

LIBSRCML_PATH=""
if os.path.exists('../../../bin/libsrcml.dylib') :
    LIBSRCML_PATH = "../../../bin/libsrcml.dylib"
elif os.path.exists('../../../bin/libsrcml.so') :
    LIBSRCML_PATH = "../../../bin/libsrcml.so"
elif os.path.exists('../../../bin/libsrcml.dll') :
    LIBSRCML_PATH = "../../../bin/libsrcml.dll"
elif os.path.exists('../bin/libsrcml.dylib') :
    LIBSRCML_PATH = "../bin/libsrcml.dylib"
elif os.path.exists('../bin/libsrcml.so') :
    LIBSRCML_PATH = "../bin/libsrcml.so"
elif os.path.exists('../bin/libsrcml.dll') :
    LIBSRCML_PATH = "../bin/libsrcml.dll"
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

libsrcml = cdll.LoadLibrary(LIBSRCML_PATH)

# void srcml_cleanup_globals();
libsrcml.srcml_cleanup_globals.restype = None
libsrcml.srcml_cleanup_globals.argtypes = []

# int srcml_version_number();
libsrcml.srcml_version_number.restype = c_int
libsrcml.srcml_version_number.argtypes = []

# const char* srcml_version_string();
libsrcml.srcml_version_string.restype = c_char_p
libsrcml.srcml_version_string.argtypes = []

def cleanup_globals() :
    libsrcml.srcml_cleanup_globals()

def version_number() :
    return libsrcml.srcml_version_number()

def version_string() :
    return libsrcml.srcml_version_string()

# int srcml(const char* input_filename, const char* output_filename);
libsrcml.srcml.restype = c_int
libsrcml.srcml.argtypes = [c_char_p, c_char_p]

# int srcml_set_encoding  (const char* encoding);
libsrcml.srcml_set_encoding.restype = c_int
libsrcml.srcml_set_encoding.argtypes = [c_char_p]

# int srcml_set_language  (const char* language);
libsrcml.srcml_set_language.restype = c_int
libsrcml.srcml_set_language.argtypes = [c_char_p]

# int srcml_set_filename  (const char* filename);
libsrcml.srcml_set_filename.restype = c_int
libsrcml.srcml_set_filename.argtypes = [c_char_p]

# int srcml_set_directory (const char* directory);
libsrcml.srcml_set_directory.restype = c_int
libsrcml.srcml_set_directory.argtypes = [c_char_p]

# int srcml_set_version   (const char* version);
libsrcml.srcml_set_version.restype = c_int
libsrcml.srcml_set_version.argtypes = [c_char_p]

# int srcml_set_options   (int option);
libsrcml.srcml_set_options.restype = c_int
libsrcml.srcml_set_options.argtypes = [c_ulonglong]

# int srcml_enable_option    (int option);
libsrcml.srcml_enable_option.restype = c_int
libsrcml.srcml_enable_option.argtypes = [c_ulonglong]

# int srcml_disable_option  (int option);
libsrcml.srcml_disable_option.restype = c_int
libsrcml.srcml_disable_option.argtypes = [c_ulonglong]

# int srcml_set_tabstop   (int tabstop);
libsrcml.srcml_set_tabstop.restype = c_int
libsrcml.srcml_set_tabstop.argtypes = [c_int]

# int srcml_register_file_extension(const char* extension, const char* language);
libsrcml.srcml_register_file_extension.restype = c_int
libsrcml.srcml_register_file_extension.argtypes = [c_char_p, c_char_p]

# int srcml_register_namespace(const char* prefix, const char* ns);
libsrcml.srcml_register_namespace.restype = c_int
libsrcml.srcml_register_namespace.argtypes = [c_char_p, c_char_p]

# const char* srcml_get_encoding ();
libsrcml.srcml_get_encoding.restype = c_char_p
libsrcml.srcml_get_encoding.argtypes = []

# const char* srcml_get_language ();
libsrcml.srcml_get_language.restype = c_char_p
libsrcml.srcml_get_language.argtypes = []

# const char* srcml_get_filename ();
libsrcml.srcml_get_filename.restype = c_char_p
libsrcml.srcml_get_filename.argtypes = []

# const char* srcml_get_directory();
libsrcml.srcml_get_directory.restype = c_char_p
libsrcml.srcml_get_directory.argtypes = []

# const char* srcml_get_version  ();
libsrcml.srcml_get_version.restype = c_char_p
libsrcml.srcml_get_version.argtypes = []

# int         srcml_get_options  ();
libsrcml.srcml_get_options.restype = c_ulonglong
libsrcml.srcml_get_options.argtypes = []

# int         srcml_get_tabstop  ();
libsrcml.srcml_get_tabstop.restype = c_int
libsrcml.srcml_get_tabstop.argtypes = []

# int         srcml_get_namespace_size();
libsrcml.srcml_get_namespace_size.restype = c_int
libsrcml.srcml_get_namespace_size.argtypes = []

# const char* srcml_get_namespae_prefix(int pos);
libsrcml.srcml_get_namespace_prefix.restype = c_char_p
libsrcml.srcml_get_namespace_prefix.argtypes = [c_int]

# const char* srcml_get_prefix_from_uri(const char* namespace_uri);
libsrcml.srcml_get_prefix_from_uri.restype = c_char_p
libsrcml.srcml_get_prefix_from_uri.argtypes = [c_char_p]

# const char* srcml_get_namespace_uri(int pos);
libsrcml.srcml_get_namespace_uri.restype = c_char_p
libsrcml.srcml_get_namespace_uri.argtypes = [c_int]

# const char* srcml_get_uri_from_prefix(const char* prefix);
libsrcml.srcml_get_uri_from_prefix.restype = c_char_p
libsrcml.srcml_get_uri_from_prefix.argtypes = [c_char_p]

# int         srcml_get_macro_list_size();
libsrcml.srcml_get_macro_list_size.restype = c_int
libsrcml.srcml_get_macro_list_size.argtypes = []

# const char* srcml_get_macro_token(int pos);
libsrcml.srcml_get_macro_token.restype = c_char_p
libsrcml.srcml_get_macro_token.argtypes = [c_int]

# const char* srcml_get_macro_token_type(const char* token);
libsrcml.srcml_get_macro_token_type.restype = c_char_p
libsrcml.srcml_get_macro_token_type.argtypes = [c_char_p]

# const char* srcml_get_macro_type(int pos);
libsrcml.srcml_get_macro_type.restype = c_char_p
libsrcml.srcml_get_macro_type.argtypes = [c_int]

# int srcml_check_language(const char* language);
libsrcml.srcml_check_language.restype = c_int
libsrcml.srcml_check_language.argtypes = [c_char_p]

# int srcml_get_language_list_size();
libsrcml.srcml_get_language_list_size.restype = c_int
libsrcml.srcml_get_language_list_size.argtypes = []

# const char* srcml_get_language_list(int pos);
libsrcml.srcml_get_language_list.restype = c_char_p
libsrcml.srcml_get_language_list.argtypes = [c_int]

# const char* srcml_check_extension(const char* filename);
libsrcml.srcml_check_extension.restype = c_char_p
libsrcml.srcml_check_extension.argtypes = [c_char_p]

# int srcml_check_format(const char* format);
libsrcml.srcml_check_format.restype = c_int
libsrcml.srcml_check_format.argtypes = [c_char_p]

# int srcml_check_encoding(const char* encoding);
libsrcml.srcml_check_encoding.restype = c_int
libsrcml.srcml_check_encoding.argtypes = [c_char_p]

# int srcml_check_xslt();
libsrcml.srcml_check_xslt.restype = c_int
libsrcml.srcml_check_xslt.argtypes = []

# int srcml_check_exslt();
libsrcml.srcml_check_exslt.restype = c_int
libsrcml.srcml_check_exslt.argtypes = []

# const char* srcml_error_string();
libsrcml.srcml_error_string.restype = c_char_p
libsrcml.srcml_error_string.argtypes = []

def srcml(input_filename, output_filename) :
    libsrcml.srcml(input_filename, output_filename)

def set_encoding(encoding) :
    check_return(libsrcml.srcml_set_encoding(encoding))

def set_language(language) :
    check_return(libsrcml.srcml_set_language(language))

def set_filename(filename) :
    check_return(libsrcml.srcml_set_filename(filename))

def set_directory(directory) :
    check_return(libsrcml.srcml_set_directory(directory))

def set_version(version) :
    check_return(libsrcml.srcml_set_version(version))

def set_options(options) :
    check_return(libsrcml.srcml_set_options(options))

def enable_option(option) :
    check_return(libsrcml.srcml_enable_option(option))

def disable_option(option) :
    check_return(libsrcml.srcml_disable_option(option))

def set_tabstop(tabstop) :
    check_return(libsrcml.srcml_set_tabstop(tabstop))

def register_file_extension(extension, language) :
    check_return(libsrcml.srcml_register_file_extension(extension, language))

def register_namespace(prefix, ns) :
    check_return(libsrcml.srcml_register_namespace(prefix, ns))

def register_macro(token, type) :
    check_return(libsrcml.srcml_register_macro(token, type))

def get_encoding() :
    return libsrcml.srcml_get_encoding()

def get_language() :
    return libsrcml.srcml_get_language()

def get_filename() :
    return libsrcml.srcml_get_filename()

def get_directory() :
    return libsrcml.srcml_get_directory()

def get_version() :
    return libsrcml.srcml_get_version()

def get_options() :
    return libsrcml.srcml_get_options()

def get_tabstop() :
    return libsrcml.srcml_get_tabstop()

def get_namespace_size() :
    return libsrcml.srcml_get_namespace_size()

def get_namespace_prefix(pos) :
    return libsrcml.srcml_get_namespace_prefix(pos)

def get_prefix_from_uri(ns) :
    return libsrcml.srcml_get_prefix_from_uri(ns)

def get_namespace_uri(pos) :
    return libsrcml.srcml_get_namespace_uri(pos)

def get_uri_from_prefix(prefix) :
    return libsrcml.srcml_get_uri_from_prefix(prefix)

def get_macro_list_size() :
    return libsrcml.srcml_get_macro_list_size()

def get_macro_token(pos) :
    return libsrcml.srcml_get_macro_token(pos)

def get_macro_token_type(token) :
    return libsrcml.srcml_get_macro_token_type(token)

def get_macro_type(pos) :
    return libsrcml.srcml_get_macro_type(pos)

def check_language(language) :
    return libsrcml.srcml_check_language(language)

def get_language_list_size() :

    return libsrcml.srcml_get_language_list_size()

def get_language_list(pos) :

    return libsrcml.srcml_get_language_list(pos)

def check_extension(filename) :
    return libsrcml.srcml_check_extension(filename)

def check_format(format) :
    return libsrcml.srcml_check_format(format)

def check_encoding(encoding) :
    return libsrcml.srcml_check_encoding(encoding)

def check_xslt() :
    return libsrcml.srcml_check_xslt()

def check_exslt() :
    return libsrcml.srcml_check_exslt()

def error_string() :
    return libsrcml.srcml_error_string()

