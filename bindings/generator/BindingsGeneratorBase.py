##
# @file BindingsGeneratorBase.py
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

import lxml.etree as ET

XPathNamespaces = {
    "src": "http://www.sdml.info/srcML/src",
    "cpp": "http://www.sdml.info/srcML/cpp"
}

class FunctionInfo:
    def __init__(self):
        self.returnType = None
        self.parameterTypes = []
        self.functionName = ""

    def clear(self):
        self.returnType = None
        self.parameterTypes = []
        self.functionName = ""

class BindingGenerator(object):
    def __init__(self):
        self.srcmlAPI = None
        self.constantMacros = []
        self.functionPointerTypes = []
        self.functionDecls = []


    def run(self, srcMLedHeaderFilePath, bindingOutputFilePath):
        # Loading XML File.
        self.srcmlAPI = ET.parse(srcMLedHeaderFilePath)

        # Gathering things to use during code generation.
        self.gatherStaticConstantMacros()
        self.gatherFunctionPointerTypes()
        self.gatherFunctionDecls()

        # Open the stream to insert bindings into
        bindingsFile = open(bindingOutputFilePath, "w")
        
        # Write the initial part of any header file
        # into the documentation.
        bindingsFile.write(self.startBindings() + "\n")

        for constant in self.constantMacros:
            bindingsFile.write(self.defineConstantFromMacro(constant[0], constant[1]) + "\n")

        currentFuncInfo = FunctionInfo()
        for funcPtr in self.functionPointerTypes:
            bindingsFile.write(self.defineFuncPtrType(currentFuncInfo) + "\n")

        currentFuncInfo = FunctionInfo()
        for funcDecl in self.functionDecls:
            bindingsFile.write(self.buildFunctionBinding(currentFuncInfo) + "\n")
            pass

        bindingsFile.write(self.endBindings() + "\n")
        bindingsFile.close()

    def gatherStaticConstantMacros(self):
        macroList = self.srcmlAPI.xpath("//cpp:define[cpp:macro/src:name[.!='__LIBSRCML_DECL'] and cpp:value]", namespaces=XPathNamespaces)
        self.constantMacros = [
            (
                x.xpath("cpp:macro/src:name/text()", namespaces=XPathNamespaces)[0],
                x.xpath("cpp:value/text()", namespaces=XPathNamespaces)[0]
            )
            for x in macroList
        ]

    def gatherFunctionPointerTypes(self):
        typicalFunctionPointerList = self.srcmlAPI.xpath("//src:function_decl[contains(.,'(')]", namespaces=XPathNamespaces)
        erroniousFunctionPointerList = self.srcmlAPI.xpath("//src:argument/src:expr/src:call/src:call", namespaces=XPathNamespaces)
        print typicalFunctionPointerList
        print erroniousFunctionPointerList


    def gatherFunctionDecls(self):
        pass