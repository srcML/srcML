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
            # for ns_index in range(3):
            self.assertTupleEqual(("","http://www.sdml.info/srcML/src"), archive_writer.xml_namespaces[0], "Incorrect namespace: {0} {1}".format(*archive_writer.xml_namespaces[0]))
            self.assertTupleEqual(("Google","http://www.google.com"), archive_writer.xml_namespaces[1], "Incorrect namespace: {0} {1}".format(*archive_writer.xml_namespaces[1]))
            self.assertTupleEqual(("banana","www.banana.com"), archive_writer.xml_namespaces[2], "Incorrect namespace: {0} {1}".format(*archive_writer.xml_namespaces[2]))

