from LoadData import *
from DocData import *
import os, re
import xml.sax
import xml.sax.saxutils as saxutils


XPathNamespaces = {
    "src": "http://www.sdml.info/srcML/src",
    "cpp": "http://www.sdml.info/srcML/cpp"
}

# Tag name constants
ExtFuncDocTag = "ExtFuncDoc"
NameTag = "Name"
LangsTag = "Langs"
TagsTag = "Tags"
TagTag = "Tag"
TestDataTag = "TestData"
DescTag = "Desc"
ExtFuncTag = "ExtFunc"
XPathExtensionFunctionsTag = "XPathExtensionFunctions"
ExamplesTag = "Examples"
ExampleTag = "Example"

# attribute constants
fileAttr = "file"
nameAttr = "name"
pathAttr = "path"
langAttr = "lang"
expectedCountAttr = "expectedCount"
typeAttr = "type"
queryAttr = "query"
resultCountsAttr = "resultCounts"

# Traversal States
WaitingForExtFunc = "WaitingForExtensionFunction"
ProcessingExtFunc = "ProcessingExtensionFunction"
ReadingExamples = "RadingExamples"
ReadingXPath = "ReadingXPath"
ReadingName = "ReadingName"
ReadingTags = "ReadingTags"
ReadingDesc = "ReadingDescription"
ReadingLangs = "ReadingLanguages"
ReadingFileInfo = "ReadingFileTag"

languages = set(["C", "C++", "C#", "Java"])

#
#   Takes a file read from the index and parses the XPath data and executes
#   related tests.
#
class XPathExtFuncDocHandler(xml.sax.ContentHandler):
    def __init__(self):
        self.currentXPathExtFunction = None
        self.state = WaitingForExtFunc
        self.tempContentBuffer = ""
        self.docInfo = XPathExtFuncDocInfo()
        self.locator = None
        self.testReports = []


    def endOfMessageFormatting(self):
        return "At line: {0}".format(self.locator.getLineNumber())

    def unexpectedTag(self, name):
        raise Exception("Unexpected Tag named: {0} ".format(name) + self.endOfMessageFormatting())

    def setDocumentLocator(self, docLocator):
        self.locator = docLocator

    def startElement(self, name, attrs):
        if self.state == WaitingForExtFunc:
            if name == XPathExtensionFunctionsTag:
                pass
    
            elif name == ExtFuncTag:
                self.state = ProcessingExtFunc
                self.currentXPathExtFunction = XPExtFuncInfo()
    
            else:
                self.unexpectedTag(name)

        elif self.state == ProcessingExtFunc:
            if name == NameTag:
                self.state = ReadingName
            elif name == LangsTag:
                self.state = ReadingLangs
            elif name == DescTag:
                self.state = ReadingDesc
            elif name == TagsTag:
                self.state = ReadingTags
            elif name == ExamplesTag:
                self.state = ReadingExamples
            else:
                self.unexpectedTag(name)

        elif self.state == ReadingTags:
            if name == TagTag:
                tagNameList = attrs[nameAttr].split(":")
                tempTag = srcMLTag()
                if len(tagNameList) > 1:
                    tempTag.ns = tagNameList[0]
                    if tempTag.ns == "src":
                        tempTag.ns = ""
                tempTag.tag = tagNameList[-1]
                self.currentXPathExtFunction.tags.append(tempTag)
            else:
                self.unexpectedTag(name)
        elif self.state == ReadingExamples:
            if name == ExampleTag:
                # print "Example!"
                nextExample = XPExtFuncExample()
                if "title" in attrs:
                    nextExample.title = attrs["title"]

                nextExample.desc = attrs["desc"]
                nextExample.xpath = attrs["xpath"]
                self.currentXPathExtFunction.examples.append(nextExample)

        elif self.state == ReadingLangs:
            raise Exception("Unexpected tag in state: {0} Received tag: {1}".format(self.state, name) + self.endOfMessageFormatting())
        elif self.state == ReadingXPath:
            raise Exception("Unexpected tag in state: {0} Received tag: {1}".format(self.state, name) + self.endOfMessageFormatting())
        elif self.state == ReadingName:
            raise Exception("Unexpected tag in state: {0} Received tag: {1}".format(self.state, name) + self.endOfMessageFormatting())
        elif self.state == ReadingDesc:
            raise Exception("Unexpected tag in state: {0} Received tag: {1}".format(self.state, name) + self.endOfMessageFormatting())
        else:
            raise Exception("Unknown state: {0}".format(self.state))

    def endElement(self, name):
        if self.state == WaitingForExtFunc:
            pass

        elif self.state == ProcessingExtFunc:
            if name == ExtFuncTag:
                self.state = WaitingForExtFunc
                self.docInfo.extFuncs.append(self.currentXPathExtFunction)
                self.currentXPathExtFunction = None
            else:
                self.unexpectedTag(name)

        elif self.state == ReadingFileInfo:
            if name == FileTag:
                self.state = ReadingExamples

        elif self.state == ReadingExamples:
            if name == ExamplesTag:
                self.state = ProcessingExtFunc
            elif name == ExampleTag:
                pass
            else:
                self.unexpectedTag(name)

        elif self.state == ReadingName:
            self.currentXPathExtFunction.names = self.tempContentBuffer.split(" OR ")
            self.tempContentBuffer = ""
            self.state = ProcessingExtFunc

        elif self.state == ReadingTags:
            if name == TagTag:
                pass
            elif name == TagsTag:
                self.state = ProcessingExtFunc

        elif self.state == ReadingDesc:
            self.currentXPathExtFunction.desc = self.tempContentBuffer
            self.tempContentBuffer = ""
            self.state = ProcessingExtFunc

        elif self.state == ReadingLangs:
            self.currentXPathExtFunction.langs = [l.strip() for l in self.tempContentBuffer.split(",")]
            self.tempContentBuffer = ""
            self.state = ProcessingExtFunc

        else:
            raise Exception("Unknown state: {0}".format(self.state))

    def characters(self, content):
        if self.state == ReadingDesc or self.state == ReadingName or self.state == ReadingXPath or self.state == ReadingLangs:
            self.tempContentBuffer += content
        else:
            pass


def loadXPathExtFuncData(inputFileName):
    handler = XPathExtFuncDocHandler()
    xml.sax.parse(inputFileName, handler)
    # print handler.docInfo
    handler.docInfo.languages = ["C", "C++", "C#", "Java"]
    return handler.docInfo
