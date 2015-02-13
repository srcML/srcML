##
# @file writable_unit.py
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
from exceptions import invalid_srcml_language, invalid_srcml_encoding
from private_helpers import *

class writable_unit(object):
    """
    This class represents a unit within an archive containing source code that
    has been processed using srcml.
    """

    def __init__(self, unit_ptr, language=None, src_encoding=None, filename=None, directory=None, version=None, timestamp=None, hash_of_unit=None):
        """
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

        directory - A directory attribute associated with the XML of the unit. If None 
            it's not output onto the current unit.

        version - A version attribute associated with the XML of the unit. If None 
            it's not output onto the current unit.

        timestamp - A timestamp attribute to associated with the unit. If None
            it's not output into the archive.

        hash_of_unit - The hash of a unit is set by default so typically there is no
            need to set it manually. To disable the setting of the hash of a unit
            the OPTION_HASH must be disabled within the archive.


        precondition unit_ptr can not be None.
        """
        assert unit_ptr is not None, "Invalid srcml_unit pointer provided"

        # Copying parameters to their correct properties.
        self.srcml_unit = unit_ptr
        self.language = language
        self.src_encoding = src_encoding
        self.filename = filename
        self.directory = directory
        self.version = version
        self.timestamp = timestamp
        self.hash = hash_of_unit

    @property
    def src_encoding(self):
        return unit_get_src_encoding(self.srcml_unit)
    
    @src_encoding.setter
    def src_encoding(self, value):
        assert isinstance(value, (str)) or value is None, "src_encoding must be a string or None"
        if value == None:
            unit_set_src_encoding(self.srcml_unit, value)
            # self._src_encoding = value
        else:
            if check_encoding(value) == 0:
                raise invalid_srcml_encoding("Invalid source code encoding", value)
            unit_set_src_encoding(self.srcml_unit, value)


    @property
    def language(self):
        return unit_get_language(self.srcml_unit)

    @language.setter
    def language(self, value):
        assert isinstance(value, (str)) or value is None, "language must be a string or None"
        if value == None:
            unit_set_language(self.srcml_unit, value)
        else:
            if check_language(value) == 0:
                raise invalid_srcml_language("Invalid language", value)
            unit_set_language(self.srcml_unit, value)


    @property
    def filename(self):
        return unit_get_filename(self.srcml_unit)
    @filename.setter
    def filename(self, value):
        assert value is None or isinstance(value, str), "Invalid filename type must be a string or None."
        unit_set_filename(self.srcml_unit, value)

        
    @property
    def directory(self):
        return unit_get_directory(self.srcml_unit)
    @directory.setter
    def directory(self, value):
        assert value is None or isinstance(value, str), "Invalid directory type must be a string or None."
        unit_set_directory(self.srcml_unit, value)


    @property
    def version(self):
        return unit_get_version(self.srcml_unit)
    @version.setter
    def version(self, value):
        assert value is None or isinstance(value, str), "Invalid version type must be a string or None."
        unit_set_version(self.srcml_unit, value)

            
    @property
    def timestamp(self):
        return unit_get_timestamp(self.srcml_unit)
    @timestamp.setter
    def timestamp(self, value):
        assert value is None or isinstance(value, str), "Invalid timestamp type must be a string or None."
        unit_set_timestamp(self.srcml_unit, value)

    
    @property
    def hash(self):
        return unit_get_hash(self.srcml_unit)
    @hash.setter
    def hash(self, value):
        assert value is None or isinstance(value, str), "Invalid hash type must be a string or None."
        unit_set_hash(self.srcml_unit, value)

    
    @property
    def revision(self):
        return unit_get_revision(self.srcml_unit)


    def parse(self, **kwargs):
        """
        Takes source code from an input source and processes it with srcml
        and stores it within this unit.

        Possible input sources:
        1) file name
        2) memory_buffer
        3) I/O functions
        4) I/O interface
        5) python file stream
        6) python string

        Recognized argument structures:
        1) from filename
        myUnit.parse(filename='file.cpp')

        2) from memory_buffer
        mem_buff = srcml.memory_buffer()
        myUnit.parse(buffer = mem_buff)

        3) using I/O callbacks
        myUnit.parse(context =ctxtObj, read= readFunc, close= closeFunc)

        4) Using I/O interface: This is the same type of interface that's used by other
        classes such as archive. The classes interface provides a read and close functions just like
        the ones used with the context callbacks, but with a self instead of a context parameter.
        myUnit.parse(context = myReaderContext())

        5) from file stream
        myUnit.parse(stream = open("filepath.cpp"))

        6) from string
        Passing source code directly into the unit.
        myUnit.parse(source_code=my_code)

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
        if STREAM_PARAM in kwargs:
            if len(kwargs) == 1:
                self.parse(context=stream_context(kwargs[STREAM_PARAM]))
            else:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))

        elif FILENAME_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            unit_parse_filename(self.srcml_unit, kwargs[FILENAME_PARAM])

        elif BUFFER_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            self.buff = kwargs[BUFFER_PARAM]
            unit_parse_memory(
                self.srcml_unit,
                self.buff.buff,
                self.buff.size
            )

        elif CONTEXT_PARAM in kwargs:
            if len(kwargs) > 3 or len(kwargs) == 2:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            elif len(kwargs) == 1:
                self._ctxt = kwargs[CONTEXT_PARAM]
                self._read_helper = read_callback(cb_read_helper)
                self._close_read_helper = close_callback(cb_close_helper)
                unit_parse_io(
                    self.srcml_unit,
                    self._ctxt,
                    self._read_helper,
                    self._close_read_helper
                )
            else:
                self._ctxt = kwargs[CONTEXT_PARAM]
                self._read_helper = read_callback(kwargs[READ_CB_PARAM])
                self._close_read_helper = close_callback(kwargs[CLOSE_CB_PARAM])
                unit_parse_io(
                    self.srcml_unit,
                    self._ctxt,
                    self._read_helper,
                    self._close_read_helper
                )
        elif SOURCE_CODE_PARAM in kwargs:
            self.parse(context=str_reader_context(kwargs[SOURCE_CODE_PARAM], len(kwargs[SOURCE_CODE_PARAM])))
        else:
            raise Exception("No known parameters")


    def get_xml_fragment(self):
        raise NotImplementedError()

    def get_standalone_xml(self, encoding=None):
        raise NotImplementedError()

    def write_start_unit(self):
        raise NotImplementedError()

    def write_end_unit(self):
        raise NotImplementedError()

    def write_start_element(self, element_name, ns_prefix=None, ns_uri=None):
        raise NotImplementedError()

    def write_end_element(self):
        raise NotImplementedError()

    def write_namespace(self, prefix,uri):
        raise NotImplementedError()

    def write_attribute(self, attribute_name, content, ns_prefix = None, ns_uri=None):
        raise NotImplementedError()

    def write_string(self, content):
        raise NotImplementedError()