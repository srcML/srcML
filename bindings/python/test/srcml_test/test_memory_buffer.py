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
        
    def test_default_construction(self):
        buff = memory_buffer()
        self.assertIsNotNone(buff.buff, "Didn't find private member _buff")
        self.assertIsNotNone(buff.size, "Didn't find private member _size")
        self.assertEqual(buff.size.value, 0, "Incorrect size")
        self.assertEqual(buff.buff.value, None, "Incorrect buffer data.")
        buff = None

    def test_string_construction(self):
        expected = "string"
        buff = memory_buffer("string")
        self.assertEqual(len(expected), len(buff), "Incorrect size")
        self.assertEqual(expected, str(buff), "Incorrect buffer data.")
        buff = None


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

    @expect_exception(ValueError)
    def test_allocate_neg(self):
        with memory_buffer() as buff:
            self.assertEqual(len(buff), 0, "Incorrect size")
            buff.allocate(-5)

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

    @expect_exception(IndexError)
    def test_get_index_neg_index(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            i = buff[-1]

    @expect_exception(IndexError)
    def test_get_index_past_end_index(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            i = buff[5]

    @expect_exception(IndexError)
    def test_set_index_neg_index(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            buff[-1] = 4

    @expect_exception(IndexError)
    def test_set_index_past_end_index(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            buff[5] = 6


    def test_load_from_string(self):
        with memory_buffer() as buff:
            expected = "This Is My String"
            buff.load_from_string(expected)
            self.assertEqual(expected, buff.to_string(), "Incorrect buffer content")

    def test_load_from_string__UTF16(self):
        with memory_buffer() as buff:
            expected = "This Is My String"
            buff.load_from_string(expected, "UTF-16")
            actual = buff.to_string("UTF-16")
            self.assertEqual(expected, actual, "Incorrect buffer content. Expected: {0}. Actual: {1}".format(expected, actual))

    @expect_exception(TypeError)
    def test_load_from_string_TypeError(self):
        with memory_buffer() as buff:
            buff.load_from_string(None)

    def test_to_list(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            for i in range(5):
                buff[i] = i
            self.assertListEqual([i for i in range(5)], buff.to_list(), "Incorrect list data returned")

    def test_to_string(self):
        with memory_buffer() as buff:
            expected = "This Is My String"
            buff.load_from_string(expected, "UTF-16")
            actual = buff.to_string("UTF-16")
            self.assertEqual(expected, actual, "Incorrect buffer content. Expected: {0}. Actual: {1}".format(expected, actual))
            
    def test___iter__(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            for i in range(5):
                buff[i] = i
            self.assertListEqual( [0, 1, 2, 3, 4], [x for x in buff], "Incorrect data returned")


    def test_set(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            for i in range(5):
                buff[i] = 0
            
            buff.set(0, [1])

            self.assertListEqual(
                [1, 0, 0, 0, 0],
                [x for x in buff],
                "Incorrect list data returned"
            )

    def test_set_full_copy(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            for i in range(5):
                buff[i] = 0
            
            buff.set(0, [1, 2, 3, 4, 5])

            self.assertListEqual(
                [x for x in range(1, 6)],
                [x for x in buff],
                "Incorrect list data returned"
            )

    def test_set_index_testing(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            for i in range(5):
                buff[i] = 0
            
            buff.set(1, [1, 2, 3, 4])

            self.assertListEqual(
                [x for x in range(5)],
                [x for x in buff],
                "Incorrect list data returned"
            )

    def test_set_index_offset(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            for i in range(5):
                buff[i] = 0
            
            buff.set(1, [1, 2, 3, 4], 1)

            self.assertListEqual(
                [0, 2, 3, 4, 0],
                [x for x in buff],
                "Incorrect list data returned. Expected: {0}, Actual: {1}".format([x for x in range(1,5)],[x for x in buff])
            )

    def test_set_index_count(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            for i in range(5):
                buff[i] = 0
            
            buff.set(1, [1, 2, 3, 4], count = 3)

            self.assertListEqual(
                [0, 1, 2, 3, 0],
                [x for x in buff],
                "Incorrect list data returned. Expected: {0}, Actual: {1}".format([x for x in range(1,5)],[x for x in buff])
            )

    def test_set_index_offset_count(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            for i in range(5):
                buff[i] = 0
            
            buff.set(1, [1, 2, 3, 4], 1, 3)

            self.assertListEqual(
                [0, 2, 3, 4, 0],
                [x for x in buff],
                "Incorrect list data returned. Expected: {0}, Actual: {1}".format([x for x in range(1,5)],[x for x in buff])
            )

    def test_set_index_offset_count(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            for i in range(5):
                buff[i] = 0
            
            buff.set(0, [1, 2, 3, 4, 5], 0, 5)

            self.assertListEqual(
                [1, 2, 3, 4, 5],
                [x for x in buff],
                "Incorrect list data returned. Expected: {0}, Actual: {1}".format([x for x in range(1,5)],[x for x in buff])
            )

    @expect_exception(ValueError)
    def test_set_index_offset_count_invalid_combo(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            for i in range(5):
                buff[i] = 0
            
            buff.set(0, [1, 2, 3, 4, 5], 1, 5)

            self.assertListEqual(
                [1, 2, 3, 4, 5],
                [x for x in buff],
                "Incorrect list data returned. Expected: {0}, Actual: {1}".format([x for x in range(1,5)],[x for x in buff])
            )
    @expect_exception(IndexError)
    def test_set_invalid_index_neg(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            buff.set(-1, [1, 2, 3, 4], 1, 3)

    @expect_exception(IndexError)
    def test_set_invalid_index_past_end(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            buff.set(5, [1, 2, 3, 4], 1, 3)

    @expect_exception(IndexError)
    def test_set_invalid_offset_neg(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            buff.set(0, [1, 2, 3, 4], -1, 3)

    @expect_exception(IndexError)
    def test_set_invalid_offset_past_end(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            buff.set(0, [1, 2, 3, 4], -1, 3)

    @expect_exception(ValueError)
    def test_set_invalid_count_neg(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            buff.set(0, [1, 2, 3, 4], 1, -1)

    @expect_exception(ValueError)
    def test_set_invalid_count_past_end(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            buff.set(0, [1, 2, 3, 4], 1, 5)


    def test_zero_out(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            buff.set(0, [0, 1, 2, 3, 4])
            buff.zero_out()
            for i in range(5):
                self.assertEqual(0, buff[i], "Incorrect buffer value at index: {0}. Value: {1}".format(i, buff[i]))


    def test_zero_out_index(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            buff.set(0, [8, 1, 2, 3, 4])
            buff.zero_out(1)
            self.assertEqual(8, buff[0], "Incorrect buffer value at index: {0}. Value: {1}".format(0, buff[0]))
            for i in range(1, 5):
                self.assertEqual(0, buff[i], "Incorrect buffer value at index: {0}. Value: {1}".format(i, buff[i]))

    @expect_exception(IndexError)
    def test_zero_out_index_neg(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            buff.set(0, [8, 1, 2, 3, 4])
            buff.zero_out(-1)

    @expect_exception(IndexError)
    def test_zero_out_index_past_end(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            buff.set(0, [8, 1, 2, 3, 4])
            buff.zero_out(5)

    def test_zero_out_count(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            buff.set(0, [8, 1, 2, 3, 4])
            buff.zero_out(count = 3)
            for i in range(3):
                self.assertEqual(0, buff[i], "Incorrect buffer value at index: {0}. Value: {1}".format(i, buff[i]))
            self.assertEqual(3, buff[3], "Incorrect buffer value at index: {0}. Value: {1}".format(3, buff[3]))
            self.assertEqual(4, buff[4], "Incorrect buffer value at index: {0}. Value: {1}".format(4, buff[4]))

    @expect_exception(ValueError)
    def test_zero_out_count_neg(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            buff.set(0, [8, 1, 2, 3, 4])
            buff.zero_out(count = -3)

    @expect_exception(ValueError)
    def test_zero_out_count_past_end(self):
        with memory_buffer() as buff:
            buff.allocate(5)
            buff.set(0, [8, 1, 2, 3, 4])
            buff.zero_out(count = 5)