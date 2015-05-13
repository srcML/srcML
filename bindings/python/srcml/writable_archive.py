##
# @file writable_archive.py
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
from writable_archive_settings import writable_archive_settings
from private_helpers import *
from exceptions import invalid_srcml_language
from writable_unit import writable_unit
from archive_xml_namespaces import archive_xml_namespaces
from archive_macros import archive_macros

class writable_archive(object):
    """
    A writable_archive is an archive that data can be written into.
    """


    def __init__(self, settings, **kwargs):
        """
        Creates and opens an archive for writing.

        An archive can be opened, for writing, in several ways:
            1) Using a python stream.
            2) Using a filename,
            3) Using a memory_buffer.
            4) Using a I/O callback functions and context.
            5) Using a I/O interface.

        Ways to call open_write():
            1) Python Stream:
                strm = open("somefile.xml","2")
                archive.open_write(stream = strm)

            Note: the python stream has an optional argument close_stream, which defaults to true
            and doesn't close the stream if false.

            3) filename:
                archive.open_write(filename = "somefile.xml")
                When opening using a file name an optional compression parameter may be used
                parameter compression_factor - Should be a value from 0 to 9. 0 means no compression, 9 means max compression.

            4) srcml.memory_buffer (a python string representation
                    of a srcml archive can be constructed using
                    the srcml.memory_buffer):
                mem_buff = srcml.memory_buffer()
                archive.open_write(buffer = mem_buff)

            5) I/O callbacks:
                context = memory_to_write_to
                archive.open_write(context = context, write = write_func, close = close_func)

            6) I/O interface:
                writerContext = MyMemoryWriter()
                archive.open_write(context = writerContext)

        Description of callbacks and interfaces:
            1) write/close callback signature:
                def write(context, buffer, size_of_data_in_buffer):
                    return zero_for_sucess_not_zero_for_failure

                def close(context):
                    return zero_for_sucess_not_zero_for_failure

                A context can by any python object.

            2) I/O interface is an object that implements write and close 
                similar to those above but doesn't have a context argument.
                    
                class MyWriterInterface:
                    def writer(self, buffer, size_of_data_in_buffer):
                        return number_of_bytes_writen_to_buffer # or -1 for error

                    def close(self):
                        return zero_for_sucess_not_zero_for_failure

        Precondition - The provided settings object must be an instance of a writable_archive_settings
            object.
            One of the provided means of opening an archive for writing must exist.
        """
        assert isinstance(settings,  writable_archive_settings), "Invalid settings object"
        # Creating a new instance of an archive so that it can be handled
        # correctly when opened
        self.srcml_archive = archive_create()
        if self.srcml_archive == None:
            raise MemoryError("Failed to allocate native srcml archive.")
        # Loading the settings into the new archive.
        
        if settings.filename is not None:
            archive_set_filename(self.srcml_archive, settings.filename)

        if settings.url is not None:
            archive_set_url(self.srcml_archive, settings.url)

        if settings.default_language is not None:
            archive_set_language(self.srcml_archive, settings.default_language)

        if settings.tab_stop != 8:
            archive_set_tabstop(self.srcml_archive, settings.tab_stop)

        if settings.version is not None:
            archive_set_version(self.srcml_archive, settings.version)

        if settings.parser_options != settings.default_parser_options:
            archive_set_options(self.srcml_archive, settings.parser_options)

        if settings.xml_encoding is not None:
            archive_set_xml_encoding(self.srcml_archive, settings.xml_encoding)

        if settings.src_encoding is not None:
            archive_set_src_encoding(self.srcml_archive, settings.src_encoding)

        if len(settings.macros) > 0:
            for macro in settings.macros.iteritems():
                if not isinstance(macro[0], str):
                    raise TypeError("Invalid macro token. token must be a string.")

                if not isinstance(macro[1], str):
                    raise TypeError("Invalid macro type. Type must be a string.")
                archive_register_macro(self.srcml_archive, *macro)

        if len(settings.xml_namespaces) > 0:
            for ns in settings.xml_namespaces:
                if not isinstance(ns[0], str):
                    raise TypeError("Invalid XML namespace Prefix. Prefix must be a string.")

                if not isinstance(ns[1], str):
                    raise TypeError("Invalid XML namespace URI. URI must be a string.") 
                archive_register_namespace(self.srcml_archive, ns[0], ns[1])


        self._proc_instr = None
        if settings.processing_instruction is not None:
            archive_set_processing_instruction(self.srcml_archive, *settings.processing_instruction)
            self._proc_instr = (
                archive_get_processing_instruction_target(self.srcml_archive),
                archive_get_processing_instruction_data(self.srcml_archive)
            )
            # self._proc_instr = archive_processing_instruction(self.srcml_archive)
        self._xml_namespaces = archive_xml_namespaces(self.srcml_archive)
        self._macros = archive_macros(self.srcml_archive)

        # Opening archive
        self._open_write(**kwargs)



    def _open_write(self, **kwargs):
        if STREAM_PARAM in kwargs:
            if len(kwargs) == 1:
                self._open_write(context=write_stream_context(kwargs[STREAM_PARAM]))
            elif len(kwargs) == 2:
                self._open_write(context=write_stream_context(kwargs[STREAM_PARAM], kwargs[CLOSE_STREAM_PARAM]))
            else:
                raise ValueError("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))

        elif FILENAME_PARAM in kwargs:
            if len(kwargs) > 2:
                raise ValueError("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            if len(kwargs) == 2:
                archive_write_open_filename(self.srcml_archive, kwargs[FILENAME_PARAM], kwargs[COMPRESSION_FACTOR_PARAM])
            else:
                archive_write_open_filename(self.srcml_archive, kwargs[FILENAME_PARAM], 0)

        elif BUFFER_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise ValueError("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            self._ctxt = kwargs[BUFFER_PARAM]
            archive_write_open_memory(
                self.srcml_archive,
                self._ctxt.buff,
                self._ctxt.size
            )

        elif CONTEXT_PARAM in kwargs:
            if len(kwargs) > 3 or len(kwargs) == 2:
                raise ValueError("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            elif len(kwargs) == 1:
                self._ctxt = kwargs[CONTEXT_PARAM]
                self._write_cb_helper = write_callback(cb_write_helper)
                self._close_write_cb_helper = close_callback(cb_close_helper)
                archive_write_open_io(
                    self.srcml_archive,
                    self._ctxt,
                    self._write_cb_helper,
                    self._close_write_cb_helper
                )
            else:
                self._ctxt = kwargs[CONTEXT_PARAM]
                self._write_cb_helper = write_callback(kwargs[WRITE_CB_PARAM])
                self._close_write_cb_helper = close_callback(kwargs[CLOSE_CB_PARAM])
                archive_write_open_io(
                    self.srcml_archive,
                    self._ctxt,
                    self._write_cb_helper,
                    self._close_write_cb_helper
                )
        else:
            raise ValueError("Invalid parameter combination")

    # Parts of pythonic interface
    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        self.close()
        archive_free(self.srcml_archive)
        self.srcml_archive = None

    def __del__(self):
        if self.srcml_archive != None:
            self.close()
            archive_free(self.srcml_archive)

    @property
    def xml_encoding(self):
        """
        Returns the encoding being used to output into XML. If this is None then
        UTF-8 is being used.
        """
        return archive_get_xml_encoding(self.srcml_archive)

    @property
    def src_encoding(self):
        """
        Returns the default encoding that a unit expects when parsing
        source code. This can be overridden on an individual unit by
        setting the src_encoding of that unit.
        """
        return archive_get_src_encoding(self.srcml_archive)

    @property
    def default_language(self):
        """
        The default programming language used when parsing source code
        within a unit.
        """
        return archive_get_language(self.srcml_archive)

    @property
    def filename(self):
        """
        The filename attribute set on an archive.
        """
        return archive_get_filename(self.srcml_archive)

    @property
    def url(self):
        """
        The url attribute set on an archive.
        """
        return archive_get_url(self.srcml_archive)

    @property
    def version(self):
        """
        The version attribute set on an archive.
        """
        return archive_get_version(self.srcml_archive)

    @property
    def revision(self):
        """
        The current revision of srcml that's being used to parse
        source code within a unit.
        """
        return archive_get_revision(self.srcml_archive)

    @property
    def tab_stop(self):
        """
        The number of spaces a tab character is worth when calculating column 
        position.
        """
        return archive_get_tabstop(self.srcml_archive)

    @property
    def parser_options(self):
        """
        The options used for processing srcml.
        """
        return archive_get_options(self.srcml_archive)

    @parser_options.setter
    def parser_options(self, value):
        """
        The options that can be used to configure the parser.
        """
        if not isinstance(value, (int, ling)):
            raise TypeError("Invalid value provided to parser_options. Options must be a long or an int.")
        archive_set_options(self.srcml_archive, value)

    @property
    def processing_instruction(self):
        """
        Returns a processing instruction.
        """
        return self._proc_instr

    @property
    def xml_namespaces(self):
        """
        Returns an immutable collection of XML namespaces
        associated with the current archive.
        """
        return self._xml_namespaces

    @property
    def macros(self):
        """
        Returns an immutable collection of macros associated with an archive.
        """
        return self._macros

    def register_file_extension(self, file_extension, language):
        """
        Associate a language with a file extension. This effects how language is
        determined when loading a file using the parse function that uses
        a filename.
        """
        if file_extension is None or not isinstance(file_extension, str):
            raise TypeError("Invalid file_extension. file_extension must be a string and can not be None.")

        if language is None or not isinstance(language, str):
            raise TypeError("Invalid language. language must be a string and cannot be None.")

        if check_language(language) == 0:
            raise invalid_srcml_language("Invalid language provided.", language)
        archive_register_file_extension(self.srcml_archive, file_extension, language)

    def check_file_extension(self, file_name_or_path):
        """
        Get the programming language associated with a file name/path.
        """
        if file_name_or_path is None or not isinstance(file_name_or_path, str):
            raise TypeError("Invalid file_name_or_path. file_name_or_path must be a string and can not be None.")
        ret = archive_check_extension(self.srcml_archive,file_name_or_path)
        if ret == None:
            raise KeyError("Invalid language for file extension. The file name or path:\"{0}\" doesn't have a recognized extension within the current archive.".format(file_name_or_path))
        return ret

    def create_unit(self, language=None, src_encoding=None, filename=None, url=None, version=None, timestamp=None, hash_of_unit=None):
        """
        writable_unit creation factory. This function creates a unit and sets the parameters provided corresponding to
        the attributes of a writable_unit.

        Parameter descriptions:

        language - The programming language associated with the source code that's
            going to be writing into the unit. The language can be inferred when using
            reading from a filename or if the language is set on the archive that was
            used to create the unit. The language, is written into the unit even if
            it's set to None. Not setting the language prior to calling parse
            on a unit, where the language cannot be inferred, results in a
            language_not_set exception.

        src_encoding - The encoding to expected when reading source code. If not
            set the default encoding used is Latin-1.

        filename - A filename attribute associated with then XML of the unit. If None 
            it's not output onto the current unit.

        url - A url attribute associated with the XML of the unit. If None 
            it's not output onto the current unit.

        version - A version attribute associated with the XML of the unit. If None 
            it's not output onto the current unit.

        timestamp - A timestamp attribute to associated with the unit. If None
            it's not output into the archive.

        hash_of_unit - The hash of a unit is set by default so typically there is no
            need to set it manually. To disable the setting of the hash of a unit
            the OPTION_HASH must be disabled within the archive.
        """
        unit_ptr = unit_create(self.srcml_archive)
        if unit_ptr == None:
            raise MemoryError("Failed to allocate native srcml_unit.")
        return writable_unit(unit_ptr, language, src_encoding, filename, url, version, timestamp, hash_of_unit)

    def write(self, unit):
        """
        Write a unit into an archive. This accepts readable and writable units.
        """
        if unit is None:
            raise TypeError("Unable to write unit into archive. Unit is None")
        write_unit(self.srcml_archive, unit.srcml_unit)

    def close(self):
        """
        Closes a writable archive.
        """
        archive_close(self.srcml_archive)