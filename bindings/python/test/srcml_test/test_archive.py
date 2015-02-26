##
# @file test_archive.py
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

import srcml, os, unittest, ctypes, StringIO, sys
import lxml.etree as et

class TestArchive(unittest.TestCase):

    def setUp(self):
        #print ""
        pass
    
    def tearDown(self):
        print "\nDone"



#     def test_newArchive(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         archive = None


#     def test_newArchive_kwargs(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         encodingExpected = "banana"
#         src_encodingExpected = "waffles"
#         languageExpected = srcml.LANGUAGE_CXX
#         filenameExpected = "aardvark.cpp.cxx"
#         directoryExpected = "/someplace"
#         versionExpected = "1234"
#         tabstopExpected = 90
#         optionsExpected = srcml.OPTION_HASH | srcml.OPTION_LINE | srcml.OPTION_PSEUDO_BLOCK
#         archive = srcml.archive(
#             encoding = encodingExpected,
#             src_encoding = src_encodingExpected,
#             language = languageExpected,
#             filename = filenameExpected,
#             directory = directoryExpected,
#             version = versionExpected,
#             tabstop = tabstopExpected,
#             options = optionsExpected,
#             processing_instruction = ("target", "data"),
#             macros = {"to_replace": "src:type", "to_replace2": "src:name"},
#             xml_namespaces = {"waffles": "http://aardvark.or", "banana": "http://bananarepublic.com"},
#         )
#         self.assertEqual(archive.encoding, encodingExpected, "Incorrect value for encoding.")
#         self.assertEqual(archive.src_encoding, src_encodingExpected, "Incorrect value for src_encoding.")
#         self.assertEqual(archive.language, languageExpected, "Incorrect value for language.")
#         self.assertEqual(archive.filename, filenameExpected, "Incorrect value for filename.")
#         self.assertEqual(archive.directory, directoryExpected, "Incorrect value for directory.")
#         self.assertEqual(archive.version, versionExpected, "Incorrect value for version.")
#         self.assertEqual(archive.tabstop, tabstopExpected, "Incorrect value for tabstop.")
#         self.assertEqual(archive.options & optionsExpected, optionsExpected, "Incorrect value for options.")
#         self.assertEqual(archive.processing_instruction[0], "target", "Incorrect value for processing_instruction.")
#         self.assertEqual(archive.processing_instruction[1], "data", "Incorrect value for processing_instruction.")
#         self.assertEqual(len(archive.macros), 2, "incorrect # of macros")
#         self.assertEqual(len(archive.xml_namespaces), 9, "incorrect # of namespaces")
#         archive = None

#     def test_encodingAttr(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         self.assertIsNone(archive.encoding, "Incorrect value for encoding.")
#         expected = "encoding"
#         archive.encoding = expected
#         self.assertEqual(archive.encoding, expected, "Incorrect value for encoding.")
#         archive = None

#     def test_srcEncodingAttr(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         self.assertIsNone(archive.src_encoding, "Incorrect value for src_encoding.")
#         expected = "encoding"
#         archive.src_encoding = expected
#         self.assertEqual(archive.src_encoding, expected, "Incorrect value for src_encoding.")
#         archive = None

#     def test_languageAttr(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         self.assertIsNone(archive.language, "Incorrect value for language.")
#         expected = srcml.LANGUAGE_CXX
#         archive.language = expected
#         self.assertEqual(archive.language, expected, "Incorrect value for language.")
#         archive = None

#     def test_filenameAttr(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         self.assertIsNone(archive.filename, "Incorrect value for filename.")
#         expected = "aardvark.cpp"
#         archive.filename = expected
#         self.assertEqual(archive.filename, expected, "Incorrect value for filename.")
#         archive = None

#     def test_directoryAttr(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         self.assertIsNone(archive.directory, "Incorrect value for directory.")
#         expected = "SomeDirectory"
#         archive.directory = expected
#         self.assertEqual(archive.directory, expected, "Incorrect value for directory.")
#         archive = None

#     def test_versionAttr(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         self.assertIsNone(archive.version, "Incorrect value for version.")
#         expected = "1402"
#         archive.version = expected
#         self.assertEqual(archive.version, expected, "Incorrect value for version.")
#         archive = None

#     def test_tabstopAttr(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         self.assertEqual(archive.tabstop, 8, "Incorrect value for tabstop.")
#         expected = 5
#         archive.tabstop = expected
#         self.assertEqual(archive.tabstop, expected, "Incorrect value for tabstop.")
#         archive = None


