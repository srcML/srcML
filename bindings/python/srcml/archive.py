##
# @file archive.py
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

from unit import unit
from bindings import *
from memory_buffer import memory_buffer

_ENCODING_ATTR = "encoding"
_SRC_ENCODING_ATTR = "src_encoding"
_LANGUAGE_ATTR = "language"
_FILENAME_ATTR = "filename"
_DIRECTORY_ATTR = "directory"
_VERSION_ATTR = "version"
_TABSTOP_ATTR = "tabstop"
_OPTIONS_ATTR = "options"
archive_attr_lookup = dict(
{
    _ENCODING_ATTR: (archive_get_encoding, archive_set_encoding,),
    _SRC_ENCODING_ATTR : (archive_get_src_encoding, archive_set_src_encoding,),
    _LANGUAGE_ATTR : (archive_get_language, archive_set_language,),
    _FILENAME_ATTR : (archive_get_filename, archive_set_filename,),
    _DIRECTORY_ATTR : (archive_get_directory, archive_set_directory,),
    _VERSION_ATTR : (archive_get_version, archive_set_version,),
    _TABSTOP_ATTR : (archive_get_tabstop, archive_set_tabstop,),
    _OPTIONS_ATTR : (archive_get_options, archive_set_options,),
})

class archive(object):
    __doc__ = """
    This class provides access to units within an archive using either
    a reading or writing interface depending on how an archive's been opened.

    Special attributes don't appear within the __dict__ of this class because
    they are actually member of the srcml_archive native C class.
    Special Attributes:
        - encoding
        - src_encoding
        - language
        - filename
        - directory
        - version
        - tabstop
        - options

    Valid options for options attribute

    """

    def __init__(self, **kwargs):
        """
        Constructs a srcml archive.
        Options for kwargs:
            - encoding
            - src_encoding
            - language
            - filename
            - directory
            - version
            - tabstop
            - options - a unsigned long long combination of options 
        """
        self.srcml_archive = create_archive()
        def _getAttr(attr):
            if attr in kwargs:
                self.__setattr__(attr, kwargs[attr])
        for attr in [key for key in archive_attr_lookup.keys()]:
            _getAttr(attr)

    def __del__(self):
        """
        Clean up a srcML archive.
        """
        free_archive(self.srcml_archive)

    def __getattr__(self, attrName):
        """
        This is used in order to provide an interface to all attributes
        that are held, natively, by srcml archive.
        """
        if attrName in archive_attr_lookup:
            return archive_attr_lookup[attrName][0](self.srcml_archive)
        else:
            return super(archive, self).__getattr__(attrName)

    def __setattr__(self, attrName, value):
        """
        This is used in order to provide an interface to all attributes
        that are held, natively, by srcml archive.
        """
        if attrName in archive_attr_lookup:
            archive_attr_lookup[attrName][1](self.srcml_archive, value)
        else:
            return super(archive, self).__setattr__(attrName, value)


    # Options functions.
    def enable_options(self, option):
        """
        Turns the provided option on.
        """
        srcml.archive_enable_option(self.archive, option)

    def disable_options(self, option):
        """
        Turns option off.
        """
        srcml.archive_disable_option(self.archive, option)

    def option_is_enabled(self, option):
        """
        Checks if an option is on or not.
        """
        return (self.options & option) == option


