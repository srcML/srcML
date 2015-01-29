##
# @file srcml_unit.py
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

from globals import libsrcml
from ctypes import c_int, c_void_p, c_char_p, pointer, CFUNCTYPE
from exception import *

write_callback_t = CFUNCTYPE(c_int, c_void_p, c_char_p, c_int)
read_callback_t  = CFUNCTYPE(c_int, c_void_p, c_char_p, c_int)
close_callback_t = CFUNCTYPE(c_int, c_void_p)

# int srcml_parse_unit_filename(struct srcml_unit* unit, const char* src_filename);
libsrcml.srcml_parse_unit_filename.restype = c_int
libsrcml.srcml_parse_unit_filename.argtypes = [c_void_p, c_char_p]

# int srcml_parse_unit_memory  (struct srcml_unit*, const char* src_buffer, size_t buffer_size);
libsrcml.srcml_parse_unit_memory.restype = c_int
libsrcml.srcml_parse_unit_memory.argtypes = [c_void_p, c_char_p, c_int]

# int srcml_parse_unit_FILE    (struct srcml_unit*, FILE* src_file);
libsrcml.srcml_parse_unit_FILE.restype = c_int
libsrcml.srcml_parse_unit_FILE.argtypes = [c_void_p, c_void_p]

# int srcml_parse_unit_fd      (struct srcml_unit*, int src_fd);
libsrcml.srcml_parse_unit_fd.restype = c_int
libsrcml.srcml_parse_unit_fd.argtypes = [c_void_p, c_int]

# int srcml_parse_unit_io      (struct srcml_unit*, void * context, int (*read_callback)(void * context, char * buffer, int len), int (*close_callback)(void * context));
libsrcml.srcml_parse_unit_io.restype = c_int
libsrcml.srcml_parse_unit_io.argtypes = [c_void_p, c_void_p, read_callback_t, close_callback_t]

# int srcml_write_start_unit(struct srcml_unit*);
libsrcml.srcml_write_start_unit.restype = c_int
libsrcml.srcml_write_start_unit.argtypes = [c_void_p]

# int srcml_write_end_unit(struct srcml_unit*);
libsrcml.srcml_write_end_unit.restype = c_int
libsrcml.srcml_write_end_unit.argtypes = [c_void_p]

# int srcml_write_start_element(struct srcml_unit*, const char * prefix, const char * name, const char * uri);
libsrcml.srcml_write_start_element.restype = c_int
libsrcml.srcml_write_start_element.argtypes = [c_void_p, c_char_p, c_char_p, c_char_p]

# int srcml_write_end_element(struct srcml_unit*);
libsrcml.srcml_write_end_element.restype = c_int
libsrcml.srcml_write_end_element.argtypes = [c_void_p]

# int srcml_write_namespace(struct srcml_unit*, const char * prefix, const char * uri);
libsrcml.srcml_write_namespace.restype = c_int
libsrcml.srcml_write_namespace.argtypes = [c_void_p, c_char_p, c_char_p]

# int srcml_write_attribute(struct srcml_unit*, const char * prefix, const char * name, const char * uri, const char * content);
libsrcml.srcml_write_attribute.restype = c_int
libsrcml.srcml_write_attribute.argtypes = [c_void_p, c_char_p, c_char_p, c_char_p, c_char_p]

# int srcml_write_string(struct srcml_unit*, const char * content);
libsrcml.srcml_write_string.restype = c_int
libsrcml.srcml_write_string.argtypes = [c_void_p, c_char_p]

# int srcml_unparse_unit_filename(struct srcml_unit*, const char* src_filename);
libsrcml.srcml_unparse_unit_filename.restype = c_int
libsrcml.srcml_unparse_unit_filename.argtypes = [c_void_p, c_char_p]

# int srcml_unparse_unit_memory  (struct srcml_unit*, char** src_buffer, int * src_size);
libsrcml.srcml_unparse_unit_memory.restype = c_int
libsrcml.srcml_unparse_unit_memory.argtypes = [c_void_p, c_void_p, c_void_p]

# int srcml_unparse_unit_FILE    (struct srcml_unit*, FILE* srcml_file);
libsrcml.srcml_unparse_unit_FILE.restype = c_int
libsrcml.srcml_unparse_unit_FILE.argtypes = [c_void_p, c_void_p]

# int srcml_unparse_unit_fd      (struct srcml_unit*, int srcml_fd);
libsrcml.srcml_unparse_unit_fd.restype = c_int
libsrcml.srcml_unparse_unit_fd.argtypes = [c_void_p, c_int]