#     def test_processing_instructionAttr(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         expected = ("src:type", "something",)
#         archive.processing_instruction = expected
#         self.assertEqual(archive.processing_instruction[0], "src:type", "Incorrect value for target of processing_instruction.")
#         self.assertEqual(archive.processing_instruction[1], "something", "Incorrect value for data of processing_instruction.")
#         archive = None

#     def test_optionsAttr(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         # self.assertEqual(archive.options, 8, "Incorrect value for tabstop.")
#         expected = srcml.OPTION_HASH | srcml.OPTION_LINE | srcml.OPTION_PSEUDO_BLOCK
#         archive.options = expected
#         self.assertEqual(archive.options & expected, expected, "Incorrect value for options.")
#         archive = None

#     def test_optionsAttr_2(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         self.assertEqual(archive.options & srcml.OPTION_HASH, srcml.OPTION_HASH, "Incorrect value for nested if.")
#         archive.options = archive.options & ~srcml.OPTION_HASH
#         self.assertEqual(archive.options & srcml.OPTION_HASH, 0, "Incorrect value for options.")
#         archive = None

#     def test_enable_option(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         self.assertFalse(archive.option_is_enabled(srcml.OPTION_CPPIF_CHECK), "archive option has incorrect default value.")
#         archive.enable_option(srcml.OPTION_CPPIF_CHECK)
#         self.assertTrue(archive.option_is_enabled(srcml.OPTION_CPPIF_CHECK), "archive option has incorrect value.")
#         archive = None

#     def test_disable_option(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         self.assertTrue(archive.option_is_enabled(srcml.OPTION_ARCHIVE), "archive option has incorrect default value.")
#         archive.disable_option(srcml.OPTION_ARCHIVE)
#         self.assertFalse(archive.option_is_enabled(srcml.OPTION_ARCHIVE), "archive option has incorrect value.")
#         archive = None

#     def test_marcos_proxy_len(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         self.assertEqual(len(archive.macros), 0, "Incorrect # of macros.")
#         archive.macros.update({"__LIBSRCML_DECL": "src:name"})
#         self.assertEqual(len(archive.macros), 1, "Incorrect # of macros.")
#         archive = None

#     def test_marcos_proxy_getitem_and_iteration(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         self.assertEqual(len(archive.macros), 0, "Incorrect # of macros.")
#         archive.macros.update({"__LIBSRCML_DECL": "src:name", "BANANA":"src:type", "aardvark":"src:event"})
#         self.assertEqual(len(archive.macros), 3, "Incorrect # of macros.")
#         index = 0
#         for x in archive.macros:
#             index += 1
#         self.assertEqual(index, 3, "Incorrect # of macros iterations.")
#         archive = None

#     def test_language_from_filename(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         ext = archive.language_from_filename("file.cpp")
#         self.assertEqual(ext, srcml.LANGUAGE_CXX, "Incorrect value for file extension")
#         archive = None

#     def test_register_file_ext(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         archive.register_file_ext("banana", srcml.LANGUAGE_JAVA)
#         ext = archive.language_from_filename("file.banana")
#         self.assertEqual(ext, srcml.LANGUAGE_JAVA, "Incorrect value for file extension")
#         archive = None

#     def test_ns_proxy_len(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         self.assertEqual(len(archive.xml_namespaces), 7, "Incorrect # of xml namespaces.")
#         archive.xml_namespaces.update({"waffles": "http://www.google.com"})
#         self.assertEqual(len(archive.xml_namespaces), 8, "Incorrect # of xml namespaces.")
#         archive = None

#     def test_ns_proxy_getitem_and_iteration(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         self.assertEqual(len(archive.xml_namespaces), 7, "Incorrect # of xml namespace.")
#         archive.xml_namespaces.update({"aardvark": "http://aardvark.org", "banana": "http://bananarepublic.com"})
#         self.assertEqual(len(archive.xml_namespaces), 9, "Incorrect # of xml namespace.")
#         index = 0
#         for x in archive.xml_namespaces:
#             index += 1
#         self.assertEqual(index, 9, "Incorrect # of xml namespace iterations.")
#         archive = None

#     def test_ns_proxy_get_uri(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         expectedURI = "http://aardvark.org"
#         archive.xml_namespaces.update({"aardvark": expectedURI, "banana": "http://bananarepublic.com"})
#         actual = archive.xml_namespaces.get_uri("aardvark")
#         self.assertEqual(actual, expectedURI, "Didn't get expected URI.")
#         archive = None

