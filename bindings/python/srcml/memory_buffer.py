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

    This will leak memory if the owner of this object doesn't call free or
    if this isn't used with a "with" statement.
    """

    def __init__(self):
        self._buff = ctypes.c_char_p(0)
        self._size = ctypes.c_int(0)


    def __enter__(self):
        return memory_buffer()

    def __exit__(self, type, value, traceback):
        self.free()

    def __del__(self):
        if self != None:
            if self._buff != None:
                self.free()

    def allocate(self, allocation_size):
        if not isinstance(allocation_size, (int, long)):
            raise TypeError("Invalid type", "allocation_size", allocation_size)

        if allocation_size <= 0:
            raise ValueError("Invalid allocation size", "allocation_size", allocation_size)

        self.free()
        self._buff = bindings.malloc(allocation_size)
        self._size = ctypes.c_int(allocation_size)

    def free(self):
        if self._buff.value != 0:
            bindings.free(self._buff)
            self._buff.value = 0
            self._size.value = 0

    def __getitem__(self, key):
        return self._buff.value[key]

    def __setitem__(self, key, value):
        self._buff.value[key] = value

    def __len__(self):
        return self._size.value

    def __str__(self):
        return self._buff.value if self._buff != ctypes.c_char_p() else ""

    def __repr__(self):
        return "Memory Buffer: {0}".format(self._buff.value)
    
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

