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

_ENCODING_PROP = "encoding"
_SRC_ENCODING_PROP = "src_encoding"
_LANGUAGE_PROP = "language"
_FILENAME_PROP = "filename"
_DIRECTORY_PROP = "directory"
_VERSION_PROP = "version"
_TABSTOP_PROP = "tabstop"

archive_attr_lookup = dict(
{
    _ENCODING_PROP: (archive_get_encoding, archive_set_encoding,),
    _SRC_ENCODING_PROP : (archive_get_src_encoding, archive_set_src_encoding,),
    _LANGUAGE_PROP : (archive_get_language, archive_set_language,),
    _FILENAME_PROP : (archive_get_filename, archive_set_filename,),
    _DIRECTORY_PROP : (archive_get_directory, archive_set_directory,),
    _VERSION_PROP : (archive_get_version, archive_set_version,),
    _TABSTOP_PROP : (archive_get_tabstop, archive_set_tabstop,),
})

class archive(object):
    __doc__ = """
    This class provides access to units within an archive using either
    a reading or writing interface depending on how an archive's been opened.
    """

    def __init__(self, **kwargs):
        self.srcml_archive = create_archive()
        def _getProp(prop):
            if prop in kwargs:
                self.__setattr__(prop, kwargs[prop])
        
        _getProp(_ENCODING_PROP)
        _getProp(_SRC_ENCODING_PROP)
        _getProp(_LANGUAGE_PROP)
        _getProp(_FILENAME_PROP)
        _getProp(_DIRECTORY_PROP)
        _getProp(_VERSION_PROP)
        _getProp(_TABSTOP_PROP)


    def __del__(self):
        free_archive(self.srcml_archive)



    def __getattr__(self, attrName):
        if attrName in archive_attr_lookup:
            return archive_attr_lookup[attrName][0](self.srcml_archive)
        else:
            return super(archive, self).__getattr__(attrName)

    def __setattr__(self, attrName, value):
        if attrName in archive_attr_lookup:
            archive_attr_lookup[attrName][1](self.srcml_archive, value)
        else:
            return super(archive, self).__setattr__(attrName, value)