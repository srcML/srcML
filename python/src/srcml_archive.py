##
# @file srcml_archive.py
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
from ctypes import c_ushort, c_int, c_size_t, c_void_p, c_char_p, pointer, c_ulonglong, CFUNCTYPE

from srcml_unit import srcml_unit
from exception import *

write_callback_t = CFUNCTYPE(c_int, c_void_p, c_char_p, c_size_t)
read_callback_t  = CFUNCTYPE(c_int, c_void_p, c_char_p, c_size_t)
close_callback_t = CFUNCTYPE(c_int, c_void_p)

# struct srcml_archive* srcml_archive_create();
libsrcml.srcml_archive_create.restype = c_void_p
libsrcml.srcml_archive_create.argtypes = []

# struct srcml_archive* srcml_archive_clone(const struct srcml_archive*);
libsrcml.srcml_archive_clone.restype = c_void_p
libsrcml.srcml_archive_clone.argtypes = [c_void_p]

# int srcml_archive_write_open_filename(struct srcml_archive*, const char* srcml_filename, unsigned short compression);
libsrcml.srcml_archive_write_open_filename.restype = c_int
libsrcml.srcml_archive_write_open_filename.argtypes = [c_void_p, c_char_p, c_ushort]

# int srcml_archive_write_open_memory  (struct srcml_archive*, char** buffer, size_t * size);
libsrcml.srcml_archive_write_open_memory.restype = c_int
libsrcml.srcml_archive_write_open_memory.argtypes = [c_void_p, c_void_p, c_void_p]

# int srcml_archive_write_open_FILE    (struct srcml_archive*, FILE* srcml_file);
libsrcml.srcml_archive_write_open_FILE.restype = c_int
libsrcml.srcml_archive_write_open_FILE.argtypes = [c_void_p, c_void_p]

# int srcml_archive_write_open_fd      (struct srcml_archive*, int srcml_fd);
libsrcml.srcml_archive_write_open_fd.restype = c_int
libsrcml.srcml_archive_write_open_fd.argtypes = [c_void_p, c_int]

# int srcml_archive_write_open_io      (struct srcml_archive*, void * context, int (*write_callback)(void * context, const char * buffer, size_t len), int (*close_callback)(void * context));
libsrcml.srcml_archive_write_open_io.restype = c_int
libsrcml.srcml_archive_write_open_io.argtypes = [c_void_p, c_void_p, write_callback_t, close_callback_t]

# int srcml_archive_read_open_filename(struct srcml_archive*, const char* srcml_filename);
libsrcml.srcml_archive_read_open_filename.restype = c_int
libsrcml.srcml_archive_read_open_filename.argtypes = [c_void_p, c_char_p]

# int srcml_archive_read_open_memory  (struct srcml_archive*, const char* buffer, size_t buffer_size);
libsrcml.srcml_archive_read_open_memory.restype = c_int
libsrcml.srcml_archive_read_open_memory.argtypes = [c_void_p, c_char_p, c_size_t]

# int srcml_archive_read_open_FILE    (struct srcml_archive*, FILE* srcml_file);
libsrcml.srcml_archive_read_open_FILE.restype = c_int
libsrcml.srcml_archive_read_open_FILE.argtypes = [c_void_p, c_void_p]

# int srcml_archive_read_open_fd      (struct srcml_archive*, int srcml_fd);
libsrcml.srcml_archive_read_open_fd.restype = c_int
libsrcml.srcml_archive_read_open_fd.argtypes = [c_void_p, c_int]

# int srcml_archive_read_open_io      (struct srcml_archive*, void * context, int (*read_callback)(void * context, char * buffer, size_t len), int (*close_callback)(void * context));
libsrcml.srcml_archive_read_open_io.restype = c_int
libsrcml.srcml_archive_read_open_io.argtypes = [c_void_p, c_void_p, read_callback_t, close_callback_t]

# void srcml_archive_free(struct srcml_archive* archive);
libsrcml.srcml_archive_free.restype = None
libsrcml.srcml_archive_free.argtypes = [c_void_p]

