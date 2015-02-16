##
# @file test_readable_archive_settings.py
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

class temp_test_transform(xsltransform_base):

    def __init__(self):
        super(temp_test_transform, self).__init__()
        pass


    def apply(self, archive_ptr):
        pass

class test_readable_archive_settings(unittest.TestCase):

    def setUp(self):
        pass
    
    def tearDown(self):
        pass

    def test_default_constructor(self):
        subject = readable_archive_settings()
        self.assertIsNone(subject.xml_encoding, "Incorrect default value")
        self.assertIsNone(subject.src_encoding, "Incorrect default value")
        self.assertEqual([], subject.xsltransformations, "Incorrect default value")

    def test_constructor(self):
        expected_xml_encoding="UTF-8"
        expected_src_encoding ="UTF-8"
        expected_transform = temp_test_transform()
        subject = readable_archive_settings(
            xml_encoding=expected_xml_encoding,
            src_encoding=expected_src_encoding,
            xsltransformations=[expected_transform]
        )
        self.assertEqual(expected_xml_encoding, subject.xml_encoding, "Incorrect value")
        self.assertEqual(expected_src_encoding, subject.src_encoding, "Incorrect value")
        self.assertEqual([expected_transform], subject.xsltransformations, "Incorrect value")

    @expect_exception(TypeError)
    def test_constructor_invalid_transformation(self):
        subject = readable_archive_settings(xsltransformations=[object()]) 


    def test_xml_encoding(self):
        subject = readable_archive_settings()
        self.assertIsNone(subject.xml_encoding, "Incorrect default value")
        expected = "UTF-8"
        subject.xml_encoding = expected
        self.assertEqual(expected, subject.xml_encoding, "Incorrect value")

    @expect_exception(invalid_srcml_encoding)
    def test_xml_encoding_invalid_encoding(self):
        subject = readable_archive_settings()
        subject.xml_encoding = "bananalkmadssldkmka;kdfavqinrfq]"

    def test_src_encoding(self):
        subject = readable_archive_settings()
        self.assertIsNone(subject.src_encoding, "Incorrect default value")
        expected = "UTF-8"
        subject.src_encoding = expected
        self.assertEqual(expected, subject.src_encoding, "Incorrect value")


    @expect_exception(invalid_srcml_encoding)
    def test_src_encoding_invalid_encoding(self):
        subject = readable_archive_settings()
        subject.src_encoding = "bananalkmadssldkmkakdfavqinrfq]"


    def test_copy(self):
        expected_xml_encoding="UTF-8"
        expected_src_encoding ="UTF-8"
        expected_transform = temp_test_transform()
        subject = readable_archive_settings(
            xml_encoding=expected_xml_encoding,
            src_encoding=expected_src_encoding,
            xsltransformations=[expected_transform]
        )
        subject2 = subject.copy()
        self.assertEqual(subject2.xml_encoding, subject.xml_encoding, "Incorrect value")
        self.assertEqual(subject2.src_encoding, subject.src_encoding, "Incorrect value")
        self.assertEqual(subject2.xsltransformations, subject.xsltransformations, "Incorrect value")

    def test_clear(self):
        expected_xml_encoding="UTF-8"
        expected_src_encoding ="UTF-8"
        expected_transform = temp_test_transform()
        subject = readable_archive_settings(
            xml_encoding=expected_xml_encoding,
            src_encoding=expected_src_encoding,
            xsltransformations=[expected_transform]
        )
        self.assertEqual(expected_xml_encoding, subject.xml_encoding, "Incorrect value")
        self.assertEqual(expected_src_encoding, subject.src_encoding, "Incorrect value")
        self.assertEqual([expected_transform], subject.xsltransformations, "Incorrect value")        
        subject.clear()
        self.assertIsNone(subject.xml_encoding, "Incorrect default value")
        self.assertIsNone(subject.src_encoding, "Incorrect default value")
        self.assertEqual([], subject.xsltransformations, "Incorrect default value")



    def test_copy(self):
        expected_xml_encoding="UTF-8"
        expected_src_encoding ="UTF-8"
        expected_transform = temp_test_transform()
        subject = readable_archive_settings(
            xml_encoding=expected_xml_encoding,
            src_encoding=expected_src_encoding,
            xsltransformations=[expected_transform]
        )
        subject2 = subject.copy()
        self.assertEqual(subject2.xml_encoding, subject.xml_encoding, "Incorrect value")
        self.assertEqual(subject2.src_encoding, subject.src_encoding, "Incorrect value")
        self.assertEqual(subject2.xsltransformations, subject.xsltransformations, "Incorrect value")

    def test___eq__(self):
        expected_xml_encoding="UTF-8"
        expected_src_encoding ="UTF-8"
        expected_transform = temp_test_transform()
        subject = readable_archive_settings(
            xml_encoding=expected_xml_encoding,
            src_encoding=expected_src_encoding,
            xsltransformations=[expected_transform]
        )

        subject2 = readable_archive_settings(
            xml_encoding=expected_xml_encoding,
            src_encoding=expected_src_encoding,
            xsltransformations=[expected_transform]
        )
        self.assertEqual(subject2, subject, "Comparison Failure")


    def test___ne__(self):
        expected_xml_encoding="UTF-8"
        expected_src_encoding ="UTF-8"
        expected_transform = temp_test_transform()
        subject = readable_archive_settings(
            xml_encoding=expected_xml_encoding,
            src_encoding=expected_src_encoding,
            xsltransformations=[expected_transform]
        )

        subject2 = readable_archive_settings(
            xml_encoding=expected_xml_encoding,
            xsltransformations=[expected_transform]
        )
        self.assertNotEqual(subject2, subject, "Comparison Failure")