# int srcml_unparse_unit_io      (struct srcml_unit*, void * context, int (*write_callback)(void * context, const char * buffer, int len), int (*close_callback)(void * context));
libsrcml.srcml_unparse_unit_io.restype = c_int
libsrcml.srcml_unparse_unit_io.argtypes = [c_void_p, c_void_p, write_callback_t, close_callback_t]

# struct srcml_unit* srcml_unit_new(struct srcml_archive* archive);
libsrcml.srcml_unit_new.restype = c_void_p
libsrcml.srcml_unit_new.argtypes = [c_void_p]

# void srcml_unit_free(struct srcml_unit*);
libsrcml.srcml_unit_free.restype = None
libsrcml.srcml_unit_free.argtypes = [c_void_p]

# int srcml_unit_set_src_encoding (struct srcml_unit*, const char* encoding);
libsrcml.srcml_unit_set_src_encoding.restype = c_int
libsrcml.srcml_unit_set_src_encoding.argtypes = [c_void_p, c_char_p]

# int srcml_unit_set_language (struct srcml_unit*, const char* language);
libsrcml.srcml_unit_set_language.restype = c_int
libsrcml.srcml_unit_set_language.argtypes = [c_void_p, c_char_p]

# int srcml_unit_set_filename (struct srcml_unit*, const char* filename);
libsrcml.srcml_unit_set_filename.restype = c_int
libsrcml.srcml_unit_set_filename.argtypes = [c_void_p, c_char_p]

# int srcml_unit_set_directory(struct srcml_unit*, const char* directory);
libsrcml.srcml_unit_set_directory.restype = c_int
libsrcml.srcml_unit_set_directory.argtypes = [c_void_p, c_char_p]

# int srcml_unit_set_version  (struct srcml_unit*, const char* version);
libsrcml.srcml_unit_set_version.restype = c_int
libsrcml.srcml_unit_set_version.argtypes = [c_void_p, c_char_p]

# int srcml_unit_set_timestamp  (struct srcml_unit*, const char* timestamp);
libsrcml.srcml_unit_set_timestamp.restype = c_int
libsrcml.srcml_unit_set_timestamp.argtypes = [c_void_p, c_char_p]

# int srcml_unit_set_hash  (struct srcml_unit*, const char* hash);
libsrcml.srcml_unit_set_hash.restype = c_int
libsrcml.srcml_unit_set_hash.argtypes = [c_void_p, c_char_p]

# const char* srcml_unit_get_src_encoding (const struct srcml_unit*);
libsrcml.srcml_unit_get_src_encoding.restype = c_char_p
libsrcml.srcml_unit_get_src_encoding.argtypes = [c_void_p]

# const char* srcml_unit_get_revision (const struct srcml_unit*);
libsrcml.srcml_unit_get_revision.restype = c_char_p
libsrcml.srcml_unit_get_revision.argtypes = [c_void_p]

# const char* srcml_unit_get_language (const struct srcml_unit*);
libsrcml.srcml_unit_get_language.restype = c_char_p
libsrcml.srcml_unit_get_language.argtypes = [c_void_p]

# const char* srcml_unit_get_filename (const struct srcml_unit*);
libsrcml.srcml_unit_get_filename.restype = c_char_p
libsrcml.srcml_unit_get_filename.argtypes = [c_void_p]

# const char* srcml_unit_get_directory(const struct srcml_unit*);
libsrcml.srcml_unit_get_directory.restype = c_char_p
libsrcml.srcml_unit_get_directory.argtypes = [c_void_p]

# const char* srcml_unit_get_version  (const struct srcml_unit*);
libsrcml.srcml_unit_get_version.restype = c_char_p
libsrcml.srcml_unit_get_version.argtypes = [c_void_p]

# const char* srcml_unit_get_timestamp  (const struct srcml_unit*);
libsrcml.srcml_unit_get_timestamp.restype = c_char_p
libsrcml.srcml_unit_get_timestamp.argtypes = [c_void_p]

# const char* srcml_unit_get_hash  (const struct srcml_unit*);
libsrcml.srcml_unit_get_hash.restype = c_char_p
libsrcml.srcml_unit_get_hash.argtypes = [c_void_p]

# const char* srcml_unit_get_raw_xml      (const struct srcml_unit*);
libsrcml.srcml_unit_get_raw_xml.restype = c_char_p
libsrcml.srcml_unit_get_raw_xml.argtypes = [c_void_p]