# void srcml_archive_close(struct srcml_archive*);
libsrcml.srcml_archive_close.restype = None
libsrcml.srcml_archive_close.argtypes = [c_void_p]

# int srcml_archive_set_src_encoding  (struct srcml_archive*, const char* src_encoding);
libsrcml.srcml_archive_set_src_encoding.restype = c_int
libsrcml.srcml_archive_set_src_encoding.argtypes = [c_void_p, c_char_p]

# int srcml_archive_set_xml_encoding  (struct srcml_archive*, const char* encoding);
libsrcml.srcml_archive_set_xml_encoding.restype = c_int
libsrcml.srcml_archive_set_xml_encoding.argtypes = [c_void_p, c_char_p]

# int srcml_archive_set_language  (struct srcml_archive*, const char* language);
libsrcml.srcml_archive_set_language.restype = c_int
libsrcml.srcml_archive_set_language.argtypes = [c_void_p, c_char_p]

# int srcml_archive_set_url (struct srcml_archive*, const char* url)
libsrcml.srcml_archive_set_url.restype = c_int
libsrcml.srcml_archive_set_url.argtypes = [c_void_p, c_char_p]

# int srcml_archive_set_version   (struct srcml_archive*, const char* version)
libsrcml.srcml_archive_set_version.restype = c_int
libsrcml.srcml_archive_set_version.argtypes = [c_void_p, c_char_p]

# int srcml_archive_set_options   (struct srcml_archive*, int option);
libsrcml.srcml_archive_set_options.restype = c_int
libsrcml.srcml_archive_set_options.argtypes = [c_void_p, c_ulonglong]

# int srcml_archive_enable_option    (struct srcml_archive*, int option);
libsrcml.srcml_archive_enable_option.restype = c_int
libsrcml.srcml_archive_enable_option.argtypes = [c_void_p, c_ulonglong]

# int srcml_archive_disable_option  (struct srcml_archive*, int option);
libsrcml.srcml_archive_disable_option.restype = c_int
libsrcml.srcml_archive_disable_option.argtypes = [c_void_p, c_ulonglong]

# int srcml_archive_set_tabstop   (struct srcml_archive*, size_t tabstop);
libsrcml.srcml_archive_set_tabstop.restype = c_int
libsrcml.srcml_archive_set_tabstop.argtypes = [c_void_p, c_size_t]

# int srcml_archive_register_file_extension(struct srcml_archive*, const char* extension, const char* language);
libsrcml.srcml_archive_register_file_extension.restype = c_int
libsrcml.srcml_archive_register_file_extension.argtypes = [c_void_p, c_char_p, c_char_p]

# int srcml_archive_register_namespace(struct srcml_archive*, const char* prefix, const char* ns);
libsrcml.srcml_archive_register_namespace.restype = c_int
libsrcml.srcml_archive_register_namespace.argtypes = [c_void_p, c_char_p, c_char_p]

# int srcml_archive_register_macro(struct srcml_archive*, const char* token, const char* type);
libsrcml.srcml_archive_register_macro.restype = c_int
libsrcml.srcml_archive_register_macro.argtypes = [c_void_p, c_char_p, c_char_p]

# const char* srcml_archive_get_src_encoding (const struct srcml_archive*);
libsrcml.srcml_archive_get_src_encoding.restype = c_char_p
libsrcml.srcml_archive_get_src_encoding.argtypes = [c_void_p]

# const char* srcml_archive_get_xml_encoding (const struct srcml_archive*);
libsrcml.srcml_archive_get_xml_encoding.restype = c_char_p
libsrcml.srcml_archive_get_xml_encoding.argtypes = [c_void_p]

# const char* srcml_archive_get_revision (const struct srcml_archive*);
libsrcml.srcml_archive_get_revision.restype = c_char_p
libsrcml.srcml_archive_get_revision.argtypes = [c_void_p]

