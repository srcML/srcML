##
# @file memory_buffer.py
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

import ctypes, bindings



class memory_buffer:
    """
    A memory buffer used to assist with memory allocation and 
    passing native memory between python and libsrcml.
    """

    def __init__(self):
        self.buff = ctypes.c_char_p(0)
        self.size = ctypes.c_int(0)


    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        self.free()

    def __del__(self):
        self.free()

    def allocate(self, allocation_size):
        if not isinstance(allocation_size, (int, long)):
            raise TypeError("Invalid type", "allocation_size", allocation_size)

        if allocation_size <= 0:
            raise ValueError("Invalid allocation size", "allocation_size", allocation_size)

        self.free()
        self.buff = bindings.malloc(allocation_size)
        self.size = ctypes.c_int(allocation_size)

    def free(self):
        if self.buff != 0:
            bindings.free(self.buff)
            self.buff = ctypes.c_char_p(0)
            self.size.value = 0

    def __getitem__(self, key):
        return ((ctypes.c_byte * self.size.value).from_address(self.buff))[key]

    def __setitem__(self, key, value):
        ((ctypes.c_byte * self.size.value).from_address(self.buff))[key] = value

    def __len__(self):
        return self.size.value

    def __str__(self):
        return self.buff.value if self.buff != ctypes.c_char_p() else ""

    def __repr__(self):
        return "Memory Buffer: {0}".format(self.buff.value)
    
    def __iter__(self):
        raise NotImplementedError()

    def to_string(self, encoding = None):
        raise NotImplementedError()

    def to_list(self, encoding = None):
        raise NotImplementedError()

    def set(self, index, buffer_or_byte, offset = 0, count = None):
        raise NotImplementedError()

    def zero_out(self, offset = 0, count = None):
        raise NotImplementedError()

    def load_from_string(self, buffer, encoding = None):
        raise NotImplementedError()


    # class MemoryBuffer
    #     void allocate(int allocationSize)
    #     byte get(int index)
    #     void set(int index, byte data)
    #     void set(int index, byte[] data)
    #     void set(int index, byte[] data, int offSet)
    #     void set(int index, byte[] data, int offSet, int count)
    #     int size()
    #     void zeroOut()
    #     void zeroOut(int startingIndex)
    #     void zeroOut(int startingIndex, int count)
    #     void free()
    #     void loadFromString(String str)
    #     String toString()
    #     byte[] toArray()

