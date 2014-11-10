##
# @file test_archive.py
#
# @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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

import srcml, os, unittest, ctypes

class TestArchive(unittest.TestCase):

    def setUp(self):
        pass
    
    def tearDown(self):
        pass

    def test_newArchive(self):
        archive = srcml.archive()
        archive = None


    def test_newArchive_kwargs(self):
        encodingExpected = "banana"
        src_encodingExpected = "waffles"
        languageExpected = srcml.LANGUAGE_CXX
        filenameExpected = "aardvark.cpp.cxx"
        directoryExpected = "/someplace"
        versionExpected = "1234"
        tabstopExpected = 90
        optionsExpected = srcml.OPTION_HASH | srcml.OPTION_LINE | srcml.OPTION_PSEUDO_BLOCK
        archive = srcml.archive(
            encoding = encodingExpected,
            src_encoding = src_encodingExpected,
            language = languageExpected,
            filename = filenameExpected,
            directory = directoryExpected,
            version = versionExpected,
            tabstop = tabstopExpected,
            options = optionsExpected,
            processing_instruction = ("target", "data"),
            macros = {"to_replace": "src:type", "to_replace2": "src:name"},
            xml_namespaces = {"waffles": "http://aardvark.or", "banana": "http://bananarepublic.com"},
        )
        self.assertEqual(archive.encoding, encodingExpected, "Incorrect value for encoding.")
        self.assertEqual(archive.src_encoding, src_encodingExpected, "Incorrect value for src_encoding.")
        self.assertEqual(archive.language, languageExpected, "Incorrect value for language.")
        self.assertEqual(archive.filename, filenameExpected, "Incorrect value for filename.")
        self.assertEqual(archive.directory, directoryExpected, "Incorrect value for directory.")
        self.assertEqual(archive.version, versionExpected, "Incorrect value for version.")
        self.assertEqual(archive.tabstop, tabstopExpected, "Incorrect value for tabstop.")
        self.assertEqual(archive.options & optionsExpected, optionsExpected, "Incorrect value for options.")
        self.assertEqual(archive.processing_instruction[0], "target", "Incorrect value for processing_instruction.")
        self.assertEqual(archive.processing_instruction[1], "data", "Incorrect value for processing_instruction.")
        self.assertEqual(len(archive.macros), 2, "incorrect # of macros")
        self.assertEqual(len(archive.xml_namespaces), 9, "incorrect # of namespaces")
        archive = None

    def test_encodingAttr(self):
        archive = srcml.archive()
        self.assertIsNone(archive.encoding, "Incorrect value for encoding.")
        expected = "encoding"
        archive.encoding = expected
        self.assertEqual(archive.encoding, expected, "Incorrect value for encoding.")
        archive = None

    def test_srcEncodingAttr(self):
        archive = srcml.archive()
        self.assertIsNone(archive.src_encoding, "Incorrect value for src_encoding.")
        expected = "encoding"
        archive.src_encoding = expected
        self.assertEqual(archive.src_encoding, expected, "Incorrect value for src_encoding.")
        archive = None

    def test_languageAttr(self):
        archive = srcml.archive()
        self.assertIsNone(archive.language, "Incorrect value for language.")
        expected = srcml.LANGUAGE_CXX
        archive.language = expected
        self.assertEqual(archive.language, expected, "Incorrect value for language.")
        archive = None

    def test_filenameAttr(self):
        archive = srcml.archive()
        self.assertIsNone(archive.filename, "Incorrect value for filename.")
        expected = "aardvark.cpp"
        archive.filename = expected
        self.assertEqual(archive.filename, expected, "Incorrect value for filename.")
        archive = None

    def test_directoryAttr(self):
        archive = srcml.archive()
        self.assertIsNone(archive.directory, "Incorrect value for directory.")
        expected = "SomeDirectory"
        archive.directory = expected
        self.assertEqual(archive.directory, expected, "Incorrect value for directory.")
        archive = None

    def test_versionAttr(self):
        archive = srcml.archive()
        self.assertIsNone(archive.version, "Incorrect value for version.")
        expected = "1402"
        archive.version = expected
        self.assertEqual(archive.version, expected, "Incorrect value for version.")
        archive = None

    def test_tabstopAttr(self):
        archive = srcml.archive()
        self.assertEqual(archive.tabstop, 8, "Incorrect value for tabstop.")
        expected = 5
        archive.tabstop = expected
        self.assertEqual(archive.tabstop, expected, "Incorrect value for tabstop.")
        archive = None


    def test_processing_instructionAttr(self):
        archive = srcml.archive()
        expected = ("src:type", "something",)
        archive.processing_instruction = expected
        self.assertEqual(archive.processing_instruction[0], "src:type", "Incorrect value for target of processing_instruction.")
        self.assertEqual(archive.processing_instruction[1], "something", "Incorrect value for data of processing_instruction.")
        archive = None

    def test_optionsAttr(self):
        archive = srcml.archive()
        # self.assertEqual(archive.options, 8, "Incorrect value for tabstop.")
        expected = srcml.OPTION_HASH | srcml.OPTION_LINE | srcml.OPTION_PSEUDO_BLOCK
        archive.options = expected
        self.assertEqual(archive.options & expected, expected, "Incorrect value for options.")
        archive = None

    def test_optionsAttr_2(self):
        archive = srcml.archive()
        self.assertEqual(archive.options & srcml.OPTION_HASH, srcml.OPTION_HASH, "Incorrect value for nested if.")
        archive.options = archive.options & ~srcml.OPTION_HASH
        self.assertEqual(archive.options & srcml.OPTION_HASH, 0, "Incorrect value for options.")
        archive = None



    def test_enable_option(self):
        archive = srcml.archive()
        self.assertFalse(archive.option_is_enabled(srcml.OPTION_CPPIF_CHECK), "archive option has incorrect default value.")
        archive.enable_option(srcml.OPTION_CPPIF_CHECK)
        self.assertTrue(archive.option_is_enabled(srcml.OPTION_CPPIF_CHECK), "archive option has incorrect value.")
        archive = None

    def test_disable_option(self):
        archive = srcml.archive()
        self.assertTrue(archive.option_is_enabled(srcml.OPTION_ARCHIVE), "archive option has incorrect default value.")
        archive.disable_option(srcml.OPTION_ARCHIVE)
        self.assertFalse(archive.option_is_enabled(srcml.OPTION_ARCHIVE), "archive option has incorrect value.")
        archive = None

    def test_marcos_proxy_len(self):
        archive = srcml.archive()
        self.assertEqual(len(archive.macros), 0, "Incorrect # of macros.")
        archive.macros.update({"__LIBSRCML_DECL": "src:name"})
        self.assertEqual(len(archive.macros), 1, "Incorrect # of macros.")
        archive = None

    def test_marcos_proxy_getitem_and_iteration(self):
        archive = srcml.archive()
        self.assertEqual(len(archive.macros), 0, "Incorrect # of macros.")
        archive.macros.update({"__LIBSRCML_DECL": "src:name", "BANANA":"src:type", "aardvark":"src:event"})
        self.assertEqual(len(archive.macros), 3, "Incorrect # of macros.")
        # self.assertEqual(len(archive.macros), 3, "Incorrect # of macros.")
        index = 0
        for x in archive.macros:
            index += 1
        self.assertEqual(index, 3, "Incorrect # of macros iterations.")
        archive = None

    def test_language_from_filename(self):
        archive = srcml.archive()
        ext = archive.language_from_filename("file.cpp")
        self.assertEqual(ext, srcml.LANGUAGE_CXX, "Incorrect value for file extension")
        archive = None

    def test_register_file_ext(self):
        archive = srcml.archive()
        archive.register_file_ext("banana", srcml.LANGUAGE_JAVA)
        ext = archive.language_from_filename("file.banana")
        self.assertEqual(ext, srcml.LANGUAGE_JAVA, "Incorrect value for file extension")
        archive = None

    def test_ns_proxy_len(self):
        archive = srcml.archive()
        self.assertEqual(len(archive.xml_namespaces), 7, "Incorrect # of xml namespaces.")
        archive.xml_namespaces.update({"waffles": "http://www.google.com"})
        self.assertEqual(len(archive.xml_namespaces), 8, "Incorrect # of xml namespaces.")
        archive = None

    def test_ns_proxy_getitem_and_iteration(self):
        archive = srcml.archive()
        self.assertEqual(len(archive.xml_namespaces), 7, "Incorrect # of xml namespace.")
        archive.xml_namespaces.update({"aardvark": "http://aardvark.org", "banana": "http://bananarepublic.com"})
        self.assertEqual(len(archive.xml_namespaces), 9, "Incorrect # of xml namespace.")
        index = 0
        for x in archive.xml_namespaces:
            index += 1
        self.assertEqual(index, 9, "Incorrect # of xml namespace iterations.")
        archive = None

    def test_ns_proxy_get_uri(self):
        archive = srcml.archive()
        expectedURI = "http://aardvark.org"
        archive.xml_namespaces.update({"aardvark": expectedURI, "banana": "http://bananarepublic.com"})
        actual = archive.xml_namespaces.get_uri("aardvark")
        self.assertEqual(actual, expectedURI, "Didn't get expected URI.")
        archive = None

    def test_ns_proxy_get_prefix(self):
        archive = srcml.archive()
        expectedURI = "http://aardvark.org"
        expectedPrefix = "aardvark"
        archive.xml_namespaces.update({expectedPrefix: expectedURI, "banana": "http://bananarepublic.com"})
        actual = archive.xml_namespaces.get_prefix(expectedURI)
        self.assertEqual(actual, expectedPrefix, "Didn't get expected prefix.")
        archive = None