# const char* srcml_archive_get_language (const struct srcml_archive*);
libsrcml.srcml_archive_get_language.restype = c_char_p
libsrcml.srcml_archive_get_language.argtypes = [c_void_p]

# const char* srcml_archive_get_url(const struct srcml_archive*);
libsrcml.srcml_archive_get_url.restype = c_char_p
libsrcml.srcml_archive_get_url.argtypes = [c_void_p]

# const char* srcml_archive_get_version  (const struct srcml_archive*);
libsrcml.srcml_archive_get_version.restype = c_char_p
libsrcml.srcml_archive_get_version.argtypes = [c_void_p]

# int         srcml_archive_get_options  (const struct srcml_archive*);
libsrcml.srcml_archive_get_options.restype = c_ulonglong
libsrcml.srcml_archive_get_options.argtypes = [c_void_p]

# size_t         srcml_archive_get_tabstop  (const struct srcml_archive*);
libsrcml.srcml_archive_get_tabstop.restype = c_size_t
libsrcml.srcml_archive_get_tabstop.argtypes = [c_void_p]

# size_t         srcml_archive_get_namespace_size(const struct srcml_archive* archive);
libsrcml.srcml_archive_get_namespace_size.restype = c_size_t
libsrcml.srcml_archive_get_namespace_size.argtypes = [c_void_p]

# const char* srcml_archive_get_namespace_prefix(const struct srcml_archive* archive, size_t pos);
libsrcml.srcml_archive_get_namespace_prefix.restype = c_char_p
libsrcml.srcml_archive_get_namespace_prefix.argtypes = [c_void_p, c_int]

# const char* srcml_archive_get_prefix_from_uri(const struct srcml_archive* archive, const char* namespace_uri);
libsrcml.srcml_archive_get_prefix_from_uri.restype = c_char_p
libsrcml.srcml_archive_get_prefix_from_uri.argtypes = [c_void_p, c_char_p]

# const char* srcml_archive_get_namespace_uri(const struct srcml_archive* archive, size_t pos);
libsrcml.srcml_archive_get_namespace_uri.restype = c_char_p
libsrcml.srcml_archive_get_namespace_uri.argtypes = [c_void_p, c_int]

# const char* srcml_archive_get_uri_from_prefix(const struct srcml_archive* archive, const char* prefix);
libsrcml.srcml_archive_get_uri_from_prefix.restype = c_char_p
libsrcml.srcml_archive_get_uri_from_prefix.argtypes = [c_void_p, c_char_p]

# size_t         srcml_archive_get_macro_list_size(const struct srcml_archive* archive);
libsrcml.srcml_archive_get_macro_list_size.restype = c_size_t
libsrcml.srcml_archive_get_macro_list_size.argtypes = [c_void_p]

# const char* srcml_archive_get_macro_token(const struct srcml_archive* archive, size_t pos);
libsrcml.srcml_archive_get_macro_token.restype = c_char_p
libsrcml.srcml_archive_get_macro_token.argtypes = [c_void_p, c_int]

# const char* srcml_archive_get_macro_token_type(const struct srcml_archive* archive, const char* namespace_uri);
libsrcml.srcml_archive_get_macro_token_type.restype = c_char_p
libsrcml.srcml_archive_get_macro_token_type.argtypes = [c_void_p, c_char_p]

# const char* srcml_archive_get_macro_type(const struct srcml_archive* archive, size_t pos);
libsrcml.srcml_archive_get_macro_type.restype = c_char_p
libsrcml.srcml_archive_get_macro_type.argtypes = [c_void_p, c_int]

# const char* srcml_archive_check_extension(struct srcml_archive* archive, const char* filename);
libsrcml.srcml_archive_check_extension.restype = c_char_p
libsrcml.srcml_archive_check_extension.argtypes = [c_void_p, c_char_p]

# int srcml_write_unit(struct srcml_archive*, const struct srcml_unit*);
libsrcml.srcml_write_unit.restype = c_int
libsrcml.srcml_write_unit.argtypes = [c_void_p, c_void_p]

