##
# @file BindingsGeneratorBase.py
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

import lxml.etree as ET
import cStringIO, string

XPathNamespaces = {
    "src": "http://www.sdml.info/srcML/src",
    "cpp": "http://www.sdml.info/srcML/cpp"
}

class ParamTypeInfo:
    def __init__(self):
        self.type = ""
        self.name = None

    def __eq__(self, other):
        return self.type == other.type

class FunctionPtrInfo:
    def __init__(self):
        self.returnType = None
        self.parameterTypes = []
        self.functionName = ""

    def clear(self):
        self.returnType = None
        self.parameterTypes = []
        self.functionName = ""

    def  __hash__(self):
        return self.returnType.__hash__() + sum([x.type.__hash__() for x in self.parameterTypes]) + self.functionName.__hash__()

    def __eq__(self, other):
        return (self.returnType == other.returnType
            and len(self.parameterTypes) == len(other.parameterTypes)
            and cmp(self.parameterTypes, other.parameterTypes) == 0
            and self.functionName == other.functionName)

    def __repr__(self):
        return str(self)

    def __str__(self):
        return """{0.returnType} {0.functionName}({1})""".format(self, ",".join([x.type for x in self.parameterTypes]))

PARAM_IN = "in"
PARAM_OUT = "out"
PARAM_IN_OUT = "in/out"

class ParamInfo:
    def __init__(self):
        self.name = ""
        self.actualType = ""
        self.userType = ""
        self.callingConvention = PARAM_IN

class FunctionDeclInfo:
    def __init__(self):
        self.name = ""
        self.returnType = None
        self.params = []

    def __str__(self):
        output = ""
        if self.returnType != None:
            output += "{0.returnType}".format(self)

        return output + """ {0.name}({1})""".format(
            self,
            ",".join([x.actualType for x in self.params])
        )

def extractNormalizedTypeName(element):
    out = cStringIO.StringIO()
    if element.text != None and not all(c in string.whitespace for c in element.text):
        out.write(element.text)
    def extract(currentElement):
        if currentElement.text != None and currentElement.text != "LIBSRCML_DECL" and not all(c in string.whitespace for c in currentElement.text):
            out.write(currentElement.text + " ")
        for chld in currentElement.iterchildren():
            extract(chld)

    for chld in element.iterchildren():
        extract(chld)

    return out.getvalue()

