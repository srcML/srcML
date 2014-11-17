##
# @file unit.py
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

from bindings import *
from private_helpers import *
import ctypes

_unit_attr_lookup = dict({
    ENCODING_ATTR       : (unit_get_encoding, unit_set_encoding,),
    LANGUAGE_ATTR       : (unit_get_language, unit_set_language,),
    FILENAME_ATTR       : (unit_get_filename, unit_set_filename,),
    DIRECTORY_ATTR      : (unit_get_directory, unit_set_directory,),
    VERSION_ATTR        : (unit_get_version, unit_set_version,),
    TIMESTAMP_ATTR      : (unit_get_timestamp, unit_set_timestamp,),
    HASH_ATTR           : (unit_get_hash, unit_set_hash,),
})



class unit:
    __doc__ = """
    This class represents a native srcml_unit instance.
    A unit is used for reading and writing to/from archives.
    a unit that is being read from an archive cannot be written
    to the same archive unless the archive is re-opened
    for writing.


    Attribute access or modification may change the internal representation
    of an archive which can then be written into another archive.
    """

    def __init__(self, unit_ptr):
        self.srcml_unit = unit_ptr

    def __del__(self):
        free_unit(self.srcml_unit)
    
    def __getattr__(self, attrName):
        """
        Provides access to native attributes as well as
        additional python attributes.
        """
        if attrName in _unit_attr_lookup:
            return _unit_attr_lookup[attrName][0](self.srcml_unit)
        elif attrName in self.__dict__:
            return self.__dict__[attrName]
        else:
            raise KeyError("Attribute doesn't exist. Attribute: {0}".format(attrName))

    def __setattr__(self, attrName, value):
        """
        Provides mutative access to native attributes as well as
        additional python attributes.
        """
        if attrName in _unit_attr_lookup:
            _unit_attr_lookup[attrName][1](self.srcml_unit, value)
        else:
            self.__dict__[attrName] = value

    def srcml_revision(self):
        return unit_get_revision(self.srcml_unit)

    def xml(self):
        """Return an xml representation of the current unit."""
        return unit_get_xml(self.srcml_unit)

    def unparse(self, **kwargs):
        """
        Extract source code from srcml format. This removes all srcml
        elements and leaves only the original text for srcml.

        The extracted source is handled in one of the following ways:
        1) extract to file name
        2) extract to srcml.memory_buffer.
        3) extract to file descriptor
        4) extract to I/O functions
        5) extract to I/O interface
        6) extract to python file stream
        7) extract to python string
        8) extract to a file descriptor
        Recognized argument structures:
        1) to filename
        myUnit.unparse(filename='file.cpp')

        2) to memory_buffer
        mem_buff = srcml.memory_buffer()
        myUnit.unparse(buff = mem_buff)

        3) to file descriptor
        myUnit.unparse(fd = myFileDescriptor)

        4) using I/O callbacks
        myUnit.unparse(context =ctxtObj, write_cb= writeFunc, close_cb= closeFunc)

        5) Using I/O interface: this is the same type of interface that's used by other
        classes that provides a write and close functions just like the ones used with the context
        callbacks.
        myUnit.unparse(context = myWriterContext())

        6) to file stream
        myUnit.unparse(stream = open("filepath.cpp"))

        Note: the python stream has an optional argument close_stream, which defaults to true
        and doesn't close the stream if false.

        7) to string
        if called with no arguments a python string is returned containing the extracted file.

        8) to file descriptor
        myUnit.unparse(fd=myFileDescriptor)

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
        # if STREAM_PARAM in kwargs:
        #     if len(kwargs) == 1:
        #         self.open_write(context=_stream_context(kwargs[STREAM_PARAM]))
        #     elif len(kwargs) == 2:
        #         self.open_write(context=_stream_context(kwargs[STREAM_PARAM], kwargs[CLOSE_STREAM_PARAM]))
        #     else:
        #         raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))

        # elif FILENAME_PARAM in kwargs:
        #     if len(kwargs) > 1 :
        #         raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
        #     write_open_filename(self.srcml_archive, kwargs[FILENAME_PARAM])

        # elif BUFF_PARAM in kwargs:
        #     if len(kwargs) > 1 :
        #         raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
        #     self._ctxt = kwargs[BUFF_PARAM]
        #     write_open_memory(
        #         self.srcml_archive,
        #         self._ctxt._buff,
        #         self._ctxt._size
        #     )

        # elif CONTEXT_PARAM in kwargs:
        #     if len(kwargs) > 3 or len(kwargs) == 2:
        #         raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
        #     elif len(kwargs) == 1:
        #         self._ctxt = kwargs[CONTEXT_PARAM]
        #         write_open_io(
        #             self.srcml_archive,
        #             self._ctxt,
        #             write_callback(_cb_write_helper),
        #             close_callback(_cb_close_helper)
        #         )
        #     else:
        #         self._ctxt = kwargs[CONTEXT_PARAM]
        #         write_open_io(
        #             self.srcml_archive,
        #             self._ctxt,
        #             write_callback(kwargs[WRITE_CB_PARAM]),
        #             close_callback(kwargs[CLOSE_CB_PARAM])
        #         )

        # elif FD_PARAM in kwargs:
        #     if len(kwargs) > 1 :
        #         raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
        #     write_open_fd(self.srcml_archive, kwargs[FD_PARAM])

        # else:
        #     raise Exception("No known parameters")


    def parse(self, *args, **kwargs):
        """
        Takes source code from an input source and processes it with srcml
        and stores it within this unit.

        Possible input sources:
        1) file name
        2) srcml.memory_buffer
        3) file descriptor
        4) I/O functions
        5) I/O interface
        6) python file stream
        7) python string
        8) list of python strings
        9) file descriptor

        Recognized argument structures:
        1) from filename
        myUnit.parse(filename='file.cpp')

        2) from memory_buffer
        mem_buff = srcml.memory_buffer()
        myUnit.parse(buff = mem_buff)

        3) from file descriptor
        myUnit.parse(fd = myFileDescriptor)

        4) using I/O callbacks
        myUnit.parse(context =ctxtObj, read_cb= readFunc, close_cb= closeFunc)

        5) Using I/O interface: This is the same type of interface that's used by other
        classes such as archive. The classes interface provides a read and close functions just like
        the ones used with the context callbacks, but with a self instead of a context parameter.
        myUnit.parse(context = myReaderContext())

        6) from file stream
        myUnit.parse(stream = open("filepath.cpp"))

        7) from string
        myUnit.parse(string_to_run_srcml_on)
        
        8) from list of strings
        myUnit.parse(list_of_string_to_run_srcml_on)

        9) from a file descriptor
        myUnit.parse(fd=myFileDescriptor)

        Description of callbacks and interfaces:
            1) read/close callback signature:
                def read(context, buffer, size_of_buffer):
                    return number_of_bytes_writen_to_buffer # or -1 for error

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
        if len(args) > 0:
            # Re call parse and delegate to another part of this function.
            return

        if len(kwargs) > 0:
            raise Exception("Invalid number of arguments.")

        if STREAM_PARAM in kwargs:
            if len(kwargs) == 1:
                # self.open_write(context=_stream_context(kwargs[STREAM_PARAM]))
                pass
            elif len(kwargs) == 2:
                # self.open_write(context=_stream_context(kwargs[STREAM_PARAM], kwargs[CLOSE_STREAM_PARAM]))
                pass
            else:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))

        elif FILENAME_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            # write_open_filename(self.srcml_archive, kwargs[FILENAME_PARAM])

        elif BUFF_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            self._ctxt = kwargs[BUFF_PARAM]
            # write_open_memory(
            #     self.srcml_archive,
            #     self._ctxt._buff,
            #     self._ctxt._size
            # )

        elif CONTEXT_PARAM in kwargs:
            if len(kwargs) > 3 or len(kwargs) == 2:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            elif len(kwargs) == 1:
                self._ctxt = kwargs[CONTEXT_PARAM]
                # write_open_io(
                #     self.srcml_archive,
                #     self._ctxt,
                #     write_callback(_cb_write_helper),
                #     close_callback(_cb_close_helper)
                # )
            else:
                # self._ctxt = kwargs[CONTEXT_PARAM]
                # write_open_io(
                #     self.srcml_archive,
                #     self._ctxt,
                #     write_callback(kwargs[WRITE_CB_PARAM]),
                #     close_callback(kwargs[CLOSE_CB_PARAM])
                # )
                pass

        elif FD_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            # write_open_fd(self.srcml_archive, kwargs[FD_PARAM])

        else:
            raise Exception("No known parameters")

    # Unit low-level srcml writer interface.
    def write_start_unit(self):
        write_start_unit(self.srcml_unit)

    def write_end_unit(self):
        write_end_unit(self.srcml_unit)

    def write_start_element(self, prefix, name, uri):
        write_start_element(self.srcml_unit, prefix, name, uri)

    def write_end_element(self):
        write_end_element(self.srcml_unit, prefix, name, uri)        

    def write_namespace(self, prefix, uri):
        write_namespace(self.srcml_unit, prefix, uri)

    def write_attribute(self, prefix, name, content):
        write_attribute(self.srcml_unit, prefix, name, content)

    def write_string(self, content):
        write_string(self.srcml_unit, content)


    # Operators
    def __ne__(self, other):
        if other == None:
            return self.srcml_unit != None
        else:
            return self is not other

    def __eq__(self, other):
        if other == None:
            return self.srcml_unit == None
        else:
            return self is other




