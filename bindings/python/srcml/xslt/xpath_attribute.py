##
# @file xpath_attribute.py
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

from xslt_base_class import *
from xpath import xpath
from ..bindings import *

class xpath_attribute(xpath):
    """
    An XPath query that annotates all elements that match
    the query with a given attribute. This returns the entire
    archive but with the addition of the attribute on matching
    elements.
    """
    def __init__(self, query, attribute_name, attribute_content, attribute_namespace_prefix = None, attribute_namespace_uri = None):
        super(xpath_attribute, self).__init__(query)
        self.attribute_name = attribute_name
        self.attribute_content = attribute_content
        self.attribute_namespace_prefix = attribute_namespace_prefix
        self.attribute_namespace_uri = attribute_namespace_uri

    @property
    def attribute_name(self):
        """
        The name of the attribute add to all matching elements.
        """
        return self._attribute_name

    @attribute_name.setter
    def attribute_name(self, value):
        """
        Set the name of the attribute.
        """
        assert value is None or isinstance(value, str), "Value and must be a string."
        self._attribute_name = value
    
    @property
    def attribute_content(self):
        """
        The content of the attribute.
        """
        return self._attribute_content

    @attribute_content.setter
    def attribute_content(self, value):
        """
        Set the content for the attribute.
        """
        assert value is None or isinstance(value, str), "Value and must be a string."
        self._attribute_content = value
    

    @property
    def attribute_namespace_prefix(self):
        """
        Get the XML namespace prefix for an attribute.
        """
        return self._attribute_namespace_prefix

    @attribute_namespace_prefix.setter
    def attribute_namespace_prefix(self, value):
        """
        Set the XML namespace prefix, if None it isn't used.
        """
        assert value is None or isinstance(value, str), "Value and must be a string."
        self._attribute_namespace_prefix = value
    
    @property
    def attribute_namespace_uri(self):
        """
        Returns the URI associated with the XML namespace prefix.
        """
        return self._attribute_namespace_uri
    @attribute_namespace_uri.setter
    def attribute_namespace_uri(self, value):
        """
        Set the URI associated with the XML namespace prefix.
        """
        assert value is None or isinstance(value, str), "Value and must be a string."
        self._attribute_namespace_uri = value
    

    def apply(self, arch_ptr):
        append_transform_xpath_attribute(
            arch_ptr,
            self.query,
            self.attribute_namespace_prefix,
            self.attribute_namespace_uri,
            self.attribute_name,
            self.attribute_content
        )