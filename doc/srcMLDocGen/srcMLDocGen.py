#!/usr/bin/python
#   srcMLDocGen is used to generate markdown files from srcML so that they can be
#   easily formatting and made to look nice without a lot of effort.
#
#   use grip to evaluate the created.md files.

import os, sys, xml.sax, subprocess, xml.dom.minidom as minidom, traceback
from xml.dom.minidom import parse
from os.path import join, getsize
import xml.etree.ElementTree as ET
import argparse
import grip

class Element(object):
    """ Stores information about tags related to this description. """
    def __init__(self):
        self.tag = ""
        self.ns = ""
        self.desc = ""
        self.markupOptionsRequired = []
        self.attributeStr = ""

    def getQName(self):
        nameFormat = ""
        if self.ns == "":
            nameFormat = "&lt;{0.tag}"
        else:
            nameFormat = "&lt;{0.ns}:{0.tag}"

        if self.attributeStr != "":
            nameFormat += " {0.attributeStr}"
        nameFormat += "&gt;"
        return nameFormat.format(self)

    def genMarkDown(self, mdStrm):

        mdStrm.write("* **{0}** \n".format(self.getQName(), self))
        if self.desc != "":
            mdStrm.write("     * {0.desc}\n".format(self))
        if len(self.markupOptionsRequired) > 0:
            mdStrm.write("     * **Note**: This mark up is disabled by default and is enabled using the following command line argument/s with srcML: \n{0}\n".format("\n".join(["        * {0}".format(markupOpt) for markupOpt in self.markupOptionsRequired] ) ))


class Subelement(Element):
    """ Stores information related to sub elements. """
    def __init__(self):
        super(Subelement, self).__init__()

class XPathExample:
    def __init__(self):
        self.desc = ""
        self.xpath = ""

    def genMarkdown(self, mdStrm):
        totalIndentation = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
        subindentation = "&nbsp;&nbsp;&nbsp;&nbsp;"
        mdStrm.write(
"""
{1}**Description:**<br/>
{1}{2}{0.desc}<br/>
{1}**XPath**<br/>
{1}{2}`{0.xpath}`<br/>
""".format(self, totalIndentation, subindentation))


class Example:
    def __init__(self):
        self.title = ""
        self.desc = ""
        self.sourceCodeFile = "" 
        self.sourceCode = ""
        self.srcML = ""
        self.index = 0
        self.writeIndex = False
        self.srcMLLanguage = "C++"
        self.xpathExamples = []

    def genMarkDown(self, mdStrm):

        # Logic
        #   If write index 
        #       If title
        #           write it.
        #       else
        #           write index
        #   else
        #       if title
        #           write it
        #       else
        #           write nothing.
        titleFormat = """
#### {0.title}
"""
        if self.writeIndex:
            if self.title != "":
                mdStrm.write(titleFormat.format(self))
            else:
                mdStrm.write("\n#### {0}\n".format(self.index))
        else:
            if self.title != "":
                mdStrm.write(titleFormat.format(self))

        if self.desc != "":
            mdStrm.write("""
{0.desc}
""".format(self))

        mdStrm.write("""
```{0.srcMLLanguage}
{0.sourceCode}
```
```XML
{0.srcML}
```

""".format(self))
        if len(self.xpathExamples) > 0:
            if len(self.xpathExamples) == 1:
                mdStrm.write("####XPath Query Example\n")
            else:
                mdStrm.write("####XPath Query Examples\n")
            for xpathExample in self.xpathExamples:
                xpathExample.genMarkdown(mdStrm)
            mdStrm.write("<br/>")

