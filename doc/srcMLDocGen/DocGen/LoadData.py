import os, sys, subprocess, string, traceback
import lxml.etree as ET
from DocData import *
from os.path import join, getsize
import cStringIO

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
            textCompiledXPath = ET.XPath(xpathExample.xpath, namespaces=namespaceDict)
        except Exception as e:
            print e
            print "Title=", xpathNode.xpath("ancestor::DocEntry/@title")[0]
            print "Invalid XPath expression: %s\n" % xpathExample.xpath


        descElem = locateSingleChildOf_Optional(xpathNode, "Desc")
        if descElem != None:
            xpathExample.desc =extractSubText(descElem)
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
        fileName = getAttribOrFail(exampleElement, fileAttr)
        if not os.path.isabs(fileName):
            fileName = os.path.join(dirPath, fileName)

        if not os.path.exists(fileName):
            raise Exception("ERROR: Provided example file: {0} does not exist. Unable to continue. ".format(fileName) + formatElementErrorMsg(exampleElement))

        example.sourceCodeFile = fileName
        srcMLExResults = srcMLFile(fileName, doc.srcMLLanguage)
        example.sourceCode = srcMLExResults[0]
        example.srcML = "".join(srcMLExResults[1])
        return example

    def buildEntry(entryElement):
        entry = DocEntry()

        # read in attributes.
        entry.title = getAttribOrFail(entryElement, titleAttr)
        # print entry.title
        entry.shortTitle = getAttribOrDefault(entryElement, shortTilteAttr, "")
        
        for entryPart in entryElement:
            if entryPart.tag == ElementsTag:
                buildElements(entryPart, entry)
            elif entryPart.tag == XPathQueriesTag:
                buildXPathQueries(entryPart, entry)
            elif entryPart.tag == ExampleTag:
                entry.examples.append(buildExample(entryPart))
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
            doc.entries.append(buildEntry(elem))
        elif elem.tag == OperatorEntryTag:
            doc.operators.append(buildOperatorEntry(elem))
        else:
            pass
    return doc


# TagDoc specific tag names
TagDocTag = "TagDoc"
NamespacesTag = "Namespaces"
NsTag = "Ns"
CaseTag = "Case"
ExamplesTag = "Examples"
LanguagesTag = "Languages"
LangTag = "Lang"
RefsTag = "Refs"
SeeTag = "See"
AttributesTag = "Attributes"
AttrTag = "Attr"
GrammarTag = "Grammar"
SubelementsTag = "Subelements"
TerminalCaseTag = "TerminalCase"
ValuesTag = "Values"
ValueTag = "Value"
RelaxNGHelpTag = "RelaxNGHelp"
EntryTag = "Entry"
GrammarTextTag = "GrammarText"

# TagDoc specific attribute names
fileNameAttr = "fileName"
prefixAttr = "prefix"
urlAttr = "url"
languageAttr = "language"
valueDescAttr = "valueDesc"
elemAttr = "elem"

