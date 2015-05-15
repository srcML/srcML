##
# @file config.py
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
            "size_t *": "POINTER(c_size_t)",
            "const srcml_archive *": "c_void_p",
            "FILE *": "IMPOSSIBLE",
            "int *" : "c_void_p",
            "char *" : "c_char_p",
            "void *" : "py_object",
            "const char *": "c_char_p",
            "unsigned short":"c_ushort",
            "size_t" : "c_size_t"
        })
        self.fpLookup = dict()

    def startBindings(self):
        """ This is called when the bindings started."""
        # Setting imports etc...
        return """##
# @file bindings.py
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

from ctypes import cdll, c_int, c_char_p, pointer, c_ulong, c_ulonglong, CFUNCTYPE, c_void_p, byref, py_object, c_ushort, POINTER, c_size_t
from ctypes.util import find_library
import exceptions
# libsrcmlLocation = "libsrcml.so"

libsrcml = cdll.LoadLibrary(find_library("srcml"))

libc = cdll.LoadLibrary(find_library('c'))
libc.free.restype = None
libc.free.argtypes = [c_void_p]

def free(to_free):
    libc.free(to_free)

libc.malloc.restype = c_void_p
libc.malloc.argtypes = [c_ulong]

def malloc(allocation_size):
    return libc.malloc(allocation_size)
"""


    def endBindings(self):
        """ This is called when the bindings end."""
        return ""

    def postStaticConstants(self):
        return""" """


    def postFunctionPointers(self):
        return ""

    def defineConstantFromMacro(self, name, valueString):
        return "{0} = {1}".format(name[6:], valueString.replace("SRCML_", ""))

    def resolveType(self, typeInfo):
        if isinstance(typeInfo, str):
            if self.processingFunctionPointers and typeInfo == "char *":
                return "c_void_p"
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
        if len(funcPtrInfo.parameterTypes) > 0:
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
        # if functionDeclInfo.name == "srcml_unit_get_standalone_xml":
        #     functionDeclInfo.returnType = "c_void_p"

        # Writing return type and Argument Types
        outputStrm.write("libsrcml.{0.name}.restype = {0.returnType}\n".format(functionDeclInfo))
        outputStrm.write("libsrcml.{0.name}.argtypes = [{1}]\n".format(functionDeclInfo, ", ".join([p.userType for p in functionDeclInfo.params])))

        intResultTemplate = """
def {pyName}({parameters}):
    exceptions.get_srcml_exception(libsrcml.{nativeName}({invocationArguments}), "{nativeName}", "Native function failure")
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
    return libsrcml.{nativeName}({invocationArguments})
"""
        allocateIntoTempBufferAndReturnResult = """
def {pyName}({parameters}):
    temp = libsrcml.{nativeName}({invocationArguments})
    if temp == None:
        raise MemoryError("Failed to allocate native memory")
    try:
        ret = c_char_p(temp).value
    except:
        libsrcml.srcml_free(temp)
        raise
    free(temp)
    return ret
"""


        selectedTemplate = ""
        if functionDeclInfo.returnType == "None":
            selectedTemplate = voidReturnTemplate
        elif functionDeclInfo.name == "srcml_unit_get_standalone_xml":
            selectedTemplate = intResultTemplate
        elif functionDeclInfo.returnType == "c_void_p" or functionDeclInfo.returnType == "c_char_p":
            selectedTemplate = factoryReturnTemplate
        elif functionDeclInfo.returnType == "c_int":
            if functionDeclInfo.name.find("_get_") == -1 and functionDeclInfo.name.find("check_") == -1:
                selectedTemplate = intResultTemplate    
            else:
                selectedTemplate = factoryReturnTemplate
            # if srcml.get_language_list_size()
            # selectedTemplate = intResultTemplate
        elif functionDeclInfo.returnType == "c_size_t":
            if functionDeclInfo.name.find("_get_") == -1 and functionDeclInfo.name.find("check_") == -1:
                selectedTemplate = intResultTemplate    
            else:
                selectedTemplate = factoryReturnTemplate
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