# struct srcml_unit* srcml_read_unit(struct srcml_archive*);
libsrcml.srcml_read_unit.restype = c_void_p
libsrcml.srcml_read_unit.argtypes = [c_void_p]

# int srcml_clear_transforms(struct srcml_archive*);
libsrcml.srcml_clear_transforms.restype = c_int
libsrcml.srcml_clear_transforms.argtypes = [c_void_p]

# int srcml_append_transform_xpath(struct srcml_archive*, const char* xpath_string);
libsrcml.srcml_append_transform_xpath.restype = c_int
libsrcml.srcml_append_transform_xpath.argtypes = [c_void_p, c_char_p]

# int srcml_append_transform_xslt_filename(struct srcml_archive*, const char* xslt_filename);
libsrcml.srcml_append_transform_xslt_filename.restype = c_int
libsrcml.srcml_append_transform_xslt_filename.argtypes = [c_void_p, c_char_p]

# int srcml_append_transform_xslt_memory     (struct srcml_archive*, const char* xslt_buffer, size_t size);
libsrcml.srcml_append_transform_xslt_memory.restype = c_int
libsrcml.srcml_append_transform_xslt_memory.argtypes = [c_void_p, c_char_p, c_int]

# int srcml_append_transform_xslt_FILE       (struct srcml_archive*, FILE* xslt_file);
libsrcml.srcml_append_transform_xslt_FILE.restype = c_int
libsrcml.srcml_append_transform_xslt_FILE.argtypes = [c_void_p, c_void_p]

# int srcml_append_transform_xslt_fd         (struct srcml_archive*, int xslt_fd);
libsrcml.srcml_append_transform_xslt_fd.restype = c_int
libsrcml.srcml_append_transform_xslt_fd.argtypes = [c_void_p, c_int]

# int srcml_append_transform_relaxng_filename(struct srcml_archive*, const char* relaxng_filename);
libsrcml.srcml_append_transform_relaxng_filename.restype = c_int
libsrcml.srcml_append_transform_relaxng_filename.argtypes = [c_void_p, c_char_p]

# int srcml_append_transform_relaxng_memory  (struct srcml_archive*, const char* relaxng_buffer, size_t size);
libsrcml.srcml_append_transform_relaxng_memory.restype = c_int
libsrcml.srcml_append_transform_relaxng_memory.argtypes = [c_void_p, c_char_p, c_int]

# int srcml_append_transform_relaxng_FILE    (struct srcml_archive*, FILE* relaxng_file);
libsrcml.srcml_append_transform_relaxng_FILE.restype = c_int
libsrcml.srcml_append_transform_relaxng_FILE.argtypes = [c_void_p, c_void_p]

# int srcml_append_transform_relaxng_fd      (struct srcml_archive*, int relaxng_fd);
libsrcml.srcml_append_transform_relaxng_fd.restype = c_int
libsrcml.srcml_append_transform_relaxng_fd.argtypes = [c_void_p, c_int]

# int srcml_append_transform_param           (struct srcml_archive*, const char* xpath_param_name, const char* xpath_param_value);
libsrcml.srcml_append_transform_param.restype = c_int
libsrcml.srcml_append_transform_param.argtypes = [c_void_p, c_char_p, c_char_p]

# int srcml_append_transform_stringparam     (struct srcml_archive*, const char* xpath_param_name, const char* xpath_param_value);
libsrcml.srcml_append_transform_stringparam.restype = c_int
libsrcml.srcml_append_transform_stringparam.argtypes = [c_void_p, c_char_p, c_char_p]

# int srcml_apply_transforms(struct srcml_archive* iarchive, struct srcml_archive* oarchive);
libsrcml.srcml_apply_transforms.restype = c_int
libsrcml.srcml_apply_transforms.argtypes = [c_void_p, c_void_p]

