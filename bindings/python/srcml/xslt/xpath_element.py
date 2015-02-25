##
# @file xpath_element.py
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
from xpath_attribute import xpath_attribute
from ..bindings import *

class xpath_element(xpath_attribute):
    def __init__(self, query, element_name, element_namespace_prefix = None, element_namespace_uri = None, attribute_name = None, attribute_content = None, attribute_namespace_prefix = None, attribute_namespace_uri = None):
        super(xpath_element, self).__init__(query, attribute_name, attribute_content, attribute_namespace_prefix, attribute_namespace_uri)
        self.element_name = element_name
        self.element_namespace_prefix = element_namespace_prefix
        self.element_namespace_uri = element_namespace_uri

    @property
    def element_name(self):
        return self._element_name

    @element_name.setter
    def element_name(self, value):
        self._element_name = value

    @property
    def element_namespace_prefix(self):
        return self._element_namespace_prefix

    @element_namespace_prefix.setter
    def element_namespace_prefix(self, value):
        self._element_namespace_prefix = value
    
    @property
    def element_namespace_uri(self):
        return self._element_namespace_uri

    @element_namespace_uri.setter
    def element_namespace_uri(self, value):
        self._element_namespace_uri = value
    

    def apply(self, arch_ptr):
        append_transform_xpath_element(
            arch_ptr,
            self.query,
            self.element_namespace_prefix,
            self.element_namespace_uri,
            self.element_name,
            self.attribute_namespace_prefix,
            self.attribute_namespace_uri,
            self.attribute_name,
            self.attribute_content
        )