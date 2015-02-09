##
# @file writable_archive.py
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
from writable_archive_settings import writable_archive_settings
from private_helpers import *

class writable_archive:


    def __init__(self, settings, **kwargs):
        """
        Creates and opens an archive for writing.

        An archive can be opened, for writing, in several ways:
            1) Using a python stream.
            2) Using a filename,
            3) Using a memory_buffer.
            4) Using a I/O callback functions and context.
            5) Using a I/O interface.

        Ways to call open_write():
            1) Python Stream:
                strm = open("somefile.xml","2")
                archive.open_write(stream = strm)

            Note: the python stream has an optional argument close_stream, which defaults to true
            and doesn't close the stream if false.

            3) filename:
                archive.open_write(filename = "somefile.xml")

            4) srcml.memory_buffer (a python string representation
                    of a srcml archive can be constructed using
                    the srcml.memory_buffer):
                mem_buff = srcml.memory_buffer()
                archive.open_write(buffer = mem_buff)

            5) I/O callbacks:
                context = memory_to_write_to
                archive.open_write(context = context, write = write_func, close = close_func)

            6) I/O interface:
                writerContext = MyMemoryWriter()
                archive.open_write(context = writerContext)

        Description of callbacks and interfaces:
            1) write/close callback signature:
                def write(context, buffer, size_of_data_in_buffer):
                    return zero_for_sucess_not_zero_for_failure

                def close(context):
                    return zero_for_sucess_not_zero_for_failure

                A context can by any python object.

            2) I/O interface is an object that implements write and close 
                similar to those above but doesn't have a context argument.
                    
                class MyWriterInterface:
                    def writer(self, buffer, size_of_data_in_buffer):
                        return number_of_bytes_writen_to_buffer # or -1 for error

                    def close(self):
                        return zero_for_sucess_not_zero_for_failure

        Precondition - The provided settings object must be an instance of a writable_archive_settings
            object.
            One of the provided means of opening an archive for writing must exist.
        """
        assert isinstance(settings,  writable_archive_settings), "Invalid settings object"

        # Creating a new instance of an archive so that it can be handled
        # correctly when opened
        self.srcml_archive = archive_create()
        if self.srcml_archive == None:
            raise MemoryError("Failed to allocate native srcml archive.")

        # Loading the settings into the new archive.

        if STREAM_PARAM in kwargs:
            if len(kwargs) == 1:
                self.open_write(context=write_stream_context(kwargs[STREAM_PARAM]))
            elif len(kwargs) == 2:
                self.open_write(context=write_stream_context(kwargs[STREAM_PARAM], kwargs[CLOSE_STREAM_PARAM]))
            else:
                raise ValueError("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))

        elif FILENAME_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise ValueError("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            archive_write_open_filename(self.srcml_archive, kwargs[FILENAME_PARAM])

        elif BUFFER_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise ValueError("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            self._ctxt = kwargs[BUFF_PARAM]
            write_open_memory(
                self.srcml_archive,
                self._ctxt._buff,
                self._ctxt._size
            )

        elif CONTEXT_PARAM in kwargs:
            if len(kwargs) > 3 or len(kwargs) == 2:
                raise ValueError("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            elif len(kwargs) == 1:
                self._ctxt = kwargs[CONTEXT_PARAM]
                self._write_cb_helper = write_callback(cb_write_helper)
                self._close_write_cb_helper = close_callback(cb_close_helper)
                write_open_io(
                    self.srcml_archive,
                    self._ctxt,
                    self._write_cb_helper,
                    self._close_write_cb_helper
                )
            else:
                self._ctxt = kwargs[CONTEXT_PARAM]
                self._write_cb_helper = write_callback(kwargs[WRITE_CB_PARAM])
                self._close_write_cb_helper = close_callback(kwargs[CLOSE_CB_PARAM])
                write_open_io(
                    self.srcml_archive,
                    self._ctxt,
                    self._write_cb_helper,
                    self._close_write_cb_helper
                )

        elif FD_PARAM in kwargs:
            if len(kwargs) > 1 :
                raise ValueError("Unrecognized argument combination: {0}".format(", ".join(kwargs.keys())))
            write_open_fd(self.srcml_archive, kwargs[FD_PARAM])

        else:
            raise ValueError("No known parameter combination")

    def __del__(self):
        archive_free(self.srcml_archive)
# interface WritableArchive
#     String encoding()
#     String srcEncoding()
#     String language()
#     String fileName()
#     String directory()
#     String version()
#     String revision()
#     int tabStop()
#     long options()
#     void setOptions(long newOptionSet)
#     void enableOptions(long optionsToEnable)
#     void disableOptions(long optionsToDisable)
#     ProcessingInstruction processingInstruction()
#     XmlNamespaceProxy xmlNamespaces()
#     MacroProxy macros()
#     void registerFileExtension(String fileExtension, Language lang)
#     void registerFileExtension(String fileExtension, String lang)
#     String getLanguageForExtension(String fileName)
#     WritableUnit createUnit()
#     void write(ReadableUnit unit)
#     void write(WritableUnit unit)
