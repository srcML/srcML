from globals import libsrcml
from ctypes import c_int, c_void_p, c_char_p, pointer

from srcml_unit import srcml_unit
from exception import *

# struct srcml_archive* srcml_create_archive();
libsrcml.srcml_create_archive.restype = c_void_p
libsrcml.srcml_create_archive.argtypes = []

# struct srcml_archive* srcml_clone_archive(const struct srcml_archive*);
libsrcml.srcml_clone_archive.restype = c_void_p
libsrcml.srcml_clone_archive.argtypes = [c_void_p]

# int srcml_write_open_filename(struct srcml_archive*, const char* srcml_filename);
libsrcml.srcml_write_open_filename.restype = c_int
libsrcml.srcml_write_open_filename.argtypes = [c_void_p, c_char_p]

# int srcml_write_open_memory  (struct srcml_archive*, char** buffer, int * size);
libsrcml.srcml_write_open_memory.restype = c_int
libsrcml.srcml_write_open_memory.argtypes = [c_void_p, c_void_p, c_void_p]

# int srcml_write_open_FILE    (struct srcml_archive*, FILE* srcml_file);
libsrcml.srcml_write_open_FILE.restype = c_int
libsrcml.srcml_write_open_FILE.argtypes = [c_void_p, c_void_p]

# int srcml_write_open_fd      (struct srcml_archive*, int srcml_fd);
libsrcml.srcml_write_open_fd.restype = c_int
libsrcml.srcml_write_open_fd.argtypes = [c_void_p, c_int]

# int srcml_read_open_filename(struct srcml_archive*, const char* srcml_filename);
libsrcml.srcml_read_open_filename.restype = c_int
libsrcml.srcml_read_open_filename.argtypes = [c_void_p, c_char_p]

# int srcml_read_open_memory  (struct srcml_archive*, const char* buffer, size_t buffer_size);
libsrcml.srcml_read_open_memory.restype = c_int
libsrcml.srcml_read_open_memory.argtypes = [c_void_p, c_char_p, c_int]

# int srcml_read_open_FILE    (struct srcml_archive*, FILE* srcml_file);
libsrcml.srcml_read_open_FILE.restype = c_int
libsrcml.srcml_read_open_FILE.argtypes = [c_void_p, c_void_p]

# int srcml_read_open_fd      (struct srcml_archive*, int srcml_fd);
libsrcml.srcml_read_open_fd.restype = c_int
libsrcml.srcml_read_open_fd.argtypes = [c_void_p, c_int]

# void srcml_free_archive(struct srcml_archive* archive);
libsrcml.srcml_free_archive.restype = None
libsrcml.srcml_free_archive.argtypes = [c_void_p]

# void srcml_close_archive(struct srcml_archive*);
libsrcml.srcml_close_archive.restype = None
libsrcml.srcml_close_archive.argtypes = [c_void_p]

# int srcml_archive_set_encoding  (struct srcml_archive*, const char* encoding);
libsrcml.srcml_archive_set_encoding.restype = c_int
libsrcml.srcml_archive_set_encoding.argtypes = [c_void_p, c_char_p]

# int srcml_archive_set_language  (struct srcml_archive*, const char* language);
libsrcml.srcml_archive_set_language.restype = c_int
libsrcml.srcml_archive_set_language.argtypes = [c_void_p, c_char_p]

# int srcml_archive_set_filename  (struct srcml_archive*, const char* filename);
libsrcml.srcml_archive_set_filename.restype = c_int
libsrcml.srcml_archive_set_filename.argtypes = [c_void_p, c_char_p]

# int srcml_archive_set_directory (struct srcml_archive*, const char* directory)
libsrcml.srcml_archive_set_directory.restype = c_int
libsrcml.srcml_archive_set_directory.argtypes = [c_void_p, c_char_p]

# int srcml_archive_set_version   (struct srcml_archive*, const char* version)
libsrcml.srcml_archive_set_version.restype = c_int
libsrcml.srcml_archive_set_version.argtypes = [c_void_p, c_char_p]

# int srcml_archive_set_all_options   (struct srcml_archive*, int option);
libsrcml.srcml_archive_set_all_options.restype = c_int
libsrcml.srcml_archive_set_all_options.argtypes = [c_void_p, c_int]

# int srcml_archive_set_option    (struct srcml_archive*, int option);
libsrcml.srcml_archive_set_option.restype = c_int
libsrcml.srcml_archive_set_option.argtypes = [c_void_p, c_int]

# int srcml_archive_clear_option  (struct srcml_archive*, int option);
libsrcml.srcml_archive_clear_option.restype = c_int
libsrcml.srcml_archive_clear_option.argtypes = [c_void_p, c_int]

