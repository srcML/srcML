##
# @file unit.py
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

from bindings import *
from helper_constants import *
import ctypes

_unit_attr_lookup = dict({
    ENCODING_ATTR       : (unit_get_encoding, unit_set_encoding,),
    LANGUAGE_ATTR       : (unit_get_language, unit_set_language,),
    FILENAME_ATTR       : (unit_get_filename, unit_set_filename,),
    DIRECTORY_ATTR      : (unit_get_directory, unit_set_directory,),
    VERSION_ATTR        : (unit_get_version, unit_set_version,),
    TIMESTAMP_ATTR      : (unit_get_timestamp, unit_set_timestamp,),
    HASH_ATTR           : (unit_get_hash, unit_set_hash,),
})



class unit:
    __doc__ = """
    This class represents a native srcml_unit instance.
    A unit is used for reading and writing to/from archives.
    a unit that is being read from an archive cannot be written
    to the same archive unless the archive is re-opened
    for writing.


    Attribute access or modification may change the internal representation
    of an archive which can then be written into another archive.
    """

    def __init__(self, owning_archive):
        self.srcml_unit = create_unit(owning_archive.srcml_archive)

    def __del__(self):
        free_unit(self.srcml_unit)
    
    def __getattr__(self, attrName):
        """
        Provides access to native attributes as well as
        additional python attributes.
        """
        if attrName in _unit_attr_lookup:
            return _unit_attr_lookup[attrName][0](self.srcml_unit)
        elif attrName in self.__dict__:
            return self.__dict__[attrName]
        else:
            raise KeyError("Attribute doesn't exist. Attribute: {0}".format(attrName))

    def __setattr__(self, attrName, value):
        """
        Provides mutative access to native attributes as well as
        additional python attributes.
        """
        if attrName in _unit_attr_lookup:
            _unit_attr_lookup[attrName][1](self.srcml_unit, value)
        else:
            self.__dict__[attrName] = value

    def srcml_revision(self):
        return unit_get_revision(self.srcml_unit)

    def xml(self):
        return unit_get_xml(self.srcml_unit)

# __LIBSRCML_DECL void srcml_(struct srcml_unit*);
# /* Query options of srcml unit */

# /* Convert from srcML to source code */
# __LIBSRCML_DECL int srcml_unparse_unit_filename(struct srcml_unit*, const char* src_filename);
# __LIBSRCML_DECL int srcml_unparse_unit_memory  (struct srcml_unit*, char** src_buffer, int * src_size);
# __LIBSRCML_DECL int srcml_unparse_unit_FILE    (struct srcml_unit*, FILE* srcml_file);
# __LIBSRCML_DECL int srcml_unparse_unit_fd      (struct srcml_unit*, int srcml_fd);
# __LIBSRCML_DECL int srcml_unparse_unit_io      (struct srcml_unit*, void * context, int (*write_callback)(void * context, const char * buffer, int len), int (*close_callback)(void * context));

# /* Convert to srcml.  Files/buffer can be compressed, but not a
#    source archive format (e.g., not .tar) */
# __LIBSRCML_DECL int srcml_parse_unit_filename(struct srcml_unit* unit, const char* src_filename);
# __LIBSRCML_DECL int srcml_parse_unit_memory  (struct srcml_unit*, const char* src_buffer, size_t buffer_size);
# __LIBSRCML_DECL int srcml_parse_unit_FILE    (struct srcml_unit*, FILE* src_file);
# __LIBSRCML_DECL int srcml_parse_unit_fd      (struct srcml_unit*, int src_fd);
# __LIBSRCML_DECL int srcml_parse_unit_io      (struct srcml_unit*, void * context, int (*read_callback)(void * context, char * buffer, int len), int (*close_callback)(void * context));