# __LIBSRCML_DECL int srcml_unparse_unit_filename(struct srcml_unit*, const char* src_filename);
# __LIBSRCML_DECL int srcml_unparse_unit_memory  (struct srcml_unit*, char** src_buffer, int * src_size);
# __LIBSRCML_DECL int srcml_unparse_unit_FILE    (struct srcml_unit*, FILE* srcml_file);
# __LIBSRCML_DECL int srcml_unparse_unit_fd      (struct srcml_unit*, int srcml_fd);
# __LIBSRCML_DECL int srcml_unparse_unit_io      (struct srcml_unit*, void * context, int (*write_callback)(void * context, const char * buffer, int len), int (*close_callback)(void * context));

# __LIBSRCML_DECL int srcml_parse_unit_filename(struct srcml_unit* unit, const char* src_filename);
# __LIBSRCML_DECL int srcml_parse_unit_memory  (struct srcml_unit*, const char* src_buffer, size_t buffer_size);
# __LIBSRCML_DECL int srcml_parse_unit_FILE    (struct srcml_unit*, FILE* src_file);
# __LIBSRCML_DECL int srcml_parse_unit_fd      (struct srcml_unit*, int src_fd);
# __LIBSRCML_DECL int srcml_parse_unit_io      (struct srcml_unit*, void * context, int (*read_callback)(void * context, char * buffer, int len), int (*close_callback)(void * context));