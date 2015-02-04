##
# @file test_memory_buffer.py
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

import os, unittest, ctypes

from srcml import memory_buffer
from testlib import expect_exception

class TestMemoryBuffer(unittest.TestCase):

    def setUp(self):
        pass
    
    def tearDown(self):
        pass
        
    def test_defaultConstruction(self):
        buff = memory_buffer()
        self.assertIsNotNone(buff.buff, "Didn't find private member _buff")
        self.assertIsNotNone(buff.size, "Didn't find private member _size")
        self.assertEqual(buff.size.value, 0, "Incorrect size")
        self.assertEqual(buff.buff.value, None, "Incorrect buffer data.")
        buff = None

    # def test_bufferConstruction(self):
    #     buff = memory_buffer()
    #     self.assertIsNotNone(buff._buff, "Didn't find private member _buff")
    #     self.assertIsNotNone(buff._size, "Didn't find private member _size")
    #     self.assertEqual(buff._size.value, 0, "Incorrect size")
    #     self.assertEqual(buff._buff.value, None, "Incorrect buffer data.")
    #     buff = None

    # def test_stringConstruction(self):
    #     buff = memory_buffer()
    #     self.assertIsNotNone(buff._buff, "Didn't find private member _buff")
    #     self.assertIsNotNone(buff._size, "Didn't find private member _size")
    #     self.assertEqual(buff._size.value, 0, "Incorrect size")
    #     self.assertEqual(buff._buff.value, None, "Incorrect buffer data.")
    #     buff = None


    def test_with_stmt(self):
        with memory_buffer() as buff:
            self.assertIsNotNone(buff.buff, "Didn't find private member _buff")
            self.assertIsNotNone(buff.size, "Didn't find private member _size")
            self.assertEqual(buff.size.value, 0, "Incorrect size")
            self.assertEqual(buff.buff.value, None, "Incorrect buffer data.")

    def test_allocate(self):
        with memory_buffer() as buff:
            self.assertEqual(len(buff), 0, "Incorrect size")
            buff.allocate(5)
            self.assertEqual(buff.size.value, 5, "Incorrect size")

    def test_len(self):
        buff = memory_buffer()
        self.assertEqual(len(buff), 0, "Incorrect size")
        buff.allocate(5)
        self.assertEqual(len(buff), 5, "Incorrect size")
        buff = None


    def test_indexing(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            for i in range(5):
                buff[i] = i
            for i in range(5):
                self.assertEqual(i, buff[i], "Incorrect byte value")

    def test_load_from_string(self):
        with memory_buffer() as buff:
            expected = "This Is My String"
            buff.load_from_string(expected)
            self.assertEqual(expected, buff.to_string(), "Incorrect buffer content")

    @expect_exception(TypeError)
    def test_load_from_string_NoneArgument(self):
        pass
        # with memory_buffer() as buff:
        #     expected = "This Is My String"
        #     buff.load_from_string(None)
        #     self.assertEqual(expected, buff.to_string(), "Incorrect buffer content")