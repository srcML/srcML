##
# @file writable_archive_settings.py
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

from writable_archive_xml_namespace_settings import writable_archive_xml_namespace_settings
from exceptions import invalid_srcml_language

class writable_archive_settings:
    
    def __init__(self, **kwargs):
        self._macros = dict()
        self._xml_namespaces = writable_archive_xml_namespace_settings()
        self._processing_instruction = None

        raise NotImplementedError()

    @property
    def filename(self):
        return self._filename
    @filename.setter
    def filename(self, value):
        self._filename = value
    
    @property
    def directory(self):
        return self._directory
    @directory.setter
    def directory(self, value):
        self._directory = value
    
    @property
    def default_language(self):
        return self._default_language
    @default_language.setter
    def default_language(self, value):
        self._default_language = value
    
    @property
    def tab_stop(self):
        return self._tab_stop
    @tab_stop.setter
    def tab_stop(self, value):
        self._tab_stop = value
    
    @property
    def version(self):
        return self._version
    @version.setter
    def version(self, value):
        self._version = value
    
    @property
    def parser_options(self):
        return self.parser_options
    @parser_options.setter
    def parser_options(self, value):
        self.parser_options = value
    
    @property
    def default_parser_options(self):
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
    def macros(self):
        return self._macros

    @property
    def xml_namespaces(self):
        return self._xml_namespaces

    @property
    def processing_instruction(self):
        return self._processing_instruction
    @processing_instruction.setter
    def processing_instruction(self, value):
        self._processing_instruction = value

    def set_processing_instruction(self, target, data):
        raise NotImplementedError()
    
    def clear(self):
        raise NotImplementedError()

    def copy(self):
        raise NotImplementedError()

    def __eq__(self, other):
        raise NotImplementedError()

    def __ne__(self, other):
        raise NotImplementedError()

    def __str__(self):
        raise NotImplementedError()

    def __repr__(self):
        raise NotImplementedError()

# class WritableArchiveSettings
#     String fileName()
#     void setFileName(String newFileName)
#     String directory()
#     void setDirectory(String newDirectory)
#     String defaultLanguage()
#     void setDefaultLanguage(String defaultLang)
#     void setDefaultLanguage(Language defaultLang)
#     int tabStop()
#     void setTabStop(int newTabStop)
#     String version()
#     void setVersion(String newVersion)
#     long parserOptions()
#     void resetParserOptionsToDefault()
#     String sourceCodeEncoding()
#     void setSourceCodeEncoding(String encoding)
#     String xmlEncoding()
#     void setXmlEncoding(String encoding)
#     HashMap<String, String> macros()
#     WritableArchiveXmlNamespaceSettings xmlNamespaces()
#     ProcessingInstructionImpl processingInstruction()
#     void clear()