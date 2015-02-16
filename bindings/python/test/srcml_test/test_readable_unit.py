##
# @file test_readable_unit.py
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
import os, unittest
from testlib import expect_exception

class test_readable_unit(unittest.TestCase):
    pass

# class ReadableUnit
#     ReadableUnit(long unitPtr)
#     String encoding()
#     void setEncoding(String newEncoding)
#     String language()
#     String fileName()
#     String directory()
#     String version()
#     String timeStamp()
#     String hash()
#     String toXmlString()
#     String revision()
#     void unparse(String fileName)
#     void unparse(MemoryBuffer buffer)
#     void unparse(StringBuilder builder)
#     void unparse(Writer writer)
#     void unparse(Writer writer, boolean closeStream)
#     void unparse(OutputStream strm)
#     void unparse(OutputStream strm, boolean closeStream)
#     void unparse(WritableContext context)