# srcml_unit wrapper
class srcml_unit :

    def __init__(self, archive, unit = 0) :
        self.unit = unit
        if self.unit == 0 :
            self.unit = libsrcml.srcml_unit_new(archive.archive)

    def parse_filename(self, src_filename) :
        check_return(libsrcml.srcml_parse_unit_filename(self.unit, src_filename))

    def parse_memory(self, src_buffer) :
        check_return(libsrcml.srcml_parse_unit_memory(self.unit, src_buffer, len(src_buffer)))

    def parse_FILE(self, src_file) :
        check_return(libsrcml.srcml_parse_unit_FILE(self.unit, src_file))

    def parse_fd(self, src_fd) :
        check_return(libsrcml.srcml_parse_unit_fd(self.unit, src_fd))

    def parse_io(self, context, read_callback, close_callback) :
        check_return(libsrcml.srcml_parse_unit_io(self.unit, context, read_callback, close_callback))

    def unparse_filename(self, src_filename) :
        check_return(libsrcml.srcml_unparse_unit_filename(self.unit, src_filename))

    def unparse_memory(self) :
        self.src_size = c_int()
        self.src_buffer = c_char_p()
        check_return(libsrcml.srcml_unparse_unit_memory(self.unit, pointer(self.src_buffer), pointer(self.src_size)))

    def unparse_FILE(self, src_file) :
        check_return(libsrcml.srcml_unparse_unit_FILE(self.unit, src_file))

    def unparse_fd(self, src_fd) :
        check_return(libsrcml.srcml_unparse_unit_fd(self.unit, src_fd))

    def unparse_io(self, context, write_callback, close_callback) :
        check_return(libsrcml.srcml_unparse_unit_io(self.unit, context, write_callback, close_callback))

    def write_start_unit(self) :
        check_return(libsrcml.srcml_write_start_unit(self.unit))

    def write_end_unit(self) :
        check_return(libsrcml.srcml_write_end_unit(self.unit))

    def write_start_element(self, prefix, name, uri) :
        check_return(libsrcml.srcml_write_start_element(self.unit, prefix, name, uri))

    def write_end_element(self) :
        check_return(libsrcml.srcml_write_end_element(self.unit))

    def write_namespace(self, prefix, uri) :
        check_return(libsrcml.srcml_write_namespace(self.unit, prefix, uri))

    def write_attribute(self, prefix, name, uri, content) :
        check_return(libsrcml.srcml_write_attribute(self.unit, prefix, name, uri, content))

    def write_string(self, content) :
        check_return(libsrcml.srcml_write_string(self.unit, content))

    def set_src_encoding(self, encoding) :
        check_return(libsrcml.srcml_unit_set_src_encoding(self.unit, encoding))

    def set_language(self, language) :
        check_return(libsrcml.srcml_unit_set_language(self.unit, language))

    def set_filename(self, filename) :
        check_return(libsrcml.srcml_unit_set_filename(self.unit, filename))

    def set_directory(self, directory) :
        check_return(libsrcml.srcml_unit_set_directory(self.unit, directory))

    def set_version(self, version) :
        check_return(libsrcml.srcml_unit_set_version(self.unit, version))

    def set_timestamp(self, timestamp) :
        check_return(libsrcml.srcml_unit_set_timestamp(self.unit, timestamp))

    def set_hash(self, hash) :
        check_return(libsrcml.srcml_unit_set_hash(self.unit, hash))

    def get_src_encoding(self) :
        return libsrcml.srcml_unit_get_src_encoding(self.unit)

    def get_revision(self) :
        return libsrcml.srcml_unit_get_revision(self.unit)

    def get_language(self) :
        return libsrcml.srcml_unit_get_language(self.unit)

    def get_filename(self) :
        return libsrcml.srcml_unit_get_filename(self.unit)

    def get_directory(self) :
        return libsrcml.srcml_unit_get_directory(self.unit)

    def get_version(self) :
        return libsrcml.srcml_unit_get_version(self.unit)

    def get_timestamp(self) :
        return libsrcml.srcml_unit_get_timestamp(self.unit)

    def get_hash(self) :
        return libsrcml.srcml_unit_get_hash(self.unit)

    def get_raw_xml(self) :
        return libsrcml.srcml_unit_get_raw_xml(self.unit)

    def src(self) :
        return self.src_buffer.value

    def __del__(self) :
        libsrcml.srcml_unit_free(self.unit)

