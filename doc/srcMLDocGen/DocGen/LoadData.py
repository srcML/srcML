import os, sys, subprocess, string, traceback
import lxml.etree as ET
from DocData import *
from os.path import join, getsize
import cStringIO
from ValidationChecker import *

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
validatorAttr = "validator"
navTitleAttr = "navTitle"
navSubTitleAttr = "navSubTitle"

namespaceDict={
    "src":"http://www.sdml.info/srcML/src",
    "op":"http://www.sdml.info/srcML/operator",
    "cpp":"http://www.sdml.info/srcML/cpp",
    "lit":"http://www.sdml.info/srcML/literal",
    "type":"http://www.sdml.info/srcML/modifier",
}


#
# Error and simple data extraction functions.
#
def formatElementErrorMsg(element):
    return "At line {0.sourceline}, within element: {1.tag}".format(element, element.getparent())


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
        raise Exception("ERROR: missing {0} attribute from {1.tag} tag unable to continue. ".format(attribName, element) + formatElementErrorMsg(element))


def getAttribOrDefault(element, attribName, defaultValue):
    if attribName in element.keys():
        return element.get(attribName)
    else:
        return defaultValue


def invalidDuplicateTag(element):
    raise Exception("ERROR: Unexpected duplicate tag {0.tag}. ".format(element) + formatElementErrorMsg(element))


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
    sourceCode = "".join([x.replace("\t","    ") for x in strm.readlines()])
    srcML = ""
    srcMLProc = subprocess.Popen([srcMLExec, "--language", language, "--literal", "--modifier", "--operator", fileName],  stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    errOutput = srcMLProc.stderr.readlines()
    try:
        return (sourceCode, srcMLProc.stdout.readlines())
    except Exception as ex:
        print "Failed with exception: ", traceback.format_exc(ex)
        print "Caught exception while parsing output from srcML.", ex
        raise

#
# Formatting helper.
#
def extractSubText(element):
    out = cStringIO.StringIO()
    if element.text != None:
        out.write(element.text)
    def extract(currentElement):
        if len(currentElement.attrib) > 0:
            out.write("<{0} {1}>".format(currentElement.tag, " ".join(["{0}=\"{1}\"".format(k, v) for k, v in currentElement.attrib.items()])))
        else:
            out.write("<{0}>".format(currentElement.tag))
        if currentElement.text != None:
            out.write(currentElement.text)
        for chld in currentElement.iterchildren():
            extract(chld)
            if chld.tail != None:
                out.write(chld.tail)
        out.write("</{0}>".format(currentElement.tag))

    for chld in element.iterchildren():
        extract(chld)
        if chld.tail != None:
            out.write(chld.tail)

    return out.getvalue()

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
            element.desc = extractSubText(descElem)
            #descElem.text + "".join([ET.tostring(x) + x.tail for x in descElem.iterdescendants()])

        # Locating all of the tags named reuired markup.
        for reqMarkUpElem in elemElement.findall(ReqsrcMLMarUpOptionTag):
            element.markupOptionsRequired.append(getAttribOrFail(reqMarkUpElem, srcMLOptionAttr))
            # print "Added req markup!", reqMarkUpElem.attrib[srcMLOptionAttr]
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
        try:
            testCompiledXPath = ET.XPath(xpathExample.xpath, namespaces=namespaceDict)
        except Exception as e:
            print e
            print "Title=", xpathNode.xpath("ancestor::DocEntry/@title")[0]
            print "Invalid XPath expression: %s\n" % xpathExample.xpath


        descElem = locateSingleChildOf_Optional(xpathNode, "Desc")
        if descElem != None:
            xpathExample.desc = extractSubText(descElem)
        else:
            raise Exception("ERROR: All XPath Expressions must provide a description. " + formatElementErrorMsg(element))
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
        setValidatorCurrentExampleTitle(example.title)
        fileName = getAttribOrFail(exampleElement, fileAttr)
        setValidatorCurrentExamplePath(fileName)
        if not os.path.isabs(fileName):
            fileName = os.path.join(dirPath, fileName)

        if not os.path.exists(fileName):
            raise Exception("ERROR: Provided example file: {0} does not exist. Unable to continue. ".format(fileName) + formatElementErrorMsg(exampleElement))

        example.sourceCodeFile = fileName
        srcMLExResults = srcMLFile(fileName, doc.srcMLLanguage)
        example.sourceCode = srcMLExResults[0]
        example.srcML = "".join(srcMLExResults[1])
        
        # Attempting to get validator attribute from example
        # validate(ET.ElementTree(ET.fromstring(example.srcML)))
        

        return example

    def buildEntry(entryElement):
        entry = DocEntry()

        # read in attributes.
        entry.title = getAttribOrFail(entryElement, titleAttr)
        setValidatorCurrentDocEntryTitle(entry.title)

        # print entry.title
        entry.shortTitle = getAttribOrDefault(entryElement, shortTilteAttr, "")
        
        for entryPart in entryElement:
            if entryPart.tag == ElementsTag:
                buildElements(entryPart, entry)
            elif entryPart.tag == XPathQueriesTag:
                buildXPathQueries(entryPart, entry)
            elif entryPart.tag == ExampleTag:
                entry.examples.append(buildExample(entryPart))
                incrementValidatorExampleIndex()
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
        if not os.path.isabs(fileName):
            fileName = os.path.join(dirPath, fileName)

        if not os.path.exists(fileName):
            raise Exception("ERROR: Provided example file: {0} does not exist. Unable to continue. ".format(fileName) + formatElementErrorMsg(element) )

        operatorEntry.exampleFile = fileName
        
        srcCodeAndTree = srcMLFile(fileName, doc.srcMLLanguage)
        operatorEntry.sourceCode = srcCodeAndTree[0]
        tempTree = ET.ElementTree(ET.fromstringlist(srcCodeAndTree[1]))
        locatedElements = tempTree.xpath("//src:operator", namespaces={"src": "http://www.sdml.info/srcML/src"})
        if len(locatedElements) != 1:
            if len(locatedElements) > 1:
                matchingLocatedElements = [x for x in locatedElements if x.text == operatorEntry.op]
                if len(matchingLocatedElements) == 1:
                    operatorEntry.srcML = [matchingLocatedElements[0]]
                elif len(matchingLocatedElements) == 0:
                    operatorEntry.srcML = locatedElements
                else:
                    operatorEntry.srcML = ["BUG"]
            else:
                raise Exception("ERROR: Didn't locate correct any operators within the  {0} \n\n{1}".format(srcCodeAndTree[0], srcCodeAndTree[1]))
        else:
            operatorEntry.srcML = locatedElements
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
    setValidatorCurrentDocPath(filePath)
    docFile = ET.parse(filePath)
    doc = DocConfig()

    # Loading and managing documentation xml file XML.
    root = docFile.getroot()

    # testing to make sure that we are handling the correct node.
    verifyNodeNameOrFail(root, DocConfigTag)

    # Getting all of the root attributes.
    doc.title = getAttribOrFail(root, titleAttr)
    setValidatorCurrentDocTitle(doc.title)

    doc.outputFileName = getAttribOrFail(root, outputFileAttr)
    doc.srcMLLanguage = getAttribOrFail(root, langAttr)
    doc.navTitle = getAttribOrDefault(root, navTitleAttr, doc.title)
    doc.navSubTitle = getAttribOrDefault(root, navSubTitleAttr, "Documentation")
    setValidatorCurrentsrcMLLang(doc.srcMLLanguage)


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
            doc.entries.append(buildEntry(elem))
        elif elem.tag == OperatorEntryTag:
            doc.operators.append(buildOperatorEntry(elem))
        else:
            pass
    return doc


LanguageSupportTag = "LanguageSupport"
LanguageTag = "Language"


def loadLanguageSupport(inputFileName):
    tree = ET.parse(inputFileName)

    ret = LanguageSupportInfo() 
    root = tree.getroot()
    verifyNodeNameOrFail(root, LanguageSupportTag)
    ret.outputFile = getAttribOrFail(root, outputFileAttr)


    for elem in root.iterchildren():
        if elem.tag == LanguageTag:
            currentLangInfo = LangInfo()
            
            currentLangInfo.language = getAttribOrFail(elem, langAttr)
            supportLevelXPathResult = elem.xpath("SupportLevel")
            if len(supportLevelXPathResult) != 1:
                raise Exception("Missing or to many SupportLevels specified.")
            currentLangInfo.supportLevel = supportLevelXPathResult[0].text

            linkXPathResult = elem.xpath("Link")
            for l in linkXPathResult:
                currentLangInfo.links.append((getAttribOrFail(l, nameAttr), l.text))

            standardXpathResult = elem.xpath("Standard")
            if len(standardXpathResult) > 0:
                currentLangInfo.standardOrDraft = ("Standard", standardXpathResult[0].text)

            draftXpathResult = elem.xpath("Draft")
            if len(draftXpathResult) > 0:
                currentLangInfo.standardOrDraft = ("Draft", draftXpathResult[0].text)

            descXpathResult = elem.xpath("Desc")
            for descNode in descXpathResult:
                currentLangInfo.desc += extractSubText(descNode)

            ret.supportInfo.append(currentLangInfo)
        else:
            unexpectedOrUnknownTag(elem)
    return ret
