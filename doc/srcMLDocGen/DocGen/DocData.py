import sys

class Element(object):
    """ Stores information about tags related to this description. """
    def __init__(self):
        self.tag = ""
        self.ns = ""
        self.desc = ""
        self.markupOptionsRequired = []
        self.attributeStr = ""


class Subelement(Element):
    """ Stores information related to sub elements. """
    def __init__(self):
        super(Subelement, self).__init__()

class XPathExample:
    def __init__(self):
        self.desc = ""
        self.xpath = ""

class Example:
    def __init__(self):
        self.title = ""
        self.sourceCodeFile = "" 
        self.sourceCode = ""
        self.srcML = ""
        self.index = 0

class DocEntry:
    """ This is an entry within the documentation. """
    def __init__(self):
        self.title = ""
        self.shortTitle = ""
        self.desc = ""
        self.elements = []
        self.subelements = []
        self.examples = []
        self.xpathQueries = []
        self.linkName = ""

class OperatorEntry:
    """ This is an operator entry within the document that tell me to extract
        an operator from the srcML example.
    """
    def __init__(self):
        self.title = ""
        self.op = ""
        self.exampleFile = ""
        self.srcML = ""
        self.sourceCode = ""


class Category:
    def __init__(self):
        self.title = ""
        self.entries = []
        self.operators = []


class DocConfig:
    """ Manages all pages related to a single topic/language."""
    def __init__(self):
        self.srcMLLanguage = "NOT SET"
        self.title = ""
        self.outputFileName = ""
        self.desc = ""
        self.categories = []
        self.entries = []
        self.operators = []