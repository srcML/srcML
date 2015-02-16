##
# @file test_readable_archive.py
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

class test_readable_archive(unittest.TestCase):

    @cleanup_files("test_readable_archive__open_filename.cpp.xml")
    def test_readable_archive_contructor_filename(self):
        test_filename = "test_readable_archive__open_filename.cpp.xml"
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), filename=test_filename) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(), filename=test_filename) as archive_reader:
            unit_counter = 0
            for unit in archive_reader:
                unit_counter += 1
            self.assertEqual(1, unit_counter, "Incorrect number of units read")

    def test_readable_archive_constructor_stream(self):
        output_strm = StringIO.StringIO()
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), stream=output_strm, close_stream=False) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)
        input_stream = StringIO.StringIO(output_strm.getvalue())
        with readable_archive(readable_archive_settings(), stream=input_stream) as archive_reader:
            unit_counter = 0
            for unit in archive_reader:
                unit_counter += 1
            self.assertEqual(1, unit_counter, "Incorrect number of units read")


    def test_readable_archive_constructor_memory_buffer(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            unit_counter = 0
            for unit in archive_reader:
                unit_counter += 1
            self.assertEqual(1, unit_counter, "Incorrect number of units read")


    def test_readable_archive_constructor_read_context(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        reader = read_tester(str(mem_buffer))
        with readable_archive(readable_archive_settings(), context=reader) as archive_reader:
            unit_counter = 0
            for unit in archive_reader:
                unit_counter += 1
            self.assertEqual(1, unit_counter, "Incorrect number of units read")


    def test_readable_archive_constructor_read_context_funcs(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        reader = read_tester(str(mem_buffer))
        with readable_archive(readable_archive_settings(), context=reader, read=reader.read, close=reader.close) as archive_reader:
            unit_counter = 0
            for unit in archive_reader:
                unit_counter += 1
            self.assertEqual(1, unit_counter, "Incorrect number of units read")


    def test_readable_archive_constructor_read_xml_string(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        xml = str(mem_buffer)
        with readable_archive(readable_archive_settings(), xml=xml) as archive_reader:
            unit_counter = 0
            for unit in archive_reader:
                unit_counter += 1
            self.assertEqual(1, unit_counter, "Incorrect number of units read")


    def test_readable_archive_constructor_read_xml_string(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        xml = str(mem_buffer)
        with readable_archive(readable_archive_settings(), xml=xml) as archive_reader:
            unit_counter = 0
            for unit in archive_reader:
                unit_counter += 1
            self.assertEqual(1, unit_counter, "Incorrect number of units read")


    def test_xml_encoding_implicit(self):
        mem_buffer = memory_buffer()
        expected_encoding = "UTF-16"
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX, xml_encoding=expected_encoding), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            self.assertEqual(expected_encoding, archive_reader.xml_encoding, "Incorrect encoding value.")
            unit_counter = 0
            for unit in archive_reader:
                unit_counter += 1
            self.assertEqual(1, unit_counter, "Incorrect number of units read")


    def test_xml_encoding_explicit(self):
        mem_buffer = memory_buffer()
        expected_encoding = "UTF-16"
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX, xml_encoding=expected_encoding), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(xml_encoding=expected_encoding), buffer=mem_buffer) as archive_reader:
            self.assertEqual(expected_encoding, archive_reader.xml_encoding, "Incorrect encoding value.")
            unit_counter = 0
            for unit in archive_reader:
                unit_counter += 1
            self.assertEqual(1, unit_counter, "Incorrect number of units read")



    def test_src_encoding(self):
        mem_buffer = memory_buffer()
        expected_encoding = "UTF-16"
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(src_encoding=expected_encoding), buffer=mem_buffer) as archive_reader:
            self.assertEqual(expected_encoding, archive_reader.src_encoding, "Incorrect encoding value.")
            unit_counter = 0
            for unit in archive_reader:
                unit_counter += 1
            self.assertEqual(1, unit_counter, "Incorrect number of units read")

    def test_version(self):
        mem_buffer = memory_buffer()
        expected_version = "value"
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX, version=expected_version), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            self.assertEqual(expected_version, archive_reader.version, "Incorrect value.")


    def test_filename(self):
        mem_buffer = memory_buffer()
        expected_filename = "value"
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX, filename=expected_filename), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            self.assertEqual(expected_filename, archive_reader.filename, "Incorrect value.")


    def test_directory(self):
        mem_buffer = memory_buffer()
        expected_directory = "value"
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX, directory=expected_directory), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            self.assertEqual(expected_directory, archive_reader.directory, "Incorrect value.")


    def test_revision(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            self.assertIsNotNone(archive_reader.revision, "Incorrect value.")

    def test_parser_options(self):
        mem_buffer = memory_buffer()
        expected_directory = "value"
        writable_arch_settings = writable_archive_settings(default_language=LANGUAGE_CXX)
        writable_arch_settings.parser_options ^= OPTION_ARCHIVE
        default_settings = writable_arch_settings.parser_options
        with writable_archive(writable_arch_settings, buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            self.assertEqual(default_settings, archive_reader.parser_options, "Incorrect value.")


    def test_processing_instruction(self):
        mem_buffer = memory_buffer()
        expected_processing_instruction = ("target", "data")
        writable_arch_settings = writable_archive_settings(default_language=LANGUAGE_CXX, processing_instruction=expected_processing_instruction)
        with writable_archive(writable_arch_settings, buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            self.assertEqual(expected_processing_instruction, archive_reader.processing_instruction, "Incorrect value.")


    def test_xml_namespaces(self):
        mem_buffer = memory_buffer()
        expected_processing_instruction = ("target", "data")
        writable_arch_settings = writable_archive_settings(default_language=LANGUAGE_CXX, processing_instruction=expected_processing_instruction)
        with writable_archive(writable_arch_settings, buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            self.assertEqual(expected_processing_instruction, archive_reader.processing_instruction, "Incorrect value.")