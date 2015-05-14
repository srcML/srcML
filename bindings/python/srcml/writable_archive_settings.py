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
from exceptions import invalid_srcml_language, invalid_srcml_encoding
from bindings import check_encoding, check_language, OPTION_ARCHIVE, OPTION_XML_DECL, OPTION_NAMESPACE_DECL, OPTION_HASH, OPTION_PSEUDO_BLOCK, OPTION_TERNARY

class writable_archive_settings(object):
    
    def __init__(self, **kwargs):
        """
        Construct a new set of settings for an archive. Accepts
        arguments with the same name as the properties for this object. 
        """
        self._macros = dict()
        self._xml_namespaces = writable_archive_xml_namespace_settings()
        self._processing_instruction = None
        self.filename = kwargs.pop("filename", None)
        self.url = kwargs.pop("url", None)
        self.default_language = kwargs.pop("default_language", None)
        self.tab_stop = kwargs.pop("tab_stop", 8)
        self.version = kwargs.pop("version", None)
        self.parser_options = kwargs.pop("parser_options", self.default_parser_options)
        self.xml_encoding = kwargs.pop("xml_encoding", None)
        self.src_encoding = kwargs.pop("src_encoding", None)
        temp_macros = kwargs.pop("macros", None)
        if temp_macros is not None:
            self.macros.update(temp_macros)
        temp_xml_namespaces = kwargs.pop("xml_namespaces", None)
        if temp_xml_namespaces is not None:
            if isinstance(temp_xml_namespaces, dict):
                self.xml_namespaces.update(**temp_xml_namespaces)
            else:
                self.xml_namespaces.update(*temp_xml_namespaces)
        temp_proc_instr = kwargs.pop("processing_instruction", None)
        if temp_proc_instr is not None:
            self.set_processing_instruction(*temp_proc_instr)
        if len(kwargs) != 0:
            raise AttributeError("Received unknown attributes as arguments.")

    @property
    def filename(self):
        """
        Returns the filename attribute associated with an archive.
        """
        return self._filename


    @filename.setter
    def filename(self, value):
        """
        Set the value of the filename attribute for the archive. If None
        the filename isn't written into the archive.

        Precondition - Value must be a string or None.
        """
        assert isinstance(value, (str)) or value is None, "filename must be a string or None"
        self._filename = value
    
    @property
    def url(self):
        """
        Returns the value of the url attribute of an archive.
        If None the url attribute isn't output as part of an archive.
        """
        return self._url

    @url.setter
    def url(self, value):
        """
        Set the url attribute of an archive. If None the url attribute
        isn't output as part of an archive.

        Precondition - Value must be a string or None.
        """
        assert isinstance(value, (str)) or value is None, "url must be a string or None"
        self._url = value
    
    @property
    def default_language(self):
        """
        The default programming language to expect when parsing source code into srcml.
        """
        return self._default_language

    @default_language.setter
    def default_language(self, value):
        """
        Set the default programming language used with all files associated
        with an archive.

        This can be overridden on a per unit basis by setting the language on
        a writable_unit.

        Precondition - Value must be a string or None.
        """
        assert isinstance(value, (str)) or value is None, "language must be a string or None"
        if value == None:
            self._default_language = value
        else:
            if check_language(value) == 0:
                raise invalid_srcml_language("Invalid language", value)
            self._default_language = value
    
    @property
    def tab_stop(self):
        """
        Returns the default tab stop used for calculating the column position
        within a source code file.

        Default value is 8.
        """
        return self._tab_stop

    @tab_stop.setter
    def tab_stop(self, value):
        """
        Set the tab stop. This is used for calculating the column width within
        a document. This does nothing unless the position option is enabled.

        Precondition - value must be an int or long.
        """
        assert isinstance(value, (int, long)), "tab_stop must be an int or long"
        self._tab_stop = value
    
    @property
    def version(self):
        """
        Retuns the version attribute within an archive.
        If None the attribute isn't output into the archive.
        """
        return self._version

    @version.setter
    def version(self, value):
        """
        Set the value of a version attribute associated with an archive.
        If None the attribute isn't set.
        Precondition - Value must be a string or None.
        """
        assert isinstance(value, (str)) or value is None, "version must be a string or None"
        self._version = value
    
    @property
    def parser_options(self):
        """
        The set of options being used to configure the srcml parser.
        """
        return self._parser_options

    @parser_options.setter
    def parser_options(self, value):
        """
        Set the options for parsing srcML.

        Precondition - value must be a long or int.
        """
        assert isinstance(value, (long, int)), "parser_options must be an int or long"
        self._parser_options = value
    
    @property
    def default_parser_options(self):
        """
        The constant set of default parser options used for all archives.
        """
        return OPTION_ARCHIVE | OPTION_XML_DECL | OPTION_NAMESPACE_DECL | OPTION_HASH | OPTION_PSEUDO_BLOCK | OPTION_TERNARY

    @property
    def xml_encoding(self):
        """
        Returns the encoding used for the output of a writable archive.
        If None the encoding defaults to UTF-8.
        """
        return self._xml_encoding

    @xml_encoding.setter
    def xml_encoding(self, value):
        """
        The default output encoding for XML. If None the default is UTF-8.
        Precondition - Value must be None or string.
        Raises exception of the encoding isn't supported by srcml.
        """

        assert isinstance(value, (str)) or value is None, "src_encoding must be a string or None"
        if value == None:
            self._xml_encoding = value
        else:
            if check_encoding(value) == 0:
                raise invalid_srcml_encoding("Invalid xml code encoding", value)
            self._xml_encoding = value
    
    @property
    def src_encoding(self):
        """
        Returns the user specified encoding or None if one hasn't been set.
        This encoding specifies the encoding that source files will be given
        to a unit in. This can be overridden by setting the encoding on
        a writable unit.

        If None Latin-1 is expected.
        """
        return self._src_encoding
    @src_encoding.setter
    def src_encoding(self, value):
        """
        Set the encoding to expect when it's suppled to a writable archive unit.
        None uses the default encoding Latin-1.

        Precondition - Value must be a string or None.
        """
        assert isinstance(value, (str)) or value is None, "src_encoding must be a string or None"
        if value == None:
            self._src_encoding = value
        else:
            if check_encoding(value) == 0:
                raise invalid_srcml_encoding("Invalid source code encoding", value)
            self._src_encoding = value
    
    @property
    def macros(self):
        """
        macros is a set of token and type pairs.
        """
        return self._macros

    @property
    def xml_namespaces(self):
        """
        Returns the set of XML namespace prefix to URI mappings.
        """
        return self._xml_namespaces

    @property
    def processing_instruction(self):
        """
        Returns the processing instruction tuple of target and data.
        """
        return self._processing_instruction

    @processing_instruction.setter
    def processing_instruction(self, value):
        """
        Set the processing instruction to data from a tuple or list.

        Precondition -
            The provided value must be a tuple, list or None.
            The value and must have a size greater or equal to 2.
        """
        assert isinstance(value, (tuple, list)) or value is None, "processing_instruction must be a tuple or list"
        if isinstance(value, (tuple, list)):
            assert len(value) >= 2, "processing_instruction must be a tuple or list with 2 or more elements"
        if value == None:
            self._processing_instruction = None
        else:
            self.set_processing_instruction(*value)

    def set_processing_instruction(self, target, data):
        """
        Set the processing instruction's target and data.

        Raises type error if data is None and target isn't and vise-versa.
        Sets the processing instruction to None if both target and data are
        None.

        Precondition - Both target and data must be None or a string.
        """
        assert isinstance(target, (str)) or target is None, "target must be a string or None"
        assert isinstance(data, (str)) or data is None, "data must be a string or None"
        if target is None and data is None:
            self._processing_instruction = None
        elif target is not None and data is None:
            raise TypeError("Invalid data")
        elif target is None and data is not None:
            raise TypeError("Invalid target")
        else:
            self._processing_instruction = (target, data)

    
    def clear(self):
        """
        Clear the current settings, setting things back to their default
        values.
        """
        self.filename = None
        self.url = None
        self.default_language = None
        self.tab_stop = 8
        self.version = None
        self.parser_options = self.default_parser_options
        self.xml_encoding = None
        self.src_encoding = None
        self.macros.clear()
        self.xml_namespaces.clear()
        self.processing_instruction = None

    def copy(self):
        """
        Create and return a copy of the current settings.
        """
        cpy = writable_archive_settings()
        cpy.filename = self.filename
        cpy.url = self.url
        cpy.default_language = self.default_language
        cpy.tab_stop = self.tab_stop
        cpy.version = self.version
        cpy.parser_options = self.parser_options
        cpy.xml_encoding = self.xml_encoding
        cpy.src_encoding = self.src_encoding
        cpy.macros.update(**self.macros)
        cpy.xml_namespaces.update(*self.xml_namespaces)
        cpy.processing_instruction = self.processing_instruction
        return cpy

    def __eq__(self, other):
        """
        Compare to settings to see if they contain the same
        settings.
        """
        return (
            self.filename == other.filename
            and self.url == other.url
            and self.default_language == other.default_language
            and self.tab_stop == other.tab_stop
            and self.version == other.version
            and self.parser_options == other.parser_options
            and self.xml_encoding == other.xml_encoding
            and self.src_encoding == other.src_encoding
            and self.macros == other.macros
            and self.xml_namespaces == other.xml_namespaces
            and
                (
                    (self.processing_instruction == other.processing_instruction)
                    or
                    (
                        self.processing_instruction[0] == other.processing_instruction[0]
                        and
                        self.processing_instruction[1] == other.processing_instruction[1]
                    )
                )
            )

    def __ne__(self, other):
        """
        Compare to settings to see if they are not equal.
        """
        return not(self == other)

    def __str__(self):
        raise NotImplementedError()

    def __repr__(self):
        raise NotImplementedError()