def loadTagDoc(dirPath, fileName, forceBuild = False):
    """ load the tag documentation file. """

    def buildRef(refElement):
        reference = None
        if titleAttr in refElement.attrib and urlAttr in refElement.attrib:
            reference = URLRefLink()
            reference.title = getAttribOrFail(refElement, titleAttr)
            reference.url = getAttribOrFail(refElement, urlAttr)
        else:
            raise Exception("Malformed See reference. " + formatElementErrorMsg(refElement))
        return reference


    def buildRefList(refsElement):
        refList = []
        for ref in refsElement.iterchildren():
            if ref.tag == SeeTag:
                refList.append(buildRef(ref))
            else:
                unexpectedOrUnknownTag(ref)
        return refList

    def buildExample(exampleElement):
        tagExample = TagExample()
        tagExample.title = getAttribOrDefault(exampleElement, titleAttr, "")
        tagExample.text = extractSubText(exampleElement)
        return tagExample

    def buildExamplesList(examplesElement):
        exampleList = []
        for ex in examplesElement.iterchildren():
            if ex.tag == ExampleTag:
                exampleList.append(buildExample(ex))
            else:
                unexpectedOrUnknownTag(ex)
        return exampleList

    def buildAttr(attrElement):
        attr = TagAttr()
        attr.name = getAttribOrFail(attrElement, nameAttr)
        attr.valueDesc = getAttribOrFail(attrElement, valueDescAttr)

        for elem in attrElement.iterchildren():
            if elem.tag == DescTag:
                attr.desc = extractSubText(elem)

            elif elem.tag == LanguagesTag:
                attr.languages = buildLanguageList(elem)

            elif elem.tag == ExamplesTag:
                attr.examples = buildExamplesList(elem)

            else:
                unexpectedOrUnknownTag(elem)
        return attr

    def buildAttrList(attrElement):
        attrList = []
        for elem in attrElement.iterchildren():
            if elem.tag == AttrTag:
                attrList.append(buildAttr(elem))
            else:
                unexpectedOrUnknownTag(elem)

        return attrList

    def buildLanguageList(languagesElement):
        langList = []
        for elem in languagesElement:
            if elem.tag == LangTag:
                langList.append(getAttribOrFail(elem, languageAttr))
            else:
                unexpectedOrUnknownTag(elem)
        return langList

    def buildTerminalCase(caseElement):
        terminalCase = TerminalUseCase()
        terminalCase.title = getAttribOrDefault(caseElement, titleAttr, "")
        for elem in caseElement.iterchildren():
            if elem.tag == DescTag:
                terminalCase.desc = extractSubText(elem)
            elif elem.tag == ValuesTag:
                for valElem in elem.iterchildren():
                    if valElem.tag == ValueTag:
                        termValue = TerminalValue()
                        termValue.value = extractSubText(valElem)
                        termValue.language = getAttribOrFail(valElem, langAttr).split(",")
                    else:
                        unexpectedOrUnknownTag(elem)

            elif elem.tag == LanguagesTag:
                terminalCase.languages = buildLanguageList(elem)

            elif elem.tag == AttributesTag:
                terminalCase.attrs = buildAttrList(elem)

            elif elem.tag == ExamplesTag:
                terminalCase.examples = buildExamplesList(elem)

            elif elem.tag == GrammarTag:
                terminalCase.grammar = extractSubText(elem).strip()

            else:
                unexpectedOrUnknownTag(elem)
        return terminalCase        

    def buildCase(caseElement):
        useCase = TagUseCase()
        useCase.title = getAttribOrDefault(caseElement, titleAttr, "")
        for elem in caseElement.iterchildren():
            if elem.tag == DescTag:
                useCase.desc = extractSubText(elem)

            elif elem.tag == LanguagesTag:
                useCase.languages = buildLanguageList(elem)

            elif elem.tag == AttributesTag:
                useCase.attrs = buildAttrList(elem)

            elif elem.tag == ExamplesTag:
                useCase.examples = buildExamplesList(elem)

            elif elem.tag == GrammarTag:
                useCase.grammar = extractSubText(elem).strip()

            elif elem.tag == SubelementsTag:
                for subElem in elem.iterchildren():
                    if subElem.tag == SubelementTag:
                        gatheredSubelement = TagRef()
                        gatheredSubelement.ns = getAttribOrDefault(subElem, nsAttr, "")
                        gatheredSubelement.tag = getAttribOrFail(subElem, nameAttr)
                        useCase.subelements.append(gatheredSubelement)
                    else:
                        unexpectedOrUnknownTag(subElem)
            else:
                unexpectedOrUnknownTag(elem)
        return useCase

    def buildElement(elemElement):
        tagInfo = TagInfo()

        # getting attributes.
        tagInfo.ns = getAttribOrFail(elemElement, nsAttr)
        tagInfo.tag = getAttribOrFail(elemElement, elemAttr)

        # Gather element stuffs.
        for elem in elemElement.iterchildren():
            if elem.tag == DescTag:
                tagInfo.desc = extractSubText(elem)

            elif elem.tag == CaseTag:
                tagInfo.useCases.append(buildCase(elem))

            elif elem.tag == TerminalCaseTag:
                tagInfo.useCases.append(buildTerminalCase(elem))

            elif elem.tag == RefsTag:
                tagInfo.refs = buildRefList(elem)

            else:
                unexpectedOrUnknownTag(elem)

        # Deducing list of subelements, languages
        subElementNameSet = set()
        languageSet = set()

        for useCase in tagInfo.useCases:
            if not isinstance(useCase, TerminalUseCase):
                for subElem in useCase.subelements:
                    if not subElem.comparableTagName() in subElementNameSet:
                        subElementNameSet.add(subElem.comparableTagName())
                        tagInfo.subelements.append(subElem)
            for lang in useCase.languages:
                if not lang in languageSet:
                    languageSet.add(lang)
        tagInfo.languages = list(languageSet)
        elementLookupDictionary.update({tagInfo.comparableTagName(): tagInfo})
        return tagInfo


    def buildElementsList(elementsElement):
        elemList = []
        for elem in elementsElement.iterchildren():
            if elem.tag == ElementTag:
                elemList.append(buildElement(elem))
            else:
                unexpectedOrUnknownTag(elem)

        # Deducing parent elements.
        for tagInfo in elemList:
            for parentElement in tagInfo.subelements:
                if parentElement.comparableTagName() in elementLookupDictionary:
                    # tempRef = TagRef()
                    # tempRef.ns = tagInfo.ns
                    # tempRef.tag = tagInfo.tag
                    elementLookupDictionary[parentElement.comparableTagName()].parentElements.append(tagInfo)
                else:
                    print "Didn't locate entry for element: {0}".format(parentElement.comparableTagName())

        return elemList

    def buildNs(nsElement):
        ns = NsEntry()
        ns.prefix = getAttribOrFail(nsElement, prefixAttr)
        ns.url = getAttribOrFail(nsElement, urlAttr)
        descElem = nsElement.xpath(DescTag)

        descElem = locateSingleChildOf_Optional(nsElement, DescTag)
        if descElem != None:
            ns.desc = extractSubText(descElem)
        return ns

    def buildNamespaceList(namespacesElement):
        nsList = []
        for nsElem in namespacesElement.iterchildren():
            if nsElem.tag == NsTag:
                nsList.append(buildNs(nsElem))
            else:
                unexpectedOrUnknownTag(nsElem)
        return nsList

    def buildRelaxNGEntry(entryElement):
        entry = RelaxNGEntry()
        entry.title = getAttribOrFail(entryElement, titleAttr)
        for elem in entryElement.iterchildren():
            if elem.tag == DescTag:
                entry.desc = extractSubText(elem)
        
            elif elem.tag == GrammarTextTag:
                entry.grammarText = extractSubText(elem)
        
            else:
                unexpectedOrUnknownTag(elem)
        return entry

    def buildRelaxNGHelp(relaxNGHelpElement):
        relaxNGDoc = RelaxNGHelpDoc()
        relaxNGDoc.title = getAttribOrFail(relaxNGHelpElement, titleAttr)
        for elem in relaxNGHelpElement.iterchildren():
            if elem.tag == DescTag:
                relaxNGDoc.desc = extractSubText(elem)

            elif elem.tag == EntryTag:
                relaxNGDoc.entries.append(buildRelaxNGEntry(elem))

            else:
                unexpectedOrUnknownTag(nsElem)
        return relaxNGDoc

    # Basic set up.
    filePath = join(dirPath, fileName)
    docFile = ET.parse(filePath)
    tagDoc = TagDoc()

    # Loading and managing documentation xml file XML.
    root = docFile.getroot()

    # testing to make sure that we are handling the correct node.
    verifyNodeNameOrFail(root, TagDocTag)

    # Getting all of the root attributes.
    tagDoc.title = getAttribOrFail(root, titleAttr)
    tagDoc.outputFileName = getAttribOrFail(root, fileNameAttr)

    if (os.path.exists(tagDoc.outputFileName)
            and os.path.getmtime(filePath) < os.path.getmtime(tagDoc.outputFileName)
            and not forceBuild):
        print "    Up to date."
        return None

    elementLookupDictionary = dict()

    # Traversing all documentation entries.
    for elem in root.iterchildren():
        if elem.tag == DescTag:
            tagDoc.desc = extractSubText(elem)
        elif elem.tag == RelaxNGHelpTag:
            tagDoc.relaxNGHelp = buildRelaxNGHelp(elem)
        elif elem.tag == NamespacesTag:
            tagDoc.namespaces = buildNamespaceList(elem)

        elif elem.tag == ElementsTag:
            tagDoc.tags = buildElementsList(elem)
        else:
            unexpectedOrUnknownTag(elem)
    return tagDoc
