##
# @file xslt_stringparam.py
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
from .. bindings import *
class xslt_stringparam(xsltransform_base):
    """
    Register a new string parameter for an XSL Transformation.
    """
    def __init__(self, name, value):
        """
        Create a new xslt_stringparam object.
        """
        self.name = name
        self.value = value


    @property
    def name(self):
        return self._name
    @name.setter
    def name(self, value):
        self._name = value
    

    @property
    def value(self):
        return self._value
    @value.setter
    def value(self, value):
        self._value = value
    
    def apply(self, arch_ptr):
        append_transform_stringparam(arch_ptr, name, value)