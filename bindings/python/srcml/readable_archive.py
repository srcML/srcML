##
# @file readable_archive.py
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

from bindings import *
from readable_archive_settings import readable_archive_settings
from private_helpers import *
from readable_unit import readable_unit
from archive_xml_namespaces import archive_xml_namespaces
from archive_macros import archive_macros
# import sys

class readable_archive(object):
    """
    An archive used for reading units of source code so they can be
    manipulated or inspected.
    """
    def __init__(self, settings, **kwargs):
        """
        Opens an archive for reading.

        An archive can be opened, for reading, in several ways:
            1) Using a python stream.
            2) Using a filename,
            3) Using xml from a python string.
            4) Using a srcml.memory_buffer.
            5) Using a I/O callback functions and context.
            6) Using a I/O interface.

        Ways to call open_read():
            1) Python Stream:
                strm = open("somefile.xml","r")
                archive.open_read(stream = strm)

            2) filename:
                archive.open_read(filename = "somefile.xml")

            3) raw xml string (size is optional, if not given the entire string is used):
                archive.open_read(xml = preloadedXMLString, size=lengthOfXML)

            4) srcml.memory_buffer (creating a srcml memory buffer in this way is NOT recommended,
                    a memory buffer is usually created using open_write(buff = mem_buff)):
                mem_buff = srcml.memory_buffer()
                archive.open_read(buff = mem_buff)

            5) I/O callbacks:
                context = memory_to_read_from
                archive.open_read(context = context, read_cb = read_func, close_cb = close_func)

            6) I/O interface:
                readerContext = MyReader()
                archive.open_read(context = readerContext)

        Description of callbacks and interfaces:
            1) read/close callback signature:
                def read(context, buffer, size_of_buffer):
                    return number_of_bytes_writen_to_buffer

                def close(context):
                    return zero_for_success_not_zero_for_failure

                A context can by any python object.

            2) I/O interface is an object that implements read and close 
                similar to those above but doesn't have a context argument.
                    
                class MyReaderInterface:
                    def read(self, buffer, size_of_buffer):
                        return number_of_bytes_writen_to_buffer # or -1 for error

                    def close(self):
                        return zero_for_sucess_not_zero_for_failure 
        """
        self.srcml_archive = archive_create()
        if self.srcml_archive == None:
            raise MemoryError("Failed to allocate native srcml archive.")
        # Construct special members.
        self._macros = archive_macros(self.srcml_archive)
        self._xml_namespaces = archive_xml_namespaces(self.srcml_archive)

        # Load settings here
        if settings.xml_encoding != None:
            archive_set_xml_encoding(self.srcml_archive, settings.xml_encoding)

        if settings.src_encoding != None:
            archive_set_src_encoding(self.srcml_archive, settings.src_encoding)

        # Open archive
        self._open_read(**kwargs)

        class xslt_prop_obj:
            def __init__(self, arch_ptr):
                self.srcml_archive = arch_ptr

            def apply(self, output_archive):
                apply_transforms(self.srcml_archive, output_archive.srcml_archive)

        self._xslt = xslt_prop_obj(self.srcml_archive)

        # Loading transformations,
        if len(settings.xsltransformations) > 0:
            for transform in settings.xsltransformations:
                transform.apply(self.srcml_archive)

    def _open_read(self, **kwargs):
        if STREAM_PARAM in kwargs:
            if len(kwargs) > 1:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            self._open_read(context=stream_context(kwargs[STREAM_PARAM]))

        elif FILENAME_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            archive_read_open_filename(self.srcml_archive, kwargs[FILENAME_PARAM])

        elif XML_PARAM in kwargs:
            if len(kwargs) > 2 or len(kwargs) == 2 and SIZE_PARAM not in kwargs:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            amount_to_read = 0
            if SIZE_PARAM in kwargs:
                amount_to_read = kwargs[SIZE_PARAM]
            else:
                amount_to_read = len(kwargs[XML_PARAM])
            self._open_read(context=str_reader_context(kwargs[XML_PARAM], amount_to_read))

        elif BUFFER_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            self._ctxt = kwargs[BUFFER_PARAM]
            archive_read_open_memory(self.srcml_archive, self._ctxt.buff, self._ctxt.size)

        elif CONTEXT_PARAM in kwargs:
            if len(kwargs) > 3:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            elif len(kwargs) == 2:
                raise Exception("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            elif len(kwargs) == 1:
                self._ctxt = kwargs[CONTEXT_PARAM]
                self._read_cb_helper = read_callback(cb_read_helper)
                self._read_close_helper = close_callback(cb_close_helper)
                archive_read_open_io(
                    self.srcml_archive,
                    self._ctxt,
                    self._read_cb_helper,
                    self._read_close_helper
                )
            else:
                self._ctxt = kwargs[CONTEXT_PARAM]
                self._read_cb_helper = read_callback(cb_read_multifunc_hlpr(kwargs[READ_CB_PARAM]))
                self._read_close_helper = close_callback(kwargs[CLOSE_CB_PARAM])
                archive_read_open_io(
                    self.srcml_archive,
                    self._ctxt,
                    self._read_cb_helper,
                    self._read_close_helper
                )
        else:
            raise Exception("No known parameters")



    # Parts of pythonic interface
    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        self.close()
        archive_free(self.srcml_archive)
        self.srcml_archive = None

    def __del__(self):
        if self.srcml_archive != None:
            self.close()
            archive_free(self.srcml_archive)

    def __iter__(self):
        ret = self.read()
        while ret != None:
            yield ret
            ret = self.read()

    # Properties
    @property
    def xml_encoding(self):
        """
        Returns the XML encoding that an archive was read in as.
        """
        return archive_get_xml_encoding(self.srcml_archive)

    @property
    def src_encoding(self):
        """
        The default encoding used for extracting source code files.
        This encoding can be overridden by setting the encoding on the unit 
        itself before calling unparse.

        If None is Latin-1 is used as the default encoding.
        """
        return archive_get_src_encoding(self.srcml_archive)

    @property
    def filename(self):
        """
        Gets the filename attribute associated with the archive.

        If None the filename attribute wasn't set.
        """
        return archive_get_filename(self.srcml_archive)

    @property
    def url(self):
        """
        Gets the url attribute associated with the archive.

        If None the url attribute wasn't set.
        """
        return archive_get_url(self.srcml_archive)

    @property
    def version(self):
        """
        Gets the versions attribute associated with the archive.

        If None the versions attribute wasn't set.
        """
        return archive_get_version(self.srcml_archive)

    @property
    def parser_options(self):
        """
        Returns the current parser options. This doesn't effect anything accept how
        transformations are handled within the XSLT object.

        """
        return archive_get_options(self.srcml_archive)

    @parser_options.setter
    def parser_options(self, value):
        """
        Set the current parser options for the archive.
        """
        assert value is None or isinstance(value, (long, int)), "Invalid type of value. Value must be a long or integer."
        archive_set_options(self.srcml_archive, value)
    
    @property
    def tab_stop(self):
        """
        Returns the current tabstop 
        """
        return archive_get_tabstop(self.srcml_archive)

    @property
    def revision(self):
        """
        Returns the srcML revision used to create the archive.
        If this is None then the revision wasn't present on the 
        archive and the current srcML was made with a version before
        version 0.8.0.
        """
        return archive_get_revision(self.srcml_archive)

    @property
    def processing_instruction(self):
        """
        If processing instruction was set when writing an archive it can be read
        it can be read using this property. If the processing instruction wasn't
        set then then the processing instruction wasn't set.
        """
        prefix = archive_get_processing_instruction_target(self.srcml_archive)
        if prefix == None:
            return None
        else:
            return (prefix, archive_get_processing_instruction_data(self.srcml_archive))

    @property
    def xml_namespaces(self):
        """
        Returns all of the XML namespaces registered with the archive.
        """
        return self._xml_namespaces

    @property
    def macros(self):
        """
        Returns all macros registered with the current archive.
        """
        return self._macros

    @property
    def xslt(self):
        """
        Returns an XSLT object that can be used to apply transformations
        to the current archive, which is then written into a writable_archive.

        """
        return self._xslt

    # Methods
    def read(self):
        """
        Read the next unit from within the archive. If there are no more
        units to read None is returned.
        """
        unit_ptr = read_unit(self.srcml_archive)
        if unit_ptr == None:
            return None
        return readable_unit(unit_ptr)

    def close(self):
        """
        Close the current archive.
        """
        archive_close(self.srcml_archive)