#     def test_ns_proxy_get_prefix(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         expectedURI = "http://aardvark.org"
#         expectedPrefix = "aardvark"
#         archive.xml_namespaces.update({expectedPrefix: expectedURI, "banana": "http://bananarepublic.com"})
#         actual = archive.xml_namespaces.get_prefix(expectedURI)
#         self.assertEqual(actual, expectedPrefix, "Didn't get expected prefix.")
#         archive = None


#     xml_data_header = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
# """
#     xml_data_body = """<unit xmlns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" revision="0.8.0" language="C++" filename="/home/brian/Projects/buildFiles/srcMLBuild/bindings/srcml.h.temp"><comment type="block" format="doxygen">/**
#  * @file srcml.h
#  *
#  * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
#  *
#  * The srcML Toolkit is free software; you can redistribute it and/or modify
#  * it under the terms of the GNU General Public License as published by
#  * the Free Software Foundation; either version 2 of the License, or
#  * (at your option) any later version.
#  *
#  * The srcML Toolkit is distributed in the hope that it will be useful,
#  * but WITHOUT ANY WARRANTY; without even the implied warranty of
#  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  * GNU General Public License for more details.
#  *
#  * You should have received a copy of the GNU General Public License
#  * along with the srcML Toolkit; if not, write to the Free Software
#  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#  */</comment>

# <comment type="block" format="doxygen">/**
#  * @mainpage srcML Project
#  *
#  * @section libsrcml
#  * 
#  * The srcml functions available in libsrcml for the purposes of:
#  *
#  * * Converting source code to the srcML format
#  * * Converting the srcML format back to source code
#  * * Querying and transformation in the srcML format
#  * 
#  * The general categories:
#  * 
#  * * The srcML convenience function srcml() for conversion of
#  *   individual source files to srcML, and back. Includes helper
#  *   functions to set options in the form srcml_set_*() and srcml_get_*()
#  * 
#  * * Query functions of the form srcml_check_*() to see the
#  *   capabilities of the library. E.g., what languages are supported
#  * 
#  * * API for full control over the construction of srcML archives
#  *   from multiple input source-code files, srcml_archive_*() and srcml_unit_*()
#  */</comment>
# </unit>"""

#     java_archive_file = os.path.join(os.path.dirname(__file__), "anonymous_class_java.comment.java.xml")

#     def test_open_read__xml_string(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         archive.open_read(xml=TestArchive.xml_data_header + TestArchive.xml_data_body)
#         self.assertEqual(archive.filename, "/home/brian/Projects/buildFiles/srcMLBuild/bindings/srcml.h.temp", "Incorrect value for file name.")
#         self.assertEqual(archive.language, srcml.LANGUAGE_CXX, "Incorrect value for language.")
#         unit = archive.read_unit()
#         self.assertIsNotNone(unit, "Didn't read unit/didn't find unit!")
#         self.assertEqual(unit.xml(), TestArchive.xml_data_body, "Didn't get correct unit information!")
#         archive = None

#     def test_open_read__python_stream(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         first_data_file = "test_open_read_io__python_stream.xml"
#         out_stream = open(first_data_file, "w")
#         out_stream.write(TestArchive.xml_data_header + TestArchive.xml_data_body)
#         out_stream.close()
#         in_strm = open(first_data_file, "r")
#         archive.open_read(stream=in_strm)
#         self.assertEqual(archive.filename, "/home/brian/Projects/buildFiles/srcMLBuild/bindings/srcml.h.temp", "Incorrect value for file name.")
#         self.assertEqual(archive.language, srcml.LANGUAGE_CXX, "Incorrect value for language.")
#         unit = archive.read_unit()
#         self.assertIsNotNone(unit, "Didn't read unit/didn't find unit!")
#         self.assertEqual(unit.xml(), TestArchive.xml_data_body, "Didn't get correct unit information!")
#         parsed_xml = et.fromstring(unit.xml())
#         os.remove(first_data_file)
#         archive = None

#     def test_open_read__python_stream_2(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         data_file = os.path.join(os.path.dirname(__file__), "test_data.xml")
#         in_strm = open(data_file, "r")
#         archive.open_read(stream=in_strm)
#         self.assertEqual(archive.filename, "/home/brian/Projects/buildFiles/srcMLBuild/bindings/srcml.h.temp", "Incorrect value for file name.")
#         self.assertEqual(archive.language, srcml.LANGUAGE_CXX, "Incorrect value for language.")
#         unit = archive.read_unit()
#         self.assertIsNotNone(unit, "Didn't read unit/didn't find unit!")
#         tree = et.fromstring(unit.xml())
#         tree_from_file = et.parse(data_file)
#         self.assertEqual(et.tostring(tree), et.tostring(tree_from_file), "File from tree doesn't match tree loaded from unit.")
#         archive = None

