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

import ctypes, bindings, io


class memory_buffer(object):
    """
    A memory buffer used to assist with memory allocation and 
    passing native memory between python and libsrcml.
    """

    def _make_array(self):
        return (ctypes.c_byte * self.size.value).from_address(ctypes.cast(self.buff, ctypes.c_void_p).value)

    def __init__(self, initial_string_value = None):
        """
        Construct an empty buffer or a buffer with the value of a string.
        
        initial_string_value - is used to load the buffer with an initial value.
        """
        self.buff = ctypes.c_char_p(0)
        self.size = ctypes.c_size_t(0)
        if initial_string_value is not None:
            self.load_from_string(initial_string_value)


    def __enter__(self):
        """
        Make the current buffer work with a with-statement.
        """
        return self

    def __exit__(self, type, value, traceback):
        """Cleanup from a with-statement. """
        self.free()

    def __del__(self):
        """
        Destructor cleans up on object destruction.
        """
        self.free()

    def allocate(self, allocation_size):
        """
        Allocate a new buffer. If a previous buffer was allocated
        it is freed. There is no way to append to this buffer during
        allocation all previous data is lost.
        """
        if not isinstance(allocation_size, (int, long)):
            raise TypeError("Invalid type", "allocation_size", allocation_size)

        if allocation_size <= 0:
            raise ValueError("Invalid allocation size", "allocation_size", allocation_size)

        self.free()
        self.buff = ctypes.c_char_p(bindings.malloc(allocation_size))
        self.size = ctypes.c_size_t(allocation_size)

    def free(self):
        """
        Free the currently allocated native memory.
        """
        if self.buff != ctypes.c_char_p():
            bindings.free(self.buff)
            self.buff = ctypes.c_char_p(0)
            self.size.value = 0

    def __getitem__(self, nth_element):
        """
        Return the nth element within the native buffer.
        """
        if nth_element < 0 or nth_element >= len(self):
            raise IndexError("Invalid nth_element", nth_element)
        return self._make_array()[nth_element]

    def __setitem__(self, nth_element, value):
        """
        Set the value of the nth element within the buffer.
        """
        if nth_element < 0 or nth_element >= len(self):
            raise IndexError("Invalid nth_element", nth_element)
        self._make_array()[nth_element] = value

    def __len__(self):
        """
        Returns the number of elements within the native buffer. This doesn't
        count elements that are not null only the number of bytes allocated.
        """
        return self.size.value

    def __str__(self):
        """
        Attempts to convert the current buffer into a native string.
        """
        return self.to_string()

    def __repr__(self):
        """
        Returns a partial representation of the buffer in memory.
        """
        return "Memory Buffer: Size: {1} {0}".format(self.buff, self.size)
    
    def __iter__(self):
        """
        Returns an iterator for iterating over all bytes within the buffer.
        """
        temp_array = self._make_array()
        for x in temp_array:
            yield x

    def to_string(self, encoding = None, count = None):
        """
        Attempts to return a string with a specified encoding.
        If the encoding is None then then no encoding is used
        when extracting text from the buffer.
        """
        if len(self) == 0:
            return ""
        if encoding == None:
            if count != None:
                return str(bytearray(self._make_array()[:count]))
            return str(bytearray(self._make_array()))
        else:
            if count != None:
                "\n".join(io.TextIOWrapper(io.BytesIO(self._make_array()[:count]), encoding).readlines())
            return "\n".join(io.TextIOWrapper(io.BytesIO(self._make_array()), encoding).readlines())

    def to_list(self):
        """
        Returns an immutable representation of the buffer in memory.
        """
        return [x for x in self._make_array()]

    def set(self, index, buffer_or_list, offset = 0, count = None):
        """
        Set a section of the array to a set of values.

        index - The starting position within the native buffer.
        buffer_or_list - Some iterable sequence of byte values to copy into
            the native buffer.
        offset - the starting index within buffer_or_list
        count - the number of elements to copy from buffer_or_list into the native buffer.
        """
        if index < 0 or index >= len(self):
            raise IndexError("Invalid index")

        if buffer_or_list is None:
            raise TypeError("buffer_or_list is None")

        if offset < 0 or offset >= len(buffer_or_list):
            raise IndexError("Invalid offset")

        if count == None:
            self._make_array()[index:(len(buffer_or_list) - offset + index)] = buffer_or_list[offset:(len(buffer_or_list))]
        else:
            if count < 0 or (len(self)+index) < count:
                raise ValueError("Invalid number of elements to copy")
            self._make_array()[index:index + count] = buffer_or_list[offset:offset + count]

    def zero_out(self, index = 0, count = None):
        """
        Set the value of a range of bytes within the buffer to zero.

        index -  the index within the buffer to start setting values at.
        count - the number of elements to set, if None then all elements after 
            index to the end of the buffer are set to zero.
        """
        if len(self) == 0:
            return
        if index < 0 or index >= len(self):
            raise IndexError("Invalid index")
        temp_array = self._make_array()
        if count is not None:
            if count < 0 or count > len(self) or (index + count) >= len(self):
                raise ValueError("Invalid count")
            for i in range(index, index + count):
                temp_array[i] = 0
        else:
            for i in range(index, len(self)):
                temp_array[i] = 0

    def load_from_string(self, inputString, encoding = None):
        """
        Sets the buffer to the contents of the string. If an encoding is specified
        then that encoding is used to translate the string into the specified encoding
        before copying its value into the memory buffer.

        This function allocates a new buffer and frees the previous buffer.
        inputString - The input string.
        encoding - The encoding to convert the string to so that it can be 
            stored within the native buffer.
        """
        if inputString is None:
            raise TypeError("inputString is None")
        if encoding != None:
            input_byte_array = [ord(c) for c in inputString.encode(encoding)]
        else:
            input_byte_array = bytearray(inputString)
        self.free()
        self.allocate(len(input_byte_array))
        temp_array = self._make_array()
        temp_array[:] = input_byte_array[:]