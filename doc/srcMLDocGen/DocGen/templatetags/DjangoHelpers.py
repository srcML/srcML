from django.conf import settings
from django import template
import os, sys, re
import lxml.etree as ET
import lxml.sax as lxmlSAX
import xml.sax.saxutils as SAXUtils
from xml.sax.handler import ContentHandler
import cStringIO
from DocGen import *
import DocGen.TagTracker

register = template.Library()

uriToPrefix = {
    "http://www.sdml.info/srcML/operator": "op",
    "http://www.sdml.info/srcML/src": "src",
    "http://www.sdml.info/srcML/cpp": "cpp",
    "http://www.sdml.info/srcML/literal": "lit",
    "http://www.sdml.info/srcML/modifier": "type",
}


tagListing = set(["<src:unit>"])

def getTagListing():
    return tagListing

def clearTagListing():
    tagListing = set()

def formatTagName(ns, name, attr):
    tag = "<" 
    tag += "{0}:{1}".format(uriToPrefix[ns], name)
    if len(attr) > 0:
        tag += " " + " ".join(["{0}=\"{1}\"".format(k[1], v) for k, v in attr.items()])
    tag +=">"
    return tag

keywordDictionary = {
    "c++":set([x.strip() for x in open("DocGen/templatetags/CppKeywords.txt","r").readlines()]),
    "c":set([x.strip() for x in open("DocGen/templatetags/CKeywords.txt","r").readlines()]),
    "java":set([x.strip() for x in open("DocGen/templatetags/JavaKeywords.txt","r").readlines()]),
    "c#":set([x.strip() for x in open("DocGen/templatetags/CSharpKeywords.txt","r").readlines()])
}

cKeywordRegex = re.compile("\\b(?P<kw>{0})\\b".format( "|".join(keywordDictionary["c"])), re.I | re.M)
cppKeywordRegEx = re.compile("\\b(?P<kw>{0})\\b".format( "|".join(keywordDictionary["c++"])))
javaKeywordRegEx = re.compile("\\b(?P<kw>{0})\\b".format( "|".join(keywordDictionary["java"])))

keywordRegExDictionary = {
    "c++":cppKeywordRegEx,
    "c":cKeywordRegex,
    "java":javaKeywordRegEx,
    "c#":re.compile("\\b(?P<kw>{0})\\b".format( "|".join(keywordDictionary["c#"])))
}
kwSubPattern = "<span class=\"kw\">\g<kw></span>"
grammarOutputFileName = "srcMLGrammar.html"
linkStart = "<a href=\"{0}#{1}\" title=\"View grammar for tag.\">"
linkEnd = "</a>"

class SyntaxHighlighter(ContentHandler):
    htmlEscapeTable = {}
    # '"': "&quot;", "'": "&apos;" 
    spanStart = "<span class=\"{0}\">"
    spanEnd = "</span>"
    # def makeLinkStart(self, name):

    def __init__(self, codeIssrcML, language, gammarDocumentationFile):
        self.out = cStringIO.StringIO()
        self.content = ""
        self.displayXML = codeIssrcML
        self.language = language
        self.keywordRegEx = keywordRegExDictionary[language.lower()]
        self.iscomment = False
        self.grammarFileURL = gammarDocumentationFile

    def getNormalizedStyleName(self, name):
        return uriToPrefix[name[0]] + "_" + name[1]

    def getResolvedTagName(self, name):
        prefix = uriToPrefix[name[0]]
        if prefix == "src":
            return name[1]
        else:
            return "{0}:{1}".format(prefix, name[1])

    def writeTagStart(self, name, attr):
        self.out.write(SyntaxHighlighter.spanStart.format("srcMLElement"))
        if len(attr) > 0:
            self.out.write("&lt;{0} {1}&gt;".format(self.getResolvedTagName(name),  " ".join(["<span class=\"srcMLAttribName\">{0}</span><span class=\"srcMLAttribEquals\">=</span><span class=\"srcMLAttribValue\">&quot;{1}&quot;</span>".format(k[1], v) for k, v in attr.items()])))
        else:
            self.out.write("&lt;{0}&gt;".format(self.getResolvedTagName(name)))
        self.out.write(SyntaxHighlighter.spanEnd)
    
    def writeTagEnd(self, name):
        self.out.write(SyntaxHighlighter.spanStart.format("srcMLElement") )
        self.out.write("&lt;/{0}&gt;".format(self.getResolvedTagName(name)))
        self.out.write(SyntaxHighlighter.spanEnd)

    def endDocument(self):
        self.content = self.out.getvalue()
        self.out.close()

    def startElementNS(self, name, qname, attributes):
        if name[1] == "unit":
            return
        tagListing.add(formatTagName(name[0], name[1], attributes))
        DocGen.TagTracker.Tracker.recordTag(uriToPrefix[name[0]], name[1], dict() if len(attributes) ==0 else dict([(k[1],v) for k, v in attributes.items()]))
        if name[1] == "comment":
            self.iscomment = True
        self.out.write(SyntaxHighlighter.spanStart.format(self.getNormalizedStyleName(name)))
        if self.displayXML:
            tagLinkName = "tag_"
            if uriToPrefix[name[0]] != "src":
                tagLinkName += uriToPrefix[name[0]] + "_"
            tagLinkName += name[1]
            self.out.write(linkStart.format(self.grammarFileURL, tagLinkName))
            self.writeTagStart(name, attributes)
            self.out.write(linkEnd)


    def endElementNS(self, name, qname):
        if name[1] == "unit":
            return
        if name[1] == "comment":
            self.iscomment = False
        self.out.write(SyntaxHighlighter.spanEnd)
        if self.displayXML:
            tagLinkName = "tag_"
            if uriToPrefix[name[0]] != "src":
                tagLinkName += uriToPrefix[name[0]] + "_"
            tagLinkName += name[1]
            self.out.write(linkStart.format(self.grammarFileURL, tagLinkName))
            self.writeTagEnd(name)
            self.out.write(linkEnd)



    def characters(self, data):
            
        toWrite = data
        if self.displayXML:
            toWrite = SAXUtils.escape(SAXUtils.escape(toWrite, SyntaxHighlighter.htmlEscapeTable))
        else:
            toWrite = SAXUtils.escape(toWrite, SyntaxHighlighter.htmlEscapeTable)
        if not self.iscomment:
            toWrite = self.keywordRegEx.sub(kwSubPattern, toWrite)
        self.out.write(toWrite)


