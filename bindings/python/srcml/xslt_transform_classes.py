##
# @file xslt_transform_classes.py
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

from private_helpers import *
from bindings import *

class xpath:
    """
    Registers an XPath transformation with the srcml.archive class.

    Used in conjunction with the xslt attribute of the srcml.archive class
    """
    def __init__(self, xpath_str):
        self.xpath = xpath_str

    def __call__(self, srcml_archive_obj):
        append_transform_xpath(srcml_archive_obj, self.xpath)


class xpath_attribute:
    """
    Registers the attribute to associate with an XPath transformation
    Elements can be tagged within a unit using the xpath_attribute.

    Used in conjunction with the xslt attribute of the srcml.archive class
    """
    def __init__(self, xpath_str, attr_name, attr_value, attr_prefix=None, attr_namespace_uri=None):
        self.xpath = xpath_str
        self.prefix = attr_prefix
        self.namespace_uri = attr_namespace_uri
        self.name = attr_name
        self.value = attr_value

    def __call__(self, srcml_archive_obj):
        append_transform_xpath_attribute(srcml_archive_obj, self.xpath, self.prefix, self.namespace_uri, self.name, self.value)

class xpath_element:
    """
    Register an XPath transformation that removes the elements from their current unit and places them within 
    a new unit within the specified element.

    Used in conjunction with the xslt attribute of the srcml.archive class
    """
    def __init__(self, xpath_str, element_name, element_prefix=None, element_ns_uri=None,
            attr_prefix=None, attr_namespace_uri=None, attr_name=None, attr_value=None
        ):
        self.xpath = xpath_str
        self.prefix = element_prefix
        self.namespace_uri = element_ns_uri
        self.name = element_name
        self.attribute_prefix = attr_prefix
        self.attribute_namespace_uri = attr_namespace_uri
        self.attribute_name = attr_name
        self.attribute_value = attr_value

    def __call__(self, srcml_archive_obj):
        append_transform_xpath_element(
            srcml_archive_obj,
            self.xpath,
            self.prefix,
            self.namespace_uri,
            self.name,
            self.attribute_prefix,
            self.attribute_namespace_uri,
            self.attribute_name,
            self.attribute_value)

class xslt_transform:
    """
    This class represents an XSLT style sheet loaded from memory, file, memory, etc...

    Used in conjunction with the xslt attribute of the srcml.archive class
    """

    def __init__(self, **kwargs):
        """
        An xslt stylesheet can be registered as a transformation in several ways:
            1) Using a python stream.
            2) Using a filename,
            3) Using a python string.
            4) Using a C file descriptor (this is used to interface with other
                C libraries that use file descriptors).

        Ways to call xslt_transform():
            1) Python Stream:
                archive.xslt.transforms.append(xslt_transform(stream = strm))

            2) filename:
                archive.xslt.transforms.append(xslt_transform(filename = "stylesheet-filename.xsl"))

            3) xslt string (size is optional, if not given the entire string is used):
                archive.xslt.transforms.append(xslt_transform(xslt = preloaded_XSLT_string, sie=length_of_xslt))

            4) file descriptor:
                archive.xslt.transforms.append(xslt_transform(fd = file_descriptor))
        """
        if STREAM_PARAM in kwargs:
            if len(kwargs) > 1:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            strm = kwargs[STREAM_PARAM]
            
            def register_stream(srcml_archive):
                strm_content = ctypes.create_string_buffer("".join(strm.readlines()))
                append_transform_xslt_memory(srcml_archive, strm_content, len(strm_content))

            self._register_func = register_stream

        elif FILENAME_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            filename = kwargs[FILENAME_PARAM]

            def register_filename(srcml_archive):
                append_transform_xslt_filename(srcml_archive, filename)

            self._register_func = register_filename

        elif XSLT_PARAM in kwargs:
            if len(kwargs) > 2 or len(kwargs) == 2 and SIZE_PARAM not in kwargs:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))

            xslt_str = kwargs[XSLT_PARAM]
            if SIZE_PARAM in kwargs:
                amount_to_read = kwargs[SIZE_PARAM]
            else:
                amount_to_read = len(xslt_str)

            def register_xslt_str(srcml_archive):
                strm_content = ctypes.create_string_buffer(xslt_str)
                append_transform_xslt_memory(srcml_archive, strm_content, len(amount_to_read))

            self._register_func = register_xslt_str

        elif FD_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            # read_open_fd(self.srcml_archive, )
            file_descriptor = kwargs[FD_PARAM]
            def register_fd(srcml_archive):
                append_transform_xslt_fd(srcml_archive, file_descriptor)

            self._register_func = register_fd

        else:
            raise Exception("No known parameters")



    def __call__(self, srcml_archive_obj):
        self._register_func(srcml_archive_obj)


