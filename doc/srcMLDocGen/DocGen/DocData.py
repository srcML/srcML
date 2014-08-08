import sys

class Element(object):
    """ Stores information about tags related to this description. """
    def __init__(self):
        self.tag = ""
        self.ns = ""
        self.desc = ""
        self.markupOptionsRequired = []
        self.attributeStr = ""

    def QName(self):
        nameFormat = ""
        if self.ns == "":
            nameFormat = "&lt;{0.tag}"
        else:
            nameFormat = "&lt;{0.ns}:{0.tag}"

        if self.attributeStr != "":
            nameFormat += " {0.attributeStr}"
        nameFormat += "&gt;"
        return nameFormat.format(self)

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

    def linkName(self):
        return "-".join(self.title.strip().replace("#","").replace(",", "").lower().split())


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


class IndexEntry:
    """ An entry within the index of links that's sorted via alphabetic order instead of
        logical related element ordering
    """
    def __init__(self):
        self.title = ""
        self.shortTitle = ""
        self.category = ""
        self.isOperator = False
        self.language = ""
        self.element = []
        self.subelements = []
        self.linkName = ""
        self.basePageName = ""
        self.op = ""

    def __lt__(self, other):
        return self.title.lower() < other.title.lower()

    def indexLetter(self):
        return self.title[0].upper()
