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


class readable_archive_settings(object):
    def __init__(self, xml_encoding=None, src_encoding=None, xslt_transformations=[]):
        self._xslt_transforms = list()
        raise NotImplementedError()

    @property
    def xml_encoding(self):
        return self._xml_encoding

    @xml_encoding.setter
    def xml_encoding(self, value):
        self._xml_encoding = value
    
    @property
    def src_encoding(self):
        return self._src_encoding

    @src_encoding.setter
    def src_encoding(self, value):
        self._src_encoding = value
    
    @property
    def xsltransforms(self):
        return self._xslt_transforms

    def copy(self):
        raise NotImplementedError()

    def clear(self):
        raise NotImplementedError()
# class ReadableArchiveSettings {
#     String xmlEncoding()
#     void setXmlEncoding(String encoding)
#     String sourceCodeEncoding()
#     void setSourceCodeEncoding(String encoding)
#     ArrayList<XslTransformBase> xslTransformations()
#     void clear()