##
# @file archive.py
#
# @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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

from unit import unit
from bindings import *
from memory_buffer import memory_buffer
from private_helpers import *
import ctypes




class _macro_proxy:
    __doc__ ="""
    Provides a dictionary proxy interface for macro
    representations within srcML a srcml archive.
    """
    def __init__(self, archive):
        self.srcml_archive = archive

    def update(self, *args, **kwargs):
        """ Update the dictionary with a dictionary comprehension AND/OR using kwargs"""
        if len(args) > 0:
            for arg in args:
                if isinstance(arg, dict):
                    for macro in arg.iteritems():
                        archive_register_macro(self.srcml_archive, macro[0], macro[1])
                else:
                    raise TypeError("Expecting type dict got type:".format(arg))
        # Handling kwargs
        for k, v in kwargs.iteritems():
            archive_register_macro(self.srcml_archive, k, v)

    def __len__(self):
        """ Returns number of registered macros elements."""
        return archive_get_macro_list_size(self.srcml_archive)

    def __getitem__(self, index):
        """Returns an item registered within the macro."""
        return (archive_get_macro_token(self.srcml_archive, index), archive_get_macro_type(self.srcml_archive, index),) 

    def __iter__(self):
        """TODO: Return iteration over keys."""
        def itemsGen():
            for x in range(len(self)):
                yield self[x]
        return (k for k in itemsGen())

class _xml_namespaces_proxy:
    __doc__ ="""
    Provides a dictionary proxy interface for xml namespaces registered 
    with the srcml archive.
    """

    def __init__(self, archive):
        self.srcml_archive = archive

    def update(self, *args, **kwargs):
        """Update the list of xml namespaces with other nsprefix namespace pair."""
        if len(args) > 0:
            for arg in args:
                if isinstance(arg, dict):
                    for ns in arg.iteritems():
                        archive_register_namespace(self.srcml_archive, ns[0], ns[1])
                else:
                    raise TypeError("Expecting type dict got type:".format(arg))
        # Handling kwargs
        for k, v in kwargs.iteritems():
            archive_register_namespace(self.srcml_archive, k, v)

    def __len__(self):
        """Returns the number of registered namespaces."""
        return archive_get_namespace_size(self.srcml_archive)
        

    def __getitem__(self, index):
        """Returns a namespace prefix, namespace uri pair."""
        return (archive_get_namespace_prefix(self.srcml_archive, index), archive_get_namespace_uri(self.srcml_archive, index),) 

    def __iter__(self):
        """Returns a generator for iterating over ns prefix, ns uri pairs."""
        def itemsGen():
            for x in range(len(self)):
                yield self[x]
        return (k for k in itemsGen())

    def get_uri(self, prefix):
        """Resolves prefix to a uri."""
        return archive_get_uri_from_prefix(self.srcml_archive, prefix)

    def get_prefix(self, prefix):
        """Resolves uri to a prefix."""
        return archive_get_prefix_from_uri(self.srcml_archive, prefix)




# Attribute processing        
def _get_processing_instruction(archive):
    return (archive_get_processing_instruction_target(archive), archive_get_processing_instruction_data(archive),)

def _set_processing_instruction(archive, value):
    archive_set_processing_instruction(archive, value[0], value[1])

_PRIVATE_READER_CONTEXT_ATTR = "_ctxt"
_archive_attr_lookup = dict(
{
    ENCODING_ATTR: (archive_get_encoding, archive_set_encoding,),
    SRC_ENCODING_ATTR : (archive_get_src_encoding, archive_set_src_encoding,),
    LANGUAGE_ATTR : (archive_get_language, archive_set_language,),
    FILENAME_ATTR : (archive_get_filename, archive_set_filename,),
    DIRECTORY_ATTR : (archive_get_directory, archive_set_directory,),
    VERSION_ATTR : (archive_get_version, archive_set_version,),
    TABSTOP_ATTR : (archive_get_tabstop, archive_set_tabstop,),
    OPTIONS_ATTR : (archive_get_options, archive_set_options,),
    PROCESSING_INSTRUCTION_ATTR : (_get_processing_instruction, _set_processing_instruction)
})



