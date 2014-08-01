import os, sys, subprocess, string, traceback
import lxml.etree as ET
from DocData import DocConfig, Element, Subelement, XPathExample, Example, DocEntry, OperatorEntry, Category, DocConfig
from os.path import join, getsize

# Constants
srcMLExec = "srcml"

# Tag Names
DocConfigTag = "DocConfig"
DocEntryTag = "DocEntry"
DescTag = "Desc"
ElementsTag = "Elements"
ElementTag = "Element"
SubelementTag = "Subelement"
ReqsrcMLMarUpOptionTag = "RequiredMarkup"
ExampleTag = "Example"
OperatorEntryTag = "OperatorEntry"
AttrStrTag = "AttribStr"
CategoryTag = "Category"
XPathQueriesTag = "XPathQueries"
XPathTag = "XPath"

# Attribute names
langAttr="lang"
nsAttr = "ns"
nameAttr="name"
outputFileAttr="outputFile"
titleAttr="title"
shortTilteAttr = "shortTitle"
srcMLOptionAttr="srcMLOption"
fileAttr="file"
opAttr = "op"
exampleAttr ="example"
pathAttr = "path"


#
# Error and simple data extraction functions.
#

def formatElementErrorMsg(element):
    return "At line {0.sourceline}, within element: {0.tag}".format(element)

def verifyNodeNameOrFail(element, expectedName):
    if element.tag != expectedName:
        raise Exception("ERROR: Expected element with name: {1}. Actual name: {0.tag}".format(element, expectedName) + formatElementErrorMsg(element))

def unexpectedOrUnknownTag(element):
    if isinstance(element, ET._Comment):
        return
    raise Exception("ERROR: Unexpected/unknown tag: {0.tag}. ".format(element) + formatElementErrorMsg(element) )

# Loads the attributes for the doc
def getAttribOrFail(element, attribName):
    if attribName in element.keys():
        return element.get(attribName)
    else:
        raise Exception("ERROR: missing {0} attribute from {1.tag} tag unable to continue. ".format(attribName, element) + formatElementErrorMsg(element) )

def getAttribOrDefault(element, attribName, defaultValue):
    if attribName in element.keys():
        return element.get(attribName)
    else:
        return defaultValue

def invalidDuplicateTag(element):
    raise Exception("ERROR: Unexpected duplicate tag {0.tag}. ".format(element) + formatElementErrorMsg(element) )

def locateSingleChildOf_Optional(element, childTagName):
    childElements = element.findall("./{0}".format(childTagName))

    if childElements == None:
        return None
    elif len(childElements) > 1:
        invalidDuplicateTag(childElements[1])
    elif len(childElements) == 0:
        return None
    else:
        return childElements[0]


