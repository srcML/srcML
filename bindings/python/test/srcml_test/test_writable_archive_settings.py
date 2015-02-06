##
# @file test_writable_archive_settings.py
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

from srcml import writable_archive_settings, writable_archive_xml_namespace_settings
from srcml.exceptions import *
import os, unittest, ctypes
from testlib import expect_exception

class test_writable_archive_settings(unittest.TestCase):

    def setUp(self):
        pass
    
    def tearDown(self):
        pass


    def test_constructor_default(self):
        subject = writable_archive_settings()
        self.assertIsNone(subject.filename, "Incorrect default value")
        self.assertIsNone(subject.directory, "Incorrect default value")
        self.assertIsNone(subject.default_language, "Incorrect default value")
        self.assertEqual(8, subject.tab_stop, "Incorrect default value")
        self.assertIsNone(subject.version, "Incorrect default value")
        self.assertEqual(subject.default_parser_options, subject.parser_options, "Incorrect default value")
        self.assertIsNone(subject.xml_encoding, "Incorrect default value")
        self.assertIsNone(subject.src_encoding, "Incorrect default value")
        self.assertIsInstance(subject.macros, dict, "Incorrect default value")
        self.assertEqual(0, len(subject.macros), "Incorrect default value")
        self.assertIsInstance(subject.xml_namespaces, writable_archive_xml_namespace_settings, "Incorrect default value")
        self.assertEqual(0, len(subject.xml_namespaces), "Incorrect default value")
        self.assertIsNone(subject.processing_instruction, "Incorrect default value")


    def test_constructor_kwargs(self):
        filename_data = "filename"
        directory_data = "directory"
        language_data = "C++"
        tab_stop_data = 50
        version_data = "version"
        xml_encoding_data ="UTF-8"
        src_encoding_data = "UTF-16",
        parser_options_data = 12345
        macro_data = {"x":"y", "a":"b"}
        xml_ns_data = [("prefix1","uri1"), ("prefix2","uri2")]
        pi_data = ("target", "data")
        subject = writable_archive_settings(
            filename=filename_data,
            directory=directory_data,
            default_language=language_data,
            tab_stop=tab_stop_data,
            version=version_data,
            parser_options=parser_options_data,
            xml_encoding=xml_encoding_data,
            src_encoding=src_encoding_data,
            macros=macro_data,
            xml_namespaces=xml_ns_data,
            processing_instruction=pi_data
        )

        self.assertEqual(filename_data, subject.filename, "Incorrect value")
        self.assertEqual(directory_data, subject.directory, "Incorrect value")
        self.assertEqual(language_data, subject.default_language, "Incorrect value")
        self.assertEqual(tab_stop_data, subject.tab_stop, "Incorrect value")
        self.assertEqual(version_data, subject.version, "Incorrect value")
        self.assertEqual(parser_options_data, subject.parser_options, "Incorrect value")
        self.assertEqual(xml_encoding_data, subject.xml_encoding, "Incorrect value")
        self.assertEqual(src_encoding_data, subject.src_encoding, "Incorrect value")

        self.assertEqual(2, len(subject.macros), "Incorrect value")
        for macro_pair in macro_data.iteritems():
            self.assertTrue(macro_pair[0] in subject.macros, "Didn't locate expected macro data. Token: {0} Type: {1}".format(*macro_pair))
            self.assertEqual(macro_pair[1], subject.macros[macro_pair[0]], "Didn't locate expected macro type. Token: {0} Type: {1}".format(*macro_pair))

        self.assertEqual(2, len(subject.xml_namespaces), "Incorrect value")
        for ns_pair in xml_ns_data:
            self.assertTrue(ns_pair in subject.xml_namespaces, "Didn't locate expected namespace: Prefix: {0} URI: {1}".format(*ns_pair))

        self.assertTupleEqual(pi_data, subject.processing_instruction, "Incorrect value")

    def test_filename(self):
        subject = writable_archive_settings()
        self.assertIsNone(subject.filename, "Incorrect default value")
        expected = "filename.txt"
        subject.filename = expected
        self.assertEqual(expected, subject.filename, "Incorrect value")


    def test_directory(self):
        subject = writable_archive_settings()
        self.assertIsNone(subject.directory, "Incorrect default value")
        expected = "foo/bar/"
        subject.directory = expected
        self.assertEqual(expected, subject.directory, "Incorrect value")

    def test_default_language(self):
        subject = writable_archive_settings()
        self.assertIsNone(subject.default_language, "Incorrect default value")
        expected = "foo/bar/"
        subject.default_language = expected
        self.assertEqual(expected, subject.default_language, "Incorrect value")