##
# @file test_archive_xml_namespaces.py
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
from testlib import expect_exception, cleanup_files
import unittest, os, StringIO
import lxml.etree as et

def write_test_func(ctxt, buffer, size):
    ctxt.write(buffer)
    return size

def close_test_func(ctxt):
    return 0

class TestArchiveXMLNamespaces(unittest.TestCase):

    def setUp(self):
        pass
    
    def tearDown(self):
        pass


    def test_writable_archive_xml_namespace_size_default(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(), buffer=mem_buffer) as archive_writer:
            self.assertIsNotNone(archive_writer.xml_namespaces, "Incorrect value")
            self.assertEqual(1, len(archive_writer.xml_namespaces), "Incorrect value")


    def test_writable_archive_xml_namespace_size(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(xml_namespaces={"banana":"www.banana.com", "Google":"http://www.google.com"}), buffer=mem_buffer) as archive_writer:
            self.assertIsNotNone(archive_writer.xml_namespaces, "Incorrect value")
            self.assertEqual(3, len(archive_writer.xml_namespaces), "Incorrect value")


    def test_writable_archive_xml_namespace___getitem__(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(xml_namespaces={"banana":"www.banana.com", "Google": "http://www.google.com"}), buffer=mem_buffer) as archive_writer:
            self.assertIsNotNone(archive_writer.xml_namespaces, "Incorrect value")
            self.assertEqual(3, len(archive_writer.xml_namespaces), "Incorrect value")
            self.assertTupleEqual(("","http://www.sdml.info/srcML/src"), archive_writer.xml_namespaces[0], "Incorrect namespace: {0} {1}".format(*archive_writer.xml_namespaces[0]))
            self.assertTupleEqual(("Google","http://www.google.com"), archive_writer.xml_namespaces[1], "Incorrect namespace: {0} {1}".format(*archive_writer.xml_namespaces[1]))
            self.assertTupleEqual(("banana","www.banana.com"), archive_writer.xml_namespaces[2], "Incorrect namespace: {0} {1}".format(*archive_writer.xml_namespaces[2]))

    @expect_exception(TypeError)
    def test_writable_archive_xml_namespace___getitem__None(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(xml_namespaces={"banana":"www.banana.com", "Google": "http://www.google.com"}), buffer=mem_buffer) as archive_writer:
            x = archive_writer.xml_namespaces[None]

    @expect_exception(TypeError)
    def test_writable_archive_xml_namespace___getitem__non_number(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(xml_namespaces={"banana":"www.banana.com", "Google": "http://www.google.com"}), buffer=mem_buffer) as archive_writer:
            x = archive_writer.xml_namespaces[object()]

    @expect_exception(IndexError)
    def test_writable_archive_xml_namespace___getitem__neg_index(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(xml_namespaces={"banana":"www.banana.com", "Google": "http://www.google.com"}), buffer=mem_buffer) as archive_writer:
            x = archive_writer.xml_namespaces[-1]

    @expect_exception(IndexError)
    def test_writable_archive_xml_namespace___getitem__past_end(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(xml_namespaces={"banana":"www.banana.com", "Google": "http://www.google.com"}), buffer=mem_buffer) as archive_writer:
            x = archive_writer.xml_namespaces[3]

    def test_writable_archive_xml_namespace___iter__(self):
        mem_buffer = memory_buffer()
        remaining_namespaces = dict({"":"http://www.sdml.info/srcML/src", "Google":"http://www.google.com", "banana":"www.banana.com"})
        with writable_archive(writable_archive_settings(xml_namespaces={"banana":"www.banana.com", "Google": "http://www.google.com"}), buffer=mem_buffer) as archive_writer:
            for ns in archive_writer.xml_namespaces:
                if ns[0] in remaining_namespaces:
                    self.assertEqual(remaining_namespaces[ns[0]], ns[1], "Didn't Get correct values: Actual: {0} Expected: {1}".format(ns[1], remaining_namespaces[ns[0]]))
                    del remaining_namespaces[ns[0]]
                else:
                    self.fail("Namespace not registered: {0}".format(ns))

            self.assertEqual(0, len(remaining_namespaces), "Incorrect namespaces specified. {0}".format(remaining_namespaces))

    def test_writable_archive_xml_namespace_prefix_to_uri(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(xml_namespaces={"banana":"www.banana.com", "Google": "http://www.google.com"}), buffer=mem_buffer) as archive_writer:
            self.assertEqual("http://www.google.com", archive_writer.xml_namespaces.prefix_to_uri("Google"), "Didn't correctly resolve prefix to URI")

    @expect_exception(TypeError)
    def test_writable_archive_xml_namespace_prefix_to_uri(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(xml_namespaces={"banana":"www.banana.com", "Google": "http://www.google.com"}), buffer=mem_buffer) as archive_writer:
            x = archive_writer.xml_namespaces.prefix_to_uri(None)

    @expect_exception(TypeError)
    def test_writable_archive_xml_namespace_prefix_to_uri(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(xml_namespaces={"banana":"www.banana.com", "Google": "http://www.google.com"}), buffer=mem_buffer) as archive_writer:
            x = archive_writer.xml_namespaces.prefix_to_uri(list())

    @expect_exception(KeyError)
    def test_writable_archive_xml_namespace_prefix_to_uri(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(xml_namespaces={"banana":"www.banana.com", "Google": "http://www.google.com"}), buffer=mem_buffer) as archive_writer:
            x = archive_writer.xml_namespaces.prefix_to_uri("aardvark")


    def test_writable_archive_xml_namespace_uri_to_prefix(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(xml_namespaces={"banana":"www.banana.com", "Google": "http://www.google.com"}), buffer=mem_buffer) as archive_writer:
            self.assertEqual("Google", archive_writer.xml_namespaces.uri_to_prefix("http://www.google.com"), "Didn't correctly resolve  URI to prefix")

    @expect_exception(TypeError)
    def test_writable_archive_xml_namespace_uri_to_prefix(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(xml_namespaces={"banana":"www.banana.com", "Google": "http://www.google.com"}), buffer=mem_buffer) as archive_writer:
            x = archive_writer.xml_namespaces.uri_to_prefix(None)

    @expect_exception(TypeError)
    def test_writable_archive_xml_namespace_uri_to_prefix(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(xml_namespaces={"banana":"www.banana.com", "Google": "http://www.google.com"}), buffer=mem_buffer) as archive_writer:
            x = archive_writer.xml_namespaces.uri_to_prefix(list())

    @expect_exception(KeyError)
    def test_writable_archive_xml_namespace_uri_to_prefix(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(xml_namespaces={"banana":"www.banana.com", "Google": "http://www.google.com"}), buffer=mem_buffer) as archive_writer:
            x = archive_writer.xml_namespaces.uri_to_prefix("aardvark")


    def test_writable_archive_xml_namespace___contains__(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(xml_namespaces={"banana":"www.banana.com", "Google": "http://www.google.com"}), buffer=mem_buffer) as archive_writer:
            self.assertTrue(("Google", "http://www.google.com") in archive_writer.xml_namespaces, "Didn't correctly locate namespace")
            self.assertFalse(("google", "http://www.google.com") in archive_writer.xml_namespaces,"Incorrectly located namespace")
            self.assertFalse(None in archive_writer.xml_namespaces, "Incorrectly located None")


    def test_writable_archive_xml_namespace_contains_prefix(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(xml_namespaces={"banana":"www.banana.com", "Google": "http://www.google.com"}), buffer=mem_buffer) as archive_writer:
            self.assertTrue(archive_writer.xml_namespaces.contains_prefix("Google"), "Didn't correctly locate namespace")
            self.assertFalse(archive_writer.xml_namespaces.contains_prefix("aardvark"), "Incorrectly located namespace")
            self.assertFalse(archive_writer.xml_namespaces.contains_prefix(None), "Incorrectly located None")


    def test_writable_archive_xml_namespace_contains_uri(self):
        mem_buffer = memory_buffer()
        with writable_archive(writable_archive_settings(xml_namespaces={"banana":"www.banana.com", "Google": "http://www.google.com"}), buffer=mem_buffer) as archive_writer:
            self.assertTrue(archive_writer.xml_namespaces.contains_uri("www.banana.com"), "Didn't correctly locate namespace")
            self.assertFalse(archive_writer.xml_namespaces.contains_uri("aardvark"), "Incorrectly located namespace")
            self.assertFalse(archive_writer.xml_namespaces.contains_uri(None), "Incorrectly located None")