#
#   srcml.archive
#
class archive(object):
    __doc__ = """
    This class provides access to units within an archive using either
    a reading or writing interface depending on how an archive's been opened.

    Special attributes don't appear within the __dict__ of this class because
    they are actually member of the srcml_archive native C class.
    Special Attributes:
        - encoding
        - src_encoding
        - language
        - filename
        - directory
        - version
        - tabstop
        - options
        - processing_instruction - tuple of two elements, target and data in that order.

        - macros - special attribute that's used for interfacing with
            the internal macro representation.

        - xml_namespaces - special attribute that provides a proxy to the internal
            representation of xml namespaces.

    Valid options for options attribute:
        OPTION_ARCHIVE
        OPTION_POSITION
        OPTION_CPP_NOMACRO
        OPTION_CPP
        OPTION_XML_DECL
        OPTION_NAMESPACE_DECL
        OPTION_CPP_TEXT_ELSE
        OPTION_CPP_MARKUP_IF0
        OPTION_APPLY_ROOT
        OPTION_COMPRESS
        OPTION_NESTIF
        OPTION_HASH
        OPTION_WRAP_TEMPLATE
        OPTION_INTERACTIVE
        OPTION_XPATH_TOTAL
        OPTION_EXPRESSION
        OPTION_LINE
        OPTION_CPPIF_CHECK
        OPTION_DEBUG_TIMER
        OPTION_TERNARY
        OPTION_PSEUDO_BLOCK
        OPTION_OPTIONAL_MARKUP
        OPTION_LITERAL
        OPTION_MODIFIER
        OPTION_OPERATOR
        OPTION_DEBUG
    """

    def __init__(self, **kwargs):
        """
        Constructs a srcml archive.
        Options for kwargs:
            - encoding
            - src_encoding
            - language
            - filename
            - directory
            - version
            - tabstop
            - options
            - processing_instruction

            - xml_namespaces
            - macros
        """
        self.srcml_archive = create_archive()
        self.macros = _macro_proxy(self.srcml_archive)
        self.xml_namespaces = _xml_namespaces_proxy(self.srcml_archive)
        def _getAttr(attr):
            if attr not in _archive_attr_lookup:
                raise KeyError("Unknown argument: '{0}'".format(attr))
            self.__setattr__(attr, kwargs[attr])

        for attr in kwargs.keys():
            if attr == "xml_namespaces":
                self.xml_namespaces.update(kwargs[attr])
            elif attr == "macros":
                self.macros.update(kwargs[attr])
            else:
                _getAttr(attr)

    def __del__(self):
        """
        Clean up a srcML archive.
        """
        free_archive(self.srcml_archive)

    def __getattr__(self, attrName):
        """
        This is used in order to provide an interface to all attributes
        that are held, natively, by srcml archive.
        """
        if attrName in _archive_attr_lookup:
            return _archive_attr_lookup[attrName][0](self.srcml_archive)
        elif attrName in self.__dict__:
            return self.__dict__[attrName]
        else:
            raise KeyError("Attribute doesn't exist. Attribute: {0}".format(attrName))

    def __setattr__(self, attrName, value):
        """
        This is used in order to provide an interface to all attributes
        that are held, natively, by srcml archive.
        """
        if attrName in _archive_attr_lookup:
            _archive_attr_lookup[attrName][1](self.srcml_archive, value)
        else:
            self.__dict__[attrName] = value

    # Options functions.
    def enable_option(self, option):
        """
        Turns the provided option on.
        """
        archive_enable_option(self.srcml_archive, option)

    def disable_option(self, option):
        """
        Turns option off.
        """
        archive_disable_option(self.srcml_archive, option)

    def option_is_enabled(self, option):
        """
        Checks if an option is on or not.
        """
        return (self.options & option) == option

    # Other function
    def language_from_filename(self, filename):
        """
        Resolves the extension of the filename to a srcml language.
        """
        return archive_check_extension(self.srcml_archive, filename)

    def register_file_ext(self, ext, srcml_language):
        """
        Registers a file extension with a srcml language.
        """
        archive_register_file_extension(self.srcml_archive, ext, srcml_language)


    # Unit reading functions.
    def read_unit_header(self):
        """
        Reads the attribute information for the next unit
        within the archive.
        """
        unit_ptr = read_unit_header(self.srcml_archive)
        return unit(unit_ptr)

    def read_unit_xml(self):
        """
        Reads the xml of the next unit within the archive.
        """
        unit_ptr = read_unit_xml(self.srcml_archive)
        return unit(unit_ptr)

    def close(self):
        """Closes archive for both reading and writing."""
        close_archive(self.srcml_archive)
        if _PRIVATE_READER_CONTEXT_ATTR in self.__dict__:
            del self.__dict__[_PRIVATE_READER_CONTEXT_ATTR]

    def read_unit(self):
        """
        Reads the next unit from the archive.
        Reads entire unit.
        """
        unit_ptr = read_unit(self.srcml_archive)
        return unit(unit_ptr)

    # Read Iteration Utilities
    def unit_headers(self):
        """
        Generator function that allows for the following python code:
        for unit in archive.unit_headers():
            print unit.language
        """
        current_unit_header = self.read_unit_header()
        while current_unit_header != None:
            yield current_unit_header
            current_unit_header = self.read_unit_header()

    def units(self):
        """
        Generator that allows iteration through all units within an archive easily.
        Example:
        for unit in archive.units():
            unit_dom = lxml.etree.fromstring(unit.xml())
        """
        current_unit = self.read_unit()
        while current_unit != None:
            yield current_unit
            current_unit = self.read_unit()


    # Unit writing utilities.
    def create_unit(self):
        """
        Unit creation factory. Units are for writing different to/from srcml/source code.
        Units provide an interface for writing but one cannot write a unit to an archive that
        has been opened for reading, and vice-versa.

        A typical use case for a unit created in this manner is to use it to write source code into srcml
        then into a srcml archive.
        """
        unit_ptr = create_unit(self.srcml_archive)
        return unit(unit_ptr)


    def write_unit(self, unit):
        """Write a unit into the current archive archive."""
        write_unit(self.srcml_archive, unit.srcml_unit)


    # I/O starting functions.
    def open_read(self, **kwargs):
        """
        Opens an archive for reading.

        An archive can be opened, for reading, in several ways:
            1) Using a python stream.
            2) Using a filename,
            3) Using xml from a python string.
            4) Using a srcml.memory_buffer.
            5) Using a I/O callback functions and context.
            6) Using a I/O interface.
            7) Using a C file descriptor (this is used to interface with other
                C libraries that use file descriptors).

        Ways to call open_read():
            1) Python Stream:
                strm = open("somefile.xml","r")
                archive.open_read(stream = strm)

            2) filename:
                archive.open_read(filename = "somefile.xml")

            3) raw xml string (size is optional, if not given the entire string is used):
                archive.open_read(xml = preloadedXMLString, size=lengthOfXML)

            4) srcml.memory_buffer (creating a srcml memory buffer in this way is NOT recommended,
                    a memory buffer is usually created using open_write(buff = mem_buff)):
                mem_buff = srcml.memory_buffer()
                archive.open_read(buff = mem_buff)

            5) I/O callbacks:
                context = memory_to_read_from
                archive.open_read(context = context, read_cb = read_func, close_cb = close_func)

            6) I/O interface:
                readerContext = MyReader()
                archive.open_read(context = readerContext)

            7) file descriptor:
                archive.open_read(fd = file_descriptor)



        Description of callbacks and interfaces:
            1) read/close callback signature:
                def read(context, buffer, size_of_buffer):
                    return number_of_bytes_writen_to_buffer

                def close(context):
                    return zero_for_sucess_not_zero_for_failure

                A context can by any python object.

            2) I/O interface is an object that implements read and close 
                similar to those above but doesn't have a context argument.
                    
                class MyReaderInterface:
                    def read(self, buffer, size_of_buffer):
                        return number_of_bytes_writen_to_buffer # or -1 for error

                    def close(self):
                        return zero_for_sucess_not_zero_for_failure 
        """
        if STREAM_PARAM in kwargs:
            if len(kwargs) > 1:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            self.open_read(context=stream_context(kwargs[STREAM_PARAM]))

        elif FILENAME_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            read_open_filename(self.srcml_archive, kwargs[FILENAME_PARAM])

        elif XML_PARAM in kwargs:
            if len(kwargs) > 2 or len(kwargs) == 2 and SIZE_PARAM not in kwargs:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            amount_to_read = 0
            if SIZE_PARAM in kwargs:
                amount_to_read = kwargs[SIZE_PARAM]
            else:
                amount_to_read = len(kwargs[XML_PARAM])
            self.open_read(context=str_reader_context(kwargs[XML_PARAM], amount_to_read))

        elif BUFF_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            self._ctxt = kwargs[BUFF_PARAM]
            read_open_memory(self.srcml_archive, self._ctxt._buff, self._ctxt._size)

        elif CONTEXT_PARAM in kwargs:
            if len(kwargs) > 3:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            elif len(kwargs) == 2:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            elif len(kwargs) == 1:
                self._ctxt = kwargs[CONTEXT_PARAM]
                read_open_io(
                    self.srcml_archive,
                    self._ctxt,
                    read_callback(cb_read_helper),
                    close_callback(cb_close_helper)
                )
            else:
                self._ctxt = kwargs[CONTEXT_PARAM]
                read_open_io(
                    self.srcml_archive,
                    self._ctxt,
                    read_callback(kwargs[READ_CB_PARAM]),
                    close_callback(kwargs[CLOSE_CB_PARAM])
                )

        elif FD_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            read_open_fd(self.srcml_archive, kwargs[FD_PARAM])

        else:
            raise Exception("No known parameters")


    # Comparison operators.
    def __ne__(self, other):
        return not (self == other)

    def __eq__(self, other):
        if other == None:
            return self.srcml_archive == None
        else:
            return self is other

    def open_write(self, **kwargs):
        """
        Opens an archive for writing.

        An archive can be opened, for writing, in several ways:
            1) Using a python stream.
            2) Using a filename,
            3) Using a srcml.memory_buffer.
            4) Using a I/O callback functions and context.
            5) Using a I/O interface.
            6) Using a C file descriptor (this is used to interface with other
                C libraries that use file descriptors).

        Ways to call open_write():
            1) Python Stream:
                strm = open("somefile.xml","2")
                archive.open_write(stream = strm)

            Note: the python stream has an optional argument close_stream, which defaults to true
            and doesn't close the stream if false.

            3) filename:
                archive.open_write(filename = "somefile.xml")

            4) srcml.memory_buffer (a python string representation
                    of a srcml archive can be constructed using
                    the srcml.memory_buffer):
                mem_buff = srcml.memory_buffer()
                archive.open_write(buff = mem_buff)

            5) I/O callbacks:
                context = memory_to_write_to
                archive.open_write(context = context, write_cb = write_func, close_cb = close_func)

            6) I/O interface:
                writerContext = MyMemoryWriter()
                archive.open_write(context = writerContext)

            7) file descriptor:
                archive.open_write(fd = file_descriptor)

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
        """
        if STREAM_PARAM in kwargs:
            if len(kwargs) == 1:
                self.open_write(context=stream_context(kwargs[STREAM_PARAM]))
            elif len(kwargs) == 2:
                self.open_write(context=stream_context(kwargs[STREAM_PARAM], kwargs[CLOSE_STREAM_PARAM]))
            else:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))

        elif FILENAME_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            write_open_filename(self.srcml_archive, kwargs[FILENAME_PARAM])

        elif BUFF_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            self._ctxt = kwargs[BUFF_PARAM]
            write_open_memory(
                self.srcml_archive,
                self._ctxt._buff,
                self._ctxt._size
            )

        elif CONTEXT_PARAM in kwargs:
            if len(kwargs) > 3 or len(kwargs) == 2:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            elif len(kwargs) == 1:
                self._ctxt = kwargs[CONTEXT_PARAM]
                write_open_io(
                    self.srcml_archive,
                    self._ctxt,
                    write_callback(cb_write_helper),
                    close_callback(cb_close_helper)
                )
            else:
                self._ctxt = kwargs[CONTEXT_PARAM]
                write_open_io(
                    self.srcml_archive,
                    self._ctxt,
                    write_callback(kwargs[WRITE_CB_PARAM]),
                    close_callback(kwargs[CLOSE_CB_PARAM])
                )

        elif FD_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            write_open_fd(self.srcml_archive, kwargs[FD_PARAM])

        else:
            raise Exception("No known parameters")