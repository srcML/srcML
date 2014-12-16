##
# @file test_unit.py
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

import srcml, os, unittest, ctypes, StringIO

class TestUnit(unittest.TestCase):

    def setUp(self):
        pass
    
    def tearDown(self):
        pass

    def test_create_unit(self):
        archive = srcml.archive()
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        self.assertTrue(isinstance(unit, srcml.unit), "Incorrect result type.")
        unit = None
        archive = None

    def test_unit_encoding(self):
        archive = srcml.archive()
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        expected = "Something"
        self.assertIsNone(unit.encoding, "Encoding set when it shouldn't be.")
        unit.encoding = expected
        self.assertEqual(unit.encoding, expected, "Encoding has incorrect value..")
        unit = None
        archive = None

    def test_unit_filename(self):
        archive = srcml.archive()
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        expected = "Something"
        self.assertIsNone(unit.filename, "filename set when it shouldn't be.")
        unit.filename = expected
        self.assertEqual(unit.filename, expected, "filename has incorrect value..")
        unit = None
        archive = None

    def test_unit_directory(self):
        archive = srcml.archive()
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        expected = "Something"
        self.assertIsNone(unit.directory, "directory set when it shouldn't be.")
        unit.directory = expected
        self.assertEqual(unit.directory, expected, "directory has incorrect value..")
        unit = None
        archive = None

    def test_unit_version(self):
        archive = srcml.archive()
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        expected = "Something"
        self.assertIsNone(unit.version, "version set when it shouldn't be.")
        unit.version = expected
        self.assertEqual(unit.version, expected, "version has incorrect value..")
        unit = None
        archive = None

    def test_unit_timestamp(self):
        archive = srcml.archive()
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        expected = "Something"
        self.assertIsNone(unit.timestamp, "timestamp set when it shouldn't be.")
        unit.timestamp = expected
        self.assertEqual(unit.timestamp, expected, "timestamp has incorrect value..")
        unit = None
        archive = None

    def test_unit_hash(self):
        archive = srcml.archive()
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        expected = "Something"
        self.assertIsNone(unit.hash, "hash set when it shouldn't be.")
        unit.hash = expected
        self.assertEqual(unit.hash, expected, "hash has incorrect value..")
        unit = None
        archive = None

    def test_unit_language(self):
        archive = srcml.archive()
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        expected = srcml.LANGUAGE_CXX
        self.assertIsNone(unit.language, "language set when it shouldn't be.")
        unit.language = expected
        self.assertEqual(unit.language, expected, "language has incorrect value..")
        unit = None
        archive = None

    def test_unit_revision(self):
        archive = srcml.archive()
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        self.assertIsNotNone(unit.srcml_revision(), "Didn't get a revision number.")
        unit = None
        archive = None


    test_code = """
#include <iostream>
int main() {
    std::cout << "hello world" << std::endl;
    return 0;
}
"""

    # Testing Parse
    def test_unit_parse_str(self):
        archive = srcml.archive()
        outputStringBuffer = StringIO.StringIO()
        archive.open_write(stream=outputStringBuffer, close_stream=False)
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        self.assertIsNotNone(unit.srcml_revision(), "Didn't get a revision number.")
        unit.language = srcml.LANGUAGE_CXX
        unit.parse(TestUnit.test_code)
        archive.write_unit(unit)
        archive.close()
        self.assertTrue(len(outputStringBuffer.getvalue()) > 0, "Didn't get any output.")

    def test_unit_parse_str_int(self):
        archive = srcml.archive()
        outputStringBuffer = StringIO.StringIO()
        archive.open_write(stream=outputStringBuffer, close_stream=False)
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        self.assertIsNotNone(unit.srcml_revision(), "Didn't get a revision number.")
        unit.language = srcml.LANGUAGE_CXX
        unit.parse(TestUnit.test_code, 94)
        archive.write_unit(unit)
        archive.close()
        self.assertTrue(len(outputStringBuffer.getvalue()) > 0, "Didn't get any output.")

    def test_unit_parse_strlist(self):
        archive = srcml.archive()
        outputStringBuffer = StringIO.StringIO()
        archive.open_write(stream=outputStringBuffer, close_stream=False)
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        self.assertIsNotNone(unit.srcml_revision(), "Didn't get a revision number.")
        unit.language = srcml.LANGUAGE_CXX
        unit.parse(TestUnit.test_code.split("\n"))
        archive.write_unit(unit)
        archive.close()
        self.assertTrue(len(outputStringBuffer.getvalue()) > 0, "Didn't get any output.")

    def test_unit_parse_filename(self):
        archive = srcml.archive()
        output_filename = "test_unit_parse_filename.cpp"
        strm = open(output_filename, "w")
        strm.write(TestUnit.test_code)
        strm.close()
        outputStringBuffer = StringIO.StringIO()
        archive.open_write(stream=outputStringBuffer, close_stream=False)
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        self.assertIsNotNone(unit.srcml_revision(), "Didn't get a revision number.")
        unit.parse(filename=output_filename)
        archive.write_unit(unit)
        archive.close()
        self.assertTrue(len(outputStringBuffer.getvalue()) > 0, "Didn't get any output.")
        os.remove(output_filename)

    def test_unit_parse_memory(self):
        archive = srcml.archive()
        outputStringBuffer = StringIO.StringIO()
        archive.open_write(stream=outputStringBuffer, close_stream=False)
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        self.assertIsNotNone(unit.srcml_revision(), "Didn't get a revision number.")
        mem_buff = srcml.memory_buffer()
        mem_buff._buff = ctypes.create_string_buffer(TestUnit.test_code)
        mem_buff._size = len(TestUnit.test_code)
        unit.language=srcml.LANGUAGE_CXX
        unit.parse(buff=mem_buff)
        archive.write_unit(unit)
        archive.close()
        mem_buff._buff = ctypes.c_char_p()


    # Testing unparse
    def test_unit_unparse_str(self):
        archive = srcml.archive()
        outputStringBuffer = StringIO.StringIO()
        archive.open_write(stream=outputStringBuffer, close_stream=False)
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        self.assertIsNotNone(unit.srcml_revision(), "Didn't get a revision number.")
        unit.language = srcml.LANGUAGE_CXX
        unit.parse(TestUnit.test_code)
        archive.write_unit(unit)
        archive.close()
        self.assertTrue(len(outputStringBuffer.getvalue()) > 0, "Didn't get any output.")

        rarchive = srcml.archive()
        rarchive.open_read(xml=outputStringBuffer.getvalue())
        runit = rarchive.read_unit()
        code = runit.unparse()
        self.assertEqual(code, TestUnit.test_code)
        rarchive.close()

    def test_unit_unparse_filename(self):
        archive = srcml.archive()
        outputStringBuffer = StringIO.StringIO()
        archive.open_write(stream=outputStringBuffer, close_stream=False)
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        self.assertIsNotNone(unit.srcml_revision(), "Didn't get a revision number.")
        unit.language = srcml.LANGUAGE_CXX
        unit.parse(TestUnit.test_code)
        archive.write_unit(unit)
        archive.close()
        self.assertTrue(len(outputStringBuffer.getvalue()) > 0, "Didn't get any output.")

        data_file_name = "test_unit_unparse_filename.cpp"
        rarchive = srcml.archive()
        rarchive.open_read(xml=outputStringBuffer.getvalue())
        runit = rarchive.read_unit()
        runit.unparse(filename=data_file_name)
        code = "".join(open(data_file_name,"r").readlines())
        self.assertEqual(code, TestUnit.test_code)
        rarchive.close()
        os.remove(data_file_name)

    def test_unit_unparse_memory(self):
        archive = srcml.archive()
        outputStringBuffer = StringIO.StringIO()
        archive.open_write(stream=outputStringBuffer, close_stream=False)
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        self.assertIsNotNone(unit.srcml_revision(), "Didn't get a revision number.")
        unit.language = srcml.LANGUAGE_CXX
        unit.parse(TestUnit.test_code)
        archive.write_unit(unit)
        archive.close()
        self.assertTrue(len(outputStringBuffer.getvalue()) > 0, "Didn't get any output.")

        mem_buff = srcml.memory_buffer()
        rarchive = srcml.archive()
        rarchive.open_read(xml=outputStringBuffer.getvalue())
        runit = rarchive.read_unit()
        runit.unparse(buff=mem_buff)
        code = str(mem_buff)
        self.assertEqual(code, TestUnit.test_code, "Mismatched code and test code. Code:\"{0}\" Expected: \"{1}\"".format(code, TestUnit.test_code))
        rarchive.close()


    def test_unit_write_unit(self):
        archive = srcml.archive()
        outputStringBuffer = StringIO.StringIO()
        archive.open_write(stream=outputStringBuffer, close_stream=False)
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        self.assertIsNotNone(unit.srcml_revision(), "Didn't get a revision number.")
        unit.language = srcml.LANGUAGE_CXX
        unit.write_start_unit()
        # unit.write_string("blah blah blah")
        unit.write_end_unit()

        archive.write_unit(unit)
        archive.close()
        self.assertTrue(len(outputStringBuffer.getvalue()) > 0, "Didn't get any output.")

        rarchive = srcml.archive()
        rarchive.open_read(xml=outputStringBuffer.getvalue())
        runit = rarchive.read_unit()
        code = runit.xml()
        self.assertTrue(len(code) > 0, "Incorrect unit content. Code:\"{0}\"".format(code))
        rarchive.close()


    def test_unit_write_string(self):
        content_str = "my content str"
        archive = srcml.archive()
        outputStringBuffer = StringIO.StringIO()
        archive.open_write(stream=outputStringBuffer, close_stream=False)
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        self.assertIsNotNone(unit.srcml_revision(), "Didn't get a revision number.")
        unit.language = srcml.LANGUAGE_CXX
        unit.write_start_unit()
        unit.write_string(content_str)
        unit.write_end_unit()

        archive.write_unit(unit)
        archive.close()
        self.assertTrue(len(outputStringBuffer.getvalue()) > 0, "Didn't get any output.")

        rarchive = srcml.archive()
        rarchive.open_read(xml=outputStringBuffer.getvalue())
        runit = rarchive.read_unit()
        code = runit.unparse()
        self.assertEqual(code, content_str, "Incorrect unit content. Code:\"{0}\"".format(code))
        rarchive.close()


    def test_unit_write_element(self):
        content_str = "my content str"
        archive = srcml.archive()
        outputStringBuffer = StringIO.StringIO()
        archive.open_write(stream=outputStringBuffer, close_stream=False)
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        self.assertIsNotNone(unit.srcml_revision(), "Didn't get a revision number.")
        unit.language = srcml.LANGUAGE_CXX
        unit.write_start_unit()
        unit.write_start_element(None, "aardvark", None)
        unit.write_string(content_str)
        unit.write_end_unit()

        archive.write_unit(unit)
        archive.close()
        self.assertTrue(len(outputStringBuffer.getvalue()) > 0, "Didn't get any output.")

        rarchive = srcml.archive()
        rarchive.open_read(xml=outputStringBuffer.getvalue())
        runit = rarchive.read_unit()
        code = runit.xml()
        self.assertTrue(code.find("aardvark") != -1, "Incorrect unit content. Code:\"{0}\"".format(code))
        rarchive.close()

    def test_unit_write_namespace(self):
        content_str = "my content str"
        archive = srcml.archive()
        outputStringBuffer = StringIO.StringIO()
        archive.open_write(stream=outputStringBuffer, close_stream=False)
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        self.assertIsNotNone(unit.srcml_revision(), "Didn't get a revision number.")
        unit.language = srcml.LANGUAGE_CXX
        unit.write_start_unit()
        unit.write_start_element("", "aardvark")
        unit.write_namespace("banana", "http://banana.com")
        unit.write_string(content_str)
        unit.write_end_element()
        unit.write_end_unit()

        archive.write_unit(unit)
        archive.close()
        self.assertTrue(len(outputStringBuffer.getvalue()) > 0, "Didn't get any output.")

        rarchive = srcml.archive()
        rarchive.open_read(xml=outputStringBuffer.getvalue())
        runit = rarchive.read_unit()
        code = runit.xml()
        self.assertTrue(code.find("banana") != -1, "Incorrect unit content. Code:\"{0}\"".format(code))
        rarchive.close()

    def test_unit_write_attribute(self):
        content_str = "my content str"
        archive = srcml.archive()
        outputStringBuffer = StringIO.StringIO()
        archive.open_write(stream=outputStringBuffer, close_stream=False)
        unit = archive.create_unit()
        self.assertIsNotNone(unit, "Didn't get a unit.")
        self.assertIsNotNone(unit.srcml_revision(), "Didn't get a revision number.")
        unit.language = srcml.LANGUAGE_CXX
        unit.write_start_unit()
        unit.write_start_element("", "aardvark")
        unit.write_attribute("", "waffles", "waffleData")
        unit.write_string(content_str)
        unit.write_end_element()
        unit.write_end_unit()

        archive.write_unit(unit)
        archive.close()
        self.assertTrue(len(outputStringBuffer.getvalue()) > 0, "Didn't get any output.")

        rarchive = srcml.archive()
        rarchive.open_read(xml=outputStringBuffer.getvalue())
        runit = rarchive.read_unit()
        code = runit.xml()
        # print runit.xml()
        self.assertTrue(code.find("waffles") != -1, "Incorrect unit content. Code:\"{0}\"".format(code))
        rarchive.close()