# int srcml_archive_set_tabstop   (struct srcml_archive*, int tabstop);
libsrcml.srcml_archive_set_tabstop.restype = c_int
libsrcml.srcml_archive_set_tabstop.argtypes = [c_void_p, c_int]

# int srcml_archive_register_file_extension(struct srcml_archive*, const char* extension, const char* language);
libsrcml.srcml_archive_register_file_extension.restype = c_int
libsrcml.srcml_archive_register_file_extension.argtypes = [c_void_p, c_char_p, c_char_p]

# int srcml_archive_register_namespace(struct srcml_archive*, const char* prefix, const char* ns);
libsrcml.srcml_archive_register_namespace.restype = c_int
libsrcml.srcml_archive_register_namespace.argtypes = [c_void_p, c_char_p, c_char_p]

# const char* srcml_archive_get_encoding (const struct srcml_archive*);
libsrcml.srcml_archive_get_encoding.restype = c_char_p
libsrcml.srcml_archive_get_encoding.argtypes = [c_void_p]

# const char* srcml_archive_get_language (const struct srcml_archive*);
libsrcml.srcml_archive_get_language.restype = c_char_p
libsrcml.srcml_archive_get_language.argtypes = [c_void_p]

# const char* srcml_archive_get_filename (const struct srcml_archive*);
libsrcml.srcml_archive_get_filename.restype = c_char_p
libsrcml.srcml_archive_get_filename.argtypes = [c_void_p]

# const char* srcml_archive_get_directory(const struct srcml_archive*);
libsrcml.srcml_archive_get_directory.restype = c_char_p
libsrcml.srcml_archive_get_directory.argtypes = [c_void_p]

# const char* srcml_archive_get_version  (const struct srcml_archive*);
libsrcml.srcml_archive_get_version.restype = c_char_p
libsrcml.srcml_archive_get_version.argtypes = [c_void_p]

# int         srcml_archive_get_options  (const struct srcml_archive*);
libsrcml.srcml_archive_get_options.restype = c_int
libsrcml.srcml_archive_get_options.argtypes = [c_void_p]

# int         srcml_archive_get_tabstop  (const struct srcml_archive*);
libsrcml.srcml_archive_get_tabstop.restype = c_int
libsrcml.srcml_archive_get_tabstop.argtypes = [c_void_p]

# const char* srcml_archive_check_extension(struct srcml_archive* archive, const char* filename);
libsrcml.srcml_archive_check_extension.restype = c_char_p
libsrcml.srcml_archive_check_extension.argtypes = [c_void_p, c_char_p]

# int srcml_write_unit(struct srcml_archive*, const struct srcml_unit*);
libsrcml.srcml_write_unit.restype = c_int
libsrcml.srcml_write_unit.argtypes = [c_void_p, c_void_p]

# struct srcml_unit* srcml_read_unit(struct srcml_archive*);
libsrcml.srcml_read_unit.restype = c_void_p
libsrcml.srcml_read_unit.argtypes = [c_void_p]

# int srcml_skip_unit(struct srcml_archive*);
libsrcml.srcml_skip_unit.restype = c_int
libsrcml.srcml_skip_unit.argtypes = [c_void_p]

# struct srcml_unit* srcml_read_unit_position(struct srcml_archive*, int pos);
libsrcml.srcml_read_unit_position.restype = c_void_p
libsrcml.srcml_read_unit_position.argtypes = [c_void_p, c_int]

# int srcml_clear_transforms(struct srcml_archive*);
libsrcml.srcml_clear_transforms.restype = c_int
libsrcml.srcml_clear_transforms.argtypes = [c_void_p]

# int srcml_append_transform_xpath(struct srcml_archive*, const char* xpath_string);
libsrcml.srcml_append_transform_xpath.restype = c_int
libsrcml.srcml_append_transform_xpath.argtypes = [c_void_p]

# int srcml_append_transform_xslt(struct srcml_archive*, const char* xslt_filename);
libsrcml.srcml_append_transform_xslt.restype = c_int
libsrcml.srcml_append_transform_xslt.argtypes = [c_void_p]

# int srcml_append_transform_relaxng(struct srcml_archive*, const char* relaxng_filename);
libsrcml.srcml_append_transform_relaxng.restype = c_int
libsrcml.srcml_append_transform_relaxng.argtypes = [c_void_p]

# int srcml_apply_transforms(struct srcml_archive* iarchive, struct srcml_archive* oarchive);
libsrcml.srcml_apply_transforms.restype = c_int
libsrcml.srcml_apply_transforms.argtypes = [c_void_p, c_void_p]

