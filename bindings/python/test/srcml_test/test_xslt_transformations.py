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
from ctypes import *
import sys

xml_namespaces = {"src":"http://www.sdml.info/srcML/src"}

xslt_ident = """
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="@*|node()">
    <xsl:copy>
      <xsl:apply-templates select="@*|node()"/>
    </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
"""

relaxng_match_all = """<grammar xmlns="http://relaxng.org/ns/structure/1.0">

  <start>
    <ref name="anyElement"/>
  </start>

  <define name="anyElement">
    <element>
      <anyName/>
      <zeroOrMore>
    <choice>
      <attribute>
        <anyName/>
      </attribute>
      <text/>
      <ref name="anyElement"/>
    </choice>
      </zeroOrMore>
    </element>
  </define>

</grammar>
"""
def run_transform(*transformations):
    # print "Called run_transform"
    # sys.stdout.flush()
    temp_buffer = memory_buffer()
    with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=temp_buffer) as archive_writer:
        u = archive_writer.create_unit()
        u.parse(source_code=test_source_code_data)
        archive_writer.write(u)

    output_buffer = memory_buffer()
    with readable_archive(readable_archive_settings(xsltransformations=transformations), buffer=temp_buffer) as archive_reader:
        with writable_archive(writable_archive_settings(), buffer=output_buffer) as arch_writer:
            archive_reader.xslt.apply(arch_writer)

    readable_arch_result = readable_archive(readable_archive_settings(), buffer=output_buffer)
    return readable_arch_result


def run_transform_with_n_units(number_of_units_to_add,*transformations):
    temp_buffer = memory_buffer()
    with writable_archive(writable_archive_settings(default_language=LANGUAGE_CXX), buffer=temp_buffer) as archive_writer:
        for i in range(number_of_units_to_add):
            u = archive_writer.create_unit()
            u.parse(source_code=test_source_code_data)
            archive_writer.write(u)

    output_buffer = memory_buffer()
    with readable_archive(readable_archive_settings(xsltransformations=transformations), buffer=temp_buffer) as archive_reader:
        with writable_archive(writable_archive_settings(), buffer=output_buffer) as arch_writer:
            archive_reader.xslt.apply(arch_writer)

    return readable_archive(readable_archive_settings(), buffer=output_buffer)

