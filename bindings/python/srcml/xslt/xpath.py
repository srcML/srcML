##
# @file xpath.py
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
from .. bindings import  *

class xpath(xsltransform_base):
    """
    Executes an XPath query on an archive and stores all of the results
    from from that query in individual archive.
    """
    def __init__(self, query):
        self.query = query

    @property
    def query(self):
        """
        The XPath query to execute.
        """
        return self._query

    @query.setter
    def query(self, value):
        """
        Set the value of the XPath query to execute.
        """
        assert isinstance(value, str), "Query must be a string."
        self._query = value
    
    def apply(self, unit_ptr):
        append_transform_xpath(unit_ptr, self.query)