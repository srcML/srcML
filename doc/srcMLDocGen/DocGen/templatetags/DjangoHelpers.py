from django.conf import settings
from django import template
import os, sys
import lxml.etree as ET
import lxml.sax as lxmlSAX
import xml.sax.saxutils as SAXUtils
from xml.sax.handler import ContentHandler
import cStringIO

register = template.Library()

namesOfStyles = set()


uriToPrefix = {
    "http://www.sdml.info/srcML/operator": "op",
    "http://www.sdml.info/srcML/src": "src",
    "http://www.sdml.info/srcML/cpp": "cpp",
    "http://www.sdml.info/srcML/literal": "lit",
    "http://www.sdml.info/srcML/modifier": "type",
}

class SyntaxHighlighter(ContentHandler):
    htmlEscapeTable = {'"': "&quot;", "'": "&apos;" }
    spanStart = "<span class=\"{0}\">"
    spanEnd = "</span>"

    def __init__(self, codeIssrcML):
        self.out = cStringIO.StringIO()
        self.content = ""
        self.displayXML = codeIssrcML
        self.tagOutput = open("styleClassList2.css","a")


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
        self.out.write(SyntaxHighlighter.spanStart.format(self.getNormalizedStyleName(name)))
        if self.displayXML:
            self.writeTagStart(name, attributes)


    def endElementNS(self, name, qname):
        if name[1] == "unit":
            return
        self.out.write(SyntaxHighlighter.spanEnd)
        if self.displayXML:
            self.writeTagEnd(name)


    def characters(self, data):
        self.out.write(SAXUtils.escape(data, SyntaxHighlighter.htmlEscapeTable))


class SyntaxHighlightedNode(template.Node):
    def __init__(self, nameOfSource, language, isSourceCode):
        self.nameOfTreeLoc = nameOfSource
        self.language = language
        self.isSourceOrXml = isSourceCode

    def render(self, context):
        tree = self.nameOfTreeLoc.resolve(context)
        language = self.language.resolve(context)
        sourceCodeOrXML = self.isSourceOrXml.resolve(context)
        contentHandler = SyntaxHighlighter(sourceCodeOrXML == "srcML")
        lxmlSAX.saxify(ET.fromstring(tree), contentHandler)
        return contentHandler.content
        
@register.tag(name="HighlightSyntaxFromTree")
def highlightSyntaxFromsrcML(parser, token):
    tokens = token.split_contents()
    # print tokens
    if len(tokens) != 4:
        raise TemplateSyntaxError("Incorrect number of arguments for HighlightSyntaxFromTree, expected: 4. Got: %d" % len(tokens))

    # parser.compile_filter(tokens[1]),
    # tokens[2], # fragment_name can't be a variable.
    # [parser.compile_filter(token) for token in tokens[3:]]
    return SyntaxHighlightedNode(parser.compile_filter(tokens[1]), parser.compile_filter(tokens[2]), parser.compile_filter(tokens[3]))