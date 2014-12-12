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

import ctypes
import bindings

class memory_buffer:
    """
    A memory buffer is the result of, or input to, a srcML IO operation.
    The only way the buffer should be modified is by making a copy and instead
    modifying that.

    This represents an allocated section of memory that is used to
    more easily interface with memory I/O functions of srcML such as
    open_read_memory OR open_write_memory. The memory buffer can be used to obtain
    intermediate results during processing of archives during write operations.
    """

    def __init__(self):
        self._buff = ctypes.c_char_p()
        self._size = ctypes.c_int(0)

    def __del__(self):
        if self._buff.value != None:
            bindings.free(self._buff)
        pass

    def clear(self):
        if self._buff.value != None:
            bindings.free(self._buff)
            self._buff.value = None
            self._size.value = 0

    def __getitem__(self, key):
        return self._buff.value[ley]

    def __len__(self):
        return self._size.value

    def __str__(self):
        return self._buff.value if self._buff != ctypes.c_char_p() else ""
    

