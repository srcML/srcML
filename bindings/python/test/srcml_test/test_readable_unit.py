##
# @file test_readable_unit.py
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
from srcml.exceptions import *
import os, unittest
from testlib import *

def write_test_func(ctxt, buffer, size):
    ctxt.write(buffer)
    return size

def close_test_func(ctxt):
    return 0

class test_readable_unit(unittest.TestCase):
    
    # def test_src_encoding(self):
    #     mem_buffer = memory_buffer()
    #     with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
    #         u = archive_writer.create_unit()
    #         u.parse(source_code=test_source_code_data)
    #         archive_writer.write(u)

    #     with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
    #         u = archive_reader.read()
    #         self.assertIsNotNone(u, "Didn't correctly read unit")
    #         expected_encoding = "UTF-16"
    #         u.src_encoding = expected_encoding
    #         self.assertEqual(expected_encoding, u.src_encoding, "Didn't receive correct source encoding")
    #         temp_str = u.unparse()
    #         print temp_str
    #         unit_code = temp_str.decode(expected_encoding)
    #         self.assertEqual(test_source_code_data, unit_code, "Didn't extract correctly decoding string.")


    def test_language(self):
        mem_buffer = memory_buffer()
        expected_language = LANGUAGE_CXX
        with writable_archive(writable_archive_settings(default_language=expected_language), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            u = archive_reader.read()
            self.assertIsNotNone(u, "Didn't correctly read unit")
            self.assertEqual(expected_language, u.language, "Incorrect language read from unit")

    def test_filename(self):
        mem_buffer = memory_buffer()
        expected_filename = "value"
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.filename = expected_filename
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            u = archive_reader.read()
            self.assertIsNotNone(u, "Didn't correctly read unit")
            self.assertEqual(expected_filename, u.filename, "didn't get correct value.")

    def test_directory(self):
        mem_buffer = memory_buffer()
        expected_directory = "value"
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.url = expected_directory
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            u = archive_reader.read()
            self.assertIsNotNone(u, "Didn't correctly read unit")
            self.assertEqual(expected_directory, u.url, "didn't get correct value.")

    def test_version(self):
        mem_buffer = memory_buffer()
        expected_version = "value"
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.version = expected_version
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            u = archive_reader.read()
            self.assertIsNotNone(u, "Didn't correctly read unit")
            self.assertEqual(expected_version, u.version, "didn't get correct value.")

    def test_timestamp(self):
        mem_buffer = memory_buffer()
        expected_timestamp = "value"
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.timestamp = expected_timestamp
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            u = archive_reader.read()
            self.assertIsNotNone(u, "Didn't correctly read unit")
            self.assertEqual(expected_timestamp, u.timestamp, "didn't get correct value.")

    def test_hash(self):
        mem_buffer = memory_buffer()
        expected_hash = "value"
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.hash = expected_hash
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            u = archive_reader.read()
            self.assertIsNotNone(u, "Didn't correctly read unit")
            self.assertEqual(expected_hash, u.hash, "didn't get correct value.")

    def test_revision(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            u = archive_reader.read()
            self.assertIsNotNone(u, "Didn't correctly read unit")
            self.assertIsNotNone(u.revision, "didn't get correct value.")

    @cleanup_files("test_readable_unt__unparse_filename.cpp.xml")
    def test_unparse_filename(self):
        test_file = "test_readable_unt__unparse_filename.cpp.xml"
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            u = archive_reader.read()
            u.unparse(filename=test_file)
        self.assertTrue(os.path.exists(test_file), "Didn't correctly extract file.")
        temp = "".join(open(test_file, "r").readlines())
        self.assertEqual(test_source_code_data, temp, "Didn't correctly read file. Expected: {0}. Actual: {1}".format(test_source_code_data, temp))

    def test_unparse_stream(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)
        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            str_stream = StringIO.StringIO()
            u = archive_reader.read()
            u.unparse(stream=str_stream, close_stream=False)
            self.assertEqual(test_source_code_data, str_stream.getvalue(), "Didn't get correct source code.Expected: {0}. Actual: {1}".format(test_source_code_data, str_stream.getvalue()))

    def test_unparse_io_callbacks(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)
        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            u = archive_reader.read()
            test_context = write_tester()
            u.unparse(context=test_context)
            self.assertEqual(test_source_code_data, test_context.buffer.getvalue(), "Didn't get correct source code.")

    def test_unparse_io_context(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)
        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            u = archive_reader.read()
            strm = StringIO.StringIO()
            u.unparse(context=strm, write=write_test_func, close=close_test_func)
            self.assertEqual(test_source_code_data, strm.getvalue(), "Didn't get correct source code.")

    def test_unparse_io_memory(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)
        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            u = archive_reader.read()
            temp_buffer = memory_buffer()
            u.unparse(buffer=temp_buffer)
            self.assertEqual(test_source_code_data, str(temp_buffer), "Didn't get correct source code.")

    def test_unparse_tostring(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)
        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            u = archive_reader.read()
            ret = u.unparse()
            self.assertEqual(test_source_code_data, ret, "Didn't get correct source code.")

    def test_to_standalone_xml(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)
        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            u = archive_reader.read()
            unit_xml = u.get_standalone_xml()
            self.assertIsNotNone(unit_xml, "Didn't get xml")

    def test_to_xml_fragment(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)
        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            u = archive_reader.read()
            unit_xml = u.get_xml_fragment()
            self.assertIsNotNone(unit_xml, "Didn't get xml")