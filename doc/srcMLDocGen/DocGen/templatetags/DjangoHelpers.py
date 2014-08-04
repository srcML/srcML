from django.conf import settings
from django import template
import os, sys
import lxml.etree as ET
import lxml.sax as lxmlSAX
import xml.sax.saxutils as SAXUtils
from xml.sax.handler import ContentHandler
import cStringIO

register = template.Library()

class SyntaxHighlighter(ContentHandler):
    htmlEscapeTable = {'"': "&quot;", "'": "&apos;" }
    spanStart = "<span class=\"{0}\">"
    spanEnd = "</span>"
    def writeTagStart(self, name, attr):
        pass

    def __init__(self, codeIssrcML):
        self.out = cStringIO.StringIO()
        self.content = ""
        self.displayXML = codeIssrcML

    def endDocument(self):
        self.content = self.out.getvalue()
        self.out.close()

    def startElementNS(self, name, qname, attributes):
        # print qname.__class__
        print name.__class__
        # print qname
        #print name
        self.out.write(SyntaxHighlighter.spanStart.format("derp"))
        if self.displayXML:
            self.out.write(SyntaxHighlighter.spanStart.format(name[1]))
            self.writeTagStart(name, attributes)
            self.out.write(SyntaxHighlighter.spanEnd)


    def endElementNS(self, name, qname):
        if self.displayXML:
            self.out.write(SyntaxHighlighter.spanEnd)

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