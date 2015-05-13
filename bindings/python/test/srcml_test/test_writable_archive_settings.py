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

from srcml import *
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
        self.assertIsNone(subject.url, "Incorrect default value")
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
        url_data = "url"
        language_data = "C++"
        tab_stop_data = 50
        version_data = "version"
        xml_encoding_data ="UTF-8"
        src_encoding_data = "UTF-16"
        parser_options_data = 12345
        macro_data = {"x":"y", "a":"b"}
        xml_ns_data = [("prefix1","uri1"), ("prefix2","uri2")]
        pi_data = ("target", "data")
        subject = writable_archive_settings(
            filename=filename_data,
            url=url_data,
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
        self.assertEqual(url_data, subject.url, "Incorrect value")
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


    def test_url(self):
        subject = writable_archive_settings()
        self.assertIsNone(subject.url, "Incorrect default value")
        expected = "foo/bar/"
        subject.url = expected
        self.assertEqual(expected, subject.url, "Incorrect value")

    def test_default_language(self):
        subject = writable_archive_settings()
        self.assertIsNone(subject.default_language, "Incorrect default value")
        expected = LANGUAGE_CXX
        subject.default_language = expected
        self.assertEqual(expected, subject.default_language, "Incorrect value")

    @expect_exception(invalid_srcml_language)
    def test_default_language_invalid_language(self):
        subject = writable_archive_settings()
        subject.default_language = "badLanguage"

    def test_tab_stop(self):
        subject = writable_archive_settings()
        self.assertEqual(8, subject.tab_stop, "Incorrect default value")
        expected = 100
        subject.tab_stop = expected
        self.assertEqual(expected, subject.tab_stop, "Incorrect value")

    def test_version(self):
        subject = writable_archive_settings()
        self.assertIsNone(subject.version, "Incorrect default value")
        expected = "thingy"
        subject.version = expected
        self.assertEqual(expected, subject.version, "Incorrect value")

    def test_parser_options(self):
        subject = writable_archive_settings()
        self.assertEqual(subject.default_parser_options, subject.parser_options, "Incorrect default value")
        expected = 123
        subject.parser_options = expected
        self.assertEqual(expected, subject.parser_options, "Incorrect value")


    def test_xml_encoding(self):
        subject = writable_archive_settings()
        self.assertIsNone(subject.xml_encoding, "Incorrect default value")
        expected = "UTF-8"
        subject.xml_encoding = expected
        self.assertEqual(expected, subject.xml_encoding, "Incorrect value")

    @expect_exception(invalid_srcml_encoding)
    def test_xml_encoding_invalid_encoding(self):
        subject = writable_archive_settings()
        subject.xml_encoding = "bananalkmadssldkmka;kdfavqinrfq]"

    def test_src_encoding(self):
        subject = writable_archive_settings()
        self.assertIsNone(subject.src_encoding, "Incorrect default value")
        expected = "UTF-8"
        subject.src_encoding = expected
        self.assertEqual(expected, subject.src_encoding, "Incorrect value")


    @expect_exception(invalid_srcml_encoding)
    def test_src_encoding_invalid_encoding(self):
        subject = writable_archive_settings()
        subject.src_encoding = "bananalkmadssldkmkakdfavqinrfq]"

    def test_processing_instruction(self):
        subject = writable_archive_settings()
        self.assertIsNone(subject.processing_instruction, "Incorrect default value")
        
        subject.set_processing_instruction("x", "y")
        pi = subject.processing_instruction
        self.assertIsNotNone(pi, "Incorrect value")
        self.assertEqual("x", pi[0], "Incorrect value")
        self.assertEqual("y", pi[1], "Incorrect value")

        subject.processing_instruction = ("a", "b")
        pi = subject.processing_instruction
        self.assertIsNotNone(pi, "Incorrect value")
        self.assertEqual("a", pi[0], "Incorrect value")
        self.assertEqual("b", pi[1], "Incorrect value")

    @expect_exception(TypeError)
    def test_proc_instr_None_data(self):
        subject = writable_archive_settings()
        subject.processing_instruction = ("x", None)

    @expect_exception(TypeError)
    def test_proc_instr_None_target(self):
        subject = writable_archive_settings()
        subject.processing_instruction = (None, "y")



    def test_clear(self):
        filename_data = "filename"
        url_data = "url"
        language_data = "C++"
        tab_stop_data = 50
        version_data = "version"
        xml_encoding_data ="UTF-8"
        src_encoding_data = "UTF-16"
        parser_options_data = 12345
        macro_data = {"x":"y", "a":"b"}
        xml_ns_data = [("prefix1","uri1"), ("prefix2","uri2")]
        pi_data = ("target", "data")
        subject = writable_archive_settings(
            filename=filename_data,
            url=url_data,
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
        self.assertEqual(url_data, subject.url, "Incorrect value")
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

        subject.clear()
        # post clear test
        self.assertIsNone(subject.filename, "Incorrect default value")
        self.assertIsNone(subject.url, "Incorrect default value")
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


    def test_copy(self):
        filename_data = "filename"
        url_data = "url"
        language_data = "C++"
        tab_stop_data = 50
        version_data = "version"
        xml_encoding_data ="UTF-8"
        src_encoding_data = "UTF-16"
        parser_options_data = 12345
        macro_data = {"x":"y", "a":"b"}
        xml_ns_data = [("prefix1","uri1"), ("prefix2","uri2")]
        pi_data = ("target", "data")
        subject = writable_archive_settings(
            filename=filename_data,
            url=url_data,
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
        self.assertEqual(url_data, subject.url, "Incorrect value")
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

        subject2 = subject.copy()
        # post clear test
        self.assertEqual(subject2.filename, subject.filename, "Incorrect value")
        self.assertEqual(subject2.url, subject.url, "Incorrect value")
        self.assertEqual(subject2.default_language, subject.default_language, "Incorrect value")
        self.assertEqual(subject2.tab_stop, subject.tab_stop, "Incorrect value")
        self.assertEqual(subject2.version, subject.version, "Incorrect value")
        self.assertEqual(subject2.parser_options, subject.parser_options, "Incorrect value")
        self.assertEqual(subject2.xml_encoding, subject.xml_encoding, "Incorrect value")
        self.assertEqual(subject2.src_encoding, subject.src_encoding, "Incorrect value")
        self.assertEqual(subject2.macros, subject.macros, "Incorrect value")
        self.assertEqual(subject2.xml_namespaces, subject.xml_namespaces, "Incorrect value")
        self.assertTupleEqual(subject2.processing_instruction, subject.processing_instruction, "Incorrect value")

    def test___eq__(self):
        filename_data = "filename"
        url_data = "url"
        language_data = "C++"
        tab_stop_data = 50
        version_data = "version"
        xml_encoding_data ="UTF-8"
        src_encoding_data = "UTF-16"
        parser_options_data = 12345
        macro_data = {"x":"y", "a":"b"}
        xml_ns_data = [("prefix1","uri1"), ("prefix2","uri2")]
        pi_data = ("target", "data")
        subject = writable_archive_settings(
            filename=filename_data,
            url=url_data,
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

        subject2 = writable_archive_settings(
            filename=filename_data,
            url=url_data,
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
        self.assertTrue(subject == subject2, "Didn't correctly match")
        subject2.filename = "banana"
        self.assertFalse(subject == subject2, "Didn't correctly match")

    def test___ne__(self):
        filename_data = "filename"
        url_data = "url"
        language_data = "C++"
        tab_stop_data = 50
        version_data = "version"
        xml_encoding_data ="UTF-8"
        src_encoding_data = "UTF-16"
        parser_options_data = 12345
        macro_data = {"x":"y", "a":"b"}
        xml_ns_data = [("prefix1","uri1"), ("prefix2","uri2")]
        pi_data = ("target", "data")
        subject = writable_archive_settings(
            filename=filename_data,
            url=url_data,
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

        subject2 = writable_archive_settings(
            filename=filename_data,
            url=url_data,
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
        self.assertFalse(subject != subject2, "Didn't correctly match")
        subject2.filename = "banana"
        self.assertTrue(subject != subject2, "Didn't correctly match")