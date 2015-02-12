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
    """
    The archive_xml_namespace class represents all namespaces
    associated with a readable_archive or writable_archive.
    """

    def __init__(self, srcml_archive_ptr):
        """
        This is a utility class that provides an interface into the xml namespaces
        registered within an archive.

        """
        self.srcml_archive = srcml_archive_ptr

    def __len__(self):
        """
        Returns the number of XML namespaces mappings associated with the archive.
        """
        return archive_get_namespace_size(self.srcml_archive)

    def __getitem__(self, index):
        """
        Returns an XML namespace at index. The XML namespaces are not in any particular order.
        """
        if not isinstance(index, (int, long)) or index is None:
            raise TypeError("Invalid index. Index must be an int or long and not be None.", index)

        if index < 0 or index >= len(self):
            raise IndexError("Invalid index", index)

        return (archive_get_namespace_prefix(self.srcml_archive, index), archive_get_namespace_uri(self.srcml_archive, index))

    def __contains__(self, other):
        """
        Checks to see if a namespace tuple exists within the collection of mappings.
        """
        return self.contains_xml_namespace_mapping(other)

    def contains_prefix(self, other_prefix):
        """
        Checks to see if a prefix exists within the namespace collection
        """
        if other_prefix == None:
            return False

        return len([ns for ns in self if ns[0] == other_prefix]) > 0

    def contains_uri(self, other_uri):
        """
        Checks to see if a URI exists within the namespace collection
        """
        if other_uri == None:
            return False
        return len([ns for ns in self if ns[1] == other_uri]) > 0

    def contains_xml_namespace_mapping(self, other_tuple):
        """
        Checks to see if a namespace tuple exists within the collection of mappings.
        """
        if other_tuple == None:
            return False
        return len([ns for ns in self if ns[0] == other_tuple[0] and ns[1] == other_tuple[1]]) > 0

    def __iter__(self):
        """
        Iterate over all of the XML namespace mappings.
        Each mapping is returned as a tuple of prefix and URI
        """
        for x in range(len(self)):
            yield self[x]

    def prefix_to_uri(self, prefix):
        """
        Resolve a prefix to a URI. The prefix must be a non-None string. If the
        prefix doesn't exist a KeyError is raised.
        """
        if prefix is None or not isinstance(prefix, str):
            raise TypeError("Invalid type for prefix. prefix must be a string and not None.")

        uri = archive_get_uri_from_prefix(self.srcml_archive, prefix)
        if uri == None:
            raise KeyError("Unable to locate prefix: {0}".format(prefix), prefix)
        return uri

    def uri_to_prefix(self, uri):
        """
        Resolve a URI to a prefix. The URI must be a non-None string. If 
        the URI doesn't exist a KeyError is raised.
        """
        if uri is None or not isinstance(uri, str):
            raise TypeError("Invalid type for uri. uri must be a string and not None.")

        prefix = archive_get_prefix_from_uri(self.srcml_archive, uri)
        if prefix == None:
            raise KeyError("Unable to locate uri: {0}".format(uri), uri)
        return prefix

