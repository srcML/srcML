##
# @file writable_archive_xml_namespace_settings.py
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

class writable_archive_xml_namespace_settings:
    """
    This class implements a bimap that maintains a unique set of
    xml namespaces and URIs and the relationships between them.

    Each item within the bimap is considered to be a mapping between
    a prefix and URI bidirectionally.

    All of the special syntactic functions work on prefixes such as del and [] operators.
    The only exception to this is the "in" operator.

    """
    def __init__(self, *args, **kwargs):
        self.prefix_mappings = dict()
        self.uri_mappings = dict()
        self.update(*args)
        self.update(**kwargs)
            

    # Inspection
    def __contains__(self, other):
        """
        If other is a tuple then it checks for both
        to see if item 0 and item 1 exist as the same pair.

        if other is not a tuple and it is assumed to be a string and
        this returns true if it exists as either a prefix or uri.
        """
        raise NotImplementedError()

    def has_prefix(self, prefix):
        raise NotImplementedError()

    def has_uri(self, uri):
        raise NotImplementedError()

    def contains_mapping(self, prefix_or_mapping, uri = None):
        raise NotImplementedError()

    def __len__(self):
        """
        Returns the number XML namespace and prefix mappings within this
        bimap.
        """
        return len(self.prefix_mappings)


    # Item accessors/Mutators
    def __getitem__(self, prefix):
        return self.get_by_prefix(prefix)

    def __setitem__(self, prefix, value):
        raise NotImplementedError()

    def get_by_uri(self, uri):
        if uri == None:
            raise TypeError("Invalid URI")
        if uri not in self.uri_mappings:
            raise KeyError("URI doesn't exist within collection")
        return self.uri_mappings[uri]

    def get_by_prefix(self, prefix):
        if prefix == None:
            raise TypeError("Invalid prefix")
        if prefix not in self.prefix_mappings:
            raise KeyError("Prefix doesn't exist within collection")
        return self.prefix_mappings[prefix]


    # Item removal
    def __delitem__(self, prefix):
        raise NotImplementedError()

    def remove_by_uri(self, uri):
        raise NotImplementedError()

    def remove_by_prefix(self, prefix):
        raise NotImplementedError()

    def remove(self, mapping):
        raise NotImplementedError()


    # Equality comparison.
    def __eq__(self, other):
        raise NotImplementedError()

    def __ne__(self, other):
        raise NotImplementedError()


    # Iteration functions
    def __iter__(self):
        raise NotImplementedError()

    def items(self):
        raise NotImplementedError()

    def iterprefixes(self):
        raise NotImplementedError()

    def iteruris(self):
        raise NotImplementedError()

    def prefixes(self):
        raise NotImplementedError()

    def uris(self):
        raise NotImplementedError()


    # Other Container functionality.
    def clear(self):
        raise NotImplementedError()


    def _update_single(self, new_tuple):
        if new_tuple == None:
            raise TypeError("can't add None tuple")
        if len(new_tuple) < 2:
            raise TypeError("Invalid tuple missing prefix and/or uri")

        if new_tuple[0] == None:
            raise TypeError("Invalid XML namespace prefix")

        if new_tuple[1] == None:
            raise TypeError("Invalid XML namespace uri")
        prefix_exists = new_tuple[0] in self.prefix_mappings
        uri_exists = new_tuple[1] in self.uri_mappings
        if prefix_exists and uri_exists:
            located_tuple = self[new_tuple[0]]
            if located_tuple[1] != new_tuple[1]:
                raise KeyError("Invalid tuple. XML Namespace can be inserted into map because the prefix and URI belong to another URI and prefix respectively. Prefix: {0} URI: {1}".format(*new_tuple))
        elif prefix_exists and not uri_exists:
            current_tuple = self.prefix_mappings[new_tuple[0]]
            del self.prefix_mappings[current_tuple[0]]
            del self.uri_mappings[current_tuple[1]]
            copied_tuple = (new_tuple[0], new_tuple[1])
            self.prefix_mappings[new_tuple[0]] = copied_tuple
            self.uri_mappings[new_tuple[1]] = copied_tuple
        elif not prefix_exists and uri_exists:
            current_tuple = self.uri_mappings[new_tuple[1]]
            del self.prefix_mappings[current_tuple[0]]
            del self.uri_mappings[current_tuple[1]]
            copied_tuple = (new_tuple[0], new_tuple[1])
            self.prefix_mappings[new_tuple[0]] = copied_tuple
            self.uri_mappings[new_tuple[1]] = copied_tuple
        elif not prefix_exists and not uri_exists:
            copied_tuple = (new_tuple[0], new_tuple[1])
            self.prefix_mappings[new_tuple[0]] = copied_tuple
            self.uri_mappings[new_tuple[1]] = copied_tuple
        else:
            raise Exception("Unreachable code some how reached!!")



    def update(self, *args, **kwargs):
        """
        Update the current dictionary using a tuples or
        using the kwargs to specify XML namespace pairs.
        """
        for new_ns in args:
            self._update_single(new_ns)
        for ns in zip(kwargs.keys(), kwargs.values()):
            self._update_single(ns)

    def copy(self):
        return writable_archive_xml_namespace_settings(self.items())



    # Debugging functionality 
    def __repr__(self):
        return str(self)

    def __str__(self):
        return ", ".join(["{0}:{1}".format(*x) for x in self.prefix_mappings.values()])