archiveOutputFilePath = "${PY_ARCHIVE_CLASS_FILE}"
unitOutputFilePath = "${PY_UNIT_CLASS_FILE}"
class PyL2Bindings(L2BindingGeneratorBase):
    def __init__(self, l1Bindigs, archFile, untFile):
        super(PyL2Bindings, self).__init__(l1Bindigs, archFile, untFile)



class TestSuiteGenerator(TestSuiteGeneratorBase):
    """
    This class assists with test suite generation for all of the test suite bindings.
    """
    def __init__(self):
        super(TestSuiteGenerator, self).__init__()

    # Pure-virtual functionality!
    def startTestFile(self):
        return """##
# @file test.py
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

import sys

sys.path.append("${SRCML_PY_LIB_SEARCH_PATH_LOCATION}")

import srcml, os, unittest, ctypes
"""
    def startTestClass(self, name):
        return """

class IOContext:
    def __init__(self):
        # print "Created IO Context!"
        self.calledRead = False
        self.calledWrite = False
        self.calledClose = False


def writeCallback(ctxt, buffer, size):
    ctxt.calledWrite = True
    return 0

def closeCallback(ctxt):
    ctxt.calledClose = True
    return 0

def readCallback(ctxt, buffer, size):
    ctxt.calledRead = True
    return 0

class TestBindings(unittest.TestCase):



    def setUp(self):
        pass
    
    def tearDown(self):
        pass
"""

    def endTestClass(self):
        return ""

    def endTestFile(self):
        return """"""

 
    # Test Function Generating Handler
    def startTestFuncGen(self, testTitle):
        return "def test_{tstName}(self):".format(tstName=testTitle)

    def endTestFuncGen(self):
        return ""

    # Test Generating parts without a start and end function pairs
    def genCall(self, assignResultTo, callName, parameters):
        outStr = ""
        if assignResultTo != None:
            outStr += assignResultTo + " = "

        if callName[:6] == "srcml_":
            return outStr + "srcml." + callName[6:] + "(" +", ".join(parameters) +")"

        elif callName == "srcml":
            return outStr + "srcml.srcml(" + ", ".join(parameters) +")"

        elif callName == DELETE_FILE_FUNC:
            return outStr + "os.remove(" + ", ".join(parameters) + ")"

        else:
            return outStr + callName + "(" + ", ".join(parameters) +")"

    def genTestStatement(self, testCmpType, actualVariable, expectedVariable, messageBase):
        outStr = "self."
        if testCmpType == TEST_ARE_EQUAL:
            outStr += "assertEqual"
        else:
            raise Exception("Not Assertion type is not implemented yet! Comparison type: {0}".format(testCmpType))
        outStr += "(" + actualVariable + ", " + expectedVariable
        if messageBase != None:
            outStr += ", " + "\"{0}\"".format(messageBase)
        return outStr + ")"


    def genTestIOContext(self, ctxtVariable, didWrite, didRead, didClose, messageBase):
        outStr = ""
        if didWrite:
            outStr += "self.assertTrue(" + ctxtVariable + ".value.calledWrite, " + "\"{0}\"".format(messageBase) + ")"
        else:
            outStr += "self.assertFalse(" + ctxtVariable + ".value.calledWrite, " + "\"{0}\"".format(messageBase) + ")"
        outStr  +="\n" + self.getIndentStr()
        
        if didRead:
            outStr += "self.assertTrue(" + ctxtVariable + ".value.calledRead, " + "\"{0}\"".format(messageBase) + ")"
        else:
            outStr += "self.assertFalse(" + ctxtVariable + ".value.calledRead, " + "\"{0}\"".format(messageBase) + ")"
        outStr  +="\n" + self.getIndentStr()

        if didClose:
            outStr += "self.assertTrue(" + ctxtVariable + ".value.calledClose, " + "\"{0}\"".format(messageBase) + ")"
        else:
            outStr += "self.assertFalse(" + ctxtVariable + ".value.calledClose, " + "\"{0}\"".format(messageBase) + ")"
        return outStr

    def genUnaryTestStatement(self, testCmpType, actualVariable, messageBase):
        outStr = "self."
        hasSpecialFirstParam = False
        if testCmpType == TEST_IS_NOT_NULL:
            outStr += "assertIsNotNone"
        elif testCmpType == TEST_FILE_EXISTS:
            outStr += "assertTrue( os.path.exists(" + actualVariable + ")"
            hasSpecialFirstParam = True
        elif testCmpType == TEST_INT_HAS_CONTENT:
            outStr += "assertTrue(" + actualVariable + ".value > 0"
            hasSpecialFirstParam = True
        elif testCmpType == TEST_BUFFER_HAS_CONTENT:
            outStr += "assertTrue(" + actualVariable + ".value is not None and len(" + actualVariable + ".value) > 0"
            hasSpecialFirstParam = True
        # elif testCmpType == TEST_CALLBACK_CONTEXT:
        #     outStr += "assertTrue(" + actualVariable + ".value is not None and len(" + actualVariable + ".value) > 0"
        #     hasSpecialFirstParam = True
        else:
            raise Exception("Not Assertion type is not implemented yet! Comparison type: {0}".format(testCmpType))


        if not hasSpecialFirstParam:
            outStr += "(" + actualVariable
        if messageBase != None:
            outStr += ", " + "\"{0}\"".format(messageBase)
        return outStr + ")"

    def genVariableDecl(self, variableNativeType, variableName, variableInitializerValue):
        if variableNativeType == BUFFER_REF_TYPE:
            return "{0} = ctypes.c_char_p()".format(variableName)
        
        elif variableNativeType == INT_REF_TYPE:
            return "{0} = ctypes.c_int()".format(variableName)

        elif variableNativeType == SIZE_T_REF_TYPE:
            return "{0} = ctypes.c_size_t()".format(variableName)

        elif variableNativeType == WRITE_CALLBACK_TYPE:
            return "{0} = srcml.write_callback(writeCallback)".format(variableName)

        elif variableNativeType == READ_CALLBACK_TYPE:
            return "{0} = srcml.read_callback(readCallback)".format(variableName)

        elif variableNativeType == CLOSE_CALLBACK_TYPE:
            return "{0} = srcml.close_callback(closeCallback)".format(variableName)

        elif variableNativeType == CALLBACK_CTXT_TYPE:
            return "{1}_temp = IOContext()\n{0}{1} = ctypes.py_object({1}_temp)".format(self.getIndentStr(), variableName)

        if variableInitializerValue == None:
            return "{0} = None".format(variableName)
        else:
            if isinstance(variableInitializerValue, str):
                return "{0} = \"{1}\"".format(variableName, variableInitializerValue)
            elif isinstance(variableInitializerValue, int):
                return "{0} = {1}".format(variableName, variableInitializerValue)
            else:
                raise Exception("Variable value not handled yet! Type: {0}, Value: {1}".format(variableInitializerValue.__class__.__name__, variableInitializerValue))


    def genLineComment(self, str):
        return ""

    def genIncrementVariableExpr(self, variableName, incrementBy = 1):
        return ""

    # more complex pseudo-AST functionality.
    def startWhileLoop(self, conditionVariable, testForValue):
        # assert False, "Not Implemented!"
        return ""

    def endWhileLoop(self):
        # assert False, "Not Implemented!"
        return ""

if __name__ == "__main__":
    genCode = GenPythonCode()
    genCode.run(srcmlXMLHeaderFile, outputFilePath)
    # l2Generator = PyL2Bindings(genCode, archiveOutputFilePath, unitOutputFilePath)
    # l2Generator.run()

    testSuiteLocation = "${PY_TEST_SUITE_OUTPUT_FILE_PATH}"
    suiteGenerator = TestSuiteGenerator()
    suiteGenerator.run(testSuiteLocation)