#     def test_open_read__python_file_obj(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         data_file = os.path.join(os.path.dirname(__file__), "test_data.xml")
#         archive.open_read(stream=file(data_file))
#         self.assertEqual(archive.filename, "/home/brian/Projects/buildFiles/srcMLBuild/bindings/srcml.h.temp", "Incorrect value for file name.")
#         self.assertEqual(archive.language, srcml.LANGUAGE_CXX, "Incorrect value for language.")
#         unit = archive.read_unit()
#         self.assertIsNotNone(unit, "Didn't read unit/didn't find unit!")
#         tree = et.fromstring(unit.xml())
#         tree_from_file = et.parse(data_file)
#         self.assertEqual(et.tostring(tree), et.tostring(tree_from_file), "File from tree doesn't match tree loaded from unit.")
#         archive = None

#     def test_open_read__filename(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         data_file = os.path.join(os.path.dirname(__file__), "test_data.xml")
#         archive.open_read(filename=data_file)
#         self.assertEqual(archive.filename, "/home/brian/Projects/buildFiles/srcMLBuild/bindings/srcml.h.temp", "Incorrect value for file name.")
#         self.assertEqual(archive.language, srcml.LANGUAGE_CXX, "Incorrect value for language.")
#         unit = archive.read_unit()
#         self.assertIsNotNone(unit, "Didn't read unit/didn't find unit!")
#         tree = et.fromstring(unit.xml())
#         tree_from_file = et.parse(data_file)
#         self.assertEqual(et.tostring(tree), et.tostring(tree_from_file), "File from tree doesn't match tree loaded from unit.")
#         archive = None

#     """
#     def test_open_read__memory_buffer(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         data_file = os.path.join(os.path.dirname(__file__), "test_data.xml")
#         mem_buff = srcml.memory_buffer()
#         initial_buffer = "".join(open(data_file, 'r').readlines())
#         mem_buff._buff = ctypes.create_string_buffer(initial_buffer)
#         mem_buff._size = len(initial_buffer)
#         archive.open_read(buff=mem_buff)
#         self.assertEqual(archive.filename, "/home/brian/Projects/buildFiles/srcMLBuild/bindings/srcml.h.temp", "Incorrect value for file name.")
#         self.assertEqual(archive.language, srcml.LANGUAGE_CXX, "Incorrect value for language.")
#         unit = archive.read_unit()
#         self.assertIsNotNone(unit, "Didn't read unit/didn't find unit!")
#         tree = et.fromstring(unit.xml())
#         tree_from_file = et.parse(data_file)
#         self.assertEqual(et.tostring(tree), et.tostring(tree_from_file), "File from tree doesn't match tree loaded from unit.")
#         archive = None
#         mem_buff.free()
#         mem_buff._buff = ctypes.c_char_p()
#    """     

#     def test_open_read__units(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         archive.open_read(filename=TestArchive.java_archive_file)
#         self.assertEqual(archive.language, srcml.LANGUAGE_JAVA, "Incorrect value for language.")
#         unit_count = 0
#         for u in archive.units():
#             unit_count += 1
#         self.assertEqual(unit_count, 6,"Incorrect # of units.")
#         archive = None

#     # def test_open_read__unit_headers(self):
#     #     archive = srcml.archive()
#     #     archive.open_read(filename=TestArchive.java_archive_file)
#     #     self.assertEqual(archive.language, srcml.LANGUAGE_JAVA, "Incorrect value for language.")
#     #     unit_count = 0
#     #     for u in archive.unit_headers():
#     #         unit_count += 1
#     #     self.assertEqual(unit_count, 6,"Incorrect # of units.")
#     #     archive = None


#     # Testing xml writer interface function.
#     def test_open_write__python_stream(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         strm = StringIO.StringIO()
#         archive.open_write(stream=strm, close_stream=False)
#         archive.close()
#         self.assertTrue(len(strm.getvalue())>0, "didn't correctly write to stream.")

#     def test_open_write__filename(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         outputFile = "test_open_write__filename.xml"
#         archive.open_write(filename=outputFile)
#         archive.close()
#         self.assertTrue(os.path.exists(outputFile), "Didn't result in file creation")
#         xml_text = "".join(open(outputFile, "r").readlines())
#         self.assertTrue(len(xml_text)>0, "Didn't write XML to file.")
#         os.remove(outputFile)

