##
# @file test_archive_macros.py
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

from srcml import *
from testlib import expect_exception, cleanup_files
import unittest, os, StringIO
import lxml.etree as et

class TestArchiveMacros(unittest.TestCase):

    def setUp(self):
        pass
    
    def tearDown(self):
        pass

    def test_writable_archive_macros___len__(self):
        mem_buffer = memory_buffer()
        with writable_archive(
            writable_archive_settings(
                macros={
                    "token":"type",
                    "token1":"type2"
                }), buffer=mem_buffer) as archive_writer:

            self.assertIsNotNone(archive_writer.macros, "Incorrect value")
            self.assertEqual(2, len(archive_writer.macros), "Incorrect value")

    def test_writable_archive_macros___getitem__(self):
        mem_buffer = memory_buffer()
        with writable_archive(
            writable_archive_settings(
                macros={
                    "token":"type",
                    "token2":"type2"
                }), buffer=mem_buffer) as archive_writer:
            self.assertEqual(("token","type"), archive_writer.macros[1], "Didn't return correct macros: {0}".format(archive_writer.macros[1]))
            self.assertEqual(("token2","type2"), archive_writer.macros[0], "Didn't return correct macros: {0}".format(archive_writer.macros[0]))

    @expect_exception(TypeError)
    def test_writable_archive_macros___getitem___None(self):
        mem_buffer = memory_buffer()
        with writable_archive(
            writable_archive_settings(
                macros={
                    "token":"type",
                    "token2":"type2"
                }), buffer=mem_buffer) as archive_writer:
            x = archive_writer[None]

    @expect_exception(TypeError)
    def test_writable_archive_macros___getitem___non_int_or_long(self):
        mem_buffer = memory_buffer()
        with writable_archive(
            writable_archive_settings(
                macros={
                    "token":"type",
                    "token2":"type2"
                }), buffer=mem_buffer) as archive_writer:
            x = archive_writer.macros["waffles"]


    @expect_exception(IndexError)
    def test_writable_archive_macros___getitem___negative(self):
        mem_buffer = memory_buffer()
        with writable_archive(
            writable_archive_settings(
                macros={
                    "token":"type",
                    "token2":"type2"
                }), buffer=mem_buffer) as archive_writer:
            x = archive_writer.macros[-1]


    @expect_exception(IndexError)
    def test_writable_archive_macros___getitem___past_end(self):
        mem_buffer = memory_buffer()
        with writable_archive(
            writable_archive_settings(
                macros={
                    "token":"type",
                    "token2":"type2"
                }), buffer=mem_buffer) as archive_writer:
            x = archive_writer.macros[2]

    def test_writable_archive_macros___iter__(self):
        mem_buffer = memory_buffer()
        remaining_macros = dict({
                    "token":"type",
                    "token1":"type2"
                })
        with writable_archive(
            writable_archive_settings(
                macros={
                    "token":"type",
                    "token1":"type2"
                }), buffer=mem_buffer) as archive_writer:
            for macro in archive_writer.macros:
                self.assertTrue(macro[0] in remaining_macros, "Didn't locate correct macros")
                result = remaining_macros[macro[0]]
                self.assertEqual(result, macro[1], "Mismatched macros")
                del remaining_macros[macro[0]]


    def test_writable_archive_macros_token_to_type(self):
        mem_buffer = memory_buffer()
        with writable_archive(
            writable_archive_settings(
                macros={
                    "token":"type",
                    "token2":"type2"
                }), buffer=mem_buffer) as archive_writer:
            self.assertEqual("type", archive_writer.macros.token_to_type("token"), "Didn't return correct type for token")
            self.assertEqual("type2", archive_writer.macros.token_to_type("token2"), "Didn't return correct type for token")

    @expect_exception(TypeError)
    def test_writable_archive_macros_token_to_type__type_error(self):
        mem_buffer = memory_buffer()
        with writable_archive(
            writable_archive_settings(
                macros={
                    "token":"type",
                    "token2":"type2"
                }), buffer=mem_buffer) as archive_writer:
            x = archive_writer.macros.token_to_type(None)


    @expect_exception(KeyError)
    def test_writable_archive_macros_token_to_type__key_error(self):
        mem_buffer = memory_buffer()
        with writable_archive(
            writable_archive_settings(
                macros={
                    "token":"type",
                    "token1":"type2"
                }), buffer=mem_buffer) as archive_writer:
            x = archive_writer.macros.token_to_type("aardvark")


    def test_writable_archive_macros___contains__(self):
        mem_buffer = memory_buffer()
        with writable_archive(
            writable_archive_settings(
                macros={
                    "token":"type",
                    "token1":"type2"
                }), buffer=mem_buffer) as archive_writer:
            self.assertTrue(("token","type") in archive_writer.macros, "Didn't locate expected macro")
            self.assertFalse(("banana","type") in archive_writer.macros, "Didn't locate expected macro")
            self.assertFalse(None in archive_writer.macros, "Didn't locate expected macro")

    def test_writable_archive_macros_contains_token(self):
        mem_buffer = memory_buffer()
        with writable_archive(
            writable_archive_settings(
                macros={
                    "token":"type",
                    "token1":"type2"
                }), buffer=mem_buffer) as archive_writer:
            self.assertTrue(archive_writer.macros.contains_token("token"), "Didn't locate expected macro")
            self.assertFalse(archive_writer.macros.contains_token("banana"), "Didn't locate expected macro")
            self.assertFalse(archive_writer.macros.contains_token(None), "Didn't locate expected macro")

    def test_writable_archive_macros_contains_type(self):
        mem_buffer = memory_buffer()
        with writable_archive(
            writable_archive_settings(
                macros={
                    "token":"type",
                    "token1":"type2"
                }), buffer=mem_buffer) as archive_writer:
            self.assertTrue(archive_writer.macros.contains_type("type"), "Didn't locate expected macro")
            self.assertFalse(archive_writer.macros.contains_type("banana"), "Didn't locate expected macro")
            self.assertFalse(archive_writer.macros.contains_type(None), "Didn't locate expected macro")

        

