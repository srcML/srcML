##
# @file test_xslt_transformations.py
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
from srcml.xslt import *
import os, unittest
from testlib import *
import lxml.etree as et

# def write_test_func(ctxt, buffer, size):
#     ctxt.write(buffer)
#     return size

# def close_test_func(ctxt):
#     return 0

xml_namespaces = {"src":"http://www.sdml.info/srcML/src"}

def run_transform(*transformations):
    temp_buffer = memory_buffer()
    with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=temp_buffer) as archive_writer:
        u = archive_writer.create_unit()
        u.parse(source_code=test_source_code_data)
        archive_writer.write(u)

    output_buffer = memory_buffer()
    with readable_archive(readable_archive_settings(xsltransformations=transformations), buffer=temp_buffer) as archive_reader:
        with writable_archive(writable_archive_settings(), buffer=output_buffer) as arch_writer:
            archive_reader.xslt.apply(arch_writer)

    return readable_archive(readable_archive_settings(), buffer=output_buffer)

class test_xslt_transformations(unittest.TestCase):

    def test_xpath_transformation(self):
        with run_transform(xpath("//src:expr")) as transformed_archive:
            counter = 0
            for u in transformed_archive:
                counter += 1
            self.assertEqual(2, counter, "Incorrect # of expressions located.")

    def test_xpath_attribute_transformation(self):
        expected_attr = "test_attr"
        with run_transform(xpath_attribute("//src:expr", expected_attr, "content")) as transformed_archive:
            u = transformed_archive.read()
            xml_unit = et.XML(u.get_standalone_xml())
            located_expressions = xml_unit.xpath("//src:expr[@test_attr]", namespaces=xml_namespaces)
            self.assertEqual(2, len(located_expressions), "Incorrect # of expressions located.")

    def test_xpath_element_transformation(self):
        expected_elem = "my_element"
        with run_transform(xpath_element("//src:expr", expected_elem)) as transformed_archive:
            u = transformed_archive.read()
            xml_unit = et.XML(u.get_standalone_xml())
            located_expressions = xml_unit.xpath("//src:my_element", namespaces=xml_namespaces)
            self.assertEqual(2, len(located_expressions), "Incorrect # of expressions located.")



    def test_xsltransform_filename(self):
        raise NotImplementedError()

    def test_xsltransform_xslt_string(self):
        raise NotImplementedError()

    def test_xsltransform_stream(self):
        raise NotImplementedError()

    def test_xsltransform_memory_buffer(self):
        raise NotImplementedError()

    def test_xsltransform_context(self):
        raise NotImplementedError()

    def test_xsltransform_context_with_functions(self):
        raise NotImplementedError()


    def test_param(self):
        raise NotImplementedError()

    def test_stringparam(self):
        raise NotImplementedError() 


    def test_relaxng_filename(self):
        raise NotImplementedError()

    def test_relaxng_string(self):
        raise NotImplementedError()

    def test_relaxng_stream(self):
        raise NotImplementedError()

    def test_relaxng_memory_buffer(self):
        raise NotImplementedError()

    def test_relaxng_context(self):
        raise NotImplementedError()

    def test_relaxng_context_with_functions(self):
        raise NotImplementedError()