class SyntaxHighlightedNode(template.Node):
    def __init__(self, nameOfSource, language, isSourceCode, grammarDocURL=grammarOutputFileName):
        self.nameOfTreeLoc = nameOfSource
        self.language = language
        self.isSourceOrXml = isSourceCode
        self.grammarFile = grammarDocURL

    def render(self, context):
        tree = self.nameOfTreeLoc.resolve(context)
        language = self.language.resolve(context)
        sourceCodeOrXML = self.isSourceOrXml.resolve(context)
        grammarFileURL = ""
        if isinstance(self.grammarFile, str):
            grammarFileURL = self.grammarFile
        else:
            grammarFileURL = self.grammarFile.resolve(context)
        contentHandler = SyntaxHighlighter(sourceCodeOrXML == "srcML", language, grammarFileURL)
        lxmlSAX.saxify(ET.fromstring(tree), contentHandler)
        return contentHandler.content
        
@register.tag(name="HighlightSyntaxFromTree")
def highlightSyntaxFromsrcML(parser, token):
    tokens = token.split_contents()
    if len(tokens) < 4 and len(tokens) >5:
        raise Exception("Incorrect number of arguments for HighlightSyntaxFromTree, expected: either 4 or 5. Got: %d" % len(tokens))

    if len(tokens) == 4:
        return SyntaxHighlightedNode(parser.compile_filter(tokens[1]), parser.compile_filter(tokens[2]), parser.compile_filter(tokens[3]))
    else:
        return SyntaxHighlightedNode(parser.compile_filter(tokens[1]), parser.compile_filter(tokens[2]), parser.compile_filter(tokens[3]), parser.compile_filter(tokens[4]))


def formatTagOrStrOutStr(var):
    if isinstance(var, str):
        return var
    elif isinstance(var, ET._Element):
        return "&lt;{0}&gt;{1}&lt;/{0}&gt;".format(var.tag[var.tag.find('}')+1:], SAXUtils.escape(SAXUtils.escape(var.text, SyntaxHighlighter.htmlEscapeTable), SyntaxHighlighter.htmlEscapeTable))
    else:
        raise Exception("Invalid or unhandled type.")

class GetOperatorCodeNode(template.Node):
    def __init__(self, operatorListVar):
        self.operatorListVariable = operatorListVar

    def render(self, context):
        opList = self.operatorListVariable.resolve(context)
        return "<br/>".join([formatTagOrStrOutStr(x) for x in opList])
        
@register.tag(name="GetOperators")
def getOperators(parser, token):
    tokens = token.split_contents()
    if len(tokens) != 2:
        raise TemplateSyntaxError("Incorrect number of arguments for GetOperatorCode, expected: 1. Got: %d" % len(tokens))
    return GetOperatorCodeNode(parser.compile_filter(tokens[1]))

@register.filter
def isIn(lookingFor, collection):
    return lookingFor in collection



@register.filter(name = "strip")
def stripSpaces(value):
    return value.strip()

@register.filter(name = "isTerminalUseCase")
def isTerminalUseCase(obj):
    print obj.__class__.__name__
    return isinstance(obj, TerminalUseCase)

formatToLinkRegEx = re.compile("\W")
@register.filter(name="makeFunctionNameLink")
def makeFunctionNameLink(callNameList):
    assert isinstance(callNameList, list), "Received object that wasn't a list"
    return formatToLinkRegEx.sub("_"," ".join(callNameList))


class TrackPageNameNode(template.Node):
    def __init__(self, pageNameIndicator):
        self.pageNameRef = pageNameIndicator

    def render(self, context):
        pageName = self.pageNameRef.resolve(context)
        DocGen.TagTracker.Tracker.setCurrentPageURL(pageName)
        return ""
        
@register.tag(name="TrackPageName")
def TrackPageName(parser, token):
    tokens = token.split_contents()
    if len(tokens) != 2:
        raise TemplateSyntaxError("Incorrect number of arguments for TrackPageName, expected: 2. Got: %d" % len(tokens))
    return TrackPageNameNode(parser.compile_filter(tokens[1]))

class TrackAnchorIdNode(template.Node):
    def __init__(self, anchorIdIndicator):
        self.anchorIdRef = anchorIdIndicator

    def render(self, context):
        anchorId = self.anchorIdRef.resolve(context)
        DocGen.TagTracker.Tracker.setCurrentAnchor(anchorId)
        return ""

@register.tag(name="TrackAnchorId")
def TrackAnchorId(parser, token):
    tokens = token.split_contents()
    if len(tokens) != 2:
        raise TemplateSyntaxError("Incorrect number of arguments for TrackAnchorId, expected: 2. Got: %d" % len(tokens))
    return TrackAnchorIdNode(parser.compile_filter(tokens[1]))
   


