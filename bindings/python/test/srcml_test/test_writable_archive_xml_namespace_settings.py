##
# @file test_writable_archive_xml_namespace_settings.py
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

import os, unittest, ctypes

from srcml import writable_archive_xml_namespace_settings
from testlib import expect_exception

class test_writable_archive_xml_namespace_settings(unittest.TestCase):

    def setUp(self):
        pass
    
    def tearDown(self):
        pass

    def test_constructor_default(self):
        subject = writable_archive_xml_namespace_settings()
        self.assertEqual(0, len(subject), "Incorrect # of default namespaces")

    def test_constructor_iterable(self):
        subject = writable_archive_xml_namespace_settings(("x","google.com"), ("aardvark", "aardvark.com"))
        # print str(subject)
        self.assertEqual(2, len(subject), "Incorrect # of default namespaces. Actual # of items: {0}".format(len(subject)))
        self.assertTrue("x" in subject, "didn't correctly locate value")
        self.assertTrue("aardvark" in subject, "didn't correctly locate value")
        self.assertTrue(("aardvark", "aardvark.com") in subject, "didn't correctly locate mapping")

    def test_constructor_kwargs(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        self.assertEqual(2, len(subject), "Incorrect # of default namespaces")
        self.assertTrue("x" in subject, "didn't correctly locate value")
        self.assertTrue("aardvark" in subject, "didn't correctly locate value")
        self.assertTrue(("aardvark", "aardvark.com") in subject, "didn't correctly locate mapping")

    def test___contains__mapping(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        self.assertTrue("google.com" in subject, "didn't correctly locate value")
        self.assertTrue(("aardvark", "aardvark.com") in subject, "didn't correctly locate mapping")
        self.assertFalse(("aardvark", "somthing") in subject, "didn't correctly locate mapping")
        self.assertFalse(("something", "aardvark.com") in subject, "didn't correctly locate mapping")
        self.assertFalse(("something", "something") in subject, "didn't correctly locate mapping")

    @expect_exception(TypeError)
    def test___contains__mapping_None(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        self.assertFalse(None in subject, "didn't correctly locate value")

    def test_has_prefix(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        self.assertTrue(subject.has_prefix("x"), "didn't correctly locate value")
        self.assertTrue(subject.has_prefix("aardvark"), "didn't correctly locate value")
        self.assertFalse(subject.has_prefix("aardvark.com"), "didn't correctly locate value")

    @expect_exception(TypeError)
    def test_has_prefix_None(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        self.assertFalse(subject.has_prefix(None), "didn't correctly locate value")

    def test_has_uri(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        self.assertTrue(subject.has_uri("google.com"), "didn't correctly locate value")
        self.assertTrue(subject.has_uri("aardvark.com"), "didn't correctly locate value")
        self.assertFalse(subject.has_uri("aardvark"), "didn't correctly locate value")
        

    @expect_exception(TypeError)
    def test_has_uri_None(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        self.assertFalse(subject.has_uri(None), "didn't correctly locate value")

    def test_contains_mapping(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        self.assertTrue(subject.contains_mapping("aardvark", "aardvark.com"), "didn't correctly locate mapping")
        self.assertFalse(subject.contains_mapping("aardvark", "somthing") , "didn't correctly locate mapping")
        self.assertFalse(subject.contains_mapping("something", "aardvark.com"), "didn't correctly locate mapping")
        self.assertFalse(subject.contains_mapping("something", "something"), "didn't correctly locate mapping")

        # with 1 parameter
        self.assertTrue(subject.contains_mapping( ("aardvark", "aardvark.com")), "didn't correctly locate mapping")
        self.assertFalse(subject.contains_mapping(("aardvark", "somthing")) , "didn't correctly locate mapping")
        self.assertFalse(subject.contains_mapping(("something", "aardvark.com")), "didn't correctly locate mapping")
        self.assertFalse(subject.contains_mapping(("something", "something")), "didn't correctly locate mapping")

    @expect_exception(TypeError)
    def test_contains_mapping_None(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        subject.contains_mapping(None)

    def test_len(self):
        subject1 = writable_archive_xml_namespace_settings()
        self.assertEqual(0, len(subject1), "Incorrect # of default mappings")
        subject2 = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        self.assertEqual(2, len(subject2), "Incorrect # of mappings")
        subject2.update(src="www.sdml.org/srcml/src")
        self.assertEqual(3, len(subject2), "Incorrect # of mappings")
        subject2.remove_by_prefix("src")
        self.assertEqual(2, len(subject2), "Incorrect # of mappings")


    def test_getitem(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        self.assertTupleEqual(("x", "google.com"), subject["x"], "Didn't get correct mapping")

    @expect_exception(KeyError)
    def test_getitem(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        x = subject["src"]

    @expect_exception(TypeError)
    def test_getitem_None(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        i = subject[None]

    def test_setitem_new_mapping(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        expected_uri = "somthing-new.com"
        expected_prefix = "new"
        subject[expected_prefix] = expected_uri
        self.assertEqual(3, len(subject), "Incorrect # of objects")
        self.assertTrue("google.com" in subject, "didn't correctly locate value")
        self.assertTrue(("aardvark", "aardvark.com") in subject, "didn't correctly locate mapping")
        self.assertTrue((expected_prefix, expected_uri) in subject, "didn't correctly locate mapping")

    def test_setitem_update_mapping(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        expected_uri = "somthing-new.com"
        subject["x"] = expected_uri
        self.assertEqual(2, len(subject), "Incorrect # of objects")
        self.assertTrue(("x", expected_uri) in subject, "didn't correctly locate value")
        self.assertTrue(("aardvark", "aardvark.com") in subject, "didn't correctly locate mapping")

    @expect_exception(KeyError)
    def test_setitem_invalid_update_mapping(self):
        invalid_mapping_ns = "aardvark.com"
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark=invalid_mapping_ns)
        subject["x"] = invalid_mapping_ns

    @expect_exception(TypeError)
    def test_setitem_None_value(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        subject["src"] = None

    @expect_exception(TypeError)
    def test_setitem_None_prefix(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        subject[None] = "banana"

    def test_get_by_uri(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        self.assertTupleEqual(("x", "google.com"), subject.get_by_uri("google.com"), "Didn't get correct mapping")

    @expect_exception(TypeError)
    def test_get_by_uri_None(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        x = subject.get_by_uri(None)

    @expect_exception(TypeError)
    def test_get_by_uri_bad_uri(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        x = subject.get_by_uri(None)

    def test_get_by_prefix(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        self.assertTupleEqual(("x", "google.com"), subject.get_by_prefix("x"), "Didn't get correct mapping")

    @expect_exception(TypeError)
    def test_get_by_prefix_None(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        x = subject.get_by_prefix(None)

    @expect_exception(KeyError)
    def test_get_by_prefix_bad_prefix(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        x = subject.get_by_prefix("something")

    def test___delitem__(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        self.assertEqual(2, len(subject), "Incorrect # of mappings")
        del subject["x"]
        self.assertEqual(1, len(subject), "Incorrect # of mappings")
        self.assertFalse(("x", "google.com") in subject, "didn't correctly locate value")
        self.assertTrue(("aardvark", "aardvark.com") in subject, "didn't correctly locate value")


    @expect_exception(TypeError)
    def test___delitem__None(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        del subject[None]


    def test_remove_by_uri(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        self.assertEqual(2, len(subject), "Incorrect # of mappings")
        subject.remove_by_uri("google.com")
        self.assertEqual(1, len(subject), "Incorrect # of mappings")
        self.assertFalse(("x", "google.com") in subject, "didn't correctly locate value")
        self.assertTrue(("aardvark", "aardvark.com") in subject, "didn't correctly locate value")


    @expect_exception(TypeError)
    def test_remove_by_uri_None(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        subject.remove_by_uri(None)


    def test_remove_by_prefix(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        self.assertEqual(2, len(subject), "Incorrect # of mappings")
        subject.remove_by_prefix("x")
        self.assertEqual(1, len(subject), "Incorrect # of mappings")
        self.assertFalse(("x", "google.com") in subject, "didn't correctly locate value")
        self.assertTrue(("aardvark", "aardvark.com") in subject, "didn't correctly locate value")

    @expect_exception(TypeError)
    def test_remove_by_prefix_None(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        subject.remove_by_prefix(None)

    def test_remove(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        self.assertEqual(2, len(subject), "Incorrect # of mappings")
        subject.remove(("x", "google.com"))
        self.assertEqual(1, len(subject), "Incorrect # of mappings")
        self.assertFalse(("x", "google.com") in subject, "didn't correctly locate value")
        self.assertTrue(("aardvark", "aardvark.com") in subject, "didn't correctly locate value")

    @expect_exception(TypeError)
    def test_remove_None_prefix(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        subject.remove((None, "uri"))

    @expect_exception(TypeError)
    def test_remove_None_uri(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        subject.remove(("prefix", None))

    @expect_exception(TypeError)
    def test_remove_None_mapping(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        subject.remove(None)


    def test___eq__(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        subject2 = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        self.assertTrue(subject == subject2, "Incorrect comparison")
        subject3 = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com", banana="somthing.com")
        self.assertFalse(subject == subject3, "Incorrect comparison")

    def test___ne__(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        subject2 = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        self.assertFalse(subject != subject2, "Incorrect comparison")
        subject3 = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com", banana="somthing.com")
        self.assertTrue(subject != subject3, "Incorrect comparison")


    def test___iter__(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        counter = 0
        remaining_namespaces = dict(x="google.com", aardvark="aardvark.com")
        for ns in subject:
            temp = remaining_namespaces[ns[0]]
            self.assertEqual(temp, ns[1], "Didn't receive correct uri")
            del remaining_namespaces[ns[0]]
            counter += 1

        self.assertEqual(2, counter, "Incorrect # of namespaces iterated over.")

    def test___iter__(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        counter = 0
        remaining_namespaces = dict(x="google.com", aardvark="aardvark.com")
        for ns in subject:
            temp = remaining_namespaces[ns[0]]
            self.assertEqual(temp, ns[1], "Didn't receive correct uri")
            del remaining_namespaces[ns[0]]
            counter += 1
        self.assertTrue(len(remaining_namespaces)==0,"Didn't visit all namespace mapping: Remaining: {0}".format(remaining_namespaces))
        self.assertEqual(2, counter, "Incorrect # of namespaces iterated over.")


    def test_iterprefixes(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        counter = 0
        remaining_namespaces = set(["x","aardvark"])
        for prefix in subject.iterprefixes():
            self.assertTrue(prefix in remaining_namespaces, "Didn't locate expected prefix: {0}".format(prefix))
            remaining_namespaces.remove(prefix)
            counter += 1
        self.assertTrue(len(remaining_namespaces)==0,"Didn't visit all namespace mapping: Remaining: {0}".format(remaining_namespaces))
        self.assertEqual(2, counter, "Incorrect # of namespaces iterated over.")


    def test_iteruris(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        counter = 0
        remaining_namespaces = set(["google.com", "aardvark.com"])
        for uri in subject.iteruris():
            self.assertTrue(uri in remaining_namespaces, "Didn't locate expected prefix: {0}".format(uri))
            remaining_namespaces.remove(uri)
            counter += 1
        self.assertTrue(len(remaining_namespaces)==0,"Didn't visit all namespace mapping: Remaining: {0}".format(remaining_namespaces))
        self.assertEqual(2, counter, "Incorrect # of namespaces iterated over.")

    def test_items(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        counter = 0
        remaining_namespaces = dict(x="google.com", aardvark="aardvark.com")
        items = subject.items()
        for ns in items:
            temp = remaining_namespaces[ns[0]]
            self.assertEqual(temp, ns[1], "Didn't receive correct uri")
            del remaining_namespaces[ns[0]]
            counter += 1
        self.assertTrue(len(remaining_namespaces)==0,"Didn't visit all namespace mapping: Remaining: {0}".format(remaining_namespaces))
        self.assertEqual(2, counter, "Incorrect # of namespaces iterated over.")

    def test_prefixes(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        counter = 0
        remaining_namespaces = set(["x","aardvark"])
        prefixes = subject.prefixes()
        for prefix in prefixes:
            self.assertTrue(prefix in remaining_namespaces, "Didn't locate expected prefix: {0}".format(prefix))
            remaining_namespaces.remove(prefix)
            counter += 1
        self.assertTrue(len(remaining_namespaces)==0,"Didn't visit all namespace mapping: Remaining: {0}".format(remaining_namespaces))
        self.assertEqual(2, counter, "Incorrect # of namespaces iterated over.")

    def test_uris(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        counter = 0
        remaining_namespaces = set(["google.com", "aardvark.com"])
        uris = subject.uris()
        for uri in uris:
            self.assertTrue(uri in remaining_namespaces, "Didn't locate expected uri: {0}".format(uri))
            remaining_namespaces.remove(uri)
            counter += 1
        self.assertTrue(len(remaining_namespaces)==0,"Didn't visit all namespace mapping: Remaining: {0}".format(remaining_namespaces))
        self.assertEqual(2, counter, "Incorrect # of namespaces iterated over.")

    def test_clear(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        self.assertEqual(2, len(subject), "Incorrect # of items")
        subject.clear()
        self.assertEqual(0, len(subject), "Incorrect # of items")

    def test_update_iterable(self):
        subject = writable_archive_xml_namespace_settings() 
        self.assertEqual(0, len(subject), "Incorrect # of items")
        subject.update(("x", "google.com"))
        self.assertEqual(1, len(subject), "Incorrect # of items")
        self.assertTrue(subject.contains_mapping(("x", "google.com")), "Incorrect namespace located")

    def test_update_iterable_prefix_modifing(self):
        subject = writable_archive_xml_namespace_settings() 
        self.assertEqual(0, len(subject), "Incorrect # of items")
        subject.update(("x", "google.com"))
        self.assertEqual(1, len(subject), "Incorrect # of items")
        self.assertTrue(subject.contains_mapping(("x", "google.com")), "Incorrect namespace located")
        subject.update(("google", "google.com"))
        self.assertEqual(1, len(subject), "Incorrect # of items")
        self.assertTrue(subject.contains_mapping(("google", "google.com")), "Incorrect namespace located")


    def test_update_iterable_uri_modifying(self):
        subject = writable_archive_xml_namespace_settings() 
        self.assertEqual(0, len(subject), "Incorrect # of items")
        subject.update(("x", "google.com"))
        self.assertEqual(1, len(subject), "Incorrect # of items")
        self.assertTrue(subject.contains_mapping(("x", "google.com")), "Incorrect namespace located")
        subject.update(("x", "http://www.google.com"))
        self.assertEqual(1, len(subject), "Incorrect # of items")
        self.assertTrue(subject.contains_mapping(("x", "http://www.google.com")), "Incorrect namespace located")

    @expect_exception(KeyError)
    def test_update_iterable_conflicting(self):
        subject = writable_archive_xml_namespace_settings() 
        self.assertEqual(0, len(subject), "Incorrect # of items")
        subject.update(("x", "google.com"), ("g", "something.com"))
        self.assertEqual(2, len(subject), "Incorrect # of items")
        self.assertTrue(subject.contains_mapping(("x", "google.com")), "Incorrect namespace located")
        subject.update(("g", "google.com"))


    @expect_exception(TypeError)
    def test_update_iterable_None(self):
        subject = writable_archive_xml_namespace_settings() 
        self.assertEqual(0, len(subject), "Incorrect # of items")
        subject.update([None])

    @expect_exception(TypeError)
    def test_update_iterable_prefix_is_None(self):
        subject = writable_archive_xml_namespace_settings() 
        self.assertEqual(0, len(subject), "Incorrect # of items")
        subject.update((None, "x"))

    @expect_exception(TypeError)
    def test_update_iterable_uri_is_None(self):
        subject = writable_archive_xml_namespace_settings() 
        self.assertEqual(0, len(subject), "Incorrect # of items")
        subject.update(("x", None))

    def test_update_kwargs(self):
        subject = writable_archive_xml_namespace_settings() 
        self.assertEqual(0, len(subject), "Incorrect # of items")
        subject.update(x="google.com")
        self.assertEqual(1, len(subject), "Incorrect # of items")
        self.assertTrue(subject.contains_mapping(("x", "google.com")), "Incorrect namespace located")

    def test_update_kwargs_prefix_modifing(self):
        subject = writable_archive_xml_namespace_settings() 
        self.assertEqual(0, len(subject), "Incorrect # of items")
        subject.update(x="google.com")
        self.assertEqual(1, len(subject), "Incorrect # of items")
        self.assertTrue(subject.contains_mapping(("x", "google.com")), "Incorrect namespace located")
        subject.update(google="google.com")
        self.assertEqual(1, len(subject), "Incorrect # of items")
        self.assertTrue(subject.contains_mapping(("google", "google.com")), "Incorrect namespace located")


    def test_update_kwargs_uri_modifying(self):
        subject = writable_archive_xml_namespace_settings() 
        self.assertEqual(0, len(subject), "Incorrect # of items")
        subject.update(x="google.com")
        self.assertEqual(1, len(subject), "Incorrect # of items")
        self.assertTrue(subject.contains_mapping(("x", "google.com")), "Incorrect namespace located")
        subject.update(x="http://www.google.com")
        self.assertEqual(1, len(subject), "Incorrect # of items")
        self.assertTrue(subject.contains_mapping(("x", "http://www.google.com")), "Incorrect namespace located")

    @expect_exception(KeyError)
    def test_update_kwargs_conflicting(self):
        subject = writable_archive_xml_namespace_settings() 
        self.assertEqual(0, len(subject), "Incorrect # of items")
        subject.update(x="google.com", g="something.com")
        self.assertEqual(2, len(subject), "Incorrect # of items. Actual # of items: {0}".format(len(subject)))
        self.assertTrue(subject.contains_mapping(("x", "google.com")), "Incorrect namespace located")
        subject.update(g="google.com")


    @expect_exception(TypeError)
    def test_update_kwargs_None(self):
        subject = writable_archive_xml_namespace_settings() 
        self.assertEqual(0, len(subject), "Incorrect # of items")
        subject.update(x=None)


    def test_copy(self):
        subject = writable_archive_xml_namespace_settings(x="google.com", aardvark="aardvark.com")
        copy_of_subject = subject.copy()
        self.assertTrue(copy_of_subject == subject, "Copy isn't equal to subject")