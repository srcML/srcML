##
# @file xml_utils.py
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


#
# Testing utilities to aid with the processing of XML so that it can be more
# easily handled during testing.
#

import lxml.etree as ET
import StringIO
import xml.sax as sax

test_source_code_data = """
#include <iostream>

using namespace std;
int main() {
    std::cout << "Hello World" << std::endl;
    return 0;
}
""".strip()

class write_tester:
    def __init__(self):
        self.buffer = StringIO.StringIO()

    def write(self, input_buffer, size):
        self.buffer.write(input_buffer)
        return len(input_buffer)

    def close(self):
        return 0

class read_tester(object):
    def __init__(self, initial_data):
        self.strm = StringIO.StringIO(initial_data)

    def read(self, buff, size):
        data = self.strm.read(size)
        buff[:len(data)] = [ord(x) for x in data[:len(data)]]

        return len(data)

    def close(self):
        return 0

def extract_text_from_xml(input_data):
    # class h
    # sax.ParseString(input_data)
    tree = ET.fromstring(input_data)
    ET.strip_tags(tree, "*")
    # print tree
    return tree.text.strip()