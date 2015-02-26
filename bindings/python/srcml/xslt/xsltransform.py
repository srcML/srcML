##
# @file xsltransform.py
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

from xslt_base_class import *
from .. bindings import *
from .. private_helpers import *
from .. memory_buffer import *
import cStringIO, ctypes

class xsltransform(xsltransform_base):
    def __init__(self, **kwargs):
        """
        Read an XSL Transformation document from one of the following sources:
        1) file name
        2) srcml.memory_buffer.
        3) I/O functions
        4) I/O interface
        5) python stream
        6) XSLT string
        
        Recognized argument structures:
        1) to filename
        xsltransform(filename='file.xsl')

        2) to memory_buffer
        mem_buff = srcml.memory_buffer()
        xsltransform(mem_buff)

        3) using I/O callbacks
        xsltransform(context =ctxtObj, read= readFunc, close= closeFunc)

        4) Using I/O interface: this is the same type of interface that's used by other
        classes that provides a write and close functions just like the ones used with the context
        callbacks.
        xsltransform(context = myWriterContext())

        5) to file stream
        xsltransform(stream = open("filepath.cpp"))

        6) xslt 
        xsltransform(xslt=string_of_xslt_document)

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
        self._dispatch = None
        if STREAM_PARAM in kwargs:
            if len(kwargs) == 1:
                strm = kwargs[STREAM_PARAM]
                self.buffer = memory_buffer()
                try:
                    self.buffer.load_from_string("".join(strm.readlines))
                except:
                    self.buffer.load_from_string(strm.getvalue())
                self._dispatch = xsltransform._buffer_dispatch
            else:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))

        elif FILENAME_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            self.filename = kwargs[FILENAME_PARAM]
            self._dispatch = xsltransform._filename_dispatch

        elif BUFFER_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            self.buffer = kwargs[BUFFER_PARAM]
            self._dispatch = xsltransform._buffer_dispatch

        elif CONTEXT_PARAM in kwargs:
            if len(kwargs) > 3 or len(kwargs) == 2:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            elif len(kwargs) == 1:
                self._ctxt = kwargs[CONTEXT_PARAM]
                temp_buffer = memory_buffer()
                buffer_size = 4096
                temp_buffer.allocate(buffer_size)
                faux_array_buffer = (ctypes.c_byte * buffer_size).from_address(ctypes.cast(temp_buffer.buff, ctypes.c_void_p).value)
                temp_buffer.zero_out()
                output_stream = cStringIO.StringIO()
                bytes_read = self._ctxt.read(faux_array_buffer, buffer_size)
                if bytes_read == -1:
                    raise Exception("Encountered an error while reading.")
                while bytes_read > 0:

                    output_stream.write(temp_buffer.to_string(count = bytes_read))
                    temp_buffer.zero_out()
                    bytes_read = self._ctxt.read(faux_array_buffer, buffer_size)
                    if bytes_read == -1:
                        raise Exception("Encountered an error while reading.")

                if self._ctxt.close() == -1:
                    raise Exception("Encountered an error while reading.")
                self.buffer = memory_buffer()
                self.buffer.load_from_string(output_stream.getvalue())
                output_stream.close()
                self._dispatch = xsltransform._buffer_dispatch
                
            else:
                self._ctxt = kwargs[CONTEXT_PARAM]
                read_func = kwargs[READ_CB_PARAM]
                close_func = kwargs[CLOSE_CB_PARAM]
                temp_buffer = memory_buffer()
                temp_buffer.zero_out()
                buffer_size = 4096
                temp_buffer.allocate(buffer_size)
                temp_buffer.zero_out()
                faux_array_buffer = (ctypes.c_byte * buffer_size).from_address(ctypes.cast(temp_buffer.buff, ctypes.c_void_p).value)
                bytes_read = read_func(self._ctxt, faux_array_buffer, buffer_size)
                if bytes_read == -1:
                    raise Exception("Encountered an error while reading.")
                output_stream = cStringIO.StringIO()
                while bytes_read > 0:
                    output_stream.write(temp_buffer.to_string(count = bytes_read))
                    temp_buffer.zero_out()
                    bytes_read = read_func(self._ctxt, faux_array_buffer, buffer_size)
                    if bytes_read == -1:
                        raise Exception("Encountered an error while reading.")
                if close_func(self._ctxt) == -1:
                    raise Exception("Encountered an error while reading.")
                self.buffer = memory_buffer()
                self.buffer.load_from_string(output_stream.getvalue())
                output_stream.close()
                self._dispatch = xsltransform._buffer_dispatch
        elif XSLT_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            self.buffer = memory_buffer()
            self.buffer.load_from_string(kwargs[XSLT_PARAM])
            self._dispatch = xsltransform._buffer_dispatch
        else:
            raise Exception("No known parameters")



    def _buffer_dispatch(self, arch_ptr):
        append_transform_xslt_memory(arch_ptr, self.buffer.buff, self.buffer.size)

    def _filename_dispatch(self, arch_ptr):
        append_transform_xslt_filename(arch_ptr, self.filename)

    def apply(self, arch_ptr):
        self._dispatch(self, arch_ptr)