##
# @file config.py
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

srcmlHPath = "${SRCML_H_HEADER_PATH}"
generatorPath = "${GENERATOR_LIB_DIR}"
pythonBindingsLocation = "${SRCML_PY_BINDINGS_LOCATION}"
srcmlXMLHeaderFile = "${SRCML_H_XML_OUTPUT_PATH}"
outputFilePath = "${PY_BINDINGS_OUTPUT_FILE}"

import sys

sys.path.append(generatorPath)
from generator import *

class GenPythonCode(BindingGenerator):
    def __init__(self):
        super(GenPythonCode, self).__init__()

    def startBindings(self):
        """ This is called when the bindings started."""
        return ""

    def endBindings(self):
        """ This is called when the bindings end."""
        return ""

    def defineConstantFromMacro(self, name, valueString):
        return "{0} = {1}".format(name, valueString)

    def resolveType(self, type):
        return ""

    def defineFuncPtrType(self, funcPtrInfo):
        return ""

    def buildFunctionBinding(self, functionDeclInfo):
        return ""

genCode = GenPythonCode()
genCode.run(srcmlXMLHeaderFile, outputFilePath)