# srcml_archive wrapper
class srcml_archive :

    def __init__(self, archive = 0) :
        self.archive = archive
        if self.archive == 0 :
            self.archive = libsrcml.srcml_archive_create()

    def clone(self) :
        return srcml_archive(libsrcml.srcml_archive_clone(self.archive))

    def write_open_filename(self, srcml_filename, compression) :
        check_return(libsrcml.srcml_archive_write_open_filename(self.archive, srcml_filename, compression))

    def write_open_memory(self) :
        self.buffer = c_char_p()
        self.size = c_size_t()
        check_return(libsrcml.srcml_archive_write_open_memory(self.archive, pointer(self.buffer), pointer(self.size)))

    def write_open_FILE(self, srcml_file) :
        check_return(libsrcml.srcml_archive_write_open_FILE(self.archive, srcml_file))

    def write_open_fd(self, srcml_fd) :
        check_return(libsrcml.srcml_archive_write_open_fd(self.archive, srcml_fd))

    def write_open_io(self, context, write_callback, close_callback) :
        check_return(libsrcml.srcml_archive_write_open_io(self.archive, context, write_callback, close_callback))

    def read_open_filename(self, srcml_filename) :
        check_return(libsrcml.srcml_archive_read_open_filename(self.archive, srcml_filename))

    def read_open_memory(self, buffer) :
        check_return(libsrcml.srcml_archive_read_open_memory(self.archive, buffer, len(buffer)))

    def read_open_FILE(self, srcml_file) :
        check_return(libsrcml.srcml_archive_read_open_FILE(self.archive, srcml_file))

    def read_open_fd(self, srcml_fd) :
        check_return(libsrcml.srcml_archive_read_open_fd(self.archive, srcml_fd))

    def read_open_io(self, context, read_callback, close_callback) :
        check_return(libsrcml.srcml_archive_read_open_fd(self.archive, context, read_callback, close_callback))

    def set_src_encoding(self, src_encoding) :
        check_return(libsrcml.srcml_archive_set_src_encoding(self.archive, src_encoding))

    def set_xml_encoding(self, encoding) :
        check_return(libsrcml.srcml_archive_set_xml_encoding(self.archive, encoding))

    def set_language(self, language) :
        check_return(libsrcml.srcml_archive_set_language(self.archive, language))

    def set_url(self, url) :
        check_return(libsrcml.srcml_archive_set_url(self.archive, url))

    def set_version(self, version) :
        check_return(libsrcml.srcml_archive_set_version(self.archive, version))

    def set_options(self, options) :
        check_return(libsrcml.srcml_archive_set_options(self.archive, options))

    def enable_option(self, option) :
        check_return(libsrcml.srcml_archive_enable_option(self.archive, option))

    def disable_option(self, option) :
        check_return(libsrcml.srcml_archive_disable_option(self.archive, option))

    def set_tabstop(self, tabstop) :
        check_return(libsrcml.srcml_archive_set_tabstop(self.archive, tabstop))

    def register_file_extension(self, extension, language) :
        check_return(libsrcml.srcml_archive_register_file_extension(self.archive, extension, language))

    def register_namespace(self, prefix, ns) :
        check_return(libsrcml.srcml_archive_register_namespace(self.archive, prefix, ns))

    def register_macro(self, token, type) :
        check_return(libsrcml.srcml_archive_register_macro(self.archive, token, type))

    def get_src_encoding(self) :
        return libsrcml.srcml_archive_get_src_encoding(self.archive)

    def get_xml_encoding(self) :
        return libsrcml.srcml_archive_get_xml_encoding(self.archive)

    def get_revision(self) :
        return libsrcml.srcml_archive_get_revision(self.archive)

    def get_language(self) :
        return libsrcml.srcml_archive_get_language(self.archive)

    def get_url(self) :
        return libsrcml.srcml_archive_get_url(self.archive)

    def get_version(self) :
        return libsrcml.srcml_archive_get_version(self.archive)

    def get_options(self) :
        return libsrcml.srcml_archive_get_options(self.archive)

    def get_tabstop(self) :
        return libsrcml.srcml_archive_get_tabstop(self.archive)

    def get_namespace_size(self) :
        return libsrcml.srcml_archive_get_namespace_size(self.archive)

    def get_namespace_prefix(self, pos) :
        return libsrcml.srcml_archive_get_namespace_prefix(self.archive, pos)

    def get_prefix_from_uri(self, ns) :
        return libsrcml.srcml_archive_get_prefix_from_uri(self.archive, ns)

    def get_namespace_uri(self, pos) :
        return libsrcml.srcml_archive_get_namespace_uri(self.archive, pos)

    def get_uri_from_prefix(self, prefix) :
        return libsrcml.srcml_archive_get_uri_from_prefix(self.archive, prefix)

    def get_macro_list_size(self) :
        return libsrcml.srcml_archive_get_macro_list_size(self.archive)

    def get_macro_token(self, pos) :
        return libsrcml.srcml_archive_get_macro_token(self.archive, pos)

    def get_macro_token_type(self, token) :
        return libsrcml.srcml_archive_get_macro_token_type(self.archive, token)

    def get_macro_type(self, pos) :
        return libsrcml.srcml_archive_get_macro_type(self.archive, pos)

    def check_extension(self, filename) :
        return libsrcml.srcml_archive_check_extension(self.archive, filename)

    def srcML(self) :
        return self.buffer.value

    def write_unit(self, unit) :
        check_return(libsrcml.srcml_write_unit(self.archive, unit.unit))

    def read_unit(self) :
        unit = libsrcml.srcml_read_unit(self.archive)

        if unit != None :
            return srcml_unit(0, unit)
        return None

    def clear_transforms(self) :
        check_return(libsrcml.srcml_clear_transforms(self.archive))

    def append_transform_xpath(self, xpath_string) :
        check_return(libsrcml.srcml_append_transform_xpath(self.archive, xpath_string))

    def append_transform_xslt_filename(self, xslt_filename) :
        check_return(libsrcml.srcml_append_transform_xslt_filename(self.archive, xslt_filename))

    def append_transform_xslt_memory(self, xslt_buffer) :
        check_return(libsrcml.srcml_append_transform_xslt_memory(self.archive, xslt_buffer, len(xslt_buffer)))

    def append_transform_xslt_FILE(self, xslt_file) :
        check_return(libsrcml.srcml_append_transform_xslt_FILE(self.archive, xslt_file))

    def append_transform_xslt_fd(self, xslt_fd) :
        check_return(libsrcml.srcml_append_transform_xslt_fd(self.archive, xslt_fd))

    def append_transform_relaxng_filename(self, relaxng_filename) :
        check_return(libsrcml.srcml_append_transform_relaxng_filename(self.archive, relaxng_filename))

    def append_transform_relaxng_memory(self, relaxng_buffer) :
        check_return(libsrcml.srcml_append_transform_relaxng_memory(self.archive, relaxng_buffer, len(relaxng_buffer)))

    def append_transform_relaxng_FILE(self, relaxng_file) :
        check_return(libsrcml.srcml_append_transform_relaxng_FILE(self.archive, relaxng_file))

    def append_transform_relaxng_fd(self, relaxng_fd) :
        check_return(libsrcml.srcml_append_transform_relaxng_fd(self.archive, relaxng_fd))

    def append_transform_param(self, xpath_param_name, xpath_param_value) :
        check_return(libsrcml.srcml_append_transform_param(self.archive, xpath_param_name, xpath_param_value))

    def append_transform_stringparam(self, xpath_param_name, xpath_param_value) :
        check_return(libsrcml.srcml_append_transform_stringparam(self.archive, xpath_param_name, xpath_param_value))

    def apply_transforms(self, oarchive) :
        check_return(libsrcml.srcml_apply_transforms(self.archive, oarchive.archive))

    def close(self) :
        libsrcml.srcml_archive_close(self.archive)

    def __del__(self) :
        libsrcml.srcml_archive_free(self.archive)
