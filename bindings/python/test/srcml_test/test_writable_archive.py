##
# @file test_writable_archive.py
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

def write_test_func(ctxt, buffer, size):
    ctxt.write(buffer)
    return size

def close_test_func(ctxt):
    return 0

class TestWritableArchive(unittest.TestCase):

    def setUp(self):
        pass
    
    def tearDown(self):
        pass

    @cleanup_files("test_constructor_filename.cpp.xml")
    def test_constructor_filename(self):
        outputFileName = "test_constructor_filename.cpp.xml"
        with writable_archive(writable_archive_settings(), filename=outputFileName) as archive_writer:
            pass
        self.assertTrue(os.path.exists(outputFileName))
        tree = et.parse(outputFileName)
        ret = tree.xpath("//src:unit", namespaces=dict(src="http://www.sdml.info/srcML/src"))
        self.assertEqual(1, len(ret), "Incorrect # of units returned")

    def test_constructor_stream(self):
        output_strm = StringIO.StringIO()
        with writable_archive(writable_archive_settings(), stream=output_strm, close_stream=False) as archive_writer:
            pass
        tree = et.XML(output_strm.getvalue())
        ret = tree.xpath("//src:unit", namespaces=dict(src="http://www.sdml.info/srcML/src"))
        self.assertEqual(1, len(ret), "Incorrect # of units returned")


    def test_constructor_memory_buffer(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            pass
        tree = et.XML(mem_buffer.to_string())
        ret = tree.xpath("//src:unit", namespaces=dict(src="http://www.sdml.info/srcML/src"))
        self.assertEqual(1, len(ret), "Incorrect # of units returned")

    @expect_exception(ValueError)
    def test_constructor_memory_buffer_invalid_args(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer, a="1") as archive_writer:
            pass

    class write_tester:
        def __init__(self):
            self.buffer = StringIO.StringIO()

        def write(self, input_buffer, size):
            self.buffer.write(input_buffer)
            return len(input_buffer)

        def close(self):
            return 0

    def test_constructor_context_class(self):
        ctxt = TestWritableArchive.write_tester()
        ctxt.buffer = StringIO.StringIO()
        with writable_archive(writable_archive_settings(), context=ctxt) as archive_writer:
            pass
        tree = et.XML(ctxt.buffer.getvalue())
        ret = tree.xpath("//src:unit", namespaces=dict(src="http://www.sdml.info/srcML/src"))
        self.assertEqual(1, len(ret), "Incorrect # of units returned")


    def test_constructor_context_functions(self):
        ctxt = TestWritableArchive.write_tester()
        temp_buffer = StringIO.StringIO()
        with writable_archive(writable_archive_settings(),
            context=temp_buffer,
            write= write_test_func,
            close=close_test_func) as archive_writer:
            pass
        tree = et.XML(temp_buffer.getvalue())
        ret = tree.xpath("//src:unit", namespaces=dict(src="http://www.sdml.info/srcML/src"))
        self.assertEqual(1, len(ret), "Incorrect # of units returned")



    def test_xml_encoding(self):
        mem_buffer = memory_buffer()
        expected = "UTF-8"
        with writable_archive(writable_archive_settings(xml_encoding=expected), buffer=mem_buffer) as archive_writer:
            self.assertEqual(expected, archive_writer.xml_encoding, "Incorrect value")

    def test_xml_encoding_default(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            self.assertIsNone(archive_writer.xml_encoding, "Incorrect default value")


    def test_src_encoding(self):
        mem_buffer = memory_buffer()
        expected = "UTF-8"
        with writable_archive(writable_archive_settings(src_encoding=expected), buffer=mem_buffer) as archive_writer:
            self.assertEqual(expected, archive_writer.src_encoding, "Incorrect value")

    def test_src_encoding_default(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            self.assertIsNone(archive_writer.src_encoding, "Incorrect default value")


    def test_default_language(self):
        mem_buffer = memory_buffer()
        expected = LANGUAGE_CXX
        with writable_archive(writable_archive_settings(default_language=expected), buffer=mem_buffer) as archive_writer:
            self.assertEqual(expected, archive_writer.default_language, "Incorrect value")

    def test_default_language_default(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            self.assertIsNone(archive_writer.default_language, "Incorrect default value")


    def test_filename(self):
        mem_buffer = memory_buffer()
        expected = "something"
        with writable_archive(writable_archive_settings(filename=expected), buffer=mem_buffer) as archive_writer:
            self.assertEqual(expected, archive_writer.filename, "Incorrect value")

    def test_filename_default(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            self.assertIsNone(archive_writer.filename, "Incorrect default value")


    def test_url(self):
        mem_buffer = memory_buffer()
        expected = "something"
        with writable_archive(writable_archive_settings(url=expected), buffer=mem_buffer) as archive_writer:
            self.assertEqual(expected, archive_writer.url, "Incorrect value")

    def test_url_default(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            self.assertIsNone(archive_writer.url, "Incorrect default value")


    def test_version(self):
        mem_buffer = memory_buffer()
        expected = "something"
        with writable_archive(writable_archive_settings(version=expected), buffer=mem_buffer) as archive_writer:
            self.assertEqual(expected, archive_writer.version, "Incorrect value")

    def test_version_default(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            self.assertIsNone(archive_writer.version, "Incorrect default value")


    def test_revision(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            self.assertIsNotNone(archive_writer.revision, "Incorrect value")


    def test_tab_stop(self):
        mem_buffer = memory_buffer()
        expected = 5
        with writable_archive(writable_archive_settings(tab_stop=expected), buffer=mem_buffer) as archive_writer:
            self.assertEqual(expected, archive_writer.tab_stop, "Incorrect value")

    def test_tab_stop_default(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            self.assertEqual(8, archive_writer.tab_stop, "Incorrect default value")


    def test_parser_options(self):
        mem_buffer = memory_buffer()
        expected = 123
        with writable_archive(writable_archive_settings(parser_options=expected), buffer=mem_buffer) as archive_writer:
            self.assertEqual(expected, archive_writer.parser_options, "Incorrect value")

    def test_parser_options_default(self):
        mem_buffer = memory_buffer()
        temp = writable_archive_settings()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            self.assertEqual(temp.default_parser_options, archive_writer.parser_options, "Incorrect default value")


    def test_processing_instruction(self):
        mem_buffer = memory_buffer()
        expected = ("target", "data")
        with writable_archive(writable_archive_settings(processing_instruction=expected), buffer=mem_buffer) as archive_writer:
            self.assertTupleEqual(expected, archive_writer.processing_instruction, "Incorrect value")

    def test_processing_instruction_default(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            self.assertIsNone(archive_writer.processing_instruction, "Incorrect default value")
    
    def test_xml_namespaces(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(
            xml_namespaces={
                "banana":"www.banana.com"
            }), buffer=mem_buffer) as archive_writer:

            self.assertIsNotNone(archive_writer.xml_namespaces, "Incorrect value")
            self.assertEqual(2, len(archive_writer.xml_namespaces), "Incorrect value")
            count = 0
            for ns in archive_writer.xml_namespaces:
                count += 1
            self.assertEqual(2, count, "Incorrect # of namespace visited")


    def test_macros(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(
            macros={
                "token":"type"
            }), buffer=mem_buffer) as archive_writer:

            self.assertIsNotNone(archive_writer.macros, "Incorrect value")
            self.assertEqual(1, len(archive_writer.macros), "Incorrect value")
            count = 0
            for ns in archive_writer.macros:
                count += 1
            self.assertEqual(1, count, "Incorrect # of macros visited")

    def test_register_and_check_file_extension(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            archive_writer.register_file_extension("xyz", LANGUAGE_CXX)
            self.assertEqual(LANGUAGE_CXX, archive_writer.check_file_extension("file.xyz"), "Incorrect language association")

    @expect_exception(TypeError)
    def test_register_file_extension_None_extension(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            archive_writer.register_file_extension(None, LANGUAGE_CXX)

    @expect_exception(TypeError)
    def test_register_file_extension_None_language(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            archive_writer.register_file_extension("xyz", None)

    @expect_exception(invalid_srcml_language)
    def test_register_file_extension_invalid_language(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            archive_writer.register_file_extension("xyz", "aardvark")
    
    @expect_exception(TypeError)
    def test_check_file_extension_None_extension(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            temp = archive_writer.check_file_extension(None)

            
    @expect_exception(KeyError)
    def test_check_file_extension_None_extension(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            temp = archive_writer.check_file_extension("aardvark")


    def test_write__writable_unit(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit(language=LANGUAGE_CXX)
            u.parse(source_code="""int main() {
    cout << "Hello World" << endl;
    return 0;
}""")       
            archive_writer.write(u)

        tree = et.XML(str(mem_buffer))
        ret = tree.xpath("/src:unit/src:unit", namespaces=dict(src="http://www.sdml.info/srcML/src"))
        self.assertEqual(1, len(ret), "Incorrect # of units returned")

    @expect_exception(TypeError)
    def test_write__None(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            archive_writer.write(None)

    def test_write__readable_unit(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit(language=LANGUAGE_CXX)
            u.parse(source_code="""int main() {
    cout << "Hello World" << endl;
    return 0;
}""")       
            archive_writer.write(u)

        secondary_buffer = memory_buffer()
        with readable_archive(readable_archive_settings(), buffer=mem_buffer) as archive_reader:
            with writable_archive(writable_archive_settings(), buffer=secondary_buffer) as archive_writer:
                u = archive_reader.read()
                archive_writer.write(u)
        tree = et.XML(str(secondary_buffer))
        ret = tree.xpath("/src:unit/src:unit", namespaces=dict(src="http://www.sdml.info/srcML/src"))
        self.assertEqual(1, len(ret), "Incorrect # of units returned")

    # def create_unit(self):
    #     raise NotImplementedError()
    #

    def test_create_unit(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            u = archive_writer.create_unit()
            self.assertIsNotNone(u, "Invalid unit returned")