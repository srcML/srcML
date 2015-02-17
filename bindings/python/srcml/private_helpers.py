##
# @file private_helpers.py
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

import ctypes

__doc__ = """
This file contains constants which are used by multiple different areas 
but are all internal to the module.
"""

STREAM_PARAM = "stream"
FILENAME_PARAM = "filename"
XML_PARAM = "xml"
SIZE_PARAM = "size"
BUFFER_PARAM = "buffer"
CONTEXT_PARAM = "context"
READ_CB_PARAM = "read"
WRITE_CB_PARAM = "write"
CLOSE_CB_PARAM = "close"
CLOSE_STREAM_PARAM = "close_stream"
XSLT_PARAM = "xslt"
RELAXNG_PARAM = "relaxng"
COMPRESSION_FACTOR_PARAM = "compression_factor"
SOURCE_CODE_PARAM = "source_code"


# Contexts for reading and writing to/from python to the libsrcml
# interface.
class str_reader_context(object):
    def __init__(self, xml_string, size):
        self.xml_str = xml_string
        self.index = 0
        self.end_index = size

    def read(self, buff, size):
        outBufferIndex = 0
        amountToWrite = min(len(self.xml_str), size - 1)
        buff[:amountToWrite] = [ord(x) for x in self.xml_str[:amountToWrite]]
        self.xml_str = self.xml_str[amountToWrite:]
        return amountToWrite

    def close(self):
        return 0

class write_stream_context(object):
    def __init__(self, stream, close_on_complete=True):
        self.strm = stream
        self.close_when_done = close_on_complete

    def write(self, buff, size):
        self.strm.write(buff)
        return size

    def close(self):
        try:
            if self.close_when_done:
                self.strm.close()
        except:
            return -1
        return 0

class stream_context(object):
    def __init__(self, stream, close_on_complete=True):
        self.strm = stream
        self.close_when_done = close_on_complete

    def read(self, buff, size):
        data = self.strm.read(size)
        buff[:len(data)] = [ord(x) for x in data[:len(data)]]
        return len(data)

    def write(self, buff, size):
        self.strm.write(buff)
        return size

    def close(self):
        try:
            if self.close_when_done:
                self.strm.close()
        except:
            return -1
        return 0

# Callback helper functions.
def cb_read_helper(ctxt, buff, size):
    return ctxt.read((ctypes.c_byte * size).from_address(buff), size)

def cb_write_helper(ctxt, buff, size):
    return ctxt.write(buff, size)

def cb_close_helper(ctxt):
    return ctxt.close()

def cb_read_multifunc_hlpr(to_call):
    def cb_read_impl(ctxt, buff, size):
        return to_call(ctxt, (ctypes.c_byte * size).from_address(buff), size)
    return cb_read_impl