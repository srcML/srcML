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

_ENCODING_ATTR = "encoding"
_SRC_ENCODING_ATTR = "src_encoding"
_LANGUAGE_ATTR = "language"
_FILENAME_ATTR = "filename"
_DIRECTORY_ATTR = "directory"
_VERSION_ATTR = "version"
_TABSTOP_ATTR = "tabstop"
_OPTIONS_ATTR = "options"


class _macro_proxy_keyiter:
    """Iterator for macro keys."""
    def __init__(self, srcml_archive, starting_index = 0):
        self.archive = srcml_archive
        self.index = starting_index

    def next(self):
        # StopIteration
        pass


class _macro_proxy:
    __doc__ ="""
    Provides a dictionary proxy interface for macro
    representations within srcML a srcml archive.
    """
# __LIBSRCML_DECL int srcml_archive_register_macro (struct srcml_archive*, const char* token, const char* type);  
# __LIBSRCML_DECL int srcml_archive_get_macro_list_size (const struct srcml_archive*);
# __LIBSRCML_DECL const char* srcml_archive_get_macro_token (const struct srcml_archive*, int pos);
# __LIBSRCML_DECL const char* srcml_archive_get_macro_token_type (const struct srcml_archive*, const char* namespace_uri);
# __LIBSRCML_DECL const char* srcml_archive_get_macro_type (const struct srcml_archive*, int pos);

    def __init__(self, srcml_archive):
        self.archive = srcml_archive

    def items(self):
        pass

    def keys(self):
        pass

    def values(self):
        pass

    def iteritems(self):
        pass

    def iterkeys(self):
        pass

    def itervalues(self):
        pass

    def update(self, *args, **kwargs):
        """ Update the dictionary with a dictionary comprehension AND/OR using kwargs"""
        if len(args) > 0:
            for arg in args:
                if isinstance(arg, dict):
                    archive_register_macro(self.archive, arg[0], arg[1])
                else:
                    raise TypeError("Expecting type dict got type:".format(arg))
        # Handling kwargs
        for k, v in kwargs.iteritems():
            archive_register_macro(self.archive, k, v)




    def pop(self, key, default=None):
        pass

    def popitem(self):
        pass

    def clear(self):
        pass

    def __len__(self):
        """ Returns number of registered macros elements."""
        return archive_get_macro_list_size(self.archive)

    def __getitem__(self, key):
        """Returns an item registered within the macro."""
        pass

    def __setitem__(self, key, value):
        """Set the value of a macro element."""
        pass

    def __delitem__(self, key):
        """For removing registered macros from archive."""
        pass

    def has_key(self, key):
        pass

    def __contains__(self, item):
        """Test if an element exists within the list of macros"""
        pass

    def __iter__(self):
        """TODO: Return iteration over keys."""
        pass

    def __reversed__(self):
        """Same as __iter__ but backwards."""
        pass

archive_attr_lookup = dict(
{
    _ENCODING_ATTR: (archive_get_encoding, archive_set_encoding,),
    _SRC_ENCODING_ATTR : (archive_get_src_encoding, archive_set_src_encoding,),
    _LANGUAGE_ATTR : (archive_get_language, archive_set_language,),
    _FILENAME_ATTR : (archive_get_filename, archive_set_filename,),
    _DIRECTORY_ATTR : (archive_get_directory, archive_set_directory,),
    _VERSION_ATTR : (archive_get_version, archive_set_version,),
    _TABSTOP_ATTR : (archive_get_tabstop, archive_set_tabstop,),
    _OPTIONS_ATTR : (archive_get_options, archive_set_options,),
})

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

        - macros - special attribute that's used for interfacing with
            the internal macro representation.
    Valid options for options attribute:
        OPTION_ARCHIVE
        OPTION_POSITION
        OPTION_CPP_NOMACRO
        OPTION_CPP =
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
            - macros
        """
        self.srcml_archive = create_archive()
        def _getAttr(attr):
            if attr in kwargs:
                self.__setattr__(attr, kwargs[attr])
        for attr in [key for key in archive_attr_lookup.keys()]:
            _getAttr(attr)

        self.macros = _macro_proxy(self.srcml_archive)



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
        if attrName in archive_attr_lookup:
            return archive_attr_lookup[attrName][0](self.srcml_archive)
        else:
            return super(archive, self).__getattr__(attrName)

    def __setattr__(self, attrName, value):
        """
        This is used in order to provide an interface to all attributes
        that are held, natively, by srcml archive.
        """
        if attrName in archive_attr_lookup:
            archive_attr_lookup[attrName][1](self.srcml_archive, value)
        else:
            return super(archive, self).__setattr__(attrName, value)

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



    # Read function.
    def open_read(self, **kwargs):
        """
        Opens an archive for reading.

        An archive can be opened, for reading, in several ways:
            1) Using a python stream.
            2) Using a python file object.
            3) Using a filename,
            4) Using xml from a python string.
            5) Using a srcml.memory_buffer.
            6) Using a I/O callback functions and context.
            7) Using a I/O interface.
            8) Using a C file descriptor (this is used to interface with other
                C libraries that use file descriptors).

        Ways to call open_read():
            1) Python Stream:
                strm = open("somefile.xml","r")
                archive.open_read(stream = strm)

            2) Python file:
                f = file("somefile.xml")
                archive.open_read(file_obj = f)

            3) filename:
                archive.open_read(filename = "somefile.xml")

            4) raw xml string:
                archive.open_read(xml = preloadedXMLString)

            5) srcml.memory_buffer (creating a srcml memory buffer in this way is NOT recommended,
                    a memory buffer is usually created using open_write(buff = mem_buff)):
                mem_buff = srcml.memory_buffer()
                archive.open_read(buff = mem_buff)

            6) I/O callbacks:
                context = memory_to_read_from
                archive.open_read(context = context, read_cb = read_func, close_cb = close_func)

            7) I/O interface:
                readerContext = MyReader()
                archive.open_read(context = readerContext)

            8) file descriptor:
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
                        return number_of_bytes_writen_to_buffer

                    def close(self):
                        return zero_for_sucess_not_zero_for_failure
        """
        pass

    def open_write(self, **kwargs):
        """
        Opens an archive for writing.

        An archive can be opened, for writing, in several ways:
            1) Using a python stream.
            2) Using a python file object.
            3) Using a filename,
            4) Using a srcml.memory_buffer.
            5) Using a I/O callback functions and context.
            6) Using a I/O interface.
            7) Using a C file descriptor (this is used to interface with other
                C libraries that use file descriptors).

        Ways to call open_write():
            1) Python Stream:
                strm = open("somefile.xml","2")
                archive.open_write(stream = strm)

            2) Python file:
                f = file("somefile.cpp.xml")
                archive.open_write(file_obj = f)

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
                        return number_of_bytes_writen_to_buffer

                    def close(self):
                        return zero_for_sucess_not_zero_for_failure
        """
        pass
