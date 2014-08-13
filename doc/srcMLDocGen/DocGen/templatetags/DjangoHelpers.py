from django.conf import settings
from django import template
import os, sys, re
import lxml.etree as ET
import lxml.sax as lxmlSAX
import xml.sax.saxutils as SAXUtils
from xml.sax.handler import ContentHandler
import cStringIO

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
    "java":set([x.strip() for x in open("DocGen/templatetags/JavaKeywords.txt","r").readlines()])
}

cKeywordRegex = re.compile("\\b(?P<kw>{0})\\b".format( "|".join(keywordDictionary["c"])), re.I | re.M)
cppKeywordRegEx = re.compile("\\b(?P<kw>{0})\\b".format( "|".join(keywordDictionary["c++"])))
javaKeywordRegEx = re.compile("\\b(?P<kw>{0})\\b".format( "|".join(keywordDictionary["java"])))
keywordRegExDictionary = {
    "c++":cppKeywordRegEx,
    "c":cKeywordRegex,
    "java":javaKeywordRegEx,

}
kwSubPattern = "<span class=\"kw\">\g<kw></span>"

class SyntaxHighlighter(ContentHandler):
    htmlEscapeTable = {'"': "&quot;", "'": "&apos;" }
    spanStart = "<span class=\"{0}\">"
    spanEnd = "</span>"

    def __init__(self, codeIssrcML, language):
        self.out = cStringIO.StringIO()
        self.content = ""
        self.displayXML = codeIssrcML
        self.language = language
        self.keywordRegEx = keywordRegExDictionary[language.lower()]
        self.iscomment = False

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
        if name[1] == "comment":
            self.iscomment = True
        self.out.write(SyntaxHighlighter.spanStart.format(self.getNormalizedStyleName(name)))
        if self.displayXML:
            self.writeTagStart(name, attributes)


    def endElementNS(self, name, qname):

        if name[1] == "unit":
            return
        if name[1] == "comment":
            self.iscomment = False
        self.out.write(SyntaxHighlighter.spanEnd)
        if self.displayXML:
            self.writeTagEnd(name)


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
    def __init__(self, nameOfSource, language, isSourceCode):
        self.nameOfTreeLoc = nameOfSource
        self.language = language
        self.isSourceOrXml = isSourceCode

    def render(self, context):
        tree = self.nameOfTreeLoc.resolve(context)
        language = self.language.resolve(context)
        sourceCodeOrXML = self.isSourceOrXml.resolve(context)
        contentHandler = SyntaxHighlighter(sourceCodeOrXML == "srcML", language)
        lxmlSAX.saxify(ET.fromstring(tree), contentHandler)
        return contentHandler.content
        
@register.tag(name="HighlightSyntaxFromTree")
def highlightSyntaxFromsrcML(parser, token):
    tokens = token.split_contents()
    if len(tokens) != 4:
        raise TemplateSyntaxError("Incorrect number of arguments for HighlightSyntaxFromTree, expected: 4. Got: %d" % len(tokens))
    return SyntaxHighlightedNode(parser.compile_filter(tokens[1]), parser.compile_filter(tokens[2]), parser.compile_filter(tokens[3]))


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
