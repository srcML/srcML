##
# @file readable_archive_settings.py
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
from exceptions import *
from xslt import *

class readable_archive_settings(object):
    """
    The settings used to configure a readable_archive.
    """
    def __init__(self, xml_encoding=None, src_encoding=None, xsltransformations=[]):
        """
        Construct settings for a new readable_archive. The settings are used at
        the construction of a readable_archive.

        The parameters of the constructor map directly to the properties
        of the readable_archive_settings instance being created.
        """
        self.xml_encoding = xml_encoding
        self.src_encoding = src_encoding
        for transform in xsltransformations:
            if not issubclass(transform, xsltransform_base):
                raise TypeError("Provided transformation is not a subclass of the xsltransform_base.")
        self._xslt_transforms = xsltransformations
        

    @property
    def xml_encoding(self):
        """
        The encoding to expect the XML to be read in as.
        when set to None an attempt is made to extract the encoding from the XML decl when reading the
        archive.
        """
        return self._xml_encoding

    @xml_encoding.setter
    def xml_encoding(self, value):
        """
        The encoding to expect the XML to be read in as.

        preconditions - values must be a string or None.
        """
        assert isinstance(value, (str)) or value is None, "src_encoding must be a string or None"
        if value == None:
            self._xml_encoding = value
        else:
            if check_encoding(value) == 0:
                raise invalid_srcml_encoding("Invalid source code encoding", value)
            self._xml_encoding = value

    
    @property
    def src_encoding(self):
        """
        The encoding used when extracting source code from XML.
        """
        return self._src_encoding

    @src_encoding.setter
    def src_encoding(self, value):
        """
        The encoding to extract any source code as. This is used when source code
        is read from a file. The returned source code uses the encoding provided here.

        preconditions - values must be a string or None.
        """
        assert isinstance(value, (str)) or value is None, "src_encoding must be a string or None"
        if value == None:
            self._src_encoding = value
        else:
            if check_encoding(value) == 0:
                raise invalid_srcml_encoding("Invalid source code encoding", value)
            self._src_encoding = value
    
    @property
    def xsltransformations(self):
        """
        Provides access to a list the list of transformations that are registered with a readable_archive.
        The transformations can be run using the readable_archive's xslt.apply() functionality to apply
        the transformations to the current archive and write it into a writable archive.
        """
        return self._xslt_transforms

    def copy(self):
        """
        Create a copy of the current archive.
        """
        return readable_archive_settings(
            self.xml_encoding,
            self.src_encoding,
            [x for x in self.xsltransformations]
        )

    def clear(self):
        """
        Set the current object back to default values.
        """
        self.xml_encoding = None
        self.src_encoding = None
        self._xslt_transforms =[]

    def __eq__(self, other):
        """
        Compare two readable_archive_settings objects to see if they are the same.
        """
        return (
            self.xml_encoding == other.xml_encoding
            and self.src_encoding == other.src_encoding
            and self.xsltransformations == other.xsltransformations
        )

        # raise NotImplementedError()

    def __ne__(self, other):
        """
        Compare two readable_archive_settings objects to see if they are not the same.
        """
        return not(self == other)