class DocEntry:
    """ This is an entry within the documentation. """
    def __init__(self):
        self.title = ""
        self.shortTitle = ""
        self.desc = ""
        self.elements = []
        self.subelements = []
        self.examples = []



    def genMarkDown(self, mdStrm):
        temp = """
## {0.title}

{0.desc}
"""
        if self.desc == "":
            temp ="""
## {0.title}
"""

        mdStrm.write(temp.format(self))
        # Writing Element/s title.
        if len(self.elements) > 1:
            mdStrm.write("""### Elements
""")
        else:
            mdStrm.write("""### Element
""")
        # Wring sub-element
        for tag in self.elements:
            tag.genMarkDown(mdStrm)

        if len(self.subelements) > 0:
            mdStrm.write("\n")
            if len(self.subelements) > 1:
                mdStrm.write("### SubElements\n")
            else:
                mdStrm.write("### SubElement\n")
            for subelem in self.subelements:
                subelem.genMarkDown(mdStrm)

        writeMultipleExamples = len(self.examples) > 1
        if writeMultipleExamples:
            mdStrm.write("""

### Examples
""")
        else:
            mdStrm.write("""

### Example
""")
        exampleIndex = 0
        for example in self.examples:

            if writeMultipleExamples:
                exampleIndex += 1
                example.index = exampleIndex
                example.writeIndex = True
            example.genMarkDown(mdStrm)



class OperatorEntry:
    """ This is an operator entry within the document that tell me to extract
        an operator from the srcML example.
    """
    def __init__(self):
        self.title = ""
        self.op = ""
        self.exampleFile = ""
        self.XPath = "//op:operator"
        self.srcML = ""
        self.srcMLExpr = ""
        self.sourceCode = ""


class Category:
    def __init__(self):
        self.title = ""
        self.entries = []


class DocConfig:
    """ Manages all pages related to a single topic/language."""
    def __init__(self):
        self.srcMLLanguage = "NOT SET"
        self.title = ""
        self.outputFileName = ""
        self.categories = []
        self.entries = []
        self.desc = ""
        self.operators = []

    tableSubHeader = """</tbody><thead>
        <tr>
            <th align="left" colspan="3"><b>{0}</b></th>
        </tr>
        </thead><tbody>
"""
    tableRow = """
        <tr>
            <td><a href="#{0}">{1}</a> </td>
            <td>{2}</td>
            <td>{3}</td>
        </tr>
"""
    def buildIndexTable(self, mdStrm):
        mdStrm.write("""
<table>
    <thead>
        <tr>
            <th>Language Element</th>
            <th>Elements</th>
            <th>Subelements</th>
        </tr>
    </thead>
    <tbody>
""")
        for cat in self.categories:
            mdStrm.write(DocConfig.tableSubHeader.format(cat.title))
            for entry in cat.entries:
                mdStrm.write(DocConfig.tableRow.format(
                    reformatTitleForAnchor(entry.title),
                    entry.shortTitle,
                    ", ".join([elem.getQName() for elem in entry.elements]),
                    ", ".join([subelem.getQName() for subelem in entry.subelements])
                ))

        if len(self.entries) > 0:
            mdStrm.write(DocConfig.tableSubHeader.format("FIXME: Not sorted!"))
            for entry in self.entries:
                mdStrm.write(DocConfig.tableRow.format(
                    reformatTitleForAnchor(entry.title),
                    entry.shortTitle,
                    ", ".join([elem.getQName() for elem in entry.elements]),
                    ", ".join([subelem.getQName() for subelem in entry.subelements])
                ))

        if len(self.operators) > 0:
            mdStrm.write(DocConfig.tableSubHeader.format("Operators"))
            mdStrm.write(DocConfig.tableRow.format(
                "operators",
                "operators",
                "&lt;op:operator&gt;",
                ""
            ))

        mdStrm.write("</tbody></table>\n")

    def genOperatorTable(self, mdStrm):
        mdStrm.write("""

## Operators
Name | Operator | srcML
--- | --- | ---
""")
        for opEntry in self.operators:
            if opEntry.op.find("|") != -1:
                mdStrm.write("{0} | <code>{1}</code> | <code>{2}</code>\n".format(
                    opEntry.title,
                    opEntry.op.replace("|","&#124;").replace("<","&lt;").replace(">","&gt;"),
                    opEntry.srcML.replace("|","&#124;").replace("<","&lt;").replace(">","&gt;")
                ))
            else:
                mdStrm.write("{0} | `{1}` | `{2}`\n".format(opEntry.title , opEntry.op, opEntry.srcML))
        mdStrm.write("\n**NOTE:** Operator mark up within srcML is not enabled by default and srcML must be run with the --operator command line option to enable it.")

    def genMarkDown(self):
        mdStrm = open(self.outputFileName, 'w')


        tempOutputstr = ""
        if self.desc != "":
            # Writing navigation links.
            tempOutputstr = """
#{0.title}
{0.desc}
"""
        else:
            tempOutputstr = """
#{0.title}
"""

        mdStrm.write(tempOutputstr.format(self))
        self.buildIndexTable(mdStrm)

        for cat in self.categories:
            for entry in cat.entries:
                entry.genMarkDown(mdStrm)

        for entry in self.entries:
            entry.genMarkDown(mdStrm)

        if len(self.operators) > 0:
            self.genOperatorTable(mdStrm)

        mdStrm.flush()
        mdStrm.close()
        grip.export(path=self.outputFileName, out_filename=self.outputFileName+".html", username="JohnQPublic", password="SuperSweetPassword78")



