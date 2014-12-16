##
# @file BindingsL2GenBase.py
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

from BindingsGeneratorBase import *

class MethodModelInfo:
    def __init__(self, xmlFuncElement):
        self.name = ""
        self.methodName = ""
        self.params = []
        self.resultType = ""

class PropertyInfo:
    def __init__(self, propertyName, getterMethod, setterMethod):
        self.setter = setterMethod
        self.getter = getterMethod
        self.name = propertyName

class ClassModel:
    def __init__(self, clsName):
        self.name = clsName
        self.propertyInfo = []
        self.methodInfo = []
        self.openReadMethods = []
        self.openWriteMethods = []
        self.freeFunction = None
        self.closeFunction = None


class L2BindingGeneratorBase(object):
    """
    This is for generating the more complex classes
    and associated types that completely cover 
    the majority of the low-level bindings.
    """
    def __init__(self, bindingGenerator, archiveOutputFile, unitOutputFile):
        self.srcmlAPI = bindingGenerator.srcmlAPI
        self.archivePath = archiveOutputFile
        self.unitPath = unitOutputFile

    def run(self):
        # Archive Class Extraction!
        possibleArchiveFunctions = [x for x in self.srcmlAPI.xpath("//src:function_decl", namespaces=XPathNamespaces)
            if x.xpath("src:name", namespaces=XPathNamespaces)[0].text.startswith("srcml_archive") or
                len(x.xpath("src:parameter_list[.//src:name[.='srcml_archive']]", namespaces=XPathNamespaces)) > 0
            ]
        # print "\n".join([extractNormalizedTypeName(x) for x in possibleArchiveFunctions])
        # print len(possibleArchiveFunctions)
        self.genUnitOutputFile()
        self.genArchiveClassFile()

    def genArchiveClassFile(self):
        strm = open(self.archivePath, "w")

        strm.close()

    def genUnitOutputFile(self):
        strm = open(self.unitPath, "w")

        strm.close()
