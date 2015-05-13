##
# @file readable_unit.py
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
from exceptions import invalid_srcml_encoding
from private_helpers import *
import cStringIO
from memory_buffer import memory_buffer

class readable_unit(object):
    """
    The readable_unit class is created by a readable archive and provides access
    to an individual unit stored within an archive.
    """

    def __init__(self, unit_ptr):
        self.srcml_unit = unit_ptr
        
    def __del__(self):
        unit_free(self.srcml_unit)

    @property
    def src_encoding(self):
        """
        Get the encoding to unparse the source code out as. If None
        the default encoding is Latin-1.
        """
        return unit_get_src_encoding(self.srcml_unit)
    
    @src_encoding.setter
    def src_encoding(self, value):
        """
        Set the default encoding for source code being unparsed in
        and processed by srcML.

        Preconditions - value must be a string or None.
        """
        assert isinstance(value, (str)) or value is None, "src_encoding must be a string or None"
        if value == None:
            unit_set_src_encoding(self.srcml_unit, value)
        else:
            if check_encoding(value) == 0:
                raise invalid_srcml_encoding("Invalid source code encoding", value)
            unit_set_src_encoding(self.srcml_unit, value)

    @property
    def language(self):
        """
        Returns the language attribute of the unit.
        """
        return unit_get_language(self.srcml_unit)

    @property
    def filename(self):
        """
        Returns the value of the filename attribute of a unit or
        None if not set.
        """
        return unit_get_filename(self.srcml_unit)
        
    @property
    def url(self):
        """
        Returns the value of the url attribute or None if not
        set.
        """
        return unit_get_url(self.srcml_unit)

    @property
    def version(self):
        """
        Returns the value of the version attribute of the unit or
        None if not set.
        """
        return unit_get_version(self.srcml_unit)

    @property
    def timestamp(self):
        """
        Returns the timestamp attribute of the unit or None
        if not set.
        """
        return unit_get_timestamp(self.srcml_unit)

    @property
    def hash(self):
        """
        Returns the hash attribute of a unit or None if not set.
        """
        return unit_get_hash(self.srcml_unit)

    @property
    def revision(self):
        """
        Returns the revision of srcML used to process the current unit.
        """
        return unit_get_revision(self.srcml_unit)

    def unparse(self, **kwargs):
        """
        Extract source code from srcml format. This removes all srcml
        elements and leaves only the original text for srcml.

        The extracted source is handled in one of the following ways:
        1) extract to file name
        2) extract to srcml.memory_buffer.
        3) extract to I/O functions
        4) extract to I/O interface
        5) extract to python file stream
        6) extract to python string
        
        Recognized argument structures:
        1) to filename
        myUnit.unparse(filename='file.cpp')

        2) to memory_buffer
        mem_buff = srcml.memory_buffer()
        myUnit.unparse(buff = mem_buff)

        3) using I/O callbacks
        myUnit.unparse(context =ctxtObj, write= writeFunc, close= closeFunc)

        4) Using I/O interface: this is the same type of interface that's used by other
        classes that provides a write and close functions just like the ones used with the context
        callbacks.
        myUnit.unparse(context = myWriterContext())

        5) to file stream
        myUnit.unparse(stream = open("filepath.cpp"))

        Note: the python stream has an optional argument close_stream, which defaults to true
        and doesn't close the stream if false.

        6) to string
        if called with no arguments a python string is returned containing the extracted file.

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
        if len(kwargs) == 0:
            temp_buffer = memory_buffer()
            self.unparse(buffer=temp_buffer)
            val = str(temp_buffer)
            return val
        elif STREAM_PARAM in kwargs:
            if len(kwargs) == 1:
                self.unparse(context=stream_context(kwargs[STREAM_PARAM]))
            elif len(kwargs) == 2:
                self.unparse(context=stream_context(kwargs[STREAM_PARAM], kwargs[CLOSE_STREAM_PARAM]))
            else:
                raise KeyError("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))

        elif FILENAME_PARAM in kwargs:
            if len(kwargs) > 2:
                raise KeyError("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            if COMPRESSION_FACTOR_PARAM not in kwargs and len(kwargs) == 2:
                raise KeyError("Invalid/Unknown parameter")
            compression_value = kwargs.pop(COMPRESSION_FACTOR_PARAM, 0)
            unit_unparse_filename(self.srcml_unit, kwargs[FILENAME_PARAM], compression_value)

        elif BUFFER_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise KeyError("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            self.buff = kwargs[BUFFER_PARAM]
            unit_unparse_memory(self.srcml_unit, self.buff.buff, self.buff.size)

        elif CONTEXT_PARAM in kwargs:
            if len(kwargs) > 3 or len(kwargs) == 2:
                raise KeyError("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            elif len(kwargs) == 1:
                self._ctxt = kwargs[CONTEXT_PARAM]
                self._write_helper = write_callback(cb_write_helper)
                self._close_read_helper = close_callback(cb_close_helper)
                unit_unparse_io(
                    self.srcml_unit,
                    self._ctxt,
                    self._write_helper,
                    self._close_read_helper
                )
            else:
                self._ctxt = kwargs[CONTEXT_PARAM]
                self._write_helper = write_callback(kwargs[WRITE_CB_PARAM])
                self._close_read_helper = close_callback(kwargs[CLOSE_CB_PARAM])
                unit_unparse_io(
                    self.srcml_unit,
                    self._ctxt,
                    self._write_helper,
                    self._close_read_helper
                )
        else:
            raise Exception("No known parameters")

    def get_standalone_xml(self, xml_encoding = None):
        """
        Returns a standalone XML document containing the current unit. this includes
        all XML namespaces associated with the archive the unit comes from.

        The XML is extracted into the provided encoding or the same encoding
        used by the archive if not specified.
        """
        
        if xml_encoding is not None and not isinstance(xml_encoding, str):
            raise TypeError("Encoding must be None or a string")
            if check_encoding(xml_encoding) == 0:
                raise invalid_srcml_encoding("Invalid XML encoding", xml_encoding)
        ret = memory_buffer()
        unit_get_xml_standalone(self.srcml_unit, xml_encoding, ret.buff, ret.size)
        strResult = ret.to_string(xml_encoding)
        del ret
        return strResult

    def get_xml_fragment(self):
        """
        Returns a unit fragment of XML from within an archive.
        """
        ret =  unit_get_xml_fragment(self.srcml_unit)
        if ret is None:
            raise MemoryError("Failed to allocate native memory for XML unit fragment.")
        return ret