#     def test_open_write__mem_buff(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         mem_buff = srcml.memory_buffer()
#         archive.open_write(buff=mem_buff)
#         archive.close()
#         self.assertTrue(str(mem_buff)>0, "Didn't XML to buffer.")

#     def test_open_write__mem_buff(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         mem_buff = srcml.memory_buffer()
#         archive.open_write(buff=mem_buff)
#         archive.close()
#         self.assertTrue(str(mem_buff)>0, "Didn't XML to buffer.")

#     # Testing operators.
#     def test_eq(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         self.assertFalse(archive == None, "Archive wasn't created correctly.")
#         self.assertTrue(archive == archive, "Archive wasn't created correctly.")

#     def test_ne(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         self.assertTrue(archive != None, "Archive wasn't created correctly.")
#         self.assertFalse(archive != archive, "Archive wasn't created correctly.")

#     def test_archive_create_unit_kwargs(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         archive = srcml.archive()
#         outputStringBuffer = StringIO.StringIO()
#         archive.open_write(stream=outputStringBuffer, close_stream=False)
#         unit = archive.create_unit(language=srcml.LANGUAGE_CXX)
#         self.assertIsNotNone(unit, "Didn't get a unit.")
#         self.assertEqual(unit.language, srcml.LANGUAGE_CXX, "Didn't get a unit.")


#     # Testing XSLT transformations.
#     def test_archive__xslt_xpath(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         iarchive = srcml.archive()
#         iarchive.open_read(filename=TestArchive.java_archive_file)

#         outputStringBuffer = StringIO.StringIO()
#         oarchive = srcml.archive()
#         oarchive.open_write(stream=outputStringBuffer, close_stream=False)

#         iarchive.xslt.transform.append(srcml.xpath("//src:expr"))
#         iarchive.xslt.apply(oarchive)
#         oarchive.close()
#         self.assertTrue(len(outputStringBuffer.getvalue()) > 0, "Archive wasn't created correctly.")

#     def test_archive__xslt_xpath_attribute(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         iarchive = srcml.archive()
#         iarchive.open_read(filename=TestArchive.java_archive_file)

#         outputStringBuffer = StringIO.StringIO()
#         oarchive = srcml.archive()
#         oarchive.open_write(stream=outputStringBuffer, close_stream=False)

#         iarchive.xslt.transform.append(srcml.xpath_attribute("//src:expr", "waffles", "syrup"))
#         iarchive.xslt.apply(oarchive)
#         oarchive.close()
#         self.assertTrue(outputStringBuffer.getvalue().find("waffles") != -1, "Archive wasn't created correctly.")


#     def test_archive__xslt_xpath_element(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         iarchive = srcml.archive()
#         iarchive.open_read(filename=TestArchive.java_archive_file)

#         outputStringBuffer = StringIO.StringIO()
#         oarchive = srcml.archive()
#         oarchive.open_write(stream=outputStringBuffer, close_stream=False)

#         iarchive.xslt.transform.append(srcml.xpath_element("//src:expr", "thingy"))
#         iarchive.xslt.apply(oarchive)
#         oarchive.close()
#         self.assertTrue(outputStringBuffer.getvalue().find("thingy") != -1, "Archive wasn't created correctly.")


#     xslt_identity_transform = """<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
#   <xsl:template match="*|@*|processing-instruction()|comment()">
#     <xsl:copy>
#       <xsl:apply-templates select="*|@*|text()|processing-instruction()|comment()"/>
#     </xsl:copy>
#   </xsl:template>
# </xsl:stylesheet>
# """
#     def test_archive__xslt_xslt_transform_filename(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         output_file_name = "test_archive__xslt_xslt_transform_filename.xsl"
#         iarchive = srcml.archive()
#         outputStringBuffer = StringIO.StringIO()

#         processing_archive = srcml.archive()
#         processing_archive.open_read(filename=TestArchive.java_archive_file)
#         iarchive.language = processing_archive.language
#         iarchive.open_write(stream=outputStringBuffer, close_stream=False)
#         for u in processing_archive.units():
#             out_unit = iarchive.create_unit()
#             u.unparse()
#             out_unit.parse(u.unparse())
#             iarchive.write_unit(out_unit)

#         f = open(output_file_name, "w")
#         f.write(TestArchive.xslt_identity_transform)
#         f.close()
#         iarchive.close()
#         transformed_xml_data = outputStringBuffer.getvalue()