class BindingGenerator(object):
    def __init__(self):
        self.srcmlAPI = None
        self.processingFunctionPointers = False
        self.constantMacros = []
        self.functionPointerTypes = []
        self.functionDecls = []

    def getTypeInfo(self, xmlType):
        if xmlType.tag == "{http://www.sdml.info/srcML/src}function_decl":
            self.processingFunctionPointers = True
            # Function Pointer Type handled first
            currentFuncPtrInfo = FunctionPtrInfo()
            xmlReturnType = xmlType.xpath("src:type", namespaces=XPathNamespaces)[0]
            currentFuncPtrInfo.returnType = self.getTypeInfo(xmlReturnType)
            # Getting function pointer name.
            nameNodeList = xmlType.xpath("*[not(self::src:type or self::src:parameter_list)]", namespaces=XPathNamespaces)
            for nameNode in nameNodeList:
                currentFuncPtrInfo.functionName += extractNormalizedTypeName(nameNode)

            # Handling Parameter List
            parameterDecls = xmlType.xpath("src:parameter_list/src:parameter/src:decl", namespaces=XPathNamespaces)
            for paramDecl in parameterDecls:
                paramType = paramDecl.xpath("src:type", namespaces=XPathNamespaces)[0]
                paramInfo = ParamTypeInfo()
                paramInfo.type = self.getTypeInfo(paramType)
                nameNode = paramDecl.xpath("src:name", namespaces=XPathNamespaces)
                if nameNode != None and len(nameNode) > 0:
                    paramInfo.name = extractNormalizedTypeName(nameNode[0])
                currentFuncPtrInfo.parameterTypes.append(paramInfo)
            self.processingFunctionPointers = False
            return currentFuncPtrInfo
        else:
            # Handling typical type name resolution hand off.
            return self.resolveType(
                extractNormalizedTypeName(xmlType).strip()
            )

    def run(self, srcMLedHeaderFilePath, bindingOutputFilePath):
        # Loading XML File.
        self.srcmlAPI = ET.parse(srcMLedHeaderFilePath)

        # Gathering things to use during code generation.

        # Open the stream to insert bindings into
        bindingsFile = open(bindingOutputFilePath, "w")
        
        # Write the initial part of any header file
        # into the documentation.
        bindingsFile.write(self.startBindings() + "\n")

        self.gatherStaticConstantMacros()
        for constant in self.constantMacros:
            bindingsFile.write(self.defineConstantFromMacro(constant[0], constant[1]) + "\n")
        bindingsFile.write("\n\n")
        bindingsFile.write(self.postStaticConstants() + "\n")

        self.gatherFunctionPointerTypes()
        bindingsFile.write("\n\n")
        for funcPtr in self.functionPointerTypes:
            bindingsFile.write(self.defineFuncPtrType(funcPtr) + "\n")
        bindingsFile.write(self.postFunctionPointers() + "\n")
        

        self.gatherFunctionDecls()
        for funcDecl in self.functionDecls:
            bindingsFile.write(self.buildFunctionBinding(funcDecl) + "\n")
            pass

        bindingsFile.write(self.endBindings() + "\n")
        bindingsFile.close()

    def gatherStaticConstantMacros(self):
        macroList = self.srcmlAPI.xpath("//cpp:define[cpp:macro/src:name[.!='LIBSRCML_DECL'] and cpp:value]", namespaces=XPathNamespaces)
        self.constantMacros = [
            (
                x.xpath("cpp:macro/src:name/text()", namespaces=XPathNamespaces)[0],
                x.xpath("cpp:value/text()", namespaces=XPathNamespaces)[0]
            )
            for x in macroList
        ]

    def gatherFunctionPointerTypes(self):
        # self.processingFunctionPointers = True
        typicalFunctionPointerList = self.srcmlAPI.xpath("//src:function_decl[src:modifier[.='*']]", namespaces=XPathNamespaces)
        # Extracting function pointers and building a list of function pointers
        functionPointerSet = set()
        for funcPtr in typicalFunctionPointerList:
            currentFuncPtrInfo = self.getTypeInfo(funcPtr)
            functionPointerSet.add(currentFuncPtrInfo)
        self.functionPointerTypes = list(functionPointerSet)
        # self.processingFunctionPointers = False


    def gatherFunctionDecls(self):
        """Suck in all of the function declarations."""
        functionDecls = self.srcmlAPI.xpath(
            "//src:function_decl[not(src:modifier[.='*'])]",
            namespaces=XPathNamespaces
        )
        for func in functionDecls:
            funcDeclInfo = FunctionDeclInfo()

            # Get return type info.
            xmlReturnType = func.xpath("src:type", namespaces=XPathNamespaces)[0]
            funcDeclInfo.returnType = self.getTypeInfo(xmlReturnType)

            # Getting function name.
            functionName = func.xpath("src:name", namespaces=XPathNamespaces)[0]
            funcDeclInfo.name = extractNormalizedTypeName(functionName)

            # Handling Parameter List
            parameters = func.xpath("src:parameter_list/src:parameter", namespaces=XPathNamespaces)
            for param in parameters:
                paramInfo = ParamInfo()
                if param[0].tag == "{http://www.sdml.info/srcML/src}decl":
                    # Extracting typical parameter info.
                    nameNode = param[0].xpath("src:name", namespaces=XPathNamespaces)
                    if nameNode != None and len(nameNode) > 0:
                        paramInfo.name = extractNormalizedTypeName(nameNode[0]).strip()
                    paramInfo.actualType = extractNormalizedTypeName(param[0][0]).strip()
                    paramInfo.userType = self.resolveType(paramInfo.actualType)
                    if paramInfo.actualType == "int *" or paramInfo.actualType == "char * *" or paramInfo.actualType == "size_t *":
                        paramInfo.callingConvention = PARAM_IN_OUT
                    
                elif param[0].tag == "{http://www.sdml.info/srcML/src}function_decl":
                    paramInfo.actualType = extractNormalizedTypeName(param[0]).strip()
                    paramInfo.fpType = self.getTypeInfo(param[0])
                    nameNode = param[0].xpath("src:name", namespaces=XPathNamespaces)
                    if nameNode != None and len(nameNode) > 0:
                        paramInfo.name = extractNormalizedTypeName(nameNode[0]).strip()
                    paramInfo.userType = self.resolveType(paramInfo.fpType)
                else:
                    raise Exception ("Unhandled tag: {0}:".format(param[0].tag))
                funcDeclInfo.params.append(paramInfo)
            self.functionDecls.append(funcDeclInfo)