class test_xslt_transformations(unittest.TestCase):


            
    def test_xsltransform_filename(self):
        test_file = "test_xsltransform_filename.xslt"
        
        out_strm = open(test_file, "w")
        out_strm.write(xslt_ident)
        out_strm.close()

        with run_transform(xsltransform(filename=test_file)) as transformed_archive:
            u = transformed_archive.read()
            self.assertEqual(test_source_code_data, u.unparse(), "Didn't get correct data from unit")

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


    @cleanup_files("test_xsltransform_filename.xslt")
    def test_xsltransform_filename(self):
        test_file = "test_xsltransform_filename.xslt"
        
        out_strm = open(test_file, "w")
        out_strm.write(xslt_ident)
        out_strm.close()

        with run_transform(xsltransform(filename=test_file)) as transformed_archive:
            u = transformed_archive.read()
            self.assertEqual(test_source_code_data, u.unparse(), "Didn't get correct data from unit")

    def test_xsltransform_xslt_string(self):
        with run_transform(xsltransform(xslt=xslt_ident)) as transformed_archive:
            u = transformed_archive.read()
            self.assertEqual(test_source_code_data, u.unparse(), "Didn't get correct data from unit")

    def test_xsltransform_stream(self):
        with run_transform(xsltransform(stream=StringIO.StringIO(xslt_ident))) as transformed_archive:
            u = transformed_archive.read()
            self.assertEqual(test_source_code_data, u.unparse(), "Didn't get correct data from unit")

    def test_xsltransform_memory_buffer(self):
        temp_buffer = memory_buffer()
        temp_buffer.load_from_string(xslt_ident)
        with run_transform(xsltransform(buffer=temp_buffer)) as transformed_archive:
            u = transformed_archive.read()
            self.assertEqual(test_source_code_data, u.unparse(), "Didn't get correct data from unit")

    def test_xsltransform_context(self):
        test_reader = read_tester(xslt_ident)
        with run_transform(xsltransform(context=test_reader)) as transformed_archive:
            u = transformed_archive.read()
            self.assertEqual(test_source_code_data, u.unparse(), "Didn't get correct data from unit")

    def test_xsltransform_context_with_functions(self):
        test_reader = read_tester(xslt_ident)

        def read(ctxt, buff, size):
            return ctxt.read(buff, size)

        def close(ctxt):
            return ctxt.close()

        with run_transform(xsltransform(context=test_reader, read=read, close=close)) as transformed_archive:
            u = transformed_archive.read()
            self.assertEqual(test_source_code_data, u.unparse(), "Didn't get correct data from unit")


    # @cleanup_files("test_relaxng_filename.xml")
    # def test_relaxng_filename(self):
    #     test_file_name = "test_relaxng_filename.xml"
        
    #     strm = open(test_file_name, "w")
    #     strm.write(relaxng_match_all)
    #     strm.close()

    #     expected_unit_count = 3
    #     with run_transform_with_n_units(expected_unit_count, relaxng_validating_transform(filename=test_file_name)) as transformed_archive:
    #         counter = 0
    #         u = transformed_archive.read()
    #         while u is not None:
    #             counter += 1
    #             u = transformed_archive.read()
    #         self.assertEqual(expected_unit_count, counter, "Incorrect # of units within output archive. Expected: {0} Actual: {1}".format(expected_unit_count, counter))
            

    # def test_relaxng_string(self):
    #     expected_unit_count = 3
    #     with run_transform_with_n_units(expected_unit_count,  relaxng_validating_transform(relaxng=relaxng_match_all)) as transformed_archive:
    #         counter = 0
    #         u = transformed_archive.read()
    #         while u is not None:
    #             counter += 1
    #             u = transformed_archive.read()
    #         self.assertEqual(expected_unit_count, counter, "Incorrect # of units within output archive. Expected: {0} Actual: {1}".format(expected_unit_count, counter))

    # def test_relaxng_stream(self):
    #     expected_unit_count = 3
    #     with run_transform_with_n_units(expected_unit_count,  relaxng_validating_transform(stream=cStringIO.StringIO(relaxng_match_all))) as transformed_archive:
    #         counter = 0
    #         u = transformed_archive.read()
    #         while u is not None:
    #             counter += 1
    #             u = transformed_archive.read()
    #         self.assertEqual(expected_unit_count, counter, "Incorrect # of units within output archive. Expected: {0} Actual: {1}".format(expected_unit_count, counter))

    # def test_relaxng_memory_buffer(self):
    #     temp_buffer = memory_buffer()
    #     temp_buffer.load_from_string(relaxng_match_all)
    #     expected_unit_count = 3
    #     with run_transform_with_n_units(expected_unit_count, relaxng_validating_transform(buffer=temp_buffer)) as transformed_archive:
    #         counter = 0
    #         u = transformed_archive.read()
    #         while u is not None:
    #             counter += 1
    #             u = transformed_archive.read()
    #         self.assertEqual(expected_unit_count, counter, "Incorrect # of units within output archive. Expected: {0} Actual: {1}".format(expected_unit_count, counter))

    # def test_relaxng_context(self):
    #     test_reader = read_tester(relaxng_match_all)
    #     expected_unit_count = 3
    #     with run_transform_with_n_units(expected_unit_count,  relaxng_validating_transform(context=test_reader)) as transformed_archive:
    #         counter = 0
    #         u = transformed_archive.read()
    #         while u is not None:
    #             counter += 1
    #             u = transformed_archive.read()
    #         self.assertEqual(expected_unit_count, counter, "Incorrect # of units within output archive. Expected: {0} Actual: {1}".format(expected_unit_count, counter))

    # def test_relaxng_context_with_functions(self):

    #     def read(ctxt, buff, size):
    #         return ctxt.read(buff, size)

    #     def close(ctxt):
    #         return ctxt.close()
    #     test_reader = read_tester(relaxng_match_all)
    #     expected_unit_count = 3
    #     with run_transform_with_n_units(expected_unit_count,  relaxng_validating_transform(context=test_reader, read=read, close=close)) as transformed_archive:
    #         counter = 0
    #         u = transformed_archive.read()
    #         while u is not None:
    #             counter += 1
    #             u = transformed_archive.read()
    #         self.assertEqual(expected_unit_count, counter, "Incorrect # of units within output archive. Expected: {0} Actual: {1}".format(expected_unit_count, counter))