# srcml_archive wrapper
class srcml_archive :

    def __init__(self, archive = 0) :
        self.archive = archive
        if self.archive == 0 :
            self.archive = libsrcml.srcml_create_archive()

    def clone(self) :
        return srcml_archive(libsrcml.srcml_clone_archive(self.archive))

    def write_open_filename(self, srcml_filename) :
        check_return(libsrcml.srcml_write_open_filename(self.archive, srcml_filename))

    def write_open_memory(self) :
        self.buffer = c_char_p()
        self.size = c_int()
        check_return(libsrcml.srcml_write_open_memory(self.archive, pointer(self.buffer), pointer(self.size)))

    def write_open_FILE(self, srcml_file) :
        check_return(libsrcml.srcml_write_open_FILE(self.archive, srcml_file))

    def write_open_fd(self, srcml_fd) :
        check_return(libsrcml.srcml_write_open_fd(self.archive, srcml_fd))

    def read_open_filename(self, srcml_filename) :
        check_return(libsrcml.srcml_read_open_filename(self.archive, srcml_filename))

    def read_open_memory(self, buffer) :
        check_return(libsrcml.srcml_read_open_memory(self.archive, buffer, len(buffer)))

    def read_open_FILE(self, srcml_file) :
        check_return(libsrcml.srcml_read_open_FILE(self.archive, srcml_file))

    def read_open_fd(self, srcml_fd) :
        check_return(libsrcml.srcml_read_open_fd(self.archive, srcml_fd))

    def set_encoding(self, encoding) :
        check_return(libsrcml.srcml_archive_set_encoding(self.archive, encoding))

    def set_language(self, language) :
        check_return(libsrcml.srcml_archive_set_language(self.archive, language))

    def set_filename(self, filename) :
        check_return(libsrcml.srcml_archive_set_filename(self.archive, filename))

    def set_directory(self, directory) :
        check_return(libsrcml.srcml_archive_set_directory(self.archive, directory))

    def set_version(self, version) :
        check_return(libsrcml.srcml_archive_set_version(self.archive, version))

    def set_all_options(self, options) :
        check_return(libsrcml.srcml_archive_set_all_options(self.archive, options))

    def set_option(self, option) :
        check_return(libsrcml.srcml_archive_set_option(self.archive, option))

    def clear_option(self, option) :
        check_return(libsrcml.srcml_archive_clear_option(self.archive, option))

    def set_tabstop(self, tabstop) :
        check_return(libsrcml.srcml_archive_set_tabstop(self.archive, tabstop))

    def register_file_extension(self, extension, language) :
        check_return(libsrcml.srcml_archive_register_file_extension(self.archive, extension, language))

    def register_namespace(self, prefix, ns) :
        check_return(libsrcml.srcml_archive_register_namespace(self.archive, prefix, ns))

    def get_encoding(self) :
        return libsrcml.srcml_archive_get_encoding(self.archive)

    def get_language(self) :
        return libsrcml.srcml_archive_get_language(self.archive)

    def get_filename(self) :
        return libsrcml.srcml_archive_get_filename(self.archive)

    def get_directory(self) :
        return libsrcml.srcml_archive_get_directory(self.archive)

    def get_version(self) :
        return libsrcml.srcml_archive_get_version(self.archive)

    def get_options(self) :
        return libsrcml.srcml_archive_get_options(self.archive)

    def get_tabstop(self) :
        return libsrcml.srcml_archive_get_tabstop(self.archive)

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

    def skip_unit(self) :
        check_return(libsrcml.srcml_skip_unit(self.archive))

    def read_unit_position(self, pos) :
         unit = libsrcml.srcml_read_unit_position(self.archive, pos)

         if unit != None :
             return srcml_unit(0, unit)
         return None

    def clear_transforms(self) :
        check_return(libsrcml.srcml_clear_transforms(self.archive))

    def append_transform_xpath(self, xpath_string) :
        check_return(libsrcml.srcml_append_transform_xpath(self.archive, xpath_string))

    def append_transform_xslt(self, xslt_filename) :
        check_return(libsrcml.srcml_append_transform_xslt(self.archive, xslt_filename))

    def append_transform_relaxng(self, relaxng_filename) :
        check_return(libsrcml.srcml_append_transform_relaxng(self.archive, relaxng_filename))

    def apply_transforms(self, oarchive) :
        check_return(libsrcml.srcml_apply_transforms(self.archive, oarchive))

    def close(self) :
        libsrcml.srcml_close_archive(self.archive)

    def __del__(self) :
        libsrcml.srcml_free_archive(self.archive)
