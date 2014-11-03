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

import sys, cStringIO

sys.path.append(generatorPath)
from generator import *

class GenPythonCode(BindingGenerator):
    def __init__(self):
        super(GenPythonCode, self).__init__()
        self.typeStrLookup = dict({
            "srcml_unit *": "c_void_p",
            "const srcml_unit *": "c_void_p",
            "int" : "c_int",
            "char * *":"c_void_p",
            "void" : "None",
            "unsigned long long" :"c_ulonglong",
            "srcml_archive *": "c_void_p",
            "const srcml_archive *": "c_void_p",
            "FILE *": "IMPOSSIBLE",
            "int *" : "c_void_p",
            "char *" : "c_char_p",
            "void *" : "c_void_p",
            "const char *": "c_char_p",
            "size_t" : "c_int"
        })
        self.fpLookup = dict()

    def startBindings(self):
        """ This is called when the bindings started."""
        # Setting imports etc...
        return """##
# @file bindings.py
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

from ctypes import cdll, c_int, c_char_p, pointer, c_ulonglong, CFUNCTYPE, c_void_p, byref
from ctypes.util import find_library

libsrcml = cdll.LoadLibrary("libsrcml.so")

libc = cdll.LoadLibrary(find_library('c'))
libc.free.restype = None
libc.free.argtypes = [c_void_p]

def free(to_free):
    libc.free(to_free)
"""


    def endBindings(self):
        """ This is called when the bindings end."""
        return ""

    def postStaticConstants(self):
        return"""
def check_result(rc):
    if rc != SRCML_STATUS_OK:
        raise Exception("srcml has encountered an error. Error Code: {0}".format(rc))
"""


    def postFunctionPointers(self):
        return ""

    def defineConstantFromMacro(self, name, valueString):
        return "{0} = {1}".format(name, valueString)

    def resolveType(self, typeInfo):
        if isinstance(typeInfo, str):
            return self.typeStrLookup[typeInfo]
        elif isinstance(typeInfo, FunctionPtrInfo):
            return self.fpLookup[typeInfo]
        else:
            raise Exception("Received unresolvable type. {0}".format(typeInfo.__class__.__name__))
        return ""

    def defineFuncPtrType(self, funcPtrInfo):
        # Removing pointer from name
        outputName = funcPtrInfo.functionName[1::]
        output = outputName + " = CFUNCTYPE("
        output += funcPtrInfo.returnType
        if len(funcPtrInfo.parameterTypes) > 1:
            output += ", "
            output += ", ".join([p.type for p in funcPtrInfo.parameterTypes])
        output += ")"
        self.fpLookup.update(dict({funcPtrInfo : outputName}))
        return output

    def buildFunctionBinding(self, functionDeclInfo):
        # Checking for an impossible parameter
        if "IMPOSSIBLE" in [x.userType for x in  functionDeclInfo.params]:
            # return "# Skipped {0} because of FILE incompatibilities".format(functionDeclInfo.name)
            return ""

        # Outputting all info for function bindings.
        outputStrm = cStringIO.StringIO()
        # Building initial from library.

        # Writing return type and Argument Types
        outputStrm.write("libsrcml.{0.name}.restype = {0.returnType}\n".format(functionDeclInfo))
        outputStrm.write("libsrcml.{0.name}.argtypes = [{1}]\n".format(functionDeclInfo, ", ".join([p.userType for p in functionDeclInfo.params])))

        intResultTemplate = """
def {pyName}({parameters}):
    check_result(libsrcml.{nativeName}({invocationArguments}))
"""
        voidReturnTemplate = """
def {pyName}({parameters}):
    libsrcml.{nativeName}({invocationArguments})
"""
        factoryReturnTemplate = """
def {pyName}({parameters}):
    return libsrcml.{nativeName}({invocationArguments})
"""
        simpleNoCheckReturnTemplate = """
def {pyName}({parameters}):
    libsrcml.{nativeName}({invocationArguments})
"""

        selectedTemplate = ""
        if functionDeclInfo.returnType == "None":
            selectedTemplate = voidReturnTemplate
        elif functionDeclInfo.returnType == "c_void_p" or functionDeclInfo.returnType == "c_char_p":
            selectedTemplate = factoryReturnTemplate
        elif functionDeclInfo.returnType == "c_int":
            if functionDeclInfo.name.find("_get_") == -1:
                selectedTemplate = intResultTemplate    
            else:
                selectedTemplate = factoryReturnTemplate
            # if srcml.get_language_list_size()
            # selectedTemplate = intResultTemplate
        elif functionDeclInfo.returnType == "c_ulonglong":
            selectedTemplate = simpleNoCheckReturnTemplate
        else:
            raise Exception("Unhandled return type: {0}".format(functionDeclInfo.returnType))

        # Gathering simply variables for template.
        if len(functionDeclInfo.name) <= 6:
            pyName = functionDeclInfo.name
        else:
            pyName = functionDeclInfo.name[6:]
        nativeName = functionDeclInfo.name

        # Computing parameters and invocation arguments
        parameters = []
        invocationArguments = []
        archiveNameCount = 0
        unitNameCount = 0
        for param in functionDeclInfo.params:
            paramName = ""
            if param.name == "":
                if param.actualType.find("srcml_archive") != -1:
                    paramName = "archive"
                    if archiveNameCount > 0:
                        paramName += "_{0}".format(archiveNameCount)
                    archiveNameCount += 1
                elif param.actualType.find("srcml_unit") != -1:
                    paramName = "unit"
                    if unitNameCount > 0:
                        paramName += "_{0}".format(unitNameCount)
                    unitNameCount += 1
                else:
                    raise Exception("Unnamed argument of unhandled type. Native parameter representation: {0}".format(functionDeclInfo.actualType))
            else:
                paramName = param.name

            parameters.append(paramName)
            # Checking to see if a variable is an output
            # in/out variable using pointer to handle
            # that case.
            if param.callingConvention == PARAM_IN_OUT:
                invocationArguments.append("byref({0})".format(paramName))
            else:
                invocationArguments.append(paramName)


        outputStrm.write(selectedTemplate.format(pyName = pyName, nativeName = nativeName, parameters=", ".join(parameters), invocationArguments = ", ".join(invocationArguments) ) )

        ret = outputStrm.getvalue()
        outputStrm.close()
        return ret


if __name__ == "__main__":
    genCode = GenPythonCode()
    genCode.run(srcmlXMLHeaderFile, outputFilePath)