def reformatTitleForAnchor(strToProcess):
    return "-".join(strToProcess.strip().replace("#","").replace(",", "").lower().split())


class DocConfigDataHandler(xml.sax.ContentHandler):
    
    # paring states
    S_PAGE = 0
    S_DOC_ENTRY = 1
    S_ELEMENTS = 2
    S_ELEMENT = 3
    S_EXAMPLE = 4
    S_SUBELEMENT = 5
    S_XPATH = 6

    def __init__(self):
        self.doc = DocConfig()
        self.category = None
        self.entry = None
        self.element = None
        self.subelement = None
        self.example = None
        self.docLocator = None
        self.xpathExample = None
        self.stateStack = []
        self.recordNextCharacters = False
        self.recordedCharacters = ""
        self.dataFolder = ""
    
    
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
    XPathTag = "XPath"


    # Attribute names
    langAttr="lang"
    outputFileAttr="outputFile"
    titleAttr="title"
    shortTilteAttr = "shortTitle"
    srcMLNSAttr="ns"
    srcMLNameAttr="name"
    srcMLOptionAttr="srcMLOption"
    fileAttr="file"
    opAttr = "op"
    exampleAttr ="example"
    pathAttr = "path"


    def getAttrFailOnMissing(self, tagName, attrName, attrs):
        if attrName in attrs:
            return attrs[attrName]
        else:
            raise Exception("ERROR: missing " + attrName + " attribute from " + tagName + " tag unable to continue. " + self.makeAtLineCol() )

    def getAttrDefaultOnMissing(self, tagName, attrName, attrs, defaultValue):
        if attrName in attrs:
            return attrs[attrName]
        else:
            return defaultValue
    

    def setDocumentLocator(self, docLocator):
        self.docLocator = docLocator

    def makeAtLineCol(self):
        return "At Line " + str(self.docLocator.getLineNumber()) + " Column: " + str(self.docLocator.getColumnNumber())

    def startElement(self, name, attrs):
        if name == DocConfigDataHandler.DocConfigTag:
            # Preconditions
            assert len(self.stateStack) == 0, "Invalid XML format can NOT have multiple nested pages"

            self.stateStack.append(DocConfigDataHandler.S_PAGE)

            if DocConfigDataHandler.titleAttr in attrs:
                self.doc.title = attrs[DocConfigDataHandler.titleAttr]
            else:
                print "WARNING: DocConfig is missing title attribute. " + self.makeAtLineCol() 

            if DocConfigDataHandler.langAttr in attrs:
                self.doc.srcMLLanguage = attrs[DocConfigDataHandler.langAttr]
            else:
                self.doc.srcMLLanguage = "C++"
                print "WARNING: Page is missing lang attribute, Defaulting to C++. " + self.makeAtLineCol() 
            
            self.doc.outputFileName = self.getAttrFailOnMissing(name, DocConfigDataHandler.outputFileAttr, attrs)
            
            # Postconditions
            assert len(self.stateStack) == 1, "Invalid XML format can NOT have multiple nested pages"


        elif name == DocConfigDataHandler.CategoryTag:
            self.category = Category()

            # Collecting title attribute
            self.category.title = self.getAttrFailOnMissing(name, DocConfigDataHandler.titleAttr, attrs)


        elif name == DocConfigDataHandler.DocEntryTag:
            self.stateStack.append(DocConfigDataHandler.S_DOC_ENTRY)

            # Create and load document entry.
            self.entry = DocEntry()
            
            # Gathering attribute related data.
            if DocConfigDataHandler.titleAttr in attrs:
                self.entry.title = attrs[DocConfigDataHandler.titleAttr]
            else:
                raise Exception("ERROR: Missing title attribute for DocEntry. " + self.makeAtLineCol())

            self.entry.shortTitle = self.getAttrDefaultOnMissing(name, DocConfigDataHandler.shortTilteAttr, attrs, attrs[DocConfigDataHandler.titleAttr])
            assert self.entry.shortTitle != "", "Didn't correctly set short title."

        elif name == DocConfigDataHandler.DescTag:

            self.recordedCharacters = ""
            self.recordNextCharacters = True



        elif name == DocConfigDataHandler.ElementsTag:            
            self.stateStack.append(DocConfigDataHandler.S_ELEMENTS)
            



        elif name == DocConfigDataHandler.ElementTag:

            self.stateStack.append(DocConfigDataHandler.S_ELEMENT)

            # Creating related tag info
            self.element = Element()
            
            # Handling attributes.
            if DocConfigDataHandler.srcMLNSAttr in attrs:
                self.element.ns = attrs[DocConfigDataHandler.srcMLNSAttr]

            if DocConfigDataHandler.srcMLNameAttr in attrs:
                self.element.tag = attrs[DocConfigDataHandler.srcMLNameAttr]
            else:
                raise Exception("ERROR: Missing srcMLName attribute from Tag tag unable to continue. " + self.makeAtLineCol())



        elif name == DocConfigDataHandler.SubelementTag:
            self.stateStack.append(DocConfigDataHandler.S_SUBELEMENT)

            # Creating related tag info
            self.subelement = Subelement()
            if DocConfigDataHandler.srcMLNSAttr in attrs:
                self.subelement.ns = attrs[DocConfigDataHandler.srcMLNSAttr]


            if DocConfigDataHandler.srcMLNameAttr in attrs:
                self.subelement.tag = attrs[DocConfigDataHandler.srcMLNameAttr]
            else:
                raise Exception("ERROR: Missing srcMLName attribute from Tag tag unable to continue. " + self.makeAtLineCol())



        elif name == DocConfigDataHandler.AttrStrTag:
            self.recordedCharacters = ""
            self.recordNextCharacters = True



        elif name == DocConfigDataHandler.ReqsrcMLMarUpOptionTag:

            if DocConfigDataHandler.srcMLOptionAttr in attrs:
                if self.element != None:
                    self.element.markupOptionsRequired.append(attrs[DocConfigDataHandler.srcMLOptionAttr])
                else:
                    self.subelement.markupOptionsRequired.append(attrs[DocConfigDataHandler.srcMLOptionAttr])
            else:
                raise Exception("ERROR: Missing srcMLOption attribute from ReqsrcMLMarUpOption tag unable to continue. " + self.makeAtLineCol())




        elif name == DocConfigDataHandler.ExampleTag:
            self.stateStack.append(DocConfigDataHandler.S_EXAMPLE)
            self.example = Example()
            self.example.srcMLLanguage = self.doc.srcMLLanguage

            #Handling the reading of attributes
            if DocConfigDataHandler.titleAttr in attrs:
                self.example.title = attrs[DocConfigDataHandler.titleAttr]

            # Handling in the locating of the file attribute.
            if DocConfigDataHandler.fileAttr in attrs:
                exampleFileName = attrs[DocConfigDataHandler.fileAttr]
                if exampleFileName.strip() == "":
                    raise Exception("ERROR: Missing file attribute for example tag. Unable to continue. " + self.makeAtLineCol())
                if not os.path.isabs(exampleFileName):
                    exampleFileName = os.path.join(self.dataFolder, exampleFileName)
                
                if not os.path.exists(exampleFileName):
                    raise Exception("ERROR: Provided example file: {0} does not exist. Unable to continue. ".format(exampleFileName) + self.makeAtLineCol())

                # Saving Path
                self.example.sourceCodeFile = exampleFileName

                strm = open(exampleFileName, "r")
                self.example.sourceCode = "".join(strm.readlines())

                srcMLProc = subprocess.Popen([srcMLExec, "--language", self.doc.srcMLLanguage, "--literal", "--modifier", "--operator", exampleFileName],  stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                errOutput = srcMLProc.stderr.readlines()
                try:
                    linesTemp = "".join(srcMLProc.stdout.readlines())
                    exampleDOM = minidom.parseString(linesTemp)
                    self.example.srcML = "".join([x.toxml() for x in exampleDOM.childNodes[0].childNodes])
                except Exception as ex:
                    # print linesTemp
                    print "Failed with exception: ", traceback.format_exc(ex)
                    print "Caught exception while parsing output from srcML.", ex
                    raise

            else:
                raise Exception("ERROR: Missing file attribute for example tag. Unable to continue. ".format(exampleFileName) + self.makeAtLineCol())

        elif name == DocConfigDataHandler.XPathTag:
            self.stateStack.append(DocConfigDataHandler.S_XPATH)
            self.xpathExample = XPathExample()
            self.xpathExample.xpath = self.getAttrFailOnMissing(name, DocConfigDataHandler.pathAttr, attrs)


        elif name == DocConfigDataHandler.OperatorEntryTag:
            # Creating operator entry
            opEntry = OperatorEntry()
            if DocConfigDataHandler.titleAttr in attrs:
                opEntry.title = attrs[DocConfigDataHandler.titleAttr]
            else:
                raise Exception("ERROR: Missing title attribute from OperatorEntry tag unable to continue. " + self.makeAtLineCol())

            if DocConfigDataHandler.opAttr in attrs:
                opEntry.op = attrs[DocConfigDataHandler.opAttr]
            else:
                raise Exception("ERROR: Missing op attribute from OperatorEntry tag unable to continue. " + self.makeAtLineCol())

            # Handling the reading and extracting of oerator entries.
            if DocConfigDataHandler.exampleAttr in attrs:
                exampleFileName = attrs[DocConfigDataHandler.exampleAttr]
                if exampleFileName.strip() == "":
                    raise Exception("ERROR: Missing example attribute for OperatorEntry tag. Unable to continue. " + self.makeAtLineCol())
                if not os.path.isabs(exampleFileName):
                    exampleFileName = os.path.join(self.dataFolder, exampleFileName)
                
                if not os.path.exists(exampleFileName):
                    raise Exception("ERROR: Provided example file: {0} does not exist. Unable to continue. ".format(exampleFileName) + self.makeAtLineCol())

                # Saving Path
                opEntry.exampleFile = exampleFileName
                strm = open(exampleFileName, "r")
                opEntry.sourceCode = "".join(strm.readlines())

                srcMLProc = subprocess.Popen([srcMLExec, "--language", self.doc.srcMLLanguage, "--literal", "--modifier", "--operator"],stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                srcMLProc.stdin.write(opEntry.sourceCode)
                srcMLProc.stdin.flush()
                srcMLProc.stdin.close()
                allSrcMLCode = "".join(srcMLProc.stdout.readlines())
                exampleDOM = minidom.parseString(allSrcMLCode)
                locatedElements = exampleDOM.getElementsByTagNameNS("http://www.sdml.info/srcML/operator", "operator")
                if len(locatedElements) != 1:
                    if len(locatedElements) > 1:
                        matchingLocatedElements = [x for x in locatedElements if x.firstChild.data == opEntry.op]
                        if len(matchingLocatedElements) == 1:
                            opEntry.srcML = matchingLocatedElements[0].toxml()
                        elif len(matchingLocatedElements) == 0:
                            opEntry.srcML = "`<br/>`".join([ elem.toxml() for elem in locatedElements])
                        else:
                            opEntry.srcML = "BUG"
                            # raise Exception("ERROR: Missing Located incorrect # of operators within OperatorEntry example file's srcML representation. {0} ".format(allSrcMLCode) + self.makeAtLineCol())
                    else:
                        raise Exception("ERROR: Missing Located incorrect # of operators within OperatorEntry example file's srcML representation. {0} ".format(allSrcMLCode) + self.makeAtLineCol())
                else:
                    opEntry.srcML = locatedElements[0].toxml()
                opEntry.srcMLExpr = "".join([x.toxml() for x in exampleDOM.childNodes[0].childNodes])
            else:
                raise Exception("ERROR: Missing example attribute from OperatorEntry tag unable to continue. " + self.makeAtLineCol())

            self.doc.operators.append(opEntry)

        else:
            raise Exception( "Unknown handled tag:" + name + self.makeAtLineCol())



    def endElement(self, name):

        if name == DocConfigDataHandler.DocConfigTag:
            # Preconditions
            self.stateStack.pop()
            # Postconditions
        elif name == DocConfigDataHandler.CategoryTag:
            self.doc.categories.append(self.category)
            self.category = None

        elif name == DocConfigDataHandler.DocEntryTag:
            # Preconditions
            assert self.entry != None, "Didn't set document entry correctly." 

            self.stateStack.pop()
            if self.category != None:
                self.category.entries.append(self.entry)
            else:
                self.doc.entries.append(self.entry)

            self.entry = None

            # Postconditions
            assert self.entry == None, "Didn't set document entry correctly." 

        elif name == DocConfigDataHandler.DescTag:
            
            if self.stateStack[-1] == DocConfigDataHandler.S_PAGE:
                self.doc.desc = self.recordedCharacters
            elif self.stateStack[-1] == DocConfigDataHandler.S_DOC_ENTRY:
                self.entry.desc = self.recordedCharacters
            elif self.stateStack[-1] == DocConfigDataHandler.S_ELEMENT:
                self.element.desc = self.recordedCharacters
            elif self.stateStack[-1] == DocConfigDataHandler.S_SUBELEMENT:
                self.subelement.desc = self.recordedCharacters
            elif self.stateStack[-1] == DocConfigDataHandler.S_EXAMPLE:
                self.example.desc = self.recordedCharacters
            elif self.stateStack[-1] == DocConfigDataHandler.S_XPATH:
                self.xpathExample.desc = self.recordedCharacters
            self.recordNextCharacters = False

        elif name == DocConfigDataHandler.AttrStrTag:
            if self.stateStack[-1] == DocConfigDataHandler.S_SUBELEMENT:
                self.subelement.attributeStr = self.recordedCharacters
            elif self.stateStack[-1] == DocConfigDataHandler.S_ELEMENT:
                self.element.attributeStr = self.recordedCharacters
            else:
                raise Exception("Invalid state containing AttribStr element. State#: {0}. ".format(self.stateStack[-1]) + self.makeAtLineCol())
            self.recordNextCharacters = False            
            self.recordedCharacters = ""
            
        elif name == DocConfigDataHandler.ElementsTag:
            self.stateStack.pop()

        elif name == DocConfigDataHandler.ElementTag:
            self.stateStack.pop()
            self.entry.elements.append(self.element)
            self.element = None

        elif name == DocConfigDataHandler.SubelementTag:
            self.stateStack.pop()
            self.entry.subelements.append(self.subelement)
            self.subelement = None
        
        elif name == DocConfigDataHandler.ReqsrcMLMarUpOptionTag:
            pass

        elif name == DocConfigDataHandler.ExampleTag:
            self.entry.examples.append(self.example)
            self.example = None
            self.stateStack.pop()

        elif name == DocConfigDataHandler.XPathTag:
            assert self.example != None, "XPath Not associated with an Example."
            self.example.xpathExamples.append(self.xpathExample)
            self.xpathExample = None
            self.stateStack.pop()

        elif name == DocConfigDataHandler.OperatorEntryTag:
            pass

        else:
            raise Exception( "Unknown handled tag:" + name + self.makeAtLineCol())

    def characters(self, content):
        if self.recordNextCharacters:
            self.recordedCharacters += content



def compliePage(docConfigPath):
    contentHandler = DocConfigDataHandler()
    contentHandler.doc = DocConfig()
    parser = xml.sax.make_parser()
    contentHandler.dataFolder = os.path.dirname(docConfigPath)
    parser.setContentHandler(contentHandler)
    try:
        parser.parse(open(docConfigPath,"r"))
        #print contentHandler.doc
        return contentHandler.doc
    except AssertionError as e:
        print "Error while parsing document:{0}. Error: {1}".format(docConfigPath, e)
        # raise

    return None
"""
Relivant xmlTag names:
<DocConfig lang="C" outputFile="CPreProc.md">
    <DocEntry title="#error">
        <Desc></Desc>
        <Elements>
            <Element srcMLNS="cpp" srcMLName="error">
                <Desc></Desc>
                <MarkUpOptions>
                    <Option srcMLOption="" />
                </MarkUpOptions>
            </Tag>
        </Tags>
        <Example title="#error" file="CPreProc-error.txt">
            <Desc></Desc>
        </Example>
    </DocEntry>
</DocConfig>
"""



# -------------------------------------------------
#                     Main
# -------------------------------------------------

if __name__ == "__main__":
    # Constants used during execution.
    pageCreationDocument = "DocConfig.xml"
    srcMLExec="srcml"
    pages = []


    # Handling the parsing of arguments
    # argParser = argparse.ArgumentParser(description="Process DocConfig.xml files into srcML documentation.")
    # argParser.add_argument("uname", help="Your github user name. This is used by grip to allow constant access to the github flavoered markdown web API.", nargs='?', type=str, default="")
    # argParser.add_argument("pw", help="Your github password. This is used by grip to allow constant access to the github flavoered markdown web API.", nargs='?', type=str, default="")

    # #parser.add_argument('integers', metavar='N', type=int, nargs='+',
    # args = argParser.parse_args()
    
    # if "help" in args:
    #     argParser.print_help()

    #                   help='an integer for the accumulator')
    #parser.add_argument('--sum', dest='accumulate', action='store_const',
    #                   const=sum, default=max,
    #                   help='sum the integers (default: find the max)')
    #
    #args = parser.parse_args()
    #print args.accumulate(args.integers)


    for root, dirs, files in os.walk(os.path.abspath("./DocData")):
        if os.path.basename(root) == "DocData":
            continue

        # print os.path.basename(root)
        if pageCreationDocument in files:
            print "-" * 80
            print "Located a Page Creation Document in", root
            try:
                resultingPage = None
                pageDocConfigPath = os.path.join(root, pageCreationDocument)
                resultingPage = compliePage(pageDocConfigPath)
                if resultingPage != None:
                    pages.append(resultingPage)
                    if ( (not os.path.exists(resultingPage.outputFileName))
                            or os.path.getmtime(pageDocConfigPath) > os.path.getmtime(resultingPage.outputFileName)
                            or os.path.getmtime(sys.argv[0]) > os.path.getmtime(resultingPage.outputFileName)):
                        resultingPage.genMarkDown()
                    
            except Exception as e:
                print "Failed with exception: ", traceback.format_exc(e)
        
    # print "Collected Pages", len(pages)
    # print "Compling C Language documentation"
    # resultingPage = compliePage("/home/brian/Projects/srcMLDocGen/DocData/CPlusPlus/DocConfig.xml")
    # resultingPage.genMarkDown()
    print "-" * 80
