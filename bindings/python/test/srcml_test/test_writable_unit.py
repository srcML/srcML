##
# @file test_writable_unit.py
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
from testlib import *
import unittest, os, StringIO
import lxml.etree as et

class test_writable_unit(unittest.TestCase):

    def setUp(self):
        pass
    
    def tearDown(self):
        pass


    def test_constructor_default(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            self.assertIsNotNone(u, "Invalid unit returned")
            self.assertIsNone(u.src_encoding, "Incorrect value")
            self.assertIsNone(u.language, "Incorrect value")
            self.assertIsNone(u.filename, "Incorrect value")
            self.assertIsNone(u.url, "Incorrect value")
            self.assertIsNone(u.version, "Incorrect value")
            self.assertIsNone(u.timestamp, "Incorrect value")
            self.assertIsNone(u.hash, "Incorrect value")
            self.assertIsNotNone(u.revision, "Incorrect value")


    def test_constructor_params_to_properties(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            expected_language = LANGUAGE_CXX
            expected_filename = "filename"
            expected_encoding = "UTF-8"
            expected_url = "url"
            expected_version = "version"
            expected_timestamp = "timestamp"
            expected_hash = "hash"

            u = archive_writer.create_unit(language=expected_language, src_encoding=expected_encoding, filename=expected_filename, url=expected_url, version=expected_version, timestamp=expected_timestamp, hash_of_unit=expected_hash)

            self.assertEqual(expected_encoding, u.src_encoding, "Incorrect value")
            self.assertEqual(expected_language, u.language, "Incorrect value")
            self.assertEqual(expected_filename, u.filename, "Incorrect value")
            self.assertEqual(expected_url, u.url, "Incorrect value")
            self.assertEqual(expected_version, u.version, "Incorrect value")
            self.assertEqual(expected_timestamp, u.timestamp, "Incorrect value")
            self.assertEqual(expected_hash, u.hash, "Incorrect value")

    def test_filename(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            expected = "something"
            self.assertIsNone(u.filename, "Incorrect default value")
            u.filename = expected
            self.assertEqual(expected, u.filename, "Incorrect value")

    def test_url(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            expected = "something"
            self.assertIsNone(u.url, "Incorrect default value")
            u.url = expected
            self.assertEqual(expected, u.url, "Incorrect value")

    def test_version(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            expected = "something"
            self.assertIsNone(u.version, "Incorrect default value")
            u.version = expected
            self.assertEqual(expected, u.version, "Incorrect value")

    def test_timestamp(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            expected = "something"
            self.assertIsNone(u.timestamp, "Incorrect default value")
            u.timestamp = expected
            self.assertEqual(expected, u.timestamp, "Incorrect value")

    def test_hash(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            expected = "something"
            self.assertIsNone(u.hash, "Incorrect default value")
            u.hash = expected
            self.assertEqual(expected, u.hash, "Incorrect value")


    def test_language(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            expected = LANGUAGE_CXX
            self.assertIsNone(u.language, "Incorrect default value")
            u.language = expected
            self.assertEqual(expected, u.language, "Incorrect value")

    @expect_exception(invalid_srcml_language)
    def test_language_invalid_language(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.language = "banana"


    def test_src_encoding(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            expected = "UTF-16"
            self.assertIsNone(u.src_encoding, "Incorrect default value")
            u.src_encoding = expected
            self.assertEqual(expected, u.src_encoding, "Incorrect value")

    @expect_exception(invalid_srcml_encoding)
    def test_src_encoding_invalid_srcml_encoding(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.src_encoding = "aardvarkz"

    @cleanup_files("writable_unit_test_parse_filename.cpp")
    def test_parse_filename(self):
        input_file = "writable_unit_test_parse_filename.cpp"
        output = open(input_file, "w")
        output.write(test_source_code_data)
        output.close()
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(filename=input_file)
            archive_writer.write(u)

        self.assertEqual(test_source_code_data, extract_text_from_xml(str(mem_buffer)), "Didn't get expected results")


    def test_parse_memory_buffer(self):
        mem_buffer = memory_buffer()
        temp_buffer = memory_buffer()
        temp_buffer.load_from_string(test_source_code_data)
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(buffer=temp_buffer)
            archive_writer.write(u)
        self.assertEqual(test_source_code_data, extract_text_from_xml(str(mem_buffer)), "Didn't get expected results")


    def test_parse_read_context(self):
        mem_buffer = memory_buffer()
        context = read_tester(test_source_code_data)
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(context=context)
            archive_writer.write(u)
        self.assertEqual(test_source_code_data, extract_text_from_xml(str(mem_buffer)), "Didn't get expected results")

    def test_get_xml_fragment(self):
        mem_buffer = memory_buffer()
        context = read_tester(test_source_code_data)
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(context=context)
            self.assertEqual(test_source_code_data, extract_text_from_xml(u.get_xml_fragment()), "Didn't get expected results")


    def test_get_standalone_xml(self):
        mem_buffer = memory_buffer()
        context = read_tester(test_source_code_data)
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(context=context)
            self.assertEqual(test_source_code_data, extract_text_from_xml(u.get_standalone_xml()), "Didn't get expected results")



