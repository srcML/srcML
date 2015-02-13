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


test_source_code_data = """
#include <iostream>

using namespace std;
int main() {
    std::cout << "Hello World" << std::endl;
    return 0;
}
"""

def extract_text_from_xml(input_data):
    tree = ET.fromstring(input_data)
    return ET.strip_tags(tree, "*").text.strip()