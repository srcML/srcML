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
        """
        Constructor takes a list of XML namespace prefix + URI tuples or kwargs using
        the parameter name as the XML namespace prefix and the value as the URI.

        For example,
            x = writable_archive_xml_namespace_settings(("go","www.google.com"), ("ya", "yahoo.com"))

        and 
            x = writable_archive_xml_namespace_settings(go ="www.google.com", ya= "yahoo.com")

        construct an object containing the same values.
        """
        self.prefix_mappings = dict()
        self.uri_mappings = dict()
        self.update(*args)
        self.update(**kwargs)
        

    def __contains__(self, other):
        """
        If other is a tuple then it checks for both
        to see if item 0 and item 1 exist as the same pair.

        if other is not a tuple and it is assumed to be a string and
        this returns true if it exists as either a prefix or uri.
        """
        if other is None:
            raise TypeError("Invalid other. other is None.")
        if isinstance(other, str):
            return self.has_prefix(other) or self.has_uri(other)
        else:
            return self.contains_mapping(other)

    def has_prefix(self, prefix):
        """
        Checks if a prefix exists within an XML mapping.
        Throws if the prefix is None.
        """
        if prefix is None:
            raise TypeError("Invalid prefix. prefix is None")
        return prefix in self.prefix_mappings

    def has_uri(self, uri):
        """
        Cheks if a URI exists as part of an XML namespace mapping.
        Throws if the URI is None
        """
        if uri is None:
            raise TypeError("Invalid uri. uri is None")
        return uri in self.uri_mappings

    def contains_mapping(self, prefix_or_mapping, uri = None):
        """
        Checks to see if an XML namespace mapping, both prefix and URI, exists together
        as a single entry within the XML namespace mappings.

        prefix_or_mapping - Is either a tuple containing a prefix and uri pair or a prefix string.
            If the mapping is a tuple uri must be None or a TypeError will be raised and if prefix_or_mapping
            is a string then URI must not be None.
            A TypeError is raised if prefix_or_mapping is None.

        uri - A URI string or None depending on the type of value within prefix_or_mapping.
        """
        if prefix_or_mapping is None:
            raise TypeError("Invalid prefix_or_mapping. prefix_or_mapping is None.")
        if isinstance(prefix_or_mapping, str):
            # prefix_or_mapping used as a string
            if uri is None:
                raise TypeError("Invalid URI for XML namespace mapping. If prefix_or_mapping is a string the URI parameter can not be None.")
            if self.has_prefix(prefix_or_mapping):
                temp_namespace_mapping = self[prefix_or_mapping]
                return temp_namespace_mapping[1] == uri
            else:
                return False            
        else:
            # prefix_or_mapping uses as a tuple
            if self.has_prefix(prefix_or_mapping[0]):
                temp_namespace_mapping = self[prefix_or_mapping[0]]
                return temp_namespace_mapping[1] == prefix_or_mapping[1]
            else:
                return False

    def __len__(self):
        """
        Returns the number XML namespace and prefix mappings within this
        bimap.
        """
        return len(self.prefix_mappings)

    def __getitem__(self, prefix):
        """
        Retrieve an XML namespace mapping using the prefix to retrieve it.
        Raises an exception of prefix is None.
        """
        return self.get_by_prefix(prefix)

    def __setitem__(self, prefix, uri):
        """
        Set, update or add a prefix and URI XML namespace mapping within
        the set of namespace. This is done relative to the prefix.

        Raises an exception of prefix or uri is None or in the even that
        the provided prefix is in conflict with a pre-existing XML namespace
        mapping.
        """
        if prefix is None:
            raise TypeError("Invalid prefix. prefix is None.")
        if uri is None:
            raise TypeError("Invalid prefix. value is None.")
        self._update_single((prefix, uri))

    def get_by_uri(self, uri):
        """
        Retrieve an XML namespace prefix and URI tuple using the URI to locate it.

        Raises an exception if the URI is None or the URI doesn't exist.
        """
        if uri == None:
            raise TypeError("Invalid URI")
        if uri not in self.uri_mappings:
            raise KeyError("URI doesn't exist within collection")
        return self.uri_mappings[uri]

    def get_by_prefix(self, prefix):
        """
        Retrieve an XML namespace prefix and URI tuple using the prefix to locate it.

        Raises an exception if the prefix is None or the prefix doesn't exist.
        """
        if prefix == None:
            raise TypeError("Invalid prefix")
        if prefix not in self.prefix_mappings:
            raise KeyError("Prefix doesn't exist within collection")
        return self.prefix_mappings[prefix]


    # Item removal
    def __delitem__(self, prefix):
        """
        Remove a XML namespace mapping using the XML namespace prefix.
        If the prefix doesn't exist then nothing is done.
        Raises a TypeError if prefix is None.
        """
        self.remove_by_prefix(prefix)


    def remove_by_uri(self, uri):
        """
        Remove an XML namespace pairing using the URI to locate it.
        If the uri doesn't exist then nothing is done.
        Returns True if an element was remove and false if not.
        Raises a TypeError if uri is None.
        """
        if self.has_uri(uri):
            ns_pair = self.uri_mappings[uri]
            del self.uri_mappings[uri]
            del self.prefix_mappings[ns_pair[0]]
            return True
        return False

    def remove_by_prefix(self, prefix):
        """
        Remove a XML namespace mapping using the XML namespace prefix.
        If the prefix doesn't exist then nothing is done.

        Returns True if an element was remove and false if not.
        Raises a TypeError if prefix is None.
        """
        if self.has_prefix(prefix):
            ns_pair = self.prefix_mappings[prefix]
            del self.prefix_mappings[prefix]
            del self.uri_mappings[ns_pair[1]]
            return True
        return False

    def remove(self, xml_namespace):
        """
        Remove a XML namespace mapping from the collection.
        This mapping must exist as a single element or no XML namespace
        pair will be removed.
        Returns True if an element was remove and false if not.
        Raises an exception if the mapping is invalid or None.
        """
        if xml_namespace is None:
            raise TypeError("Invalid xml_namespace. xml_namespace is None.")
        if xml_namespace[0] is None:
            raise TypeError("Invalid xml_namespace. prefix is None.")
        if xml_namespace[1] is None:
            raise TypeError("Invalid xml_namespace. uri is None.")

        if self.contains_mapping(xml_namespace):
            del self.prefix_mappings[xml_namespace[0]]
            del self.uri_mappings[xml_namespace[1]]
            return True
        return False

    # Equality comparison.
    def __eq__(self, other):
        """
        Compare two XML namespace mapping collections to see if they are the same.
        """
        return self.prefix_mappings == other.prefix_mappings  and self.uri_mappings == other.uri_mappings

    def __ne__(self, other):
        """
        Compare two XML namespace mapping collections to see if they are not the same.
        """
        return not (self == other)

    def __iter__(self):
        """
        Provides iteration over all of the XML namespace mappings within the collection.
        """
        for ns in self.prefix_mappings.values():
            yield ns

    def items(self):
        """
        Returns a sequence of all of the XML namespace mappings within the collection.
        """
        return [ns for ns in self.prefix_mappings.values()]

    def iterprefixes(self):
        """
        Iterates over all prefxes within the collection.
        """
        for prefix in self.prefix_mappings.keys():
            yield prefix

    def iteruris(self):
        """
        Provides iteration over all of the URIs within the collection.
        """
        for uri in self.uri_mappings.keys():
            yield uri

    def prefixes(self):
        """
        Returns a sequence of all prefixes within the collection.
        """
        return [p for p in self.prefix_mappings.keys()]

    def uris(self):
        """
        Returns a sequence of all uris within the collection.
        """
        return [u for u in self.uri_mappings.keys()]


    # Other Container functionality.
    def clear(self):
        """
        Clear the contents of the XML namespace mapping.
        """
        self.prefix_mappings.clear()
        self.uri_mappings.clear()


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
        return writable_archive_xml_namespace_settings(*self.items())



    # Debugging functionality 
    def __repr__(self):
        return str(self)

    def __str__(self):
        return ", ".join(["{0}:{1}".format(*x) for x in self.prefix_mappings.values()])