def srcMLFile(fileName, language):
    strm = open(fileName, "r")
    sourceCode = "".join(strm.readlines())
    srcML = ""
    srcMLProc = subprocess.Popen([srcMLExec, "--language", language, "--literal", "--modifier", "--operator", fileName],  stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    errOutput = srcMLProc.stderr.readlines()
    try:
        tempTree = ET.ElementTree(ET.fromstringlist(srcMLProc.stdout.readlines()))
        root = tempTree.getroot()

        if root.text != None:
            srcML += root.text
        for x in root.iterdescendants():
            srcML+= ET.tostring(x)
            if x.tail != None:
                srcML += x.tail
        # srcML += "".join([ + x.tail ])

        return (sourceCode, srcML)
    except Exception as ex:
        print "Failed with exception: ", traceback.format_exc(ex)
        print "Caught exception while parsing output from srcML.", ex
        raise


#
# Info building from here on down.
#
def loadXmlDocFile(dirPath, fileName, forceBuild = False):


    def buildElement(elemElement, elementInput):
        element = elementInput

        # Reading attributes
        element.tag = getAttribOrFail(elemElement, nameAttr)
        element.ns = getAttribOrDefault(elemElement, nsAttr, "")

        if len(elemElement) == 0:
            return element

        # Getting attribute string,
        attrStrTag = locateSingleChildOf_Optional(elemElement, AttrStrTag)
        if attrStrTag != None:
            element.attributeStr = attrStrTag.text + "".join([ET.tostring(x) + x.tail for x in attrStrTag.iterdescendants()])

        # descElem = elemElement.find(DescTag)
        descElem = locateSingleChildOf_Optional(elemElement, "Desc")
        if descElem != None:
            element.desc = descElem.text + "".join([ET.tostring(x) + x.tail for x in descElem.iterdescendants()])

        # Locating all of the tags named reuired markup.
        for reqMarkUpElem in elemElement.findall(ReqsrcMLMarUpOptionTag):
            element.markupOptionsRequired.append(getAttribOrFail(reqMarkUpElem, srcMLOptionAttr))
        return element


    def buildElements(elemElements, entry):
        for elem in elemElements.iterchildren():
            if elem.tag == ElementTag:
                entry.elements.append( buildElement(elem, Element()))
            elif elem.tag == SubelementTag:
                entry.subelements.append( buildElement(elem, Subelement()))
            else:
                unexpectedOrUnknownTag(elem)


    def buildXPathExample(xpathNode):
        xpathExample = XPathExample()
        xpathExample.xpath = getAttribOrFail(xpathNode, pathAttr)
        descElem = locateSingleChildOf_Optional(xpathNode, "Desc")
        if descElem != None:
            xpathExample.desc = descElem.text + "".join([ET.tostring(x) + x.tail for x in descElem.iterdescendants()])
        else:
            raise Exception("ERROR: All XPath Expressions must provide a description. " + formatElementErrorMsg(element) )
        return xpathExample



    def buildXPathQueries(queryElement, entry):
        for xpElem in queryElement.iterchildren():
            if xpElem.tag == XPathTag:
                entry.xpathQueries.append(buildXPathExample(xpElem))
            else:
                unexpectedOrUnknownTag(xpElem)


    def buildExample(exampleElement):
        example = Example()

        # reading Example attributes
        example.title = getAttribOrDefault(exampleElement, titleAttr, "")
        fileName = getAttribOrFail(exampleElement, fileAttr)
        if not os.path.isabs(fileName):
            fileName = os.path.join(dirPath, fileName)

        if not os.path.exists(fileName):
            raise Exception("ERROR: Provided example file: {0} does not exist. Unable to continue. ".format(fileName) + formatElementErrorMsg(element) )
        example.sourceCodeFile = fileName
        srcMLExResults = srcMLFile(fileName, doc.srcMLLanguage)
        example.sourceCode = srcMLExResults[0]
        example.srcML = srcMLExResults[1]
        return example

    def buildEntry(entryElement):
        entry = DocEntry()

        # read in attributes.
        entry.title = getAttribOrFail(entryElement, titleAttr)
        entry.shortTitle = getAttribOrDefault(entryElement, shortTilteAttr, "")
        
        for entryPart in entryElement:
            if entryPart.tag == ElementsTag:
                buildElements(entryPart, entry)
            elif entryPart.tag == XPathQueriesTag:
                buildXPathQueries(entryPart, entry)
            elif entryPart.tag == ExampleTag:
                example = buildExample(entryPart)
                example.index = len(entry.examples)
                entry.examples.append(example)
            elif entryPart.tag == DescTag:
                entry.desc = entryPart.text + "".join([ET.tostring(x) + x.tail for x in entryPart.iterdescendants()])
            else:
                unexpectedOrUnknownTag(entryPart)
        return entry
        

    def buildOperatorEntry(opElement):
        operatorEntry = OperatorEntry()
        fileName = getAttribOrFail(opElement, exampleAttr)
        operatorEntry.op = getAttribOrFail(opElement, opAttr)
        operatorEntry.title = getAttribOrFail(opElement, titleAttr)
        
        # <OperatorEntry example="C-deref-member-access.c" op="-&gt;" title="Arrow" />
        # fileName = getAttribOrFail(exampleElement, fileAttr)
        if not os.path.isabs(fileName):
            fileName = os.path.join(dirPath, fileName)

        if not os.path.exists(fileName):
            raise Exception("ERROR: Provided example file: {0} does not exist. Unable to continue. ".format(fileName) + formatElementErrorMsg(element) )

        operatorEntry.exampleFile = fileName
        
        strm = open(fileName, "r")
        sourceCode = "".join(strm.readlines())
        srcML = ""
        srcMLProc = subprocess.Popen([srcMLExec, "--language", doc.srcMLLanguage, "--literal", "--modifier", "--operator", fileName],  stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        errOutput = srcMLProc.stderr.readlines()
        tempTree = None
        try:
            tempTree = ET.ElementTree(ET.fromstringlist(srcMLProc.stdout.readlines()))
            root = tempTree.getroot()

            if root.text != None:
                srcML += root.text
            for x in root.iterdescendants():
                srcML+= ET.tostring(x)
                if x.tail != None:
                    srcML += x.tail
        except Exception as ex:
            print "Failed with exception: ", traceback.format_exc(ex)
            print "Caught exception while parsing output from srcML.", ex
            raise
        operatorEntry.sourceCode = sourceCode

        locatedElements = tempTree.xpath("//op:operator",namespaces={"op": "http://www.sdml.info/srcML/operator"})
        if len(locatedElements) != 1:
            if len(locatedElements) > 1:
                matchingLocatedElements = [x for x in locatedElements if x.text == operatorEntry.op]
                if len(matchingLocatedElements) == 1:
                    operatorEntry.srcML = [ET.tostring(matchingLocatedElements[0])]
                elif len(matchingLocatedElements) == 0:
                    operatorEntry.srcML = [ET.tostring(elem) for elem in locatedElements]
                else:
                    operatorEntry.srcML = "BUG"
            else:
                raise Exception("ERROR: Didn't locate correct any operators within the  {0} ".format(allSrcMLCode) + self.makeAtLineCol())
        return operatorEntry


    def buildCategory(catElement):
        # print "Loading category"
        category = Category()

        # Loading Catrgory attributes.
        category.title = getAttribOrFail(catElement, titleAttr)

        # Loading all doc entries into category.
        for entry in catElement.iterchildren():
            if entry.tag == DocEntryTag:
                category.entries.append(buildEntry(entry))
            elif entry.tag == OperatorEntryTag:
                category.operators.append(buildOperatorEntry(entry))
            else:
                unexpectedOrUnknownTag(entry)
        return category
    

    # Basic set up.
    filePath = join(dirPath, fileName)
    docFile = ET.parse(filePath)
    doc = DocConfig()

    # Loading and managing documentation xml file XML.
    root = docFile.getroot()

    # testing to make sure that we are handling the correct node.
    verifyNodeNameOrFail(root, DocConfigTag)

    # Getting all of the root attributes.
    doc.title = getAttribOrFail(root, titleAttr)
    doc.outputFileName = getAttribOrFail(root, outputFileAttr)
    doc.srcMLLanguage = getAttribOrFail(root, langAttr)

    if (os.path.exists(doc.outputFileName)
             and os.path.getmtime(filePath) < os.path.getmtime(doc.outputFileName)
            and not forceBuild):
        print "    Up to date."
        return None


    # attempting to locate description for entire page.
    descElem = locateSingleChildOf_Optional(root, "Desc")
    if descElem != None:
        doc.desc = descElem.text + "".join([ET.tostring(x) + x.tail for x in descElem.iterdescendants()])


    # Traversing all documentation entries.
    for elem in root.iterchildren():
        if elem.tag == CategoryTag:
            doc.categories.append(buildCategory(elem))
        elif elem.tag == DocEntryTag:
            doc.categories.append(buildEntry(elem))
        elif elem.tag == OperatorEntryTag:
            doc.operators.append(buildOperatorEntry(elem))
        else:
            pass
    return doc