#         iarchive.open_read(xml=transformed_xml_data)

#         outputStringBuffer = StringIO.StringIO()
#         oarchive = srcml.archive()
#         oarchive.open_write(stream=outputStringBuffer, close_stream=False)

#         iarchive.xslt.transform.append(srcml.xslt_transform(filename=output_file_name))
#         iarchive.xslt.apply(oarchive)
#         oarchive.close()
#         iarchive.close()
#         os.remove(output_file_name)
        
#         oarchive.open_read(xml=outputStringBuffer.getvalue())
#         transformed_unit = oarchive.read_unit()
#         iarchive.open_read(xml=transformed_xml_data)
#         input_unit = iarchive.read_unit()

#         self.assertEqual(transformed_unit.xml(), input_unit.xml(), "Archive wasn't transformed correctly.")

#     def test_archive__xslt_xslt_transform_stream(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         output_file_name = "test_archive__xslt_xslt_transform_filename.xsl"
#         iarchive = srcml.archive()
#         outputStringBuffer = StringIO.StringIO()

#         processing_archive = srcml.archive()
#         processing_archive.open_read(filename=TestArchive.java_archive_file)
#         iarchive.language = processing_archive.language
#         iarchive.open_write(stream=outputStringBuffer, close_stream=False)
#         for u in processing_archive.units():
#             out_unit = iarchive.create_unit()
#             u.unparse()
#             out_unit.parse(u.unparse())
#             iarchive.write_unit(out_unit)

#         f = open(output_file_name, "w")
#         f.write(TestArchive.xslt_identity_transform)
#         f.close()
#         iarchive.close()
#         transformed_xml_data = outputStringBuffer.getvalue()

        
#         iarchive.open_read(xml=transformed_xml_data)

#         outputStringBuffer = StringIO.StringIO()
#         oarchive = srcml.archive()
#         oarchive.open_write(stream=outputStringBuffer, close_stream=False)

#         iarchive.xslt.transform.append(srcml.xslt_transform(stream=open(output_file_name, "r")))
#         iarchive.xslt.apply(oarchive)
#         oarchive.close()
#         iarchive.close()
#         os.remove(output_file_name)
        
#         oarchive.open_read(xml=outputStringBuffer.getvalue())
#         transformed_unit = oarchive.read_unit()
#         iarchive.open_read(xml=transformed_xml_data)
#         input_unit = iarchive.read_unit()

#         self.assertEqual(transformed_unit.xml(), input_unit.xml(), "Archive wasn't transformed correctly.")


#     def test_archive__xslt_xslt_transform_stream(self):
#         print "{0}.{1}".format(self.__class__.__name__, sys._getframe().f_code.co_name)
#         output_file_name = "test_archive__xslt_xslt_transform_filename.xsl"
#         iarchive = srcml.archive()
#         outputStringBuffer = StringIO.StringIO()

#         processing_archive = srcml.archive()
#         processing_archive.open_read(filename=TestArchive.java_archive_file)
#         iarchive.language = processing_archive.language
#         iarchive.open_write(stream=outputStringBuffer, close_stream=False)
#         for u in processing_archive.units():
#             out_unit = iarchive.create_unit()
#             u.unparse()
#             out_unit.parse(u.unparse())
#             iarchive.write_unit(out_unit)

#         f = open(output_file_name, "w")
#         f.write(TestArchive.xslt_identity_transform)
#         f.close()
#         iarchive.close()
#         transformed_xml_data = outputStringBuffer.getvalue()

        
#         iarchive.open_read(xml=transformed_xml_data)

#         outputStringBuffer = StringIO.StringIO()
#         oarchive = srcml.archive()
#         oarchive.open_write(stream=outputStringBuffer, close_stream=False)

#         iarchive.xslt.transform.append(srcml.xslt_transform(stream=open(output_file_name, "r")))
#         iarchive.xslt.apply(oarchive)
#         oarchive.close()
#         iarchive.close()
#         os.remove(output_file_name)
        
#         oarchive.open_read(xml=outputStringBuffer.getvalue())
#         transformed_unit = oarchive.read_unit()
#         iarchive.open_read(xml=transformed_xml_data)
#         input_unit = iarchive.read_unit()

#         self.assertEqual(transformed_unit.xml(), input_unit.xml(), "Archive wasn't transformed correctly.")


#     test_code = """
# #include <iostream>
# int main() {
#     std::cout << "hello world" << std::endl;
#     return 0;
# }
# """
