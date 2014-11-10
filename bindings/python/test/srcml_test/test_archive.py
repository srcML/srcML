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
        archive = srcml.archive(
            encoding = encodingExpected,
            src_encoding = src_encodingExpected,
            language = languageExpected,
            filename = filenameExpected,
            directory = directoryExpected,
            version = versionExpected,
            tabstop = tabstopExpected
        )
        self.assertEqual(archive.encoding, encodingExpected, "Incorrect value for encoding.")
        self.assertEqual(archive.src_encoding, src_encodingExpected, "Incorrect value for src_encoding.")
        self.assertEqual(archive.language, languageExpected, "Incorrect value for language.")
        self.assertEqual(archive.filename, filenameExpected, "Incorrect value for filename.")
        self.assertEqual(archive.directory, directoryExpected, "Incorrect value for directory.")
        self.assertEqual(archive.version, versionExpected, "Incorrect value for version.")
        self.assertEqual(archive.tabstop, tabstopExpected, "Incorrect value for tabstop.")
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