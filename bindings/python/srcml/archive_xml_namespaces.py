##
# @file archive_xml_namespaces.py
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

from bindings import *

class archive_xml_namespaces(object):

    def __init__(self, srcml_archive_ptr):
        """
        This is a utility class that provides an interface into the xml namespaces
        registered within an archive.

        """
        self.srcml_archive = srcml_archive_ptr

    def __len__(self):
        return archive_get_namespace_size(self.srcml_archive)

    def __getitem__(self, index):
        if not isinstance(index, (int, long)) or index is None:
            raise TypeError("Invalid index. Index must be an int or long and not be None.", index)

        if index < 0 or index >= len(self):
            raise IndexError("Invalid index", index)
            
        return (archive_get_namespace_prefix(self.srcml_archive, index), archive_get_namespace_uri(self.srcml_archive, index))

    def __iter__(self):
        raise NotImplementedError()
# def archive_get_namespace_prefix(archive, pos):
#     return libsrcml.srcml_archive_get_namespace_prefix(archive, pos)
# def archive_get_prefix_from_uri(archive, namespace_uri):
#     return libsrcml.srcml_archive_get_prefix_from_uri(archive, namespace_uri)
# def archive_get_namespace_uri(archive, pos):
#     return libsrcml.srcml_archive_get_namespace_uri(archive, pos)
# def archive_get_uri_from_prefix(archive, prefix):
#     return libsrcml.srcml_archive_get_uri_from_prefix(archive, prefix)