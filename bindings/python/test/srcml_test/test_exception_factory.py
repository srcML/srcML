##
# @file test_exception_factory.py
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

from srcml import *
from srcml.exceptions import *

class test_exception_factory(unittest.TestCase):

    def setUp(self):
        pass
    
    def tearDown(self):
        pass
        
    def test_exception_factory__srcml_error(self):
        pass
        # buff = memory_buffer()
        # self.assertIsNotNone(buff._buff, "Didn't find private member _buff")
        # self.assertIsNotNone(buff._size, "Didn't find private member _size")
        # self.assertEqual(buff._size.value, 0, "Incorrect size")
        # self.assertEqual(buff._buff.value, None, "Incorrect buffer data.")
        # buff = None


    def test_exception_factory__srcml_invalid_argument(self):
        did_throw = False
        try:
            get_srcml_exception(bindings.STATUS_ERROR, "", "")
        except srcml_error as e:
            did_throw = True
        self.assertTrue(did_throw, "Incorrect exception thrown" )

    def test_exception_factory__srcml_invalid_input(self):
        did_throw = False
        try:
            get_srcml_exception(bindings.STATUS_INVALID_INPUT, "", "")
        except srcml_invalid_input as e:
            did_throw = True
        self.assertTrue(did_throw, "Incorrect exception thrown" )

    def test_exception_factory__srcml_invalid_io_operation(self):
        did_throw = False
        try:
            get_srcml_exception(bindings.STATUS_INVALID_IO_OPERATION, "", "")
        except srcml_invalid_io_operation as e:
            did_throw = True
        self.assertTrue(did_throw, "Incorrect exception thrown" )

    def test_exception_factory__srcml_io_error(self):
        did_throw = False
        try:
            get_srcml_exception(bindings.STATUS_IO_ERROR, "", "")
        except srcml_io_error as e:
            did_throw = True
        self.assertTrue(did_throw, "Incorrect exception thrown" )

    def test_exception_factory__srcml_uninitialized_unit(self):
        did_throw = False
        try:
            get_srcml_exception(bindings.STATUS_UNINITIALIZED_UNIT, "", "")
        except srcml_uninitialized_unit as e:
            did_throw = True
        self.assertTrue(did_throw, "Incorrect exception thrown" )

    def test_exception_factory__srcml_language_not_set(self):
        did_throw = False
        try:
            get_srcml_exception(bindings.STATUS_UNSET_LANGUAGE, "", "")
        except srcml_language_not_set as e:
            did_throw = True
        self.assertTrue(did_throw, "Incorrect exception thrown" )

    def test_exception_factory__srcml_no_transformation(self):
        did_throw = False
        try:
            get_srcml_exception(bindings.STATUS_NO_TRANSFORMATION, "", "")
        except srcml_no_transformation as e:
            did_throw = True
        self.assertTrue(did_throw, "Incorrect exception thrown" )