class relaxng_schema:
    """
    This class represents a relaxng schema that can be used for with
    the XSLT transformation.

    Used in conjunction with the xslt attribute of the srcml.archive class
    """

    def __init__(self, **kwargs):
        """
        RelaxNG schema registered for reading in several ways:
            1) Using a python stream.
            2) Using a filename,
            3) Using a python string.
            4) Using a C file descriptor (this is used to interface with other
                C libraries that use file descriptors).

        Ways to call xslt_transform():
            1) Python Stream:
                archive.xslt.transforms.append(relaxng_schema(stream = strm))

            2) filename:
                archive.xslt.transforms.append(relaxng_schema(filename = "stylesheet-filename.xsl"))

            3) string (size is optional, if not given the entire string is used):
                archive.xslt.transforms.append(relaxng_schema(relaxng = preloaded_RelaxNG_string, sie=length_of_xslt))

            4) file descriptor:
                archive.xslt.transforms.append(relaxng_schema(fd = file_descriptor))
        """
        if STREAM_PARAM in kwargs:
            if len(kwargs) > 1:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            strm = kwargs[STREAM_PARAM]
            
            def register_stream(srcml_archive):
                strm_content = ctypes.create_string_buffer("".join(strm.readlines()))
                append_transform_relaxng_memory(srcml_archive, strm_content, len(strm_content))

            self._register_func = register_stream

        elif FILENAME_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            filename = kwargs[FILENAME_PARAM]

            def register_filename(srcml_archive):
                append_transform_relaxng_filename(srcml_archive, filename)

            self._register_func = register_filename

        elif RELAXNG_PARAM in kwargs:
            if len(kwargs) > 2 or len(kwargs) == 2 and SIZE_PARAM not in kwargs:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))

            xslt_str = kwargs[RELAXNG_PARAM]
            if SIZE_PARAM in kwargs:
                amount_to_read = kwargs[SIZE_PARAM]
            else:
                amount_to_read = len(xslt_str)

            def register_str(srcml_archive):
                strm_content = ctypes.create_string_buffer(xslt_str)
                append_transform_relaxng_memory(srcml_archive, strm_content, len(amount_to_read))

            self._register_func = register_str

        elif FD_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            file_descriptor = kwargs[FD_PARAM]
            def register_fd(srcml_archive):
                append_transform_relaxng_fd(srcml_archive, file_descriptor)

            self._register_func = register_fd

        else:
            raise Exception("No known parameters")

    def __call__(self, srcml_archive_obj):
        self._register_func(srcml_archive_obj)


class xslt_param:
    """
    Registers an XSLT style sheet parameter.

    Used in conjunction with the xslt attribute of the srcml.archive class.
    """
    def __init__(self, name, value):
        self.param_name = name
        self.param_value = value

    def __call__(self, srcml_archive_obj):
        append_transform_param(srcml_archive_obj,self.param_name, self.param_value);

class xslt_stringparam:
    """
    Registers an XSLT style sheet string parameter.

    Used in conjunction with the xslt attribute of the srcml.archive class.
    """
    def __init__(self, name, value):
        self.param_name = name
        self.param_value = value

    def __call__(self, srcml_archive_obj):
        append_transform_stringparam(srcml_archive_obj,self.